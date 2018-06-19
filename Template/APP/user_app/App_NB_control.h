#ifndef _App_NB_control_h_include_
#define _App_NB_control_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  switch ¿ØÖÆ½Ó¿Ú
  * @{
  */
/*############################### xxxxx #######################################*/


uint32_t create_NB_Task(void);

int app_transmit_packet( uint16_t msg_num,
                         uint16_t node_type,
                         uint16_t device_type,
                         uint16_t cmd,
                         uint16_t ack,
                         uint16_t data_size,
                         uint8_t *p);









/**
  * @}
  */ 



#endif/*_App_NB_control_h_include_*/