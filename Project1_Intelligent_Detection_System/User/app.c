#include "app.h"
#include "console.h"
#include "log.h"
#include "aht20.h"
#include "test.h"
#include "mq2.h"
#include "menu.h"
#include <stdint.h>
#include "flash.h"
#include "key.h"
#include "tb6612.h"
#include "alert.h"

uint8_t key_num = 0;
uint8_t menu = 1;       // 页面编号 1：数据显示页
uint8_t option_num = 1; // 页面编号 1：默认在第一行
uint8_t menu_need_refresh = 1;
uint8_t threshold_update_flag = 0;      /// 阈值更新标志位

static void threshold_data_init(void)
{
    threshold_t flash_data;
    flash_read(THRESHOLD_FLASH_ADDR, &flash_data);
    if(flash_data.temp_threshold == 0xFFFF ||
       flash_data.hum_threshold  == 0xFFFF ||
       flash_data.mq2_threshold  == 0xFFFF ||
       flash_data.fan_threshold  == 0xFFFF)
    {
        g_td.temp_threshold = DEFAULT_TEMP_THRESHOLD;
        g_td.hum_threshold  = DEFAULT_HUM_THRESHOLD;
        g_td.mq2_threshold  = DEFAULT_MQ2_THRESHOLD;
        g_td.fan_threshold  = DEFAULT_FAN_THRESHOLD;
        flash_write(THRESHOLD_FLASH_ADDR, &g_td);
    }
    else
    {
        g_td.temp_threshold = flash_data.temp_threshold;
        g_td.hum_threshold  = flash_data.hum_threshold;
        g_td.mq2_threshold  = flash_data.mq2_threshold;
        g_td.fan_threshold  = flash_data.fan_threshold;
    }
}

threshold_t g_td = {
    .temp_threshold = 30,
    .hum_threshold = 50,
    .mq2_threshold = 5000,
    .fan_threshold = 1};

static void app_warning_alert(void)
{
    uint8_t temp_alerm = (aht20_data.temperature > g_td.temp_threshold);
    uint8_t smoke_alerm = (mq2_ppm > g_td.mq2_threshold);
    uint8_t need_fan = temp_alerm || smoke_alerm;

    static uint8_t smoke_alarm_latched = 0;
    if(smoke_alerm == 1)
    {
        if(smoke_alarm_latched == 0) 
            alert_buzzer_on();
        
        alert_waterfall_light_on();
        smoke_alarm_latched = 1;
    }
    else
    {
        if(smoke_alarm_latched == 1)
        {
            alert_buzzer_off();
            alert_waterfall_light_off();
            smoke_alarm_latched = 0;
        }
    }

    if(need_fan == 1)
    {
        if(fan_is_running == 0)
        {
            tb6612_start(tb6612_get_duty(g_td.fan_threshold));
            fan_is_running = 1;
        }
    }
    else
    {
        fan_is_running = 0;
        tb6612_stop();
    }
}

static uint16_t* app_get_threshold_by_option(uint8_t option)
{
    if (option == 1) return &g_td.temp_threshold;
    else             return &g_td.hum_threshold;
}


