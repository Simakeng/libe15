#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <stdlib.h>
#include <time.h>
#include <cheat.h>
#include <math.h>
#include <libe15-fpa.h>

#define TEST_MULTIPLIER 16

CHEAT_TEST(fixed_t_value_validation,

    fixed_t val1 = fixed_from_int(0);
    cheat_assert(val1.val == 0);

    fixed_t val2 = fixed_from_int(751);
    cheat_assert(val2.val == 751 << FIXED_WIDTH);
)

CHEAT_TEST(fixed_t_add,
    srand(time(NULL));
    uint32_t test_cnt = 1024;
    for(uint32_t i = 0; i < test_cnt; i++){
        int32_t r1 = rand() % 1000;
        int32_t r2 = rand() % 1000;
        fixed_t val1 = fixed_from_int(r1);
        fixed_t val2 = fixed_from_int(r2);
        fixed_t val3 = fixed_add(val1, val2); 
        cheat_assert(val3.val == (r1 + r2) << FIXED_WIDTH);
    }
)

#define RAND_TEST_VAL (((rand() % 16) > 8 ? 1 : -1) * (((rand() % 16) << FIXED_WIDTH) | (rand() % ((1 << FIXED_WIDTH) - 1))))

CHEAT_TEST(fixed_t_mul,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        int32_t r1 = RAND_TEST_VAL;
        int32_t r2 = RAND_TEST_VAL;
        int64_t t1 = r1,t2 = r2;
        int64_t t3 = (t1 * t2) >> FIXED_WIDTH; 
        int32_t r3 = (uint32_t)(t3 & 0x00000000FFFFFFFFU);
        fixed_t val1, val2;
        val1.val = r1;
        val2.val = r2;

        fixed_t val3 = fixed_mul(val1, val2);
        
        cheat_assert(val3.val == r3);

        if(val3.val != r3)
        {
            fprintf(stderr,"val1  : 0x%08X\n",r1);
            fprintf(stderr,"val2  : 0x%08X\n",r2);
            fprintf(stderr,"val3_T: 0x%08X\n",r3);
            fprintf(stderr,"val3  : 0x%08X\n",val3.val);
        }
    }
)

CHEAT_TEST(fixed_t_minus,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        int32_t r1 = RAND_TEST_VAL;
        int32_t r2 = RAND_TEST_VAL;
        int64_t t1 = r1,t2 = r2;
        int64_t t3 = t1 - t2; 
        int32_t r3 = (uint32_t)(t3 & 0x00000000FFFFFFFFU);
        fixed_t val1, val2;
        val1.val = r1;
        val2.val = r2;

        fixed_t val3 = fixed_sub(val1, val2);
        
        cheat_assert(val3.val == r3);

        if(val3.val != r3)
        {
            fprintf(stderr,"val1  : 0x%08X\n",r1);
            fprintf(stderr,"val2  : 0x%08X\n",r2);
            fprintf(stderr,"val3_T: 0x%08X\n",r3);
            fprintf(stderr,"val3  : 0x%08X\n",val3.val);
        }
    }

)

CHEAT_TEST(fixed_t_div,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        int32_t r1 = RAND_TEST_VAL;
        int32_t r2 = RAND_TEST_VAL;
        int64_t t1 = r1,t2 = r2;
        int64_t t3 = (t1 << FIXED_WIDTH) / t2; 
        int32_t r3 = (uint32_t)(t3 & 0x00000000FFFFFFFFU);
        fixed_t val1, val2;
        val1.val = r1;
        val2.val = r2;

        fixed_t val3 = fixed_div(val1, val2);
        
        cheat_assert(val3.val == r3);

        if(val3.val != r3)
        {
            fprintf(stderr,"val1  : 0x%08X\n",r1);
            fprintf(stderr,"val2  : 0x%08X\n",r2);
            fprintf(stderr,"val3_T: 0x%08X\n",r3);
            fprintf(stderr,"val3  : 0x%08X\n",val3.val);
        }
    }

)


