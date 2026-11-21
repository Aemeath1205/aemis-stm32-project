#pragma once

#include <stdint.h>      /* uint8_t */
#include "menu.h"        /* threshold_t */

#define DEFAULT_TEMP_THRESHOLD  30
#define DEFAULT_HUM_THRESHOLD   50
#define DEFAULT_MQ2_THRESHOLD   1000
#define DEFAULT_FAN_THRESHOLD   1

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
<<<<<<< HEAD
extern uint8_t menu;
=======
extern uint8_t menu;       // 页面编号 1：数据显示页
extern uint8_t option_num; // 页面编号 1：默认在第一行
>>>>>>> 2ef3123 (feat: 旧工程菜单/按键/阈值/风扇逻辑完善，新工程补 TIM2 与按键配置)
