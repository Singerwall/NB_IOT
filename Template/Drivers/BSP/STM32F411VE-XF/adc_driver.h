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


/**********wsn2  保留*********/
#define  xxxx_ADCx_CHANNEL                   ADC_CHANNEL_9/*保留通道*/
   
#define  xxxx_ADCx_GPIO_PORT                 GPIOB
#define  xxxx_ADCx_GPIO_PIN                  GPIO_PIN_1
#define  xxxx_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define  xxxx_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()

    
    
/********** wsn1  空气湿度 /电流 公用一个PIN *********/ 
#define  HR31_ADCx_CHANNEL                   ADC_CHANNEL_10/*温度通道*/
   
#define  HR31_ADCx_GPIO_PORT                 GPIOC
#define  HR31_ADCx_GPIO_PIN                  GPIO_PIN_0
#define  HR31_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define  HR31_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

    
/********** MCU_ADC_Change 系统电压*********/
      
#define  MCU_ADCx_CHANNEL                   ADC_CHANNEL_11/*保留通道*/
   
#define  MCU_ADCx_GPIO_PORT                 GPIOC
#define  MCU_ADCx_GPIO_PIN                  GPIO_PIN_1
#define  MCU_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define  MCU_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

       
    
/********** 电池电压检测 *********/
#define  BAT_ADCx_CHANNEL                   ADC_CHANNEL_12/*保留通道*/
   
#define  BAT_ADCx_GPIO_PORT                 GPIOC
#define  BAT_ADCx_GPIO_PIN                  GPIO_PIN_2
#define  BAT_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define  BAT_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()

    
/********** CURRENT_ADC 系统电流*********/
      
#define  CURRENT_ADCx_CHANNEL                   ADC_CHANNEL_13/*保留通道*/
   
#define  CURRENT_ADCx_GPIO_PORT                 GPIOC
#define  CURRENT_ADCx_GPIO_PIN                  GPIO_PIN_3
#define  CURRENT_ADCx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define  CURRENT_ADCx_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

    
    
/**
  * @}
  */

void MY_ADC_Init(void); 				//ADC通道初始化
u16  Get_Adc(u32 ch); 		        //获得某个通道值 
u16 Get_Adc_Average(u32 ch,u8 times);//得到某个通道给定次数采样的平均值
#endif /*__adc_driver_H*/
