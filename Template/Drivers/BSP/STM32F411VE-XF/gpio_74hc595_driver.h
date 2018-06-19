#ifndef _gpio_74hc595_driver_h_include_
#define _gpio_74hc595_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_数码管引脚分配
  * @{
  */
/*############################### xxxxx #######################################*/




/**@brief 串行数据输入*/
#define   DIO_PIN                       GPIO_PIN_1
#define   DIO_PORT                      GPIOD
#define   DIO_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   DIO_PULL                      GPIO_PULLDOWN/*下拉*/
/**@brief 时钟脉冲信号-上升沿有效*/
#define   RCLK_PIN                       GPIO_PIN_2
#define   RCLK_PORT                      GPIOD
#define   RCLK_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   RCLK_PULL                      GPIO_PULLDOWN/*下拉*/
/**@brief 打入信号-上升沿有效*/
#define   SCLK_PIN                       GPIO_PIN_0
#define   SCLK_PORT                      GPIOD
#define   SCLK_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   SCLK_PULL                      GPIO_PULLDOWN/*下拉*/








/**
  * @}
  */ 



#endif/*_gpio_74hc595_driver_h_include_*/