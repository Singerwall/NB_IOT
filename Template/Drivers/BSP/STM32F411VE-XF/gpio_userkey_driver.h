#ifndef _gpio_userkey_driver_h_include_
#define _gpio_userkey_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>





/** @defgroup  用户按键引脚分配
  * @{
  */
/*############################### USER_KEYS #######################################*/
#define USER_KEY1_PIN                             GPIO_PIN_6
#define USER_KEY1_GPIO_PORT                       GPIOD

#define USER_KEY1_GPIO_IT_MODE                    GPIO_MODE_IT_FALLING
#define USER_KEY1_GPIO_PULL                       GPIO_PULLUP

#define USER_KEY1_EXTI_IRQn_PreemptPriority           2 
#define USER_KEY1_EXTI_IRQn_SubPriority               3

#define USER_KEY1_EXTI_IRQ                        EXTI9_5_IRQn
#define USER_KEY1_EXTI_IRQHandler                 EXTI9_5_IRQHandler

void USER_KEY_IT_Init(void);

/**
  * @}
  */ 
























#endif/*_gpio_userkey_driver_h_include_*/