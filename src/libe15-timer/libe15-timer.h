/**
 * @file libe15-timer.h
 * @author Simakeng (simakeng@outlook.com)
 * @brief Timer Utilities for CMSIS SysTick Register
 * @version 0.1
 * @date 2022-04-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdint.h>

// CONFIG: System Clock Frequency
#define SYSTICK_CFG_SYS_CLK 72000000

// CONFIG: SysTick IRQ Frequency
#define SYSTICK_CFG_IRQ_FREQ 1000

/**
 * @brief Initialize SysTick Timer
 */
void systick_init(void);

/**
 * @brief Delay for a number of microseconds
 * 
 * @param us 
 */
void sys_delay_us(uint32_t us);

/**
 * @brief Delay for a number of milliseconds
 * 
 * @param ms 
 */
void sys_delay_ms(uint32_t ms);

void systick_timer_isr(void);

