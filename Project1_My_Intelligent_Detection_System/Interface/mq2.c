#include "mq2.h"

bool flag_measure_done = false;
float mq2_ppm = 0.0f;

void Mq2_Init(void)
{
    HAL_ADCEx_Calibration_Start(&MQ2_ADC);
    HAL_ADC_Start_IT(&MQ2_ADC);
}
void Mq2_Start(void)
{
    if(flag_measure_done)
    {
        flag_measure_done = false;
        uint32_t mq2_adc_value = HAL_ADC_GetValue(&MQ2_ADC);
        float voltage = (float)mq2_adc_value * 3.3f / 4095.0f;
        float voltage_out = voltage * 4.0f / 3.0f;

        mq2_ppm = pow((22.6621 * voltage_out * (5 - 0.303)) / (0.303 * (5 - voltage_out)), 
                    1.0f / 0.4843);
        mq2_ppm -= MQ2_BASIC;
        if(mq2_ppm < 0)
            mq2_ppm = 0;
        if(mq2_ppm > 10000.0f)
            mq2_ppm = 10000.0f;
    }
}