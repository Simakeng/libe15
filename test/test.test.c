#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

CHEAT_TEST(multi_file,
    cheat_assert(__BASE_FILE__ == __FILE__);
)