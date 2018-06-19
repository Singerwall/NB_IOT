/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户继电器底层驱动
  */ 

#include <includes.h>
#include "main.h"
/**
  * @brief  Configures RELAY GPIO.
  */
void RELAY_Init(void)
{

  BSP_GPIO_Init(RELAY_PORT, RELAY_PIN, RELAY_MODE, RELAY_PULL);

}

/**
  * @brief  relay开关设置.
  */
 void set_RELAY(uint8_t state)
{ 
  HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}





/**
  * @brief  获取relay状态.
  */
uint8_t get_RELAY(void)
{ 
  return HAL_GPIO_ReadPin(RELAY_PORT, RELAY_PIN);
}




