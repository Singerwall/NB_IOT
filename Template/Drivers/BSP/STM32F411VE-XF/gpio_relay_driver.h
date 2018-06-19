#ifndef _gpio_relay_driver_h_include_
#define _gpio_relay_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_xxxx���ŷ���
  * @{
  */
/*############################### �̵��� #######################################*/



#define   RELAY_PIN                       GPIO_PIN_5
#define   RELAY_PORT                      GPIOC

#define   RELAY_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   RELAY_PULL                      GPIO_PULLDOWN/*����*/



void RELAY_Init(void);
void set_RELAY(uint8_t state);
uint8_t get_RELAY(void);














/**
  * @}
  */ 



#endif/*_gpio_relay_driver_h_include_*/