#include "console.h"

static UART_HandleTypeDef* console_huart = NULL;

void console_init(UART_HandleTypeDef* huart)
{
    console_huart = huart;
}

int fputc(int ch, FILE* file)
{
    UNUSED(file); /*消除未使用变量的警告*/
    if(!console_huart) return EOF;
    if(HAL_OK != HAL_UART_Transmit(console_huart, (uint8_t*)&ch, 1, HAL_MAX_DELAY))
    {
        return EOF;
    }
    return ch; 
}

int fgetc(FILE* file)
{
    UNUSED(file);
    if(!console_huart) return EOF;
    uint8_t ch = 0;
    if(HAL_OK != HAL_UART_Receive(console_huart, &ch, 1, HAL_MAX_DELAY))
    {
        return EOF;
    }
    return ch;
}