#pragma once

#include <string.h>
#include <stdio.h>

#define LOG_ENABLE 1 /*1：打开日志输出 2：关闭日志输出*/

#if(LOG_ENABLE == 1)
/*日志启用时的逻辑*/
#define _FILE_NAME strrchr(__FILE__, '\\')  == NULL ? __FILE__   : strrchr(__FILE__, '\\')  + 1
#define  FILE_NAME strrchr(_FILE_NAME,   '/') == NULL ? _FILE_NAME : strrchr(_FILE_NAME, '/') + 1

#define LOG_DEBUG(format, ...) printf(">>>[%s][%d]"format"\r\n",FILE_NAME,__LINE__,##__VA_ARGS__)

#else
/*日志关闭时的逻辑*/
#define LOG_DEBUG(format, ...)
#endif