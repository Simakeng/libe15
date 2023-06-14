/**
 * @file libe15-devop.h
 * @author simakeng (simakeng@outlook.com)
 * @brief hardware related helper functions or macros
 * @version 0.1
 * @date 2023-06-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <libe15-errors.h>

#ifndef __LIBE15_DEVOP_H__
#define __LIBE15_DEVOP_H__

#ifndef CALL_NULLABLE
/**
 * @brief call a function and check if it returns an error.
 * the function can be null, in which case it will be ignored.
 */
#define CALL_NULLABLE(func, ...)   \
    do                             \
    {                              \
        if ((func) == ((void *)0)) \
            break;                 \
        else                       \
            (func)(__VA_ARGS__);   \
    } while (0u)
#endif // ! CALL_NULLABLE

#ifndef CALL_WITH_ERROR
/**
 * @brief call a function and check if it returns an error.
 * the function **must not** be null, but we don't check it.
 * if the function returns an error, the error will be logged and returned.
 */
#define CALL_WITH_ERROR(func, ...)                                   \
    do                                                               \
    {                                                                \
        error_t err = (func)(__VA_ARGS__);                           \
        if (FAILED(err))                                             \
        {                                                            \
            error("'" #func "' failed with error code 0x%08X", err); \
            return err;                                              \
        }                                                            \
    } while (0u)
#endif // ! CALL_WITH_ERROR

#ifndef CALL_NULLABLE_WITH_ERROR
/**
 * @brief call a function and check if it returns an error.
 * the function can be null, in which case it will be ignored.
 * if the function returns an error, the error will be logged and returned.
 */
#define CALL_NULLABLE_WITH_ERROR(func, ...)                              \
    do                                                                   \
    {                                                                    \
        if ((func) == ((void *)0))                                       \
            break;                                                       \
        else                                                             \
        {                                                                \
            error_t err = (func)(__VA_ARGS__);                           \
            if (FAILED(err))                                             \
            {                                                            \
                error("'" #func "' failed with error code 0x%08X", err); \
                return err;                                              \
            }                                                            \
        }                                                                \
    } while (0u)
#endif // ! CALL_NULLABLE_WITH_ERROR

#endif // ! __LIBE15_DEVOP_H__