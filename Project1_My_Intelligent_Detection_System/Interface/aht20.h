#pragma once
#include "i2c.h"

//***** define *****//
#define CMD_READ_STATUS 0x71

#define CMD_INIT_1      0xBE
#define CMD_INIT_2      0x08
#define CMD_INIT_3      0x00

#define CMD_MEASURE_1      0xAC
#define CMD_MEASURE_2      0x33
#define CMD_MEASURE_3      0x00

#define AHT20_ADDRESS   (0x38 << 1)
#define AHT20_I2C       hi2c2

//***** struct & enum *****//
typedef struct
{
    float temperature;
    float humidity;
}aht20_data_t;

typedef enum
{
    STATUS_INIT_NY,
    STATUS_INIT_DONE,
    STATUS_SEND_MEASURE_COMMAND_ING,
    STATUS_SEND_MEASURE_COMMAND_DONE,
    STATUS_WAIT_MEASURE_DONE,
    // STATUS_MEASURE_DONE,
    STATUS_READ_DATA_ING,
    STATUS_READ_DATA_DONE,
    STATUS_PARSE_DATA_DONE
    // STATUS_INIT_DONE
}aht20_status_t;

//***** func *****//
void Aht20_Init(void);
void Aht20_Start(void);


extern volatile aht20_data_t g_data;
extern volatile aht20_status_t g_status;
extern volatile uint32_t aht20_start_time;
// extern volatile uint8_t g_ori_data[6];