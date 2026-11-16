#pragma once

#include "adc.h"
#include "log.h"
#include "alert.h"

#define MQ2_ADC hadc1
#define MQ2_BASIC 700
#define MQ2_THRESHOLD 500

void mq2_init(void);

void mq2_read_data(void);

extern float mq2_ppm;
extern uint8_t mq2_flag;
