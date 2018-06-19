/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  
  * @author  zp.h
  * @version  V3.0.0
  * @date  05/16/2018
  * @brief  电子锁驱动
  */ 

#include <includes.h>
/**
  * @brief	电子锁初始化函数
  * @details	配置端口时钟、IO、模式等参数
  * @param NULL	无
  * @retval 	无
  */
void Electronic_lock_init(void)
{
  BSP_GPIO_Init(LOCK_PORT, LOCK_PIN, LOCK_MODE, LOCK_PULL);	/*初始状态电子锁关闭*/

}


/**
  * @brief	电磁锁开启函数
  * @details	对引脚置位打开电磁锁
  * @param NULL	无
  * @retval  NULL	无
  */
void Electronic_Lock_Open(void)
{
 HAL_GPIO_WritePin(LOCK_PORT, LOCK_PIN, GPIO_PIN_SET);
  OSTimeDly (1000); 
 HAL_GPIO_WritePin(LOCK_PORT, LOCK_PIN, GPIO_PIN_RESET);
}

/**
  * @brief	电磁锁关闭函数
  * @details	对引脚复位关闭电磁锁
  * @param NULL	无
  * @param NULL	无
  * @retval 	无
  */
void Electronic_Lock_Close(void)
{
   HAL_GPIO_WritePin(LOCK_PORT, LOCK_PIN, GPIO_PIN_RESET);
}
/**
  * @brief	获取电磁锁状态
  * @details	获取电磁锁状态
  * @param NULL	无
  * @param NULL	无
  * @retval 	无
  */
u8 Electronic_Lock_get_state(void)
{
   return HAL_GPIO_ReadPin(LOCK_PORT, LOCK_PIN);
}




