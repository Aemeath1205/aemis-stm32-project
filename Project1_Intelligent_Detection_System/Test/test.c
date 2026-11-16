#include "test.h"


void aht20_test(void)
{
    aht20_start();
    // aht20_data_t data;
    // aht20_read_data(&data);
    // printf(">>> %f %f\n", data.humidity, data.temperature);
}

void mq2_test(void)
{
    mq2_read_data();
    LOG_DEBUG("MQ2 PPM: %.2f", mq2_ppm);
}

void alert_test(void)
{
    mq2_read_data();
    LOG_DEBUG("MQ2 PPM: %.2f", mq2_ppm);
    if(mq2_ppm > MQ2_THRESHOLD)
    {
        alert_buzzer_on();
        alert_waterfall_light_on();
    }
    else
    {
        alert_buzzer_off();
        alert_waterfall_light_off();
    }
}
