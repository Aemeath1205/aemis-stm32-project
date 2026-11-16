#include "alert.h"

static uint8_t led_index = 0;

void alert_buzzer_on(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
}

void alert_buzzer_off(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
}

void alert_waterfall_light_on(void)
{
    static uint32_t last_tick = 0;
    if((HAL_GetTick() - last_tick) >= LED_INTERVAL_MS)
    {
        last_tick = HAL_GetTick();
        HAL_GPIO_WritePin(System_LED1_GPIO_Port, System_LED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(System_LED2_GPIO_Port, System_LED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(System_LED3_GPIO_Port, System_LED3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(System_LED4_GPIO_Port, System_LED4_Pin, GPIO_PIN_SET);

        switch(led_index)
        {
            case 0:
                HAL_GPIO_WritePin(System_LED1_GPIO_Port, System_LED1_Pin, GPIO_PIN_RESET);
                break;
            case 1:
                HAL_GPIO_WritePin(System_LED2_GPIO_Port, System_LED2_Pin, GPIO_PIN_RESET);
                break;
            case 2:
                HAL_GPIO_WritePin(System_LED3_GPIO_Port, System_LED3_Pin, GPIO_PIN_RESET);
                break;
            case 3:
                HAL_GPIO_WritePin(System_LED4_GPIO_Port, System_LED4_Pin, GPIO_PIN_RESET);
                break;
        }
        led_index = (led_index + 1) % 4;
    } 
}

void alert_waterfall_light_off(void)
{
    led_index = 0;
    HAL_GPIO_WritePin(System_LED1_GPIO_Port, System_LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(System_LED2_GPIO_Port, System_LED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(System_LED3_GPIO_Port, System_LED3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(System_LED4_GPIO_Port, System_LED4_Pin, GPIO_PIN_SET);
}