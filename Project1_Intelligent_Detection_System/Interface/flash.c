#include "flash.h"
#include "log.h"
#include "string.h"

void flash_write(uint32_t addr, threshold_t* data)
{
    //解锁
    HAL_FLASH_Unlock();

    //擦除
    uint32_t page_error = 0;

    FLASH_EraseInitTypeDef erase_init_struct;
    erase_init_struct.TypeErase   = FLASH_TYPEERASE_PAGES;
    erase_init_struct.PageAddress = addr;
    erase_init_struct.NbPages     = 1;

    if(HAL_OK != HAL_FLASHEx_Erase(&erase_init_struct, &page_error))
    {
        HAL_FLASH_Lock();
        LOG_DEBUG("flash_write: flash erase failed");
        return;
    }

    //写入
    uint16_t buf[4];
    buf[0] = data->temp_threshold;
    buf[1] = data->hum_threshold;
    buf[2] = data->mq2_threshold;
    buf[3] = data->fan_threshold;

    for(uint8_t i = 0; i < 4; i++)
    {
        if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i*2, buf[i]))
        {
            HAL_FLASH_Lock();
            LOG_DEBUG("flash_write: flash program failed");
            return;
        }
    }

    //最后加锁
    HAL_FLASH_Lock();
}

void flash_read(uint32_t addr, threshold_t* data)
{
    memcpy(data, (void*)addr, sizeof(threshold_t));
}