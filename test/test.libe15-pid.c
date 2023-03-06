#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <libe15-pid.h>


CHEAT_TEST(pid_value_init_test,
    pid_state_t pid_state;

    fixed_t kp = fixed_from_float(0.5f);
    fixed_t ki = fixed_from_float(0.5f);
    fixed_t kd = fixed_from_float(0.5f);

    pid_init_controller(kp, ki, kd, &pid_state);

    cheat_assert(pid_state.kp.val == kp.val);
)