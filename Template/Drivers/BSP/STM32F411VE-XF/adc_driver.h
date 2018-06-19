#ifndef __adc_driver_H
#define __adc_driver_H
#include <delay.h>



/*################################ ADC1 ######################################*/
/**
  * @brief  ADC Interface pins
  *         used to detect motion of Joystick available on Adafruit 1.8" TFT shield
  */
  

#define  ADCx                               ADC1
#define  ADCx_CLK_ENABLE()                  __HAL_RCC_ADC1_CLK_ENABLE()
#define  ADCx_CLK_DISABLE()                 __HAL_RCC_ADC1_CLK_DISABLE()


/**********wsn2  ����*********/
#define  xxxx_ADCx_CHANNEL                   ADC_CHANNEL_9/*����ͨ��*/
   
#define  xxxx_ADCx_GPIO_PORT                 GPIOB
#define  xxxx_ADCx_GPIO_PIN                  GPIO_PIN_1
#define  xxxx_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define  xxxx_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()

    
    
/********** wsn1  ����ʪ�� /���� ����һ��PIN *********/ 
#define  HR31_ADCx_CHANNEL                   ADC_CHANNEL_10/*�¶�ͨ��*/
   
#define  HR31_ADCx_GPIO_PORT                 GPIOC
#define  HR31_ADCx_GPIO_PIN                  GPIO_PIN_0
#define  HR31_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define  HR31_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

    
/********** MCU_ADC_Change ϵͳ��ѹ*********/
      
#define  MCU_ADCx_CHANNEL                   ADC_CHANNEL_11/*����ͨ��*/
   
#define  MCU_ADCx_GPIO_PORT                 GPIOC
#define  MCU_ADCx_GPIO_PIN                  GPIO_PIN_1
#define  MCU_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define  MCU_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

       
    
/********** ��ص�ѹ��� *********/
#define  BAT_ADCx_CHANNEL                   ADC_CHANNEL_12/*����ͨ��*/
   
#define  BAT_ADCx_GPIO_PORT                 GPIOC
#define  BAT_ADCx_GPIO_PIN                  GPIO_PIN_2
#define  BAT_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define  BAT_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()

    
/********** CURRENT_ADC ϵͳ����*********/
      
#define  CURRENT_ADCx_CHANNEL                   ADC_CHANNEL_13/*����ͨ��*/
   
#define  CURRENT_ADCx_GPIO_PORT                 GPIOC
#define  CURRENT_ADCx_GPIO_PIN                  GPIO_PIN_3
#define  CURRENT_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define  CURRENT_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

    
    
/**
  * @}
  */

void MY_ADC_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(u32 ch); 		        //���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u32 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ
#endif /*__adc_driver_H*/
