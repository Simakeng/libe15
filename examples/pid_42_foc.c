/**
 * @file pid_42_foc.c
 * @author simakeng (simakeng@outlook.com)
 * @brief this is a example of using libe15-pid to control a 4-wire 2-pole brushless motor.
 * @version 0.1
 * @date 2023-03-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <libe15-fpa.h>
#include <libe15-pid.h>

#define deadloop 1

/** External function examples begin ************/
/**
 * the functions below are examples, you should implement them by
 * yourself. In real application, you should use a timer to
 * generate a interrupt or use RTOS to generate accurate interval
 * for system update.
 */

/**
 * @brief delay for system update interval
 */
void foc_delay_for_update_interval(void);

/**
 * @brief get the current of pole a
 */
fixed_t foc_adc_get_current_pole_a();

/**
 * @brief get the current of pole b
 */
fixed_t foc_adc_get_current_pole_b();

/**
 * @brief set the duty of pole a
 */
void foc_pwm_set_duty_a(fixed_t duty);

/**
 * @brief set the duty of pole b
 */
void foc_pwm_set_duty_b(fixed_t duty);

/**
 * @brief get the current position of motor
 */
int32_t foc_get_current_position(void);

/**
 * @brief get the expected position of motor
 */
int32_t foc_get_expect_position(void);

/** External function examples end ************/

// define the PID controller state
pid_state_t postition_loop = {0};

pid_state_t current_iq_loop = {0};
pid_state_t current_id_loop = {0};

/** this part is a core part of FOC algorithm, but it is really motor sepicific.
 * so we just give a example here.
 * initial angle, pole pair count etc. should be set by you.
 */

// define the motor paramaters
#define MOTOR_POLAR_COUNT 50      //* magnetic field rotate 50 times for every 1 round of motor shaft rotate.
#define MOTOR_STEP_PER_ROUND 4096 //* step per round of motor, depends on encoder resolution
#define MOTOR_INITIAL_ANGLE 0     //* initial angle of motor, in step

void foc_do_Park_transform(int32_t position, fixed_t I_a, fixed_t I_b, fixed_t *I_d_out, fixed_t *I_q_out)
{
    // calculate the angle of motor shaft
    int32_t motor_pos = position - MOTOR_INITIAL_ANGLE;
    motor_pos %= MOTOR_STEP_PER_ROUND;

    // convert to fixed_t and convert to radian
    fixed_t motor_angle = fixed_from_int(motor_pos);

    // convert to electrical angle
    // * proceducaly, convert to radian first, then convert to electrical angle
    // * but radian conversion involves a division, for precision reason,
    // * we first multiply the pole pair count.
    motor_angle = fixed_mul(motor_angle, fixed_from_int(MOTOR_POLAR_COUNT));

    // * then convert to radian
    motor_angle = fixed_mul(motor_angle, FIXED_2PI);
    motor_angle = fixed_div(motor_angle, fixed_from_int(MOTOR_STEP_PER_ROUND));

    // reverse the angle
    motor_angle.val = -motor_angle.val;

    fixed_t cos_angle = fixed_cos(motor_angle);
    fixed_t sin_angle = fixed_sin(motor_angle);

    // mat rotation
    *I_d_out = fixed_sub(fixed_mul(I_a, cos_angle), fixed_mul(I_b, sin_angle));
    *I_q_out = fixed_add(fixed_mul(I_a, sin_angle), fixed_mul(I_b, cos_angle));
}

void foc_do_reverse_Park_transform(int32_t position, fixed_t I_d, fixed_t I_q, fixed_t *I_a_out, fixed_t *I_b_out)
{
    // calculate the angle of motor shaft
    int32_t motor_pos = position - MOTOR_INITIAL_ANGLE;
    motor_pos %= MOTOR_STEP_PER_ROUND;

    // convert to fixed_t and convert to radian
    fixed_t motor_angle = fixed_from_int(motor_pos);

    // convert to electrical angle
    // * proceducaly, convert to radian first, then convert to electrical angle
    // * but radian conversion involves a division, for precision reason,
    // * we first multiply the pole pair count.
    motor_angle = fixed_mul(motor_angle, fixed_from_int(MOTOR_POLAR_COUNT));

    // * then convert to radian
    motor_angle = fixed_mul(motor_angle, FIXED_2PI);
    motor_angle = fixed_div(motor_angle, fixed_from_int(MOTOR_STEP_PER_ROUND));

    fixed_t cos_angle = fixed_cos(motor_angle);
    fixed_t sin_angle = fixed_sin(motor_angle);

    // mat rotation
    *I_a_out = fixed_sub(fixed_mul(I_d, cos_angle), fixed_mul(I_q, sin_angle));
    *I_b_out = fixed_add(fixed_mul(I_d, sin_angle), fixed_mul(I_q, cos_angle));
}
int main()
{

    while (deadloop)
    {
        foc_delay_for_update_interval();

        int32_t position = foc_get_current_position();

        fixed_t current_a = foc_adc_get_current_pole_a();
        fixed_t current_b = foc_adc_get_current_pole_b();

        fixed_t Iq, Id;

        foc_do_Park_transform(position, current_a, current_b, &Id, &Iq);

        int32_t position_expect = foc_get_expect_position();
        fixed_t position_error = fixed_from_int(position_expect - position);

        pid_update_controller(&current_id_loop, position_error);

        fixed_t current_id_expect = pid_get_value(&current_id_loop);
        fixed_t current_iq_expect = FIXED_ZERO;

        fixed_t current_id_error = fixed_sub(current_id_expect, Id);
        fixed_t currend_iq_error = fixed_sub(current_iq_expect, Iq);

        pid_update_controller(&current_id_loop, current_id_error);
        pid_update_controller(&current_iq_loop, currend_iq_error);

        fixed_t duty_a, duty_b;

        foc_do_reverse_Park_transform(position, pid_get_value(&current_id_loop), pid_get_value(&current_iq_loop), &duty_a, &duty_b);

        foc_pwm_set_duty_a(duty_a);

        foc_pwm_set_duty_b(duty_b);
    }
}