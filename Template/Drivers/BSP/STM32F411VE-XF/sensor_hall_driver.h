#ifndef _sensor_hall_driver_h_include_
#define _sensor_hall_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_»ô¶ûÒý½Å·ÖÅä
  * @{
  */
/*############################### hall#######################################*/



#define   HALL_PIN                      GPIO_PIN_13
#define   HALL_PORT                     GPIOC
#define   HALL_MODE                     GPIO_MODE_INPUT
#define   HALL_PULL                     GPIO_PULLUP

/**
  * @brief  ¶ÁIO.
  */
#define HALL_SENSOR_IN()	 	((HALL_PORT->IDR&HALL_PIN)>>13)


void HALL_Init(void);


/**
  * @}
  */ 



#endif/*_sensor_hall_driver_h_include_*/