#ifndef _timer2_servo_driver_h_include_
#define _timer2_servo_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_servo 引脚分配
  * @{
  */
/*############################### 舵机 #######################################*/



#define   SERVO_TIM           		  TIM2
#define   SERVO_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE();
#define   SERVO_CLK_DISABLE()             __HAL_RCC_TIM2_CLK_DISABLE();

#define   SERVO_CHANNEL_X                  TIM_CHANNEL_3


#define   SERVO_PIN                       GPIO_PIN_10
#define   SERVO_PORT                      GPIOB
#define   SERVO_MODE                      GPIO_MODE_AF_PP/*复用推挽输出*/
#define   SERVO_PULL                      GPIO_PULLUP/*上拉*/
#define   SERVO_PIN_AF                    GPIO_AF1_TIM2/*上拉*/
#define   SERVO_PIN_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE();

void SERVO_TIM_PWM_Init(void);  
int get_servo_angle(void);
void set_servo_angle(int angle);












/**
  * @}
  */ 



#endif/*_timer2_servo_driver_h_include_*/