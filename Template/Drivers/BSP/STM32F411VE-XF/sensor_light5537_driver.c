/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户光敏传感器底层驱动
  */ 

#include <includes.h>
#include "main.h"




/**
  * @brief  Configures LIGHT5537 GPIO 上拉输入它与.
  */
void LIGHT5537_Init(void)
{
 BSP_GPIO_Init(LIGHT5537_PORT, LIGHT5537_PIN, LIGHT5537_MODE, LIGHT5537_PULL);
}


/**
  * @brief  读IO.
  */
u8 LIGHT5537_SENSOR_IN(void)
{

  return  HAL_GPIO_ReadPin(LIGHT5537_PORT, LIGHT5537_PIN);
  
}