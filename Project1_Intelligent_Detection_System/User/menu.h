#pragma once

#include "aht20.h"
#include "mq2.h"
#include "oled.h"

typedef struct
{
    uint16_t temp_threshold;
    uint16_t hum_threshold;
    uint16_t mq2_threshold;
    uint16_t fan_threshold;
} threshold_t;

//数据显示页
void menu_data_display(void);
//阈值调整页 num具体选中的行号
void menu_threshold_display(uint8_t num);
//WIFI配置页
void menu_wifi_display(uint8_t num);
//WIFI连接页 num进度数
void menu_wifi_connect_display(uint8_t num);

//
