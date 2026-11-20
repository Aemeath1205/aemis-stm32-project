#include "app.h"
#include "console.h"
#include "log.h"
#include "aht20.h"
#include "test.h"
#include "mq2.h"
#include "menu.h"

uint8_t key_num = 0;
uint8_t menu = 1;       // 页面编号 1：数据显示页
uint8_t option_num = 1; // 页面编号 1：默认在第一行

threshold_t g_td = {
    .temp_threshold = 30,
    .hum_threshold = 50,
    .mq2_threshold = 5000,
    .fan_threshold = 1};

static void app_switch_menu(void)
{
    switch (menu)
    {
    case 1:
        menu_data_display();
        break;
    case 2:
        
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }
}

static void app_handle_light_test()
{
    static uint32_t start_time = 0;
    if (HAL_GetTick() - start_time >= 500)
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
    while (1) /*后续整个应用的主题逻辑*/
    {
        // aht20_start();        /* 状态机推进一步 */
        // mq2_read_data();
        // app_handle_light_test();
        // aht20_test();
        // oled_test();
        // menu_test();
        HAL_Delay(1000);
    }
}