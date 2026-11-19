#include "app.h"

static void app_handle_light_test()
{
    static uint32_t start_time = 0;
    if(HAL_GetTick() - start_time >= 500)
    {
        HAL_GPIO_TogglePin(LED_CTRL_GPIO_Port, LED_CTRL_Pin);
        start_time = HAL_GetTick();
    }
}

void app_init(void)
{
    console_init(&huart1);
    aht20_init();
    mq2_init();
    oled_init();
}

void app_start(void)
{
    // printf("HELLO STM32\n");
    // printf("你好 STM32\n");
    // printf(">>> [%s][%d] \r\n", __FILE__, __LINE__);
    // LOG_DEBUG("%s","1");
    while(1) /*后续整个应用的主题逻辑*/
    {
        app_handle_light_test();
        // aht20_test();
        oled_test();
        HAL_Delay(1000);
    }
}