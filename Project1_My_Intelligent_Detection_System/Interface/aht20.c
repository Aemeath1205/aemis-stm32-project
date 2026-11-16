#include "aht20.h"
#include "log.h"

volatile aht20_data_t g_data;
volatile aht20_status_t g_status = STATUS_INIT_NY;
volatile uint32_t aht20_start_time;
volatile uint8_t g_ori_data[6] = {0};

//***** static func *****//
static void Send_Measure_Command(void)
{
    static uint8_t cmd[3] = {CMD_MEASURE_1, CMD_MEASURE_2, CMD_MEASURE_3};
    if(HAL_OK != HAL_I2C_Master_Transmit_DMA(&AHT20_I2C, AHT20_ADDRESS, cmd, 3))
    {
        LOG_DEBUG("Send_Measure_Command: Transmit cmd failed");
        return;
    }
    g_status = STATUS_SEND_MEASURE_COMMAND_ING;
}
static void Read_Measure_Data(void)
{
    if(HAL_OK != HAL_I2C_Master_Receive_DMA(&AHT20_I2C, AHT20_ADDRESS, g_ori_data, 6))
    {
        LOG_DEBUG("Read_Measure_Data: Receive data failed");
        return;
    }
    g_status = STATUS_READ_DATA_ING;
}
static void Parse_Data(void)
{
    if((g_ori_data[0] & 0x80) == 0x00)
    {
        //表示数据有效
        float humidity    = ((uint32_t)g_ori_data[1] << 12 )       |
                            ((uint32_t)g_ori_data[2] << 4)         |
                            ((uint32_t)g_ori_data[3] >> 4);
        float temperature = ((uint32_t)g_ori_data[3] & 0x0F) << 16 |
                            (uint32_t)g_ori_data[4] << 8           |
                            (uint32_t)g_ori_data[5];
        g_data.humidity    = (humidity * 100.0f) / (1 << 20);
        g_data.temperature = (temperature * 200.0f) / (1 << 20) - 50.0f;
        g_status = STATUS_PARSE_DATA_DONE;      
    }
    else
    {
        g_status = STATUS_INIT_DONE;
    }
}


//***** normal_func *****//
void Aht20_Init(void)
{
    HAL_Delay(40);
    uint8_t cmd1 = CMD_READ_STATUS;
    if(HAL_OK != HAL_I2C_Master_Transmit(&AHT20_I2C, AHT20_ADDRESS, &cmd1, 1, HAL_MAX_DELAY))
    {
        LOG_DEBUG("Aht20_Init: I2C Transmit failed");
        return;
    }
    uint8_t status = 0;
    if(HAL_OK != HAL_I2C_Master_Receive(&AHT20_I2C, AHT20_ADDRESS, &status, 1, HAL_MAX_DELAY))
    {
        LOG_DEBUG("Aht20_Init: I2C Receive failed");
        return;
    }
    if((status & 0x08) == 0)
    {
        uint8_t cmd2[3] = {CMD_INIT_1, CMD_INIT_2, CMD_INIT_3};
        if(HAL_OK != HAL_I2C_Master_Transmit(&AHT20_I2C, AHT20_ADDRESS, cmd2, 3, HAL_MAX_DELAY))
        {
            LOG_DEBUG("Aht20_Init: I2C Transmit Init_Cmd failed");
            return;        
        }
    }
    g_status = STATUS_INIT_DONE;
}

void Aht20_Start(void)
{
    if     (g_status == STATUS_INIT_NY) Aht20_Init();
    else if(g_status == STATUS_INIT_DONE) Send_Measure_Command();
    else if(g_status == STATUS_SEND_MEASURE_COMMAND_DONE) 
    {
        if(HAL_GetTick() - aht20_start_time >= 75) g_status = STATUS_WAIT_MEASURE_DONE;
    }
    else if(g_status == STATUS_WAIT_MEASURE_DONE) Read_Measure_Data();
    else if(g_status == STATUS_READ_DATA_DONE) Parse_Data();
    else if(g_status == STATUS_PARSE_DATA_DONE)
    {
        printf(">>> %.2f %.2f\r\n", g_data.temperature, g_data.humidity);
        g_status = STATUS_INIT_DONE;
    }
}