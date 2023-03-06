#pragma once
/**
 * @file libe15-pid.h
 * @author Simakeng (simakeng@outlook.com)
 * @brief libe15 PID Control lib
 * @version 0.1
 * @date 2023-02-24
 *
 */

#include <libe15-fpa.h>

struct __tag_pid_state_t;

typedef void (*pid_update_callback_t)(struct __tag_pid_state_t *, fixed_t);

typedef struct __tag_pid_state_t
{
    fixed_t kp;
    fixed_t ki;
    fixed_t kd;
    fixed_t error;
    fixed_t error_i;
    fixed_t error_d;
    fixed_t output;

    pid_update_callback_t callback;
} pid_state_t;




/**
 * @brief Initialize a PID controler with following
 *        paramaters, with internal state set to 0.
 *
 * @param kp p-value of PID
 * @param ki i-value of PID
 * @param kd d-value of PID
 * @param pstate handler of new instanced pid controller;
 */
void pid_init_controller(fixed_t kp, fixed_t ki, fixed_t kd, pid_state_t *pstate);

/**
 * @brief Get the output value from PID Controller.
 * @param pstate handler of PID controller;
 * @return fixed_t the output value of ID controller;
 */
fixed_t pid_get_value(pid_state_t *pstate);

/**
 * @brief get the p-value of controller
 * @param pstate handler of PID controller;
 * @return fixed_t
 */
fixed_t pid_get_kp(pid_state_t *pstate);

/**
 * @brief get the i-value of controller
 * @param pstate handler of PID controller;
 * @return fixed_t
 */
fixed_t pid_get_ki(pid_state_t *pstate);

/**
 * @brief get the d-value of controller
 * @param pstate handler of PID controller;
 * @return fixed_t
 */
fixed_t pid_get_kd(pid_state_t *pstate);

/**
 * @brief set a new value to the p-value of controller
 *
 * @param pstate handler of PID controller;
 * @param new_value new p-value
 */
void pid_set_kp(pid_state_t *pstate, fixed_t new_value);

/**
 * @brief set a new value to the i-value of controller
 *
 * @param pstate handler of PID controller;
 * @param new_value new i-value
 */
void pid_set_ki(pid_state_t *pstate, fixed_t new_value);

/**
 * @brief set a new value to the d-value of controller
 *
 * @param pstate handler of PID controller;
 * @param new_value new d-value
 */
void pid_set_kd(pid_state_t *pstate, fixed_t new_value);

/**
 * @brief reset a controller to default zero state.
 *        this function will generate a function call to
 *        previous registed callback function.
 * @param pstate handler of PID controller;
 */
void pid_reset_controller(pid_state_t *pstate);

/**
 * @brief register a callback to the controller, which
 *        will be called when controller get updated.
 *        new callback will replace old callback.
 *
 * @param pstate handler of PID controller;
 * @param callback new callback
 */
void pid_set_controller_callback(pid_state_t *pstate,
                                 pid_update_callback_t callback);

/**
 * @brief remove callbask from a controller.
 * 
 * @param pstate handler of PID controller;
 */
void pid_clear_controller_callback(pid_state_t *pstate);

/**
 * @brief using given error observation value to generate new
 *        internal state and output, this function will generate
 *        a function call to previous registed callback function.
 * 
 * @param pstate handler of PID controller;
 * @param error_observation error observation value
 */
void pid_update_controller(pid_state_t *pstate, fixed_t error_observation);
