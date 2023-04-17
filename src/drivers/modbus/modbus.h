/**
 * @file modbus.h
 * @author simakeng (simakeng@outlook.com)
 * @brief modbus slave protocol
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>
#include <libe15-errors.h>

typedef struct
{
    uint16_t reg_start_addr;
    uint32_t reg_map_len;
    error_t (*read_handler)(uint16_t addr, uint16_t *data, uint8_t *error_code_out);
    error_t (*write_handler)(uint16_t offset, uint16_t data, uint8_t *error_code_out);
    void *usr_ptr;
} modbus_reg_desc_t;

enum
{
    /// @brief no error
    MODBUS_ERR_NONE = 0,

    /**
     * @brief The function code received in the query is not an
     * allowable action for the server (or slave). This
     * may be because the function code is only
     * applicable to newer devices, and was not
     * implemented in the unit selected. It could also
     * indicate that the server (or slave) is in the wrong
     * state to process a request of this type, for
     * example because it is unconfigured and is being
     * asked to return register values.
     */
    MODBUS_ERR_ILLEGAL_FUNCTION = 0x01,

    /**
     * @brief The data address received in the query is not an
     * allowable address for the server (or slave). More
     * specifically, the combination of reference number
     * and transfer length is invalid. For a controller with
     * 100 registers, the PDU addresses the first
     * register as 0, and the last one as 99. If a request
     * is submitted with a starting register address of 96
     * and a quantity of registers of 4, then this request
     * will successfully operate (address-wise at least)
     * on registers 96, 97, 98, 99. If a request is
     * submitted with a starting register address of 96
     * and a quantity of registers of 5, then this request
     * will fail with Exception Code 0x02 “Illegal Data
     * Address” since it attempts to operate on registers
     * 96, 97, 98, 99 and 100, and there is no register
     * with address 100.
     */
    MODBUS_ERR_ILLEGAL_DATA_ADDRESS = 0x02,

    /**
     * @brief A value contained in the query data field is not an
     * allowable value for server (or slave). This
     * indicates a fault in the structure of the remainder
     * of a complex request, such as that the implied
     * length is incorrect. It specifically does NOT mean
     * that a data item submitted for storage in a register
     * has a value outside the expectation of the
     * application program, since the MODBUS protocol
     * is unaware of the significance of any particular
     * value of any particular register.
     */
    MODBUS_ERR_ILLEGAL_DATA_VALUE = 0x03,

    /**
     * @brief An unrecoverable error occurred while the server
     * (or slave) was attempting to perform the
     * requested action.
     */
    MODBUS_ERR_SLAVE_DEVICE_FAILURE = 0x04,

    /**
     * @brief Specialized use in conjunction with programming commands.
     * The server (or slave) has accepted the request
     * and is processing it, but a long duration of time
     * will be required to do so. This response is
     * returned to prevent a timeout error from occurring
     * in the client (or master). The client (or master)
     * can next issue a Poll Program Complete message
     * to determine if processing is completed.
     */
    MODBUS_ERR_ACKNOWLEDGE = 0x05,

    /**
     * @brief Specialized use in conjunction with programming commands.
     * The server (or slave) is engaged in processing a
     * long–duration program command. The client (or
     * master) should retransmit the message later when
     * the server (or slave) is free.
     */
    MODBUS_ERR_SLAVE_DEVICE_BUSY = 0x06,

    /**
     * @brief Specialized use in conjunction with function codes
     * 20 and 21 and reference type 6, to indicate that
     * the extended file area failed to pass a consistency
     * check. The server (or slave) attempted to read record
     * file, but detected a parity error in the memory.
     * The client (or master) can retry the request, but
     * service may be required on the server (or slave)
     * device.
     */
    MODBUS_ERR_MEMORY_PARITY_ERROR = 0x08,

    /**
     * @brief Specialized use in conjunction with gateways,
     * indicates that the gateway was unable to allocate
     * an internal communication path from the input
     * port to the output port for processing the request.
     * Usually means that the gateway is misconfigured
     * or overloaded.
     */
    MODBUS_ERR_GATEWAY_PATH_UNAVAILABLE = 0x0A,

    /**
     * @brief Specialized use in conjunction with gateways,
     * indicates that no response was obtained from the
     * target device. Usually means that the device is
     * not present on the network.
     */
    MODBUS_ERR_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 0x0B,
};

enum
{
    MODBUS_FN_READ_DISCRETE_INPUTS = 0x02,
    
    MODBUS_FN_READ_COILS                        = 0x01,
    MODBUS_FN_WRITE_SINGLE_COIL                 = 0x05,
    MODBUS_FN_WRITE_MULTIPLE_COILS              = 0x0F,

    MODBUS_FN_READ_INPUT_REGISTERS              = 0x04,
    MODBUS_FN_READ_HOLDING_REGISTERS            = 0x03,
    MODBUS_FN_WRITE_SINGLE_REGISTER             = 0x06,
    MODBUS_FN_WRITE_MULTIPLE_REGISTERS          = 0x10,
    MODBUS_FN_READ_WRITE_MULTIPLE_REGISTERS     = 0x17,
    MODBUS_FN_MASK_WRITE_REGISTER               = 0x16,
    MODBUS_FN_READ_FIFO_QUEUE                   = 0x18,

    MODBUS_FN_READ_FILE_RECORD                  = 0x14,
    MODBUS_FN_WRITE_FILE_RECORD                 = 0x15,
    
    MODBUS_FN_READ_EXCEPTION_STATUS             = 0x07,
    MODBUS_FN_DIAGNOSTICS                       = 0x08,
    MODBUS_FN_GET_COMM_EVENT_COUNTER            = 0x0B,
    MODBUS_FN_GET_COMM_EVENT_LOG                = 0x0C,
    MODBUS_FN_REPORT_SLAVE_ID                   = 0x11,
    MODBUS_FN_ENCAPSULATED_INTERFACE_TRANSPORT  = 0x2B,
};

typedef struct
{
    modbus_reg_desc_t *input_regs;
    uint8_t input_reg_cnt;

    modbus_reg_desc_t *holding_regs;
    uint8_t holding_reg_cnt;

    error_t (*set_send_buffer)(uint32_t size, const void *pdata);
} modbus_slave_init_t;

typedef struct
{
    const modbus_slave_init_t * desc;

    uint8_t recv_buf[32];
    uint32_t send_buf[32];
    uint32_t tx_cnt;
    uint32_t rx_cnt;

    uint8_t slave_addr;

    uint32_t rx_state;
    uint16_t crc;

} modbus_slave_t;

enum
{
    /// indicates this is the first byte of a modbus frame
    MODBUS_RECV_START,
    /// normal reciveing
    MODBUS_RECV_ACTIVE,
    /// the last byte is received, this byte is invalid
    MODBUS_RECV_END,
};

/**
 * @brief Update all internal states of a modbus slave according to the received byte.
 * @param slave the modbus slave instance
 * @param byte the received byte, 8bit
 * @param state the state of the modbus slave, one of MODBUS_RECV_
 *?@note this function should be called at every byte received in the pripheral interrupt handler.
 */
void modbus_slave_recv_handler(modbus_slave_t *slave, uint32_t byte, uint32_t state);

error_t modbus_slave_init(modbus_slave_t *slave, const modbus_slave_init_t *desc, uint8_t slave_addr);

error_t modbus_slave_set_addr(modbus_slave_t *slave, uint8_t slave_addr);