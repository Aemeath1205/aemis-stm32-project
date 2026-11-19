#pragma once
#include "i2c.h"
#include "log.h"

#define AHT20_I2C     hi2c2
#define AHT20_ADDRESS (0x38 << 1)

typedef struct 
{
    float humidity;    //湿度数据
    float temperature; //温度数据
}aht20_data_t;


typedef enum
{
    READ_DATA_INIT,               //空闲状态
    SEND_MEASURE_COMMAND_ING,     //发送测量命令中间态
    SEND_MEASURE_COMMAND_FINISH,  //测量命令发送完成态
    WAIT_MEASURE_FINISH,          //测量完成态
    READ_DATA_ING,                //读取数据中间态
    READ_DATA_FINISH,             //读取数据完成态
    DATA_PARSE_FINISH             //更新显⽰或存储数据
  //READ_DATA_INIT,               //回到空闲状态  
}aht20_status_t;

extern volatile aht20_status_t aht20_status;
extern volatile uint8_t        read_buffer[6];
extern volatile aht20_data_t   aht20_data;
extern volatile uint32_t       measure_start_tick;


//阻塞
/**
 * @brief AHT20初始化
 * 
 */
void aht20_init(void);

/**
 * @brief AHT20读取
 * 
 * @param data 
 */
void aht20_read_data(aht20_data_t* data);

//非阻塞DMA
/*发送测量命令*/
void aht20_send_measure_command(void);
/*读取数据*/
void aht20_receive_data(void);
/*解析数据*/
void aht20_data_parse(void);

/**
 * @brief 最为关键，AHT20的启动任务窗口
 * 
 */
void aht20_start(void);

