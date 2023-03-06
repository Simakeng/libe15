#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <stdlib.h>
#include <time.h>
#include <cheat.h>
#include <libe15-fpa.h>

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

#define RAND_TEST_VAL (((rand() % 16) << FIXED_WIDTH) | (rand() % ((1 << FIXED_WIDTH) - 1)))

CHEAT_TEST(fixed_t_mul,
    srand(time(NULL));
    uint32_t test_cnt = 1;
    for(uint32_t i = 0; i < test_cnt; i++){
        // int32_t r1 = RAND_TEST_VAL;
        // int32_t r2 = RAND_TEST_VAL;
        int32_t r1 = 0x000DC861;
        int32_t r2 = 0x0006AB11;
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