#include "alert.h"
#include "main.h"

static uint8_t next_led_index = 0;
/***********public func***********/
void Alert_Buzzer_ON(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
}
void Alert_Buzzer_OFF(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
}

void Alert_Waterled_ON(void)
{
    static uint32_t last_time_tick = 0;
    if(HAL_GetTick() - last_time_tick > WATERLED_CHANGE_TIME)
    {
        last_time_tick = HAL_GetTick();
        switch(next_led_index)
        {
            case 0:
                HAL_GPIO_WritePin(System_LED4_GPIO_Port, System_LED4_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(System_LED1_GPIO_Port, System_LED1_Pin, GPIO_PIN_RESET);
                break;
            case 1:
                HAL_GPIO_WritePin(System_LED1_GPIO_Port, System_LED1_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(System_LED2_GPIO_Port, System_LED2_Pin, GPIO_PIN_RESET);
                break;
            case 2:
                HAL_GPIO_WritePin(System_LED2_GPIO_Port, System_LED2_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(System_LED3_GPIO_Port, System_LED3_Pin, GPIO_PIN_RESET);
                break;
            case 3:
                HAL_GPIO_WritePin(System_LED3_GPIO_Port, System_LED3_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(System_LED4_GPIO_Port, System_LED4_Pin, GPIO_PIN_RESET);
                break;
        }
        next_led_index = (next_led_index + 1) % 4;
    }
}
void Alert_Waterled_OFF(void)
{
    next_led_index = 0;
    HAL_GPIO_WritePin(System_LED1_GPIO_Port, System_LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(System_LED2_GPIO_Port, System_LED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(System_LED3_GPIO_Port, System_LED3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(System_LED4_GPIO_Port, System_LED4_Pin, GPIO_PIN_SET);
}