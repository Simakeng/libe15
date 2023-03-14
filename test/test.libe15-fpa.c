#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#define _CRT_SECURE_NO_WARNINGS

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


CHEAT_TEST(fixed_from_int_conv,
    srand(time(NULL));
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){

        int32_t test_val = (rand() % ((1 << FIXED_WIDTH) - 1));
        if (rand() % 2)
            test_val *= -1;
        fixed_t val = fixed_from_int(test_val);

        cheat_assert(val.val == test_val << FIXED_WIDTH);
    }

)

CHEAT_TEST(fixed_atof_cont,
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        char as[256] = { 0 };
        int32_t interger = 25 - (rand() % 50);
        int32_t frac_1 = rand() % 1000;
        int32_t frac_2 = rand() % 1000;
        int32_t frac_3 = rand() % 1000;

        sprintf(as, "%d.%03d%03d%03d", interger, frac_1,frac_2,frac_3);

        fixed_t fv = fixed_atof(as);

        float rfv = strtod(as, NULL);

        fixed_t frfv = fixed_from_float(rfv);

        cheat_assert_not(abs(fv.val - frfv.val) > 1);

        if(abs(fv.val - frfv.val) > 1)
        {
            fprintf(stderr,"str : %s\n",as);
            fprintf(stderr,"rfv : %f\n",rfv);
            fprintf(stderr,"fv  : 0x%08X conv:%.07f\n",fv.val, fv.val / 65536.0);
            fprintf(stderr,"frfv : 0x%08X conv:%.07f\n",frfv.val, frfv.val / 65536.0);
        }

    }
    

)

CHEAT_TEST(fixed_ftoa_cont,
    uint32_t test_cnt = 1 << TEST_MULTIPLIER;
    for(uint32_t i = 0; i < test_cnt; i++){
        char as[256] = { 0 };

        fixed_t fv = (fixed_t){RAND_TEST_VAL};
        fixed_ftoa(fv, as);

        float ffv = fv.val / 65536.0;

        char fas[256] = { 0 };

        sprintf(fas,"%.06f",ffv);

        int a = strlen(as);
        int b = strlen(fas);
        as[a-1] = 0;
        fas[b-1] = 0;

        cheat_assert_not(strcmp(fas,as) != 0);
        if(strcmp(fas,as) != 0)
        {
            fprintf(stderr,"fv  : 0x%08X\n",fv.val);
            fprintf(stderr,"sfv : %s\n",as);
            fprintf(stderr,"ffv : %s\n",fas);
        }
    }

)