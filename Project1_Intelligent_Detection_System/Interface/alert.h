#pragma once

#include "gpio.h"
#include "mq2.h"

#define LED_INTERVAL_MS 100

void alert_buzzer_on(void);
void alert_buzzer_off(void);
void alert_waterfall_light_on(void);
void alert_waterfall_light_off(void);