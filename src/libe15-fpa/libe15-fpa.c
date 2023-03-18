/**
 * @file libe15-fpa.h
 * @author Simakeng (simakeng@outlook.com)
 * @brief libe15 Fixed Point Arithmetic Library
 * @version 0.1
 * @date 2022-03-28
 *
 */

/**
 @addtogroup libe15-fpa
*/

#include "libe15-fpa.h"
#include <cordic.h>

fixed_t fixed_sqrt(fixed_t a)
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
/**
 * @brief do CORDIC rotation to solve cos() ans sin()
 *
 * @param deg angle in radian
 * @param sin_out output - sin(deg)
 * @param cos_out output - cos(deg)
 */
void cordic_rotate(fixed_t deg, fixed_t *sin_out, fixed_t *cos_out)
{
    // fprintf(stderr, "target angle: 0x%08X : %f\n", deg.val, deg.val / 65536.0);
    fixed_t x = FIXED_ONE;
    fixed_t y = FIXED_ZERO;
    fixed_t current_deg = FIXED_ZERO;
    int i = 0;
    for (i = 0; i < 15; i++)
    {
        // const char *dir;
        fixed_t x_temp, y_temp;
        if (fixed_le(current_deg, deg))
        {
            current_deg = fixed_add(current_deg, cordic_angle[i]);
            x_temp = fixed_sub(x, fixed_move_right(y, i));
            y_temp = fixed_add(y, fixed_move_right(x, i));
            // dir = "F";
        }
        else if (fixed_ge(current_deg, deg))
        {
            current_deg = fixed_sub(current_deg, cordic_angle[i]);
            x_temp = fixed_add(x, fixed_move_right(y, i));
            y_temp = fixed_sub(y, fixed_move_right(x, i));
            // dir = "B";
        }
        else
            goto cordic_output;
        x = x_temp;
        y = y_temp;
        // double len = ((x.val / 65536.) * (x.val / 65536.) + (y.val / 65536.) * (y.val / 65536.)) * (cordic_gain[i].val / 65536.)* (cordic_gain[i].val / 65536.);
        // fprintf(stderr, "step %02d rotate %s:  0x%08X : %f y: %f x: %f len: %f\n", i, dir, current_deg.val, current_deg.val / 65536., y.val / 65536.0, x.val / 65536.0, len);
    }
cordic_output:
    y = fixed_mul(y, cordic_gain[i]);
    x = fixed_mul(x, cordic_gain[i]);
    // fprintf(stderr, "output x: 0x%08X : %f\n", x.val, x.val / 65536.0);
    // fprintf(stderr, "output y: 0x%08X : %f\n", y.val, y.val / 65536.0);
    // fprintf(stderr, "length: %f\n", (x.val / 65536.) * (x.val / 65536.) + (y.val / 65536.) * (y.val / 65536.));
    *sin_out = y;
    *cos_out = x;
}

/**
 * this function will use sine's periodicity to warp the value to
 * 0 <= v < pi / 2, and use CORDIC to calculate the sine value.
 */
fixed_t fixed_sin(fixed_t v)
{
    fixed_value_t val = v.val;
    fixed_value_t sign_bit = val >> (sizeof(fixed_value_t) * 8 - 1);

    fixed_value_t abs_val = (val + sign_bit) ^ sign_bit;

    fixed_value_t warp_val = abs_val % FIXED_2PI.val;
    // fprintf(stderr, "warp to 0x%08X : %f\n", warp_val, warp_val / 65536.0);
    // fprintf(stderr, "sign bit: %d\n",sign_bit);
    if (warp_val > FIXED_PI.val)
    {
        sign_bit = !sign_bit;
        warp_val = warp_val - FIXED_PI.val;
    }

    if (warp_val > FIXED_PI_DIV2.val)
        warp_val = FIXED_PI.val - warp_val;


    // fprintf(stderr, "warp to 0x%08X : %f\n", warp_val, warp_val / 65536.0);
    // fprintf(stderr, "sign bit: %d\n",sign_bit);
    fixed_t sin_v, cos_v;
    cordic_rotate((fixed_t){warp_val}, &sin_v, &cos_v);

    if (sign_bit)
        sin_v.val = -sin_v.val;

    return sin_v;
}