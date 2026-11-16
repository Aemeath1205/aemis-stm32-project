#pragma once
#include "console.h"
#include <string.h>
#include <stdio.h>

#define LOG_ENABLE 1

#if(LOG_ENABLE == 1)
#define FILE_NAME_1   strrchr(__FILE__, '\\') == NULL ? __FILE__ : strrchr(__FILE__, '\\') + 1
#define FILE_NAME_2 strrchr(FILE_NAME_1, '/') == NULL ? FILE_NAME_1 : strrchr(FILE_NAME_1, '/') + 1

#define LOG_DEBUG(format, ...)                \
    printf(">>>LOGDEBUG:[%s][%d] "format"\r\n",\
           FILE_NAME_2, __LINE__, ##__VA_ARGS__)

#else
#define LOG_DEBUG(format, ...)
#endif
