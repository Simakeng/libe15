# lib15

![ci status](https://github.com/Simakeng/libe15/actions/workflows/c-cpp.yml/badge.svg)

C/C++ Code library mainly designed to implement useful functionalities in code-size sensitive embedded systems such as STM32.

list:

 - `libe15_fpa` - Fixed Point Arithmetic Library
 - `libe15-timer` - Timer and Delay utility implemented via CMSIS SysTick Timer


## Fixed Point Arithmetic Library
`libe15_fpa` is a library for fixed point arithmetic. some embedded systems does not contain FPU, so it is handy to have a library to complement this.

## FOC Control
`libe15_foc` is a library of functions for implmenting Field Oriented Control.

## Timer and Delay
`libe15-timer` is a set of functions about timer and delay which has better precision than STM32CUBEMX Generated code.

in this lib, `sys_delay_ms`, `sys_delay_us` are implemented by reading and configuring `SysTick` register. config setting is done in `libe15-timer.h:14` and `libe15-timer.h:17`.if same setting is applied as STM32CUBEMX Generated code, function like `HAL_Delay` can still be used.

to use this library:
- include `libe15-timer.h` in your code.
- call `systick_init()` to initialize the timer.
- call `systick_timer_isr()` in `SysTick_Handler()` to update the timer.
