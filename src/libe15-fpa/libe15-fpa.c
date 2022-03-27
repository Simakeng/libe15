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

#include <libe15-fpa.h>

typedef struct __tag_fixed_t{
    int32_t val;
} fiexd_t;

inline void fixed_add(fiexd_t* a,fixed_t* b,fixed_t* out)
{
    out->val = a->val + b->val;
}

inline void fixed_sub(fiexd_t* a,fixed_t* b,fixed_t* out)
{
    out->val = a->val - b->val;
}

inline void fixed_mul(fiexd_t* a,fixed_t* b,fixed_t* out)
{
    int32_t temp1,temp2;
    temp1 = a->val >> 8;
    temp2 = b->val >> 8;
    out->val = temp1 * temp2;
}

inline void fixed_div(fiexd_t* a,fixed_t* b,fixed_t* out)
{
    out->val = a->val / b->val;
}

inline float fixed_to_float(fixed_t* a);

inline void fixed_from_float(float val, fiexd_t* out);

inline void fixed_from_int(int32_t val, fixed_t* out)
{
    out->val = val << FIXED_WIDTH;
}