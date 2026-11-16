#include "i2c_isr.h"
#include "i2c.h"
#include "aht20.h"

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c == &hi2c2)
    {
        g_status = STATUS_SEND_MEASURE_COMMAND_DONE;
        aht20_start_time = HAL_GetTick();
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == &hi2c2)
    {
        g_status = STATUS_READ_DATA_DONE;   
    }
    
}

