#pragma once

#include "tim.h"
#include "gpio.h"
#include "log.h"

#define LOW_SPEED    0.3
#define MEDIUM_SPEED 0.5
#define HIGH_SPEED   0.8

#define TB6612_TIM          htim4
#define TB6614_TIM_CHANNEL  TIM_CHANNEL_4
#define TB6614_RAMP_MS      300

extern uint8_t fan_is_running;

void tb6612_init(void);

void tb6612_start(float duty_target);

void tb6612_stop(void);

void tb6612_set_pwm(float duty_cycle);

float tb6612_get_duty(uint8_t num);


