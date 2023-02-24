#include "libe15-pid.h"

typedef struct
{
    fixed_t kp;
    fixed_t ki;
    fixed_t kd;
    fixed_t error;
    fixed_t error_i;
    fixed_t error_d;
    fixed_t output;

    pid_output_state_update_callback_t callback;
} pid_state_t;

void pid_init_controller(fixed_t kp, fixed_t ki, fixed_t kd, pid_state_t *pstate)
{
    pstate->kp = kp;
    pstate->ki = ki;
    pstate->kd = kd;

    pstate->callback = NULL;

    pid_reset_controller(pstate);
}

void pid_reset_controller(pid_state_t *pstate)
{
    pstate->error = fixed_from_int(0);
    pstate->error_i = fixed_from_int(0);
    pstate->error_d = fixed_from_int(0);
    pstate->output = fixed_from_int(0);

    // call callback;
    if (pstate->callback != NULL)
        pstate->callback(pstate, fixed_from_int(0));
}

void pid_update_controller(pid_state_t *pstate, fixed_t error_observation)
{
    // pstate->error_i += error_observation;
    pstate->error_i = fixed_add(pstate->error_i, error_observation);

    // pstate->error_d = error_observation - pstate->error;
    pstate->error_d = fixed_sub(error_observation, pstate->error);

    pstate->error = error_observation;

    fixed_t t = fixed_from_int(0);
    t = fixed_add(fixed_mul(pstate->kp, pstate->error), t);
    t = fixed_add(fixed_mul(pstate->ki, pstate->error_i), t);
    t = fixed_add(fixed_mul(pstate->kd, pstate->error_d), t);

    pstate->output = t;

    // call callback;
    if (pstate->callback != NULL)
        pstate->callback(pstate, t);

    return;
}