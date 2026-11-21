#include "tb6612.h"
// #include "tim.h"

static float fan_duty_now = 0.0f; //记录当前fan占空比
uint8_t fan_is_running = 0;


static float tb6612_clamp(float now_duty, float min_duty, float max_duty)
{
    if(now_duty < min_duty) return min_duty;
    if(now_duty > max_duty) return max_duty;
    return now_duty;
}

static uint8_t tb6612_write_pwm(float duty_cycle)
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&TB6612_TIM);
    if(arr < 5)
    {
        LOG_DEBUG("tb6612_write_pwm: arr too small, pwm not configured properly");
        return 1;
    }
    uint32_t ccr = (uint32_t)(duty_cycle * arr);
    __HAL_TIM_SET_COMPARE(&TB6612_TIM, TB6614_TIM_CHANNEL, ccr);

    if(__HAL_TIM_GET_COMPARE(&TB6612_TIM, TB6614_TIM_CHANNEL) != ccr)
    {
        LOG_DEBUG("tb6612_write_pwm: write pwm value failed");
        return 1;
    }

    return 0;
}

static uint8_t tb6612_set_stby(GPIO_PinState state)
{
    HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, state);
    if(HAL_GPIO_ReadPin(STBY_GPIO_Port, STBY_Pin) != state)
    {
        LOG_DEBUG("tb6612_set_stby: write pin failed");
        return 1;
    }
    return 0;
}

static void tb6612_ramp_startup(float duty_target)
{
    uint32_t steps = TB6614_RAMP_MS / 10;
    if(steps < 1) steps = 1;

    float start = fan_duty_now;
    float end   = duty_target;

    float step  = (end - start) / steps;

    for(uint32_t i = 0; i < steps; i++)
    {
        start += step;

        if((step > 0 && start > end) || (step < 0 && start < end))
        {
            start = end;
        }

        tb6612_write_pwm(start);
        HAL_Delay(10);
    }

    tb6612_write_pwm(duty_target);
    fan_duty_now = duty_target;
}

void tb6612_init(void)
{
    if(&TB6612_TIM == NULL)
    {
        LOG_DEBUG("tb6612_init: htim is null");
        return;
    }

    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);

    //设置tb6612 待机
    uint8_t res = tb6612_set_stby(GPIO_PIN_RESET);
    if(res != 0)
    {
        LOG_DEBUG("tb6612_init: set stby failed");
        return;
    }

    //设置pwm
    fan_duty_now = 0.0f;
    tb6612_write_pwm(0.0f);
    HAL_TIM_PWM_Start(&TB6612_TIM, TB6614_TIM_CHANNEL);
}

void tb6612_start(float duty_target)
{
    duty_target = tb6612_clamp(duty_target, LOW_SPEED, HIGH_SPEED);

    uint8_t res = tb6612_set_stby(GPIO_PIN_SET);
    if(res != 0)
    {
        LOG_DEBUG("tb6612_start: set stby failed");
        return;
    }
    tb6612_ramp_startup(duty_target);
}

void tb6612_stop(void)
{
    tb6612_ramp_startup(0.0f);
    tb6612_set_stby(GPIO_PIN_RESET);
}

void tb6612_set_pwm(float duty_cycle)
{
    duty_cycle = tb6612_clamp(duty_cycle, LOW_SPEED, HIGH_SPEED);
    tb6612_write_pwm(duty_cycle);
}

float tb6612_get_duty(uint8_t num)
{
    if(num == 1) return LOW_SPEED;
    if(num == 2) return MEDIUM_SPEED;
    if(num == 3) return HIGH_SPEED;

    return LOW_SPEED;
}