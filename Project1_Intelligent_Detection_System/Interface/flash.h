#pragma once

#include "menu.h"

#define THRESHOLD_FLASH_ADDR 0x807F800

void flash_write(uint32_t addr, threshold_t* data);
void flash_read(uint32_t addr, threshold_t* data);