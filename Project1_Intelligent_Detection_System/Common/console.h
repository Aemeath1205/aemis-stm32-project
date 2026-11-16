#pragma once

#include "usart.h"
#include <stdio.h>

/**
 * @brief console的初始化
 * 
 * @param huart 外部传入的串口句柄,根据实际情况传入不同串口句柄
 */
void console_init(UART_HandleTypeDef* huart);

int fputc(int ch, FILE* file);
int fgetc(FILE* file);