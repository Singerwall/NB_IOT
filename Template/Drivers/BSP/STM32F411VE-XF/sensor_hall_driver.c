/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户hall传感器底层驱动
  */ 

#include <includes.h>
#include "main.h"




/**
  * @brief  Configures HALL GPIO 上拉输入.
  */
void HALL_Init(void)
{

  BSP_GPIO_Init(HALL_PORT, HALL_PIN, HALL_MODE, HALL_PULL);

}




