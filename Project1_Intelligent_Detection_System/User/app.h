#pragma once

#include <stdint.h>      /* uint8_t */
#include "menu.h"        /* threshold_t */

/**
 * @brief 负责所有的初始化逻辑
 * 
 */
void app_init(void);

/**
 * @brief 负责整个应用的启动逻辑
 * 
 */
void app_start(void);

extern uint8_t key_num;
extern threshold_t g_td;