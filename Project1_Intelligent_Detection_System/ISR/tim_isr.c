#include "tim.h"
#include "key.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim2)
    {
        key_scan();

        //....
    }
}
