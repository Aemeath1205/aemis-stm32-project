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

void oled_test(void)
{
    char str[13];
    snprintf(str, sizeof(str), "Tem: %.2fC", aht20_data.temperature);
    oled_show_string(0, 0, str, OLED_8X16);
    snprintf(str, sizeof(str), "Hum: %.2f%%", aht20_data.humidity);
    oled_show_string(0, 16, str, OLED_8X16);
    snprintf(str, sizeof(str), "MQ2: %.2f", mq2_ppm);
    oled_show_string(0, 32, str, OLED_8X16);

    oled_update_screen();
}

void menu_test(void)
{
   static uint8_t  page = 0;
    static uint32_t t    = 0;

    if (HAL_GetTick() - t >= 5000)      /* 5 秒切一页 */
    {
        t = HAL_GetTick();
        page = (page + 1) % 6;
    }

    switch (page)
    {
        case 0: menu_data_display();        break;
        case 1: menu_threshold_display(1);  break;
        case 2: menu_threshold_display(2);  break;
        case 3: menu_threshold_display(3);  break;
        case 4: menu_threshold_display(4);  break;
        case 5: menu_wifi_display(2);       break;
    }
    
}

void alert_test(void)
{
    mq2_read_data();
    LOG_DEBUG("MQ2 PPM: %.2f", mq2_ppm);
    if (mq2_ppm > MQ2_THRESHOLD)
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
