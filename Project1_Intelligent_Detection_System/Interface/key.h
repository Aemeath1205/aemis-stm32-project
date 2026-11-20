#pragma once

#include "gpio.h"
#include "time.h"
#include "log.h"

#define KEY1 HAL_GPIO_ReadPin(SYS_SW1_GPIO_Port,SYS_SW1_Pin)
#define KEY2 HAL_GPIO_ReadPin(SYS_SW2_GPIO_Port,SYS_SW2_Pin)
#define KEY3 HAL_GPIO_ReadPin(SYS_SW3_GPIO_Port,SYS_SW3_Pin)
#define KEY4 HAL_GPIO_ReadPin(SYS_SW4_GPIO_Port,SYS_SW4_Pin)
#define KEY5 HAL_GPIO_ReadPin(EC11_KEY_GPIO_Port,EC11_KEY_Pin)

#define KEY_DELAY_TIME            10
#define KEY_LONG_TIME             1000
#define KEY_CONTINUE_TIME         500
#define KEY_CONTINUE_TRIGGER_TIME 25

typedef struct
{
    uint8_t short_code; //短按键值
    uint8_t long_code;  //长按键值

    uint8_t lock_flag;  //消抖锁
    uint8_t long_flag;  //长按标志
    uint16_t cnt;       //主计数

    uint16_t cnt2;      //连击计数
}key_status_t;


void key_init(void);
void key_scan(void);