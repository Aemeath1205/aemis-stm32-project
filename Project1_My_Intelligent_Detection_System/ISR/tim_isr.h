#pragma once

#include "tim.h"

/* HAL 回调（HAL 里已有弱定义，这里是本文件要实现的） */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
