/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û�LEDS�ײ�����
  */ 

#include <includes.h>
#include "main.h"

/**
  * @brief  Configures LED GPIO.
  */
void LEDs_Init(void)
{

  BSP_GPIO_Init(LED1_GPIO_PORT, LED1_PIN|LED2_PIN|LED3_PIN, LEDS_MODE, LEDS_PULL);

}

/**
  * @brief  led1��������.
  */
 void set_Led1(uint8_t state)
{ 
  HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}
/**
  * @brief  led2��������.
  */
 void set_Led2(uint8_t state)
{
  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, ((state == ON )? GPIO_PIN_SET : GPIO_PIN_RESET));
}
/**
  * @brief  led3��������.
  */
 void set_Led3(uint8_t state)
{
  HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, ((state == ON) ? GPIO_PIN_SET : GPIO_PIN_RESET));
}



/**
  * @brief LED���ƽӿ�
 */
void control_Led(Led_Index_TypeDef index, uint8_t state)
{
    switch(index)
    {
    case LED_1:
        set_Led1( state );
        break;
    case LED_2:
        set_Led2( state );
        break;   
    case LED_3:
        set_Led3( state );
        break;  
    default:
        assert_param(0);
    }
}
/**
  * @brief LEDȫ��
 */
void off_Leds(void)
{
    int index;
    for(index = 1; index <= LED_NUMBER; ++index)
    {
        control_Led((Led_Index_TypeDef)index, OFF);
    }
}

/**
  * @brief LEDȫ��
 */
void on_Leds(void)
{
    int index;
    for(index = 1; index <= LED_NUMBER; ++index)
    {
        control_Led((Led_Index_TypeDef)index, ON);
    }
}






