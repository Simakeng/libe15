#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <libe15-fpa.h>

CHEAT_TEST(fixed_t_value_validation,

    fixed_t val1 = fixed_from_int(0);
    cheat_assert(val1.val == 0);

    fixed_t val2 = fixed_from_int(751);
    cheat_assert(val2.val == 751 << FIXED_WIDTH);
)