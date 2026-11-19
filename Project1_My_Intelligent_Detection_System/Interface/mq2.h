#pragma once
#include "adc.h"
#include "math.h"
#include <stdbool.h>

//**************define*****************//
#define MQ2_ADC       hadc1
#define MQ2_BASIC     700
#define MQ2_THRESHOLD 500

//**************public func*****************//
void Mq2_Init(void);
void Mq2_Start(void);

extern bool flag_measure_done;
extern float mq2_ppm;