#pragma once

/* 需要 uint8_t / GPIO_PinState */
#include "gpio.h"

/* ====== 按键读取宏 ====== */
#define KEY1        HAL_GPIO_ReadPin(SYS_SW1_GPIO_Port, SYS_SW1_Pin)
#define KEY2        HAL_GPIO_ReadPin(SYS_SW2_GPIO_Port, SYS_SW2_Pin)
#define KEY3        HAL_GPIO_ReadPin(SYS_SW3_GPIO_Port, SYS_SW3_Pin)
#define KEY4        HAL_GPIO_ReadPin(SYS_SW4_GPIO_Port, SYS_SW4_Pin)
#define KEY5        HAL_GPIO_ReadPin(EC11_KEY_GPIO_Port, EC11_KEY_Pin)

/* ====== 时间阈值（单位：扫描次数，2ms 一次） ====== */
#define KEY_DELAY_TIME              10      /* 消抖 20ms */
#define KEY_CONTINUE_TIME           500     /* 连发启动 1s */
#define KEY_CONTINUE_TRIGGER_TIME   25      /* 连发间隔 50ms */
#define KEY_LONG_TIME               1000    /* 长按 2s */

/* ====== 结构体 ====== */
typedef struct
{
    uint8_t  short_code;        /* 短按键值 */
    uint8_t  long_code;         /* 长按键值 */
    uint8_t  lock_flag;         /* 消抖锁 */
    uint8_t  long_flag;         /* 长按标志 */
    uint16_t cnt;               /* 主计数 */
    uint16_t cnt2;              /* 连击计数 */
} key_status_t;

/* ====== 对外接口 ====== */
void Key_Init(void);
void Key_Scan(void);
