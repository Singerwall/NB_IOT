#ifndef _gpio_leds_driver_h_include_
#define _gpio_leds_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/** @defgroup led检索号
  * @{
  */
typedef enum
{
  LED_1        = 0,
  LED_2        = 1,
  LED_3        = 2,
  BEEP         =3
} Led_Index_TypeDef;


  /**  @brief 开关状态*/
typedef enum
{
  OFF          =0,
  ON           =1

}State_TypeDef;



/**
  * @}
  */

/** @defgroup  xf_bsp_ldes引脚分配
  * @{
  */
/*############################### LEDS #######################################*/
#define LED1_PIN                                GPIO_PIN_4
#define LED1_GPIO_PORT                          GPIOB

#define LED2_PIN                                GPIO_PIN_5
#define LED2_GPIO_PORT                          GPIOB

#define LED3_PIN                                GPIO_PIN_6
#define LED3_GPIO_PORT                          GPIOB

#define LEDS_MODE                               GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define LEDS_PULL                               GPIO_NOPULL/**/

#define LED_NUMBER                              3

void LEDs_Init(void);
void set_Led1(uint8_t state);
void set_Led2(uint8_t state);
void set_Led3(uint8_t state);
void control_Led(Led_Index_TypeDef index, uint8_t state);
void off_Leds(void);
void on_Leds(void);

/**
  * @}
  */ 



#endif/*_gpio_leds_driver_h_include_*/