CHEAT_TEST(fixed_t_min,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        int32_t r1 = RAND_TEST_VAL;
        int32_t r2 = RAND_TEST_VAL;
        int64_t t1 = r1,t2 = r2;
        int64_t t3 = r1 > r2 ? r2 : r1; 
        int32_t r3 = (uint32_t)(t3 & 0x00000000FFFFFFFFU);
        fixed_t val1, val2;
        val1.val = r1;
        val2.val = r2;

        fixed_t val3 = fixed_min(val1, val2);
        
        cheat_assert(val3.val == r3);

        if(val3.val != r3)
        {
            fprintf(stderr,"val1  : 0x%08X\n",r1);
            fprintf(stderr,"val2  : 0x%08X\n",r2);
            fprintf(stderr,"val3_T: 0x%08X\n",r3);
            fprintf(stderr,"val3  : 0x%08X\n",val3.val);
        }
    }

)

CHEAT_TEST(fixed_t_max,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        int32_t r1 = RAND_TEST_VAL;
        int32_t r2 = RAND_TEST_VAL;
        int64_t t1 = r1,t2 = r2;
        int64_t t3 = r1 < r2 ? r2 : r1; 
        int32_t r3 = (uint32_t)(t3 & 0x00000000FFFFFFFFU);
        fixed_t val1, val2;
        val1.val = r1;
        val2.val = r2;

        fixed_t val3 = fixed_max(val1, val2);
        
        cheat_assert(val3.val == r3);

        if(val3.val != r3)
        {
            fprintf(stderr,"val1  : 0x%08X\n",r1);
            fprintf(stderr,"val2  : 0x%08X\n",r2);
            fprintf(stderr,"val3_T: 0x%08X\n",r3);
            fprintf(stderr,"val3  : 0x%08X\n",val3.val);
        }
    }

)


CHEAT_TEST(fixed_t_abs,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        int32_t r1 = RAND_TEST_VAL;

        int64_t t3 = r1 < 0 ? (0-r1) : (r1); 

        int32_t r3 = (uint32_t)(t3 & 0x00000000FFFFFFFFU);
        fixed_t val1;
        val1.val = r1;

        fixed_t val3 = fixed_abs(val1);
        
        cheat_assert(val3.val == r3);

        if(val3.val != r3)
        {
            fprintf(stderr,"val1  : 0x%08X\n",r1);
            fprintf(stderr,"val3_T: 0x%08X\n",r3);
            fprintf(stderr,"val3  : 0x%08X\n",val3.val);
        }
    }

)

CHEAT_TEST(fixed_from_float_conv,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        float f1 = 1.0 * RAND_TEST_VAL / (1 << FIXED_WIDTH);
        float f2 = 1.0 * RAND_TEST_VAL / (1 << FIXED_WIDTH);

        float f3 = f1 * f2;

        fixed_t r1 = fixed_from_float(f1);
        fixed_t r2 = fixed_from_float(f2);

        fixed_t r3 = fixed_mul(r1,r2);


        cheat_assert(fabs(1.0 * r3.val / (1 << FIXED_WIDTH) - f3 ) < 0.01f);
        if(fabs(1.0 * r3.val / (1 << FIXED_WIDTH) - f3 ) > 0.01f)
        {
            fprintf(stderr,"val1  : 0x%08X\n",r1.val);
            fprintf(stderr,"val2  : 0x%08X\n",r2.val);
            fprintf(stderr,"val3  : 0x%08X\n",r3.val);

            fprintf(stderr,"f1  : %f\n",f1);
            fprintf(stderr,"f2  : %f\n",f2);
            fprintf(stderr,"f3  : %f\n",f3);

            fprintf(stderr,"dif : %f\n",fabs(1.0 * r3.val / (1 << FIXED_WIDTH) - f3 ));
        }
    }

)