static void app_switch_menu(void)
{
    switch (menu)
    {
        case 1: //显示数据页（默认页）
            menu_data_display();
            break;
        /////////////////////////////////////////////
        case 2: //调整阈值页
            if(key_num == 1) //短按key1->从上到下选择行数
            {
                key_num = 0;
                option_num++;
                if(option_num > 4)
                {
                    option_num = 1;
                }
                menu_need_refresh = 1;
            }
            
            switch(option_num)
            {
                case 1:
                case 2:
                    {
                        if(key_num == 3) //减小阈值
                        {
                            uint16_t* threshold = app_get_threshold_by_option(option_num);
                            key_num = 0;

                            if(*threshold <= 1) *threshold = 99;
                            else                (*threshold)--;

                            menu_need_refresh = 1;
                            threshold_update_flag = 1;
                        }
                        else if(key_num == 4)
                        {
                            uint16_t* threshold = app_get_threshold_by_option(option_num);
                            key_num = 0;

                            (*threshold)++;
                            if(*threshold >= 100) *threshold = 1;

                            menu_need_refresh = 1;
                            threshold_update_flag = 1;
                        }
                        break;
                    }
                case 3:
                    {
                        if(key_num == 3)
                        {
                            key_num = 0;

                            if(g_td.mq2_threshold <= 200) g_td.mq2_threshold = 5000;
                            else                          g_td.mq2_threshold -= 200;
                            
                            menu_need_refresh = 1;
                            threshold_update_flag = 1;
                        }

                        else if(key_num == 4)
                        {
                            key_num = 0;
                            g_td.mq2_threshold += 200;
                            if(g_td.mq2_threshold >= 5000) g_td.mq2_threshold = 200;

                            menu_need_refresh = 1;
                            threshold_update_flag = 1;
                        }
                        
                    }
                case 4:
                    {
                        if(key_num == 3)
                        {
                            key_num = 0;
                            
                            if(g_td.fan_threshold <= 1) g_td.fan_threshold = 3;
                            else                        g_td.fan_threshold--;
                            
                            if(fan_is_running == 1)
                            {
                                tb6612_set_pwm(tb6612_get_duty(g_td.fan_threshold));
                            }

                            threshold_update_flag = 1;
                            menu_need_refresh = 1;
                        }
                        else if(key_num == 4)
                        {
                            key_num = 0;
                            g_td.fan_threshold++;
                            if(g_td.fan_threshold >= 4) g_td.fan_threshold = 1;
                            
                            if(fan_is_running == 1)
                            {
                                tb6612_set_pwm(tb6612_get_duty(g_td.fan_threshold));
                            }

                            threshold_update_flag = 1;
                            menu_need_refresh = 1;
                        }
                    }
            }

            if(key_num == 5)
            {
                key_num = 0;
                g_td.temp_threshold = DEFAULT_TEMP_THRESHOLD;
                g_td.hum_threshold  = DEFAULT_HUM_THRESHOLD;
                g_td.mq2_threshold  = DEFAULT_MQ2_THRESHOLD;
                g_td.fan_threshold  = DEFAULT_FAN_THRESHOLD;

                threshold_update_flag = 1;
                menu_need_refresh = 1;
            }

            if(key_num == 2)
            {
                key_num = 0;
                option_num = 1;
                menu = 1;
                menu_need_refresh = 1;
                
                
                if(threshold_update_flag == 1)
                {
                    flash_write(THRESHOLD_FLASH_ADDR, &g_td);
                    threshold_update_flag = 0;
                }

                oled_clear();
                // oled_update_screen();
                return;
            }

            if(menu_need_refresh == 1)
            {
                // oled_update_screen();
                menu_threshold_display(option_num);
                // threshold_update_flag = 0;
            }
            break;
        /////////////////////////////////////////////
        case 3://wifi配置页
            if(menu_need_refresh == 1)
            {
                menu_wifi_display(option_num);
                menu_need_refresh = 0;
            }

            if(key_num == 1)
            {
                key_num = 0;
                option_num++;
                if(option_num > 3) option_num = 1;

                menu_need_refresh = 1;
            }

            if(key_num == 2)
            {
                key_num = 0;
                option_num = 1;
                menu = 1;
                menu_need_refresh = 1;

                oled_clear();
            }
            
            if(key_num == 3)
            {
                key_num = 0;
                //配网 todo
            }
            
            
            break;
        ////////////////////////////////////////////
        case 4://wifi连接页
            break;
        ///////////////////////////////////////////
        default:
            break;
    }
}

static void app_handle_light_test()
{
    static uint32_t start_time = 0;
    if (HAL_GetTick() - start_time >= 500)
    {
        HAL_GPIO_TogglePin(LED_CTRL_GPIO_Port, LED_CTRL_Pin);
        start_time = HAL_GetTick();
    }
}

void app_init(void)
{
    console_init(&huart1);
    key_init();
    aht20_init();
    mq2_init();
    oled_init();
    threshold_data_init();
    tb6612_init();
}

void app_start(void)
{
    while (1) 
    {
        app_handle_light_test();
        
        app_switch_menu();

        app_warning_alert();

        HAL_Delay(10);      
     
    }
}