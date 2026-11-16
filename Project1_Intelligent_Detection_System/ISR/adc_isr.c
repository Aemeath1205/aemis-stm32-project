#include "adc.h"
#include "mq2.h"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &MQ2_ADC)
    {
        mq2_flag = 1;
    }
}
