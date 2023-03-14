#pragma once
/**
 * @file libe15-fpa.h
 * @author Simakeng (simakeng@outlook.com)
 * @brief libe15 Fixed Point Arithmetic Library
 * @version 0.1
 * @date 2022-03-28
 *
 */

/**
 @defgroup libe15-fpa libe15-fpa
    see libe15-fpa.h
*/
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

/******************************************
 *                Configs                 *
 ******************************************/

#define FIEXD_VALUE_WIDTH 32

/**
 * fixed point value type, should be the same as cpu's int type
 */
#define fixed_value_t int32_t
#define fixed_uvalue_t uint32_t
/**
 * fixed point position change this as you need
 */
#define FIXED_WIDTH 16

/// fixed point value type
typedef struct __tag_fixed_t
{
    fixed_value_t val;
} fixed_t;

/// 0
#define FPA_ZERO ((fixed_t){(fixed_value_t)0})

/// π
#define FPA_PI ((fixed_t){(fixed_value_t)(3.1415926535897932384626433832795l * (1 << FIXED_WIDTH))})

/// INF VALUE(MAX) = 32767.99998
#define FIXED_MAX_INF ((fixed_t){(fixed_value_t)0x7FFFFFFF})

/// -INF VALUE(MIN) = 32767.99998
#define FIXED_MIN_NINF ((fixed_t){(fixed_value_t)0x80000000})

/// epsilon = 0.000015
#define FIXED_EPS ((fixed_t){(fixed_value_t)0x00000001})
/**
 * @brief add two fixed point value
 *
 * @param a operand a
 * @param b operand b
 * @return fixed_t, a + b
 */
static inline fixed_t fixed_add(fixed_t a, fixed_t b)
{
    return (fixed_t){a.val + b.val};
}
/**
 * @brief sub two fixed point value
 *
 * @param a operand a
 * @param b operand b
 * @return fixed_t a - b
 */
static inline fixed_t fixed_sub(fixed_t a, fixed_t b)
{
    return (fixed_t){a.val - b.val};
}
/**
 * @brief multiply two fixed point value
 *
 * @param a operand a
 * @param b operand b
 * @return fixed_t a * b
 */
static inline fixed_t fixed_mul(fixed_t a, fixed_t b)
{
    fixed_value_t a_hi, a_lo, b_hi, b_lo;
    a_hi = a.val >> FIXED_WIDTH;
    a_lo = a.val & ((1 << FIXED_WIDTH) - 1);
    b_hi = b.val >> FIXED_WIDTH;
    b_lo = b.val & ((1 << FIXED_WIDTH) - 1);
    fixed_uvalue_t res_hi = a_hi * b_hi;
    fixed_uvalue_t res_lo = a_lo * b_lo;
    fixed_uvalue_t res_mid = a_hi * b_lo + a_lo * b_hi;
    res_hi <<= FIXED_WIDTH;
    res_lo >>= FIXED_WIDTH;
    return (fixed_t){res_hi + res_lo + res_mid};
}

/**
 * @brief divide two fixed point value
 *
 * @param a operand a
 * @param b operand b
 * @return fixed_t a / b
 */
static inline fixed_t fixed_div(fixed_t a, fixed_t b)
{
    int64_t la = a.val;
    la = la << FIXED_WIDTH;
    return (fixed_t){la / b.val};
}

/**
 * @brief get absolute value of a fixed point value
 *
 * @param a operand a
 * @return fixed_t | a |
 */
static inline fixed_t fixed_abs(fixed_t a)
{

    fixed_value_t sign_bit = a.val >> (sizeof(fixed_value_t) * 8 - 1);
    return (fixed_t){(a.val + sign_bit) ^ sign_bit};
}

/**
 * @brief get the larger one's value of two fixed point value
 *
 * @param a operand a
 * @param b operand b
 * @return fixed_t max{a,b}
 */
static inline fixed_t fixed_max(fixed_t a, fixed_t b)
{
    fixed_value_t diff = b.val - a.val;
    return (fixed_t){
        b.val - (diff & (diff >> (sizeof(fixed_value_t) * 8 - 1)))};
}

/**
 * @brief get the smaller one's value of two fixed point value
 *
 * @param a operand a
 * @param b operand b
 * @return fixed_t min{a,b}
 */
static inline fixed_t fixed_min(fixed_t a, fixed_t b)
{
    fixed_value_t diff = b.val - a.val;
    return (fixed_t){
        a.val + (diff & (diff >> (sizeof(fixed_value_t) * 8 - 1)))};
}

