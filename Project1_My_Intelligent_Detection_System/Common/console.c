#include "console.h"
// #include "log.h"
// #include <stdio.h>

volatile static UART_HandleTypeDef* g_huart = NULL;

void Console_Usart_Init(UART_HandleTypeDef* huart)
{
    g_huart = huart;
}

int fputc(int ch, FILE* file)
{
    UNUSED(file);
    if(!g_huart) 
        // LOG_DEBUG("fputc: g_huart un_init");
        return EOF;
    if(HAL_OK != HAL_UART_Transmit(g_huart, (uint8_t*)&ch, 1, HAL_MAX_DELAY))
        // LOG_DEBUG("fputc: uart transmit failed");
        return EOF;
    return ch;
}

int fgetc(FILE* file)
{
    UNUSED(file);
    if(!g_huart) 
        // LOG_DEBUG("fgetc: g_huart un_init");
        return EOF;
    int ch = 0;
    if(HAL_OK != HAL_UART_Receive(g_huart, (uint8_t*)&ch, 1, HAL_MAX_DELAY))
        // LOG_DEBUG("fgetc: uart receive failed");
        return EOF;
    return ch;
}

