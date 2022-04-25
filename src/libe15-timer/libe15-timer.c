/**
 * @file libe15-timer.c
 * @author Simakeng (simakeng@outlook.com)
 * @brief Timer Utilities for CMSIS SysTick Register
 * @version 0.1
 * @date 2022-04-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libe15-timer.h"

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} register_prototype_CMSIS_SysTick;

#define CMSIS_SysTick ((register_prototype_CMSIS_SysTick *)0xE000E010)

#define SysTick_CTRL_ENABLE_POS 0U
#define SysTick_CTRL_ENABLE_MASK (1UL << SysTick_CTRL_ENABLE_POS)
#define SysTick_CTRL_ENABLE SysTick_CTRL_ENABLE_MASK

#define SysTick_CTRL_TICKINT_POS 1U
#define SysTick_CTRL_TICKINT_MASK (1UL << SysTick_CTRL_TICKINT_POS)

#define SysTick_CTRL_CLKSOURCE_POS 2U
#define SysTick_CTRL_CLKSOURCE_MASK (1UL << SysTick_CTRL_CLKSOURCE_POS)
#define SysTick_CTRL_CLKSOURCE(val) (val) << SysTick_CTRL_CLKSOURCE_POS

#define SysTick_CTRL_COUNTFLAG_POS 16U
#define SysTick_CTRL_COUNTFLAG_MASK (1UL << SysTick_CTRL_COUNTFLAG_POS)
#define SysTick_CTRL_COUNTFLAG SysTick_CTRL_COUNTFLAG_MASK

#define SYSTICK_RELOAD_CNT (SYSTICK_CFG_SYS_CLK / SYSTICK_CFG_IRQ_FREQ)
#define SYSTICK_RELOAD_VALUE (SYSTICK_RELOAD_CNT - 1)
#define SYSTICK_CNT_MS SYSTICK_RELOAD_VALUE
#define SYSTICK_CNT_US (SYSTICK_RELOAD_VALUE / 1000)

#ifndef NOP
#define NOP() __asm("nop")
#endif

static volatile struct
{
    uint32_t tick_cnt_hi;
    uint32_t tick_cnt;
} systick_status;

void systick_init()
{
    // disable systick
    CMSIS_SysTick->CTRL &= ~SysTick_CTRL_ENABLE;

    // clear perivous settings
    CMSIS_SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_MASK);

    // reset all status
    systick_status.tick_cnt = 0;
    systick_status.tick_cnt_hi = 0;

    // set control register
    CMSIS_SysTick->CTRL |= SysTick_CTRL_CLKSOURCE(1);

    // calculate reload value

    // set reload value
    CMSIS_SysTick->LOAD = SYSTICK_RELOAD_VALUE;

    // clear current value
    CMSIS_SysTick->VAL = 0UL;

    // re-enable systick
    CMSIS_SysTick->CTRL |= SysTick_CTRL_ENABLE;

    return;
}

/**
 * @brief Systick IRQ handler
 */
void systick_timer_isr()
{
    if (++systick_status.tick_cnt == 0)
        systick_status.tick_cnt_hi++;
}

void sys_delay_us(uint32_t us)
{
    // too short, no need for calculate
    if (us < 15)
    {
        while (us--)
        {
            uint32_t t = SYSTICK_CNT_US >> 3;
            NOP();
            NOP();
            while (t--)
            {
                NOP();
                NOP();
            }
        }
        return;
    }

    // retrive current tick count
    volatile uint32_t timer_cnt_old = CMSIS_SysTick->VAL;
    volatile uint32_t tick_cnt_old = systick_status.tick_cnt;
    volatile uint32_t tick_cnt_hi_old = systick_status.tick_cnt_hi;

    // calculate target tick count
    uint32_t timer_cnt = SYSTICK_CNT_US * us;
    uint32_t tick_cnt_us = timer_cnt / SYSTICK_RELOAD_CNT;
    uint32_t timer_cnt_us = timer_cnt % SYSTICK_RELOAD_CNT;

    uint32_t target_timer_cnt = timer_cnt_old - timer_cnt_us;
    uint32_t target_tick_cnt_hi = tick_cnt_hi_old;
    uint32_t target_tick_cnt = tick_cnt_old + tick_cnt_us;

    // if underflow, add to high counter
    if (target_timer_cnt > SYSTICK_RELOAD_CNT)
    {
        target_timer_cnt += SYSTICK_RELOAD_CNT;
        target_tick_cnt++;
    }

    // if overflow, add to high counter
    if (target_tick_cnt < tick_cnt_old)
    {
        target_tick_cnt_hi++;
    }

    while (1)
    {
        NOP();
        NOP();
        NOP();

        if (systick_status.tick_cnt_hi > target_tick_cnt_hi)
        {
            break;
        }
        else if (systick_status.tick_cnt_hi == target_tick_cnt_hi)
        {
            if (systick_status.tick_cnt > target_tick_cnt)
            {
                break;
            }
            else if (systick_status.tick_cnt == target_tick_cnt)
            {
                if (CMSIS_SysTick->VAL < target_timer_cnt)
                {
                    break;
                }
            }
        }
        NOP();
    }
}

void sys_delay_ms(uint32_t ms)
{
    if (ms < 20)
    {
        sys_delay_us(ms * 1000);
        return;
    }

    volatile uint32_t tick_cnt_old = systick_status.tick_cnt;
    volatile uint32_t tick_cnt_hi_old = systick_status.tick_cnt_hi;

    uint32_t target_tick_cnt = tick_cnt_old + ms;
    uint32_t target_tick_cnt_hi = tick_cnt_hi_old;

    if (target_tick_cnt < tick_cnt_old)
    {
        target_tick_cnt_hi++;
    }

    while (1)
    {
        NOP();
        if (systick_status.tick_cnt_hi > target_tick_cnt_hi)
        {
            break;
        }
        else if (systick_status.tick_cnt_hi == target_tick_cnt_hi)
        {
            if (systick_status.tick_cnt > target_tick_cnt)
            {
                break;
            }
        }
        NOP();
    }
}
