#ifndef _gpio_elelock_driver_h_include_
#define _gpio_elelock_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_电磁锁引脚分配
  * @{
  */
/*############################### 电磁锁 #######################################*/

#define   LOCK_PIN                       GPIO_PIN_8
#define   LOCK_PORT                      GPIOC

#define   LOCK_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   LOCK_PULL                      GPIO_PULLDOWN/*下拉*/



void Electronic_lock_init(void);    /*电子锁节点初始化*/
void Electronic_Lock_Open(void);
void Electronic_Lock_Close(void);
u8    Electronic_Lock_get_state(void);
/**
  * @}
  */ 
#endif/*_gpio_elelock_driver_h_include_*/