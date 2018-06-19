#ifndef _gpio_stepermotor_driver_h_include_
#define _gpio_stepermotor_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_stepermotor引脚分配
  * @{
  */
/*############################### xf_bsp_stepermotor引脚分配 #######################################*/


#define   INT1_PIN                       GPIO_PIN_5
#define   INT1_PORT                      GPIOA
#define   INT1_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   INT1_PULL                      GPIO_PULLDOWN/*下拉*/

#define   INT2_PIN                       GPIO_PIN_4
#define   INT2_PORT                      GPIOA
#define   INT2_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   INT2_PULL                      GPIO_PULLDOWN/*下拉*/

#define   INT3_PIN                       GPIO_PIN_7
#define   INT3_PORT                      GPIOA
#define   INT3_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   INT3_PULL                      GPIO_PULLDOWN/*下拉*/

#define   INT4_PIN                       GPIO_PIN_6
#define   INT4_PORT                      GPIOA
#define   INT4_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   INT4_PULL                      GPIO_PULLDOWN/*下拉*/

void steper_motor_Init(void);
void OwfsMotorCwTurnOneShoot(void);
void OwfsMotorCcwTurnOneShoot(void);
void control_moto_turn(u8 mode, float Coil);
void set_moto_turn( u8 on_off );










/**
  * @}
  */ 



#endif/*_gpio_stepermotor_driver_h_include_*/