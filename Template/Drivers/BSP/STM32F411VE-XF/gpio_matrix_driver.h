#ifndef _gpio_matrix_driver_h_include_
#define _gpio_matrix_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  矩阵按键引脚分配
  * @{
  */
/*############################### xxxxx #######################################*/

#define   KEYS_PORT                      GPIOD
/**行*/
#define   KEY0_PIN                       GPIO_PIN_8
#define   KEY1_PIN                       GPIO_PIN_9
#define   KEY2_PIN                       GPIO_PIN_10
#define   KEY3_PIN                       GPIO_PIN_11

#define   ROW_MODE                      GPIO_MODE_OUTPUT_PP/* 行 推挽输出*/
#define   ROW_PULL                      GPIO_PULLDOWN/*上拉*/

/**列*/
#define   KEY4_PIN                       GPIO_PIN_12
#define   KEY5_PIN                       GPIO_PIN_13
#define   KEY6_PIN                       GPIO_PIN_14
#define   KEY7_PIN                       GPIO_PIN_15

#define   COL_MODE                      GPIO_MODE_INPUT/* 列 上拉输入*/
#define   COL_PULL                      GPIO_PULLUP   /*上拉*/





void MATRIX_KEY_Init(void);
void show_key(void);








/**
  * @}
  */ 



#endif/*_gpio_matrix_driver_h_include_*/