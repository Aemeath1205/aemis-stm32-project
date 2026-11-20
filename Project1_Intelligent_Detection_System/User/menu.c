#include "menu.h"
#include "font.h"
#include "math.h"
// extern volatile aht20_data_t aht20_data;
// extern float mq2_ppm;
uint8_t fan_is_running = 0;
extern threshold_t g_td; 

static void menu_oled_show_progress_bar(uint8_t progress)
{
    static uint8_t flag = 0;
    char bar[17] = {' ', ' ', ' ', ' ', ' ', 
                    ' ', ' ', ' ', ' ', ' ', 
                    ' ', ' ', ' ', ' ', ' ',
                    ' ', '\0'};
    uint8_t block_num = progress / 10;
    bar[0] = '[';
    for(uint8_t i = 0; i < block_num; i++)
    {
        bar[i + 1] = '=';   
    }
    bar[11] = ']';

    uint8_t units_digit = progress % 10 + '0';
    uint8_t tens = ' ';
    if(progress >= 10)
    {
        tens = (progress / 10) % 10 + '0';
    }
    bar[12] = tens;
    bar[13] = units_digit;
    bar[14] = '%';
    uint8_t ch = flag ? '-' : '|';
    flag = !flag;
    bar[15] = ch;
}

//数据显示页
void menu_data_display(void)
{
    oled_show_string(0, 0, "tem: ", OLED_8X16);
    oled_show_string(0, 16, "hum: ", OLED_8X16);
    oled_show_string(0, 32, "mq2: ", OLED_8X16);
    oled_show_string(0, 48, "fan: ", OLED_8X16);

    oled_show_string(8*(5+1+4), 0, "C", OLED_8X16);
    oled_show_string(8*(5+1+4), 16,"%", OLED_8X16);
    oled_show_string(8*(5+1+4), 32,"ppm", OLED_8X16);

    aht20_start();
    uint8_t message[8];
    sprintf((char*)message, "%-4.1f", aht20_data.temperature);
    oled_show_string(OLED_8X16*5, 0, (char*)message, OLED_8X16);

    sprintf((char*)message, "%-4.1f", aht20_data.humidity);
    oled_show_string(OLED_8X16*5, 16, (char*)message, OLED_8X16);

    mq2_read_data();
    if(mq2_ppm > 9999.0f)
    {
        mq2_ppm = 9999.0f;
    }
    sprintf((char*)message, "%-4d", (int)roundf(mq2_ppm));
    oled_show_string(OLED_8X16*5, 32, (char*)message, OLED_8X16);

    if(fan_is_running == 0)
    {
        oled_show_string(OLED_8X16*5, 48, "OFF", OLED_8X16);
    }
    else if(fan_is_running == 1)
    {
        oled_show_string(OLED_8X16*5, 48, "ON ", OLED_8X16);
    }
    oled_update_screen();
}

//阈值调整页 num具体选中的行号
void menu_threshold_display(uint8_t num)
{
    oled_clear();

    oled_show_string(8, 0 , "tem: ", OLED_8X16);
    oled_show_string(8, 16, "hum: ", OLED_8X16);
    oled_show_string(8, 32, "mq2: ", OLED_8X16);
    oled_show_string(8, 48, "fan: ", OLED_8X16);

    uint8_t width2 = 8 + 8 * 5;
    oled_show_int_fixed(width2, 0, g_td.temp_threshold, 4, OLED_8X16);
    oled_show_int_fixed(width2, 16, g_td.hum_threshold, 4, OLED_8X16);
    oled_show_int_fixed(width2, 32, g_td.mq2_threshold, 4, OLED_8X16);
    oled_show_int_fixed(width2, 48, g_td.fan_threshold, 4, OLED_8X16);

    /*选中处理*/
    oled_show_string(0, (num - 1) * 16, ">", OLED_8X16);
    oled_invert_line(num - 1);

    oled_update_screen();
}

//WIFI配置页
void menu_wifi_display(uint8_t num)
{
    oled_clear();
    oled_show_string(8, 0 , "1.Reset Module", OLED_8X16);
    oled_show_string(8, 16, "2.SoftAP Mode" , OLED_8X16);
    oled_show_string(8, 32, "3.AirLink Mode", OLED_8X16);

    /*选中处理*/
    oled_show_string(0, (num - 1) * 16, ">", OLED_8X16);
    oled_invert_line(num - 1);

    oled_update_screen();
}
//WIFI连接页 num进度数
void menu_wifi_connect_display(uint8_t num)
{
    oled_show_string(0, 0, "Connect WIFI...", OLED_8X16);
    menu_oled_show_progress_bar(num);
    if(num == 99)
    {
        oled_clear();
        oled_show_string(0, 16, "Connect Success!", OLED_8X16);
        oled_update_screen();
        HAL_Delay(2000);
        oled_clear();

        //后续返回数据显示页
    }
    oled_update_screen();
}

