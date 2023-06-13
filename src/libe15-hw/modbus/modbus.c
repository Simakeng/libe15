/**
 * @file modbus.c
 * @author simakeng (simakeng@outlook.com)
 * @brief modbus slave protocol
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "modbus.h"

#include <string.h>

#include <libe15-dbg.h>

enum
{
    RX_STATE_IDLE,
    RX_STATE_IN_PROGRESS,
    RX_STATE_IGNORE,
    RX_STATE_OVERFLOW,
};

static const uint16_t modbus_crc_table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

#define MODBUS_CRC_CALC(crc, data)                 \
    do                                             \
    {                                              \
        uint8_t xor_val = ((data) ^ (crc)) & 0xFF; \
        (crc) >>= 8;                               \
        (crc) ^= modbus_crc_table[xor_val];        \
    } while (0U)

static inline uint16_t modbus_crc_calc(uint8_t *pdata, uint32_t size)
{
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < size; i++)
    {
        MODBUS_CRC_CALC(crc, pdata[i]);
    }

    return crc;
}

void modbus_slave_recv_cplt_handler(modbus_slave_t *slave)
{
    uint32_t recv_cnt = slave->rx_cnt;
    uint8_t *recv_buf = slave->recv_buf;

    uint16_t crc = slave->crc;

    uint16_t recv_crc = (recv_buf[recv_cnt - 1] << 8) | recv_buf[recv_cnt - 2];

    if (recv_crc != crc)
    {
        slave->rx_state = RX_STATE_IDLE;
        return;
    }

    uint32_t func = recv_buf[1];
    uint32_t tx_total = 0;

    switch (func)
    {
    default:
    {
        slave->send_buf[0] = slave->slave_addr;
        slave->send_buf[1] = func | 0x80;
        slave->send_buf[2] = MODBUS_ERR_ILLEGAL_FUNCTION;
        uint16_t tx_crc = modbus_crc_calc(slave->send_buf, 3);
        slave->send_buf[3] = tx_crc & 0xFF;
        slave->send_buf[4] = tx_crc >> 8;
        tx_total = 5;
    }
    break;
    }

    slave->rx_cnt = 0;

    if (tx_total == 0)
        return;

    error_t result = slave->desc->request_pdu_transmit(tx_total, slave->send_buf);
    if (result == ALL_OK)
    {
        slave->tx_total = tx_total;
    }
    else
    {
        slave->tx_total = 0;
        memset(slave->send_buf, 0, sizeof(slave->send_buf));
    }
}

uint32_t modbus_slave_send_get_data(modbus_slave_t *slave, uint8_t *data_out)
{
    if (slave->tx_total == 0)
    {
        *data_out = 0;
        return MODBUS_SEND_CPLT;
    }

    *data_out = slave->send_buf[slave->tx_cnt];
    slave->tx_cnt++;
    if (slave->tx_total == slave->tx_cnt)
    {
        slave->tx_cnt = 0;
        slave->tx_total = 0;
    }
    return MODBUS_SEND_NORMAL;
}

void modbus_slave_recv_handler(modbus_slave_t *slave, uint32_t byte, uint32_t state)
{
    switch (state)
    {
    case MODBUS_RECV_DATA:
        // this is first byte
        if (slave->rx_state == RX_STATE_IDLE)
        {
            // 0: broadcast check if this packet need to be processed
            if (byte == 0 || byte == slave->slave_addr)
            {
                memset(slave->recv_buf, 0, sizeof(slave->recv_buf));
                slave->recv_buf[0] = byte;
                slave->rx_cnt = 1;
                slave->rx_state = RX_STATE_IN_PROGRESS;
            }
            else
            {
                // if this packet is not for this slave, ignore it
                slave->rx_state = RX_STATE_IGNORE;
                goto clear_slave_recv_buf;
            }
        }
        else if (slave->rx_state == RX_STATE_IN_PROGRESS)
        {
            slave->recv_buf[slave->rx_cnt] = byte;

            int32_t crc_off = slave->rx_cnt - 2;
            if (crc_off >= 0)
                MODBUS_CRC_CALC(slave->crc, slave->recv_buf[crc_off]);
            else
                slave->crc = 0xFFFF;

            slave->rx_cnt++;

            if (slave->rx_cnt >= sizeof(slave->recv_buf))
            {
                // can't process this packet, ignore it
                slave->rx_state = RX_STATE_OVERFLOW;
                goto clear_slave_recv_buf;
            }
        }
        break;
    case MODBUS_RECV_ERROR:
        if (slave->rx_state == RX_STATE_IN_PROGRESS)
        {
            // error occurred, ignore this packet
            slave->rx_state = RX_STATE_IGNORE;
            goto clear_slave_recv_buf;
        }
        else
            break;
    case MODBUS_RECV_END:
        if (slave->rx_state == RX_STATE_IDLE)
            return;
        else if (slave->rx_state == RX_STATE_IN_PROGRESS)
            modbus_slave_recv_cplt_handler(slave);
        // there is no 'break' stmt, so it will fall through to the next case
        // reset state to RX_STATE_IDLE, crc to 0xFFFF and clear recv_buf
    default:
        slave->rx_state = RX_STATE_IDLE;
        goto clear_slave_recv_buf;
    }
    return;

clear_slave_recv_buf:
    slave->rx_cnt = 0;
    memset(slave->recv_buf, 0, sizeof(slave->recv_buf));
    slave->crc = 0xFFFF;
    return;
}

error_t modbus_slave_init(modbus_slave_t *slave, const modbus_slave_init_t *desc, uint8_t slave_addr)
{
    if (slave == NULL || desc == NULL)
        return E_INVALID_ARGUMENT;
    if (desc->holding_reg_cnt != 0 && desc->holding_regs == NULL)
        return E_INVALID_ARGUMENT;
    if (desc->input_reg_cnt != 0 && desc->input_regs == NULL)
        return E_INVALID_ARGUMENT;

    if (desc->request_pdu_transmit == NULL)
        return E_INVALID_ARGUMENT;
    memset(slave, 0, sizeof(modbus_slave_t));
    slave->desc = desc;
    slave->slave_addr = slave_addr;
    slave->rx_cnt = 0;
    slave->rx_state = RX_STATE_IDLE;
    slave->crc = 0xFFFF;
    return ALL_OK;
}

error_t modbus_slave_set_addr(modbus_slave_t *slave, uint8_t slave_addr)
{
    if (slave == NULL)
        return E_INVALID_ARGUMENT;
    slave->slave_addr = slave_addr;
    return ALL_OK;
}
