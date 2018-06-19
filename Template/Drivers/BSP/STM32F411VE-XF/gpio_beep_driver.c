/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户蜂鸣器底层驱动
  */ 

#include <includes.h>
#include "main.h"

/**
  * @brief  Configures BEEP GPIO.
  */
void BEEP_Init(void)
{

  BSP_GPIO_Init(BEEP_PORT, BEEP_PIN, BEEP_MODE, BEEP_PULL);

}

/**
  * @brief  beep开关设置.
  */
 void set_Beep(uint8_t state)
{ 
  HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}





