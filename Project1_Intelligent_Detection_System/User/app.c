#include "app.h"

void app_init(void)
{
    console_init(&huart1);
    aht20_init();
    mq2_init();
}

void app_start(void)
{
    // printf("HELLO STM32\n");
    // printf("你好 STM32\n");
    // printf(">>> [%s][%d] \r\n", __FILE__, __LINE__);
    // LOG_DEBUG("%s","1");
    while(1) /*后续整个应用的主题逻辑*/
    {
        // aht20_test();
        mq2_test();
        HAL_Delay(1000);
    }
}