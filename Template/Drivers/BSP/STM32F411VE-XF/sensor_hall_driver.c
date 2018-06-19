/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û�hall�������ײ�����
  */ 

#include <includes.h>
#include "main.h"




/**
  * @brief  Configures HALL GPIO ��������.
  */
void HALL_Init(void)
{

  BSP_GPIO_Init(HALL_PORT, HALL_PIN, HALL_MODE, HALL_PULL);

}




