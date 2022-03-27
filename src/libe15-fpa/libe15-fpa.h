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


#include <stdint.h>
/******************************************
 *                Configs                 *
 ******************************************/

/**
 * fixed point value type, should be the same as cpu's int type
 */
#define fixed_value_t int32_t

/**
 * fixed point position change this as you need
 */
#define FIXED_WIDTH 16

/// 0
#define FPA_ZERO ((fixed_value_t)0)

/// π
#define FPA_PI ((fixed_value_t)(3.14159265358979d * (1 << FIXED_WIDTH)))

struct __tag_fixed_t;

/// fixed point value type
typedef struct __tag_fixed_t fixed_t;

/**
 * @brief add two fixed point value
 * 
 * @param a operand a
 * @param b operand b
 * @return fixed_t, a + b
 */
inline fixed_t fixed_add(fixed_t a,fixed_t b);

/**
 * @brief sub two fixed point value
 * 
 * @param a operand a
 * @param b operand b
 * @return fixed_t a - b
 */
inline fixed_t fixed_sub(fixed_t a,fixed_t b);

/**
 * @brief multiply two fixed point value
 * 
 * @param a operand a
 * @param b operand b
 * @return fixed_t a * b
 */
inline fixed_t fixed_mul(fixed_t a,fixed_t b);

/**
 * @brief divide two fixed point value
 * 
 * @param a operand a
 * @param b operand b
 * @return fixed_t a / b
 */
inline fixed_t fixed_div(fixed_t a,fixed_t b);

/**
 * @brief convert fixed point value to float
 * @param a value to convert
 * @return float 
 */
inline float fixed_to_float(fixed_t a);

/**
 * @brief convert float to fixed point value
 * @param val value to convert
 * @return fixed_t 
 */
inline fixed_t fixed_from_float(float val);

/**
 * @brief convert fixed point value to int
 * @param val value to convert
 * @return int 
 */
inline fixed_t fixed_from_int(int32_t val);

/**
 * @brief parse string to get fixed point value
 * @param str string contains data
 * @return fixed_t 
 */
inline fixed_t fixed_atof(const char *str);

/**
 * @brief convert fixed point value to 10-based string into buffer
 * @param val fixed point value
 * @param str buffer to store string
 * @return int 0 if success, -1 if failed
 */
inline int fixed_ftoa(fixed_t val,char* str);