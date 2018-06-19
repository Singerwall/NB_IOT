#ifndef _gpio_74hc595_driver_h_include_
#define _gpio_74hc595_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_��������ŷ���
  * @{
  */
/*############################### xxxxx #######################################*/




/**@brief ������������*/
#define   DIO_PIN                       GPIO_PIN_1
#define   DIO_PORT                      GPIOD
#define   DIO_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   DIO_PULL                      GPIO_PULLDOWN/*����*/
/**@brief ʱ�������ź�-��������Ч*/
#define   RCLK_PIN                       GPIO_PIN_2
#define   RCLK_PORT                      GPIOD
#define   RCLK_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   RCLK_PULL                      GPIO_PULLDOWN/*����*/
/**@brief �����ź�-��������Ч*/
#define   SCLK_PIN                       GPIO_PIN_0
#define   SCLK_PORT                      GPIOD
#define   SCLK_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   SCLK_PULL                      GPIO_PULLDOWN/*����*/








/**
  * @}
  */ 



#endif/*_gpio_74hc595_driver_h_include_*/