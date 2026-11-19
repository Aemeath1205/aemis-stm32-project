#pragma once

#include "i2c.h"
#include "log.h"

#define OLED_I2C      hi2c2
#define OLED_ADDRESS (0x3C << 1)

void oled_init(void);
void oled_clear(void);
void oled_clear_area(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void oled_show_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* image);
void oled_update_screen(void);

void oled_show_char(uint8_t x, uint8_t y, char c, uint8_t font_size);
void oled_show_string(uint8_t x, uint8_t y, const char* str, uint8_t font_size);
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t font_size);

void oled_show_int_fixed(uint8_t x, uint8_t y, uint32_t num, uint8_t width, uint8_t font_size);
void oled_invert_line(uint8_t line);