#include "tim.h"
#include "key.h"

extern uint8_t key_num;
extern uint8_t menu;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim2)
    {
        key_scan();

        if(key_num == 11)
        {   
            key_num = 0;
            menu = 2;
        }
    }
}
