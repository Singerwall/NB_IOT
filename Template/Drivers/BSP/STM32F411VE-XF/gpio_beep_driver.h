#ifndef _gpio_beep_driver_h_include_
#define _gpio_beep_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_���������ŷ���
  * @{
  */
/*############################### xxxxx #######################################*/








#define   BEEP_PIN                       GPIO_PIN_4
#define   BEEP_PORT                      GPIOD

#define   BEEP_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   BEEP_PULL                      GPIO_PULLDOWN/*����*/



void BEEP_Init(void);
void set_Beep(uint8_t state);


/**
  * @}
  */ 



#endif/*_gpio_beep_driver_h_include_*/