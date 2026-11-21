#pragma once

#include "stdint.h"

/* ====== 结构体 ====== */
typedef struct
{
    uint16_t temp_threshold;    /* 温度阈值 */
    uint16_t hum_threshold;     /* 湿度阈值 */
    uint16_t mq2_threshold;     /* 烟雾浓度阈值 */
    uint16_t fan_threshold;     /* 风扇阈值 */
} threshold_t;

/* ====== 对外接口 ====== */
void Menu_DataDisplay(void);
void Menu_ThresholdDisplay(uint8_t num);
void Menu_WifiDisplay(uint8_t num);
void Menu_WifiConnectDisplay(uint8_t num);
