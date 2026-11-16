#include "i2c.h"
#include "aht20.h"

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)  
{
    if(hi2c == &AHT20_I2C)
    {
        aht20_status = SEND_MEASURE_COMMAND_FINISH;
        measure_start_tick = HAL_GetTick();
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c == &AHT20_I2C)
    {
        aht20_status = READ_DATA_FINISH;
    }
}
