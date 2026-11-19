#pragma once
#include "i2c.h"
/*define*/
#define OLED_I2C      hi2c2
#define OLED_ADDRESS (0x3C << 1)
#define COMMAND_CMD   0x00
#define WRITE_CMD     0x40
/**********public func**********/
/*Init*/
void Oled_Init(void);

/*clear buffer*/
void Oled_Clear_All(void);
void Oled_Clear_Area(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

/*string\nums -> char -> bit of buffer*/
//static void Bit_To_Buffer(uint8_t x, uint8_t y, 
//uint8_t width, uint8_t height, const uint8_t changed_data)
//                     ^
void Oled_Char_To_Bit         (uint8_t x, uint8_t y, char c, uint8_t font_size);
void Oled_String_To_Char      (uint8_t x, uint8_t y, const char* str, uint8_t font_size);
void Oled_Num_To_String       (uint8_t x, uint8_t y, uint32_t num, uint8_t font_size);
void Oled_Fixed_Nums_To_String(uint8_t x, uint8_t y, uint32_t num, uint8_t font_size, uint8_t width);

/*updat buffer to oled*/
void Oled_Update_Buffer_To_Oled(void);

/*invert line*/
void Oled_Invert_Line(uint8_t line);

