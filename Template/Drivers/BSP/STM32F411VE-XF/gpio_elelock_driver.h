#ifndef _gpio_elelock_driver_h_include_
#define _gpio_elelock_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_��������ŷ���
  * @{
  */
/*############################### ����� #######################################*/

#define   LOCK_PIN                       GPIO_PIN_8
#define   LOCK_PORT                      GPIOC

#define   LOCK_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   LOCK_PULL                      GPIO_PULLDOWN/*����*/



void Electronic_lock_init(void);    /*�������ڵ��ʼ��*/
void Electronic_Lock_Open(void);
void Electronic_Lock_Close(void);
u8    Electronic_Lock_get_state(void);
/**
  * @}
  */ 
#endif/*_gpio_elelock_driver_h_include_*/