#include "key.h"
#include "app.h"
#include "tim.h"

extern uint8_t key_num;

key_status_t key[5] = {
    {1, 11, 0, 0, 0},
    {2, 22, 0, 0, 0},
    {3, 33, 0, 0, 0},
    {4, 44, 0, 0, 0},
    {5, 55, 0, 0, 0}
};

static void key_process(GPIO_PinState status, key_status_t* key)
{
    if(status == GPIO_PIN_SET)
    {
        key->cnt = 0;
        key->cnt2 = 0;
        key->lock_flag = 0;
        if(key->long_flag == 1)
        {
            key->long_flag = 0;
            key_num = key->long_code;
        }
    }
    else if(key->lock_flag == 0)
    {
        key->cnt++;
        if(key->cnt > KEY_DELAY_TIME)
        {
            key->cnt = 0;
            key->lock_flag = 1;
            key_num = key->short_code;
        }
    }
    else if(key->cnt < KEY_CONTINUE_TIME)
    {
        key->cnt++;
    }
    else
    {
        key->cnt++;
        key->cnt2++;
        if(key->cnt2 > KEY_CONTINUE_TRIGGER_TIME)
        {
            key->cnt2 = 0;
            key_num = key->short_code;
        }
        if(key->cnt > KEY_LONG_TIME)
        {
            key->long_flag = 1;
        }
    }
}

void key_init(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

void key_scan(void)
{
    key_process(KEY1, &key[0]);
    key_process(KEY2, &key[1]);
    key_process(KEY3, &key[2]);
    key_process(KEY4, &key[3]);
    key_process(KEY5, &key[4]);
}