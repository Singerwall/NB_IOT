#ifndef _sensor_light5537_driver_h_include_
#define _sensor_light5537_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_π‚’’“˝Ω≈∑÷≈‰
  * @{
  */
/*############################### hall#######################################*/



#define   LIGHT5537_PIN                      GPIO_PIN_0
#define   LIGHT5537_PORT                     GPIOC
#define   LIGHT5537_MODE                     GPIO_MODE_INPUT
#define   LIGHT5537_PULL                     GPIO_PULLUP





void LIGHT5537_Init(void);
u8 LIGHT5537_SENSOR_IN(void);

/**
  * @}
  */ 



#endif/*_sensor_light5537_driver_h_include_*/