#include "tim.h"
#include "key.h"

extern uint8_t key_num;
// extern threshold_t g_td;
extern uint8_t menu;       // 页面编号 1：数据显示页
extern uint8_t option_num; // 页面编号 1：默认在第一行

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim2)
    {
        key_scan();

        if(key_num == 11 && menu == 1)
        {
            key_num = 0;
            menu = 2;
        }

        if(key_num == 4 && menu == 1)
        {
            key_num = 0;
            menu = 3;
        }
    }
    //...
}
