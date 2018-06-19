#ifndef _gpio_powctrl_driver_h_include_
#define _gpio_powctrl_driver_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_各模块电源控制引脚引脚分配
  * @{
  */
/*############################### xxxxx #######################################*/


typedef enum
{
  WSN1           =0,
  WSN2           =1,
  RFID1          =2,
  RFID2          =3,
  LCD            =4,
  GPS            =5,
  BL             =6,
}Pow_Ctrl_Device_TypeDef;





#define   WSN1_POW_Ctrl_PIN                       GPIO_PIN_3
#define   WSN1_POW_Ctrl_PORT                      GPIOE

#define   WSN2_POW_Ctrl_PIN                       GPIO_PIN_4
#define   WSN2_POW_Ctrl_PORT                      GPIOC

#define   RFID1_POW_Ctrl_PIN                      GPIO_PIN_15
#define   RFID1_POW_Ctrl_PORT                     GPIOE

#define   RFID2_POW_Ctrl_PIN                      GPIO_PIN_12
#define   RFID2_POW_Ctrl_PORT                     GPIOA

#define   LCD_POW_Ctrl_PIN                        GPIO_PIN_13
#define   LCD_POW_Ctrl_PORT                       GPIOD

#define   GPS_POW_Ctrl_PIN                        GPIO_PIN_7
#define   GPS_POW_Ctrl_PORT                       GPIOD

#define   BL_POW_Ctrl_PIN                         GPIO_PIN_3
#define   BL_POW_Ctrl_PORT                        GPIOB

#define   POW_Ctrl_MODE                           GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   POW_Ctrl_PULL                           GPIO_PULLUP/**/



void PowCtrl_Init(void);
void control_device_pow(Pow_Ctrl_Device_TypeDef index, uint8_t state);
void off_all_POW_Ctrl(void);
void on_all_POW_Ctrl(void);




/**
  * @}
  */ 



#endif/*_gpio_powctrl_driver_h_include_*/