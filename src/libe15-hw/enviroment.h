/**
 * @file enviroment.h
 * @author Simakeng (simakeng@outlook.com)
 * @brief Compiler and target specific stuff
 * @version 0.1
 * @date 2023-04-10
 *
 * @copyright Copyright (c) 2023
 *
 */


#include <stdint.h>

/*
 * Arm Compiler 4/5
 */
#if   defined ( __CC_ARM )
    #ifndef   weak
        #define weak                      __attribute__((weak))
    #endif
/*
 * Arm Compiler 6 (armclang)
 */
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #ifndef   weak
        #define weak                      __attribute__((weak))
    #endif
/*
 * GNU Compiler
 */
#elif defined ( __GNUC__ )
    #ifndef   weak
        #define weak                      __attribute__((weak))
    #endif
/*
 * IAR Compiler
 */
#elif defined ( __ICCARM__ )
    #ifndef   weak
        #if __ICCARM_V8
            #define weak __attribute__((weak))
        #else
            #define weak _Pragma("__weak")
        #endif
    #endif
#else /* Unknown compiler */
    #error "Unknown compiler"
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

