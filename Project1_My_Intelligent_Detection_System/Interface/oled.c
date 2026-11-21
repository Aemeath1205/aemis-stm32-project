#include "oled.h"
#include "log.h"
#include "font.h"
/*value*/
static uint8_t oled_data_buffer[8][128] = {0};

/*static func*/
// 8位对齐
static void Oled_Bit_To_Buffer(uint8_t x, uint8_t y,
                               uint8_t width, uint8_t height,
                               const uint8_t *changed_data)
{
    if (x + width > 128 || y + height > 64)
    {
        LOG_DEBUG("Oled_Bit_To_Buffer: parameter out of buffer");
        return;
    }
    uint8_t page_start = y / 8;
    uint8_t page_num = height / 8;
    for (uint8_t i = page_start; i < page_start + page_num; i++)
    {
        for (uint8_t j = x; j < x + width; j++)
        {
            oled_data_buffer[i][j] = *(changed_data++);
        }
    }
}
static void oled_write_command(const uint8_t *command, uint16_t size)
{
    uint8_t buffer[size + 1];
    buffer[0] = COMMAND_CMD;
    memcpy(&buffer[1], command, size);
    if (HAL_OK != HAL_I2C_Master_Transmit(&OLED_I2C, OLED_ADDRESS, buffer, size + 1, HAL_MAX_DELAY))
    {
        LOG_DEBUG("oled_write_commnd: i2c transmit error");
        return;
    }
}

static void oled_set_cursor(uint8_t page, uint8_t column)
{
    uint8_t cmd[3] = {
        0xB0 | (page & 0x07),
        0x00 | (column & 0x0F),
        0x10 | ((column >> 4) & 0x0F)};
    oled_write_command(cmd, 3);
}

static void oled_write_data(const uint8_t *data, uint16_t size)
{
    uint8_t buffer[size + 1];
    buffer[0] = WRITE_CMD;
    memcpy(&buffer[1], data, size);
    if (HAL_OK != HAL_I2C_Master_Transmit(&OLED_I2C, OLED_ADDRESS, buffer, size + 1, HAL_MAX_DELAY))
    {
        LOG_DEBUG("oled_write_data: i2c transmit error");
        return;
    }
}

static const uint8_t oled_init_cmds[] = {
    0x00, /* 控制字节：后面全是命令 */
    0xAE, /* 关显示 */
    0xD5,
    0x80, /* 时钟分频 */
    0xA8,
    0x3F, /* 复用率 1/64 */
    0xD3,
    0x00, /* 显示偏移 */
    0x40, /* 起始行 */
    0xA1, /* 左右正常 */
    0xC8, /* 上下正常 */
    0xDA,
    0x12, /* COM 配置 */
    0x81,
    0x7F, /* 对比度 */
    0xD9,
    0xF1, /* 预充电 */
    0xDB,
    0x30, /* VCOMH */
    0xA4, /* 跟随 RAM */
    0xA6, /* 正常显示 */
    0x8D,
    0x14, /* 开电荷泵 */
    0xAF, /* 开显示 */
};

/**********public func**********/
/*Init*/
void Oled_Init(void)
{
    HAL_Delay(300);

    if (HAL_OK != HAL_I2C_Master_Transmit(&OLED_I2C, OLED_ADDRESS,
                                          (uint8_t *)oled_init_cmds,
                                          sizeof(oled_init_cmds), 100))
        LOG_DEBUG("oled init failed");

    Oled_Clear_All();
    Oled_Update_Buffer_To_Oled();
    LOG_DEBUG("OLED init success");
}

/*clear buffer*/
void Oled_Clear_All(void)
{
    memset(oled_data_buffer, 0, sizeof(oled_data_buffer));
}
void Oled_Clear_Area(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    if (x + width > 128 || y + height > 64)
    {
        LOG_DEBUG("Oled_clear_area: parameter out of buffer");
        return;
    }
    for (uint8_t i = y; i < y + height; i++)
    {
        uint8_t page_num = i / 8;
        uint8_t page_inner_position = i % 8;
        uint8_t mask = ~(1 << page_inner_position);
        for (uint8_t j = x; j < x + width; j++)
        {
            oled_data_buffer[page_num][j] &= mask;
        }
    }
}

/*string\nums -> char -> bit of buffer*/
// static void Bit_To_Buffer(uint8_t x, uint8_t y,
// uint8_t width, uint8_t height, const uint8_t changed_data)
//                      ^
void Oled_Char_To_Bit(uint8_t x, uint8_t y,
                      char c, uint8_t font_size)
{
    if (font_size == OLED_8X16)
    {
        Oled_Bit_To_Buffer(x, y, 8, 16, OLED_F8x16[c - ' ']);
    }
}

void Oled_String_To_Char(uint8_t x, uint8_t y,
                         const char *str, uint8_t font_size)
{
    if (font_size == OLED_8X16)
    {
        while (str != NULL && *str && x < 127)
        {
            Oled_Char_To_Bit(x, y, *(str++), font_size);
            x += 8;
        }
    }
}

void Oled_Num_To_String(uint8_t x, uint8_t y,
                        uint32_t num, uint8_t font_size)
{
    if (font_size == OLED_8X16)
    {
        char str[12];
        snprintf(str, sizeof(str), "%d", num);
        Oled_String_To_Char(x, y, str, font_size);
    }
}

void Oled_Fixed_Nums_To_String(uint8_t x, uint8_t y,
                               uint32_t num, uint8_t font_size, uint8_t width)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%-*d", width, num);
    Oled_String_To_Char(x, y, buf, font_size);
}

/*updat buffer to oled*/
void Oled_Update_Buffer_To_Oled(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        oled_set_cursor(page, 0);
        oled_write_data(oled_data_buffer[page], 128);
    }
}

/*invert line*/
void Oled_Invert_Line(uint8_t line)
{
    if (line >= 4)
    {
        LOG_DEBUG("Oled_Update_Buffer_To_Oled: parameter out of buffer");
        return;
    }
    uint8_t real_start_page = line * 2;
    uint8_t real_end_page = real_start_page + 1;
    for (uint8_t i = real_start_page; i <= real_end_page; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            oled_data_buffer[i][j] = ~oled_data_buffer[i][j];
        }
    }
}
