#ifndef _App_switch_control_h_include_
#define _App_switch_control_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  switch ¿ØÖÆ½Ó¿Ú
  * @{
  */
/*############################### xxxxx #######################################*/



void app_sw_blink_interface(uint8_t index, uint16_t number_blinks, uint8_t percent, uint16_t period);
uint32_t create_Switch_Task(void);











/**
  * @}
  */ 



#endif/*_App_switch_control_h_include_*/