/**
 * @brief convert fixed point value to float
 * @param a value to convert
 * @return float
 */
static inline float fixed_to_float(fixed_t a)
{
    return 0.0f;
}
/**
 * @brief convert float to fixed point value
 * @param val value to convert
 * @return fixed_t
 */
static inline fixed_t fixed_from_float(const float val)
{
    fixed_uvalue_t fref = *(const fixed_uvalue_t *)&val;

    fixed_uvalue_t sign_bit = fref & (((fixed_uvalue_t)1) << (FIEXD_VALUE_WIDTH - 1));

    fixed_uvalue_t exp_value = fref & ((fixed_uvalue_t)0xFF) << (FIEXD_VALUE_WIDTH - 9);

    fixed_value_t fixed_val = fref ^ (sign_bit | exp_value);
    fixed_val |= (((fixed_uvalue_t)1) << (FIEXD_VALUE_WIDTH - 9));

    sign_bit >>= (FIEXD_VALUE_WIDTH - 1);

    exp_value >>= (FIEXD_VALUE_WIDTH - 9);
    exp_value -= 127;

    int32_t move_off = 23 - exp_value - 16;

    // is negative float
    if (sign_bit)
        fixed_val = -fixed_val;

    if (move_off > 0)
        // move to left
        fixed_val >>= move_off;
    else
    {
        // move to right
        move_off = -move_off;
        if (move_off >= 7)
            fixed_val = 0x7FFFFFFF;
        else
            fixed_val <<= move_off;
    }

    return (fixed_t){fixed_val};
}

/**
 * @brief convert fixed point value to int
 * @param val value to convert
 * @return int
 */
static inline fixed_t fixed_from_int(int32_t val)
{
    return (fixed_t){val << FIXED_WIDTH};
}

/**
 * @brief parse string to get fixed point value
 * @param str string contains data
 * @return fixed_t
 */
static inline fixed_t fixed_atof(const char *str)
{
    int32_t i_val = 0;
    int32_t sign = 0;
    uint32_t frac_val = 0;
    uint32_t frac_depth = 0;
    int32_t fixed_value;

    while (isspace(*str))
        *str++;

    if (*str == '-')
    {
        sign = -1;
        str++;
    }

    if (*str == '+')
        str++;

    while (isdigit(*str))
    {
        i_val *= 10;
        i_val += *str - '0';
        if (i_val > 0x7FFFu)
            if (sign == -1)
                return FIXED_MIN_NINF;
            else
                return FIXED_MAX_INF;
        str++;
    }

    if (*str == '.')
    {
        str++;

        while (isdigit(*str))
        {
            frac_val *= 10;
            frac_val += *str - '0';
            frac_depth++;

            if (frac_depth > 6)
                break;

            str++;
        }
    }

    fixed_value = i_val << FIXED_WIDTH;

    uint32_t move_left = FIXED_WIDTH;
    while (frac_depth || move_left)
    {
        if (frac_depth != 0)
        {
            if (move_left >= 4)
            {
                frac_val <<= 3;
                move_left -= 4;
                frac_val /= 5;
                frac_depth--;
            }
            else if (move_left != 0)
            {
                frac_val <<= move_left;
                move_left = 0;
            }
            else
            {
                frac_val /= 10;
                frac_depth--;
            }
        }
        else if (move_left != 0)
        {
            frac_val <<= move_left;
            move_left = 0;
        }
    }

    fixed_value += frac_val;
    if (sign)
        return (fixed_t){-fixed_value};
    else
        return (fixed_t){fixed_value};
}

/**
 * @brief convert fixed point value to 10-based string into buffer
 * @param val fixed point value
 * @param str buffer to store string
 * @return int 0 if success, -1 if failed
 */
static inline int fixed_ftoa(fixed_t val, char *str)
{
    return 0;
}

/**
 * @brief get the square root of a fixed point value
 *
 * @param a operand a
 * @return fixed_t sqrt(a)
 */
static inline fixed_t fixed_sqrt(fixed_t a)
{
    int i = 0;
    // 0.5
    const fixed_t div_2 = (fixed_t){1 << (FIXED_WIDTH - 1)};
    fixed_t t = (fixed_t){a.val >> 1};
    for (i = 0; i < 5; i++)
    {
        t = fixed_add(t, fixed_div(a, t));
        t = fixed_mul(t, div_2);
    }
    return t;
}
