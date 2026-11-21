#include "oled.h"
#include "string.h"
#include "font.h"

uint8_t oled_data_buffer[8][128] = {0};
static void oled_write_command(uint8_t command)
{
    uint8_t control_byte = 0x00;
    uint8_t buffer[2] = {control_byte, command};
    if (HAL_OK != HAL_I2C_Master_Transmit(&OLED_I2C, OLED_ADDRESS, buffer, 2, HAL_MAX_DELAY))
    {
        LOG_DEBUG("oled write command: I2C Transmit failed");
    }
}
static void oled_write_data(uint8_t *data, uint16_t size)
{
    uint8_t control_byte = 0x40;
    uint8_t buffer[size + 1];
    buffer[0] = control_byte;
    memcpy(&buffer[1], data, size);
    if (HAL_OK != HAL_I2C_Master_Transmit(&OLED_I2C, OLED_ADDRESS, buffer, size + 1, HAL_MAX_DELAY))
    {
        LOG_DEBUG("oled write data: I2C Transmit failed");
    }
}

static void oled_set_cursor(uint8_t page, uint8_t column)
{
    oled_write_command(0xB0 | (page & 0x0F));
    oled_write_command(0x00 | (column & 0x0F));
    oled_write_command(0x10 | ((column >> 4) & 0x0F));
}

void oled_init(void)
{
    HAL_Delay(300);
    // 写⼊⼀系列的命令，对 OLED 进⾏初始化
    oled_write_command(0xAE); // 设置显⽰开启/关闭，0xAE-关闭显⽰ 0xAF-开启显⽰
    oled_write_command(0xD5); // 设置时钟分频因⼦/振荡器频率
    oled_write_command(0x80); // 建议的振荡器频率
    oled_write_command(0xA8); // 设置多路复⽤率
    oled_write_command(0x3F); // 1/64占空⽐
    oled_write_command(0xD3); // 设置显⽰偏移
    oled_write_command(0x00); // 不偏移
    oled_write_command(0x40); // 设置起始⾏地址
    oled_write_command(0xA1); // 设置左右⽅向，0xA1正常，0xA0左右反置
    oled_write_command(0xC8); // 设置上下⽅向，0xC8正常，0xC0上下反置
    oled_write_command(0xDA); // 设置COM引脚硬件配置
    oled_write_command(0x12);
    oled_write_command(0x81); // 设置对⽐度
    // oled_write_command(0xCF); // 对⽐度值
    oled_write_command(0x7F); // 对⽐度值
    oled_write_command(0xD9); // 设置预充电周期
    oled_write_command(0xF1);
    oled_write_command(0xDB); // 设置VCOMH取消选择级别
    oled_write_command(0x30);
    oled_write_command(0xA4); // 全局显⽰开启/关闭
    oled_write_command(0xA6); // 设置正常/反相显⽰，0xA6正常，0xA7反相
    oled_write_command(0x8D); // 设置电荷泵
    oled_write_command(0x14); // 使能电荷泵
    oled_write_command(0xAF); // 开启显⽰
    oled_clear();
    oled_update_screen();
    LOG_DEBUG("OLED init success");
  
}
void oled_clear(void)
{
    memset(oled_data_buffer, 0, sizeof(oled_data_buffer));
}

void oled_clear_area(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    if(x > 127 || y > 63) return;
    if(x + width > 128 || y + height > 64) return;
    
    for(uint8_t i = y; i < y + height; i++)
    {
        uint8_t page = i / 8;
        uint8_t bit_position = i % 8;
        uint8_t bite_mask = ~(1 << bit_position);
        for(uint8_t j = x; j < x + width; j++)
        {
            oled_data_buffer[page][j] &= bite_mask;
        }
    }
}

//只支持8对齐(y & height)
void oled_show_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *image)
{ 
    if(x > 127 || y > 63) return;
    if(x + width > 128 || y + height > 64) return;
    oled_clear_area(x, y, width, height);

    uint8_t page_num = ((height -1) / 8) + 1;//?
    for(uint8_t page = 0; page < page_num; page++)
    {
        for(uint8_t i = 0; i < width; i++)
        {
            // if((x + i) > 127) break;
            // if((y / 8 + page) > 7) return;
            uint8_t page_index = y / 8 + page;
            oled_data_buffer[page_index][x + i] = image[page * width + i]; 
        }
    }
}

void oled_update_screen(void)
{
    for(uint8_t page = 0; page < 8; page++)
    {
        oled_set_cursor(page, 0);
        oled_write_data(oled_data_buffer[page], 128);
    }
}

void oled_show_char(uint8_t x, uint8_t y, char c, uint8_t font_size)
{
    if(font_size == OLED_8X16)
    {
        oled_show_image(x, y, 8, 16, OLED_F8x16[c - ' ']);
    }   
}

void oled_show_string(uint8_t x, uint8_t y, const char* str, uint8_t font_size)
{
    while(*str)
    {
        oled_show_char(x, y, *str, font_size);
        x += font_size;
        if(x > 127) break;
        str++;
    }   
}

void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t font_size)
{
    char str[12];
    snprintf(str, sizeof(str), "%d", num);
    oled_show_string(x, y, str, font_size);
}

void oled_show_int_fixed(uint8_t x, uint8_t y, uint32_t num, uint8_t width, uint8_t font_size)
{
    char buf[16];
    // if(width >= sizeof(buf))
    // {
    //     width = sizeof(buf) - 1;
    // }
    snprintf(buf, sizeof(buf), "%-*d", width, num);
    oled_show_string(x, y, buf, font_size);
}

void oled_invert_line(uint8_t line)
{
    if(line >= 4) return;
    uint8_t y = line * 16;
    uint8_t page_start = y / 8;
    uint8_t page_end = page_start + 1;
    for(uint8_t page = page_start; page <= page_end; ++page)
    {
        for(uint8_t x = 0; x < 128; x++)
        {
            oled_data_buffer[page][x] = ~oled_data_buffer[page][x];
        }
    }
}