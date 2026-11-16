#include "aht20.h"

volatile aht20_status_t aht20_status = READ_DATA_INIT;
volatile uint8_t        read_buffer[6] = {0};
volatile aht20_data_t   aht20_data;
volatile uint32_t       measure_start_tick = 0;

/**
 * @brief AHT20初始化
 * 
 */
void aht20_init(void)
{
    HAL_Delay(40);
    uint8_t status_cmd = 0x71;
    if(HAL_OK != HAL_I2C_Master_Transmit(&AHT20_I2C, AHT20_ADDRESS, &status_cmd, 1, HAL_MAX_DELAY))
    {
        LOG_DEBUG("AHT20 send command failed\n");
        return;
    }
    uint8_t status_data = 0;
    if(HAL_OK != HAL_I2C_Master_Receive(&AHT20_I2C, AHT20_ADDRESS, &status_data, 1, HAL_MAX_DELAY))
    {
        LOG_DEBUG("AHT20 receive command failed\n");
        return;
    }
    if((status_data & 0x08) == 0x00)
    {
        uint8_t init_cmd[3] = {0xBE, 0X08, 0X00};
        if(HAL_OK != HAL_I2C_Master_Transmit(&AHT20_I2C, AHT20_ADDRESS, init_cmd, 3, HAL_MAX_DELAY))
        {
            LOG_DEBUG("AHT20 send initialization command failed\n");
            return;
        }
    }
}

/**
 * @brief AHT20读取
 * 
 * @param data 
 */
void aht20_read_data(aht20_data_t* data)
{
    uint8_t measure_cmd[3] = {0xAC, 0x33, 0x00};
    if(HAL_OK != HAL_I2C_Master_Transmit(&AHT20_I2C, AHT20_ADDRESS, measure_cmd, 3, HAL_MAX_DELAY))
    {
        LOG_DEBUG("AHT20 send measurement command failed\n");
        return;
    }

    HAL_Delay(75);
    uint8_t receive_buffer[6] = {0};
    if(HAL_OK != HAL_I2C_Master_Receive(&AHT20_I2C, AHT20_ADDRESS, receive_buffer, 6, HAL_MAX_DELAY))
    {
        LOG_DEBUG("AHT20 receive data failed\n");
        return;
    }

    if((receive_buffer[0] & 0x80) == 0x00)
    {
        //表示数据有效
        float humidity    = ((uint32_t)receive_buffer[1] << 12 )       |
                            ((uint32_t)receive_buffer[2] << 4)         |
                            ((uint32_t)receive_buffer[3] >> 4);
        float temperature = ((uint32_t)receive_buffer[3] & 0x0F) << 16 |
                            (uint32_t)receive_buffer[4] << 8           |
                            (uint32_t)receive_buffer[5];
        data->humidity    = (humidity * 100.0f) / (1 << 20);
        data->temperature = (temperature * 200.0f) / (1 << 20) - 50.0f;
    }
}

/*发送测量命令*/
void aht20_send_measure_command(void)
{
    static uint8_t measure_cmd[3] = {0xAC, 0x33, 0x00};
    if(HAL_OK != HAL_I2C_Master_Transmit_DMA(&AHT20_I2C, AHT20_ADDRESS, measure_cmd, 3))
    {
        LOG_DEBUG("aht20 send measure command failed\n");
        return;
    }
}
/*读取数据*/
void aht20_receive_data(void)
{
    if(HAL_OK != HAL_I2C_Master_Receive_DMA(&AHT20_I2C, AHT20_ADDRESS, read_buffer, 6))
    {
        LOG_DEBUG("aht20 receive data failed\n");
        return;       
    }
}
/*解析数据*/
void aht20_data_parse(void)
{
    if((read_buffer[0] & 0x80) == 0x00)
    {
        //表示数据有效
        float humidity    = ((uint32_t)read_buffer[1] << 12 )       |
                            ((uint32_t)read_buffer[2] << 4)         |
                            ((uint32_t)read_buffer[3] >> 4);
        float temperature = ((uint32_t)read_buffer[3] & 0x0F) << 16 |
                            (uint32_t)read_buffer[4] << 8           |
                            (uint32_t)read_buffer[5];
        aht20_data.humidity    = (humidity * 100.0f) / (1 << 20);
        aht20_data.temperature = (temperature * 200.0f) / (1 << 20) - 50.0f;
        aht20_status = DATA_PARSE_FINISH;      
    }
    else
    {
        aht20_status = READ_DATA_INIT;
    }       
}

/**
 * @brief 最为关键，AHT20的启动任务窗口
 * 
 */
void aht20_start(void)
{
    if(aht20_status == READ_DATA_INIT)
    {
        aht20_send_measure_command();
        aht20_status = SEND_MEASURE_COMMAND_ING;    
    }
    else if(aht20_status == SEND_MEASURE_COMMAND_FINISH)
    {
        if(HAL_GetTick() - measure_start_tick >= 75)
        {
            aht20_status = WAIT_MEASURE_FINISH;
        }
    }
    else if(aht20_status == WAIT_MEASURE_FINISH)
    {
        aht20_receive_data();
        aht20_status = READ_DATA_ING;
    }
    else if(aht20_status == READ_DATA_FINISH)
    {
        aht20_data_parse();
    }
    else if(aht20_status == DATA_PARSE_FINISH)
    {
        printf(">>> %f %f\n", aht20_data.humidity, aht20_data.temperature);
        aht20_status = READ_DATA_INIT;
    }
}

