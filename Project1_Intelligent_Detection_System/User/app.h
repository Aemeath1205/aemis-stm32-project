#pragma once

#include "console.h"
#include "log.h"
#include "aht20.h"
#include "test.h"
#include "mq2.h"

/**
 * @brief 负责所有的初始化逻辑
 * 
 */
void app_init(void);

/**
 * @brief 负责整个应用的启动逻辑
 * 
 */
void app_start(void);
