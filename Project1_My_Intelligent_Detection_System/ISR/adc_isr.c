#include "adc_isr.h"
#include "mq2.h"
#include "adc.h"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &MQ2_ADC)
    {
        flag_measure_done = true;
    }
}
