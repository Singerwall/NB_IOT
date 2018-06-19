/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_light.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 光电传感器驱动程序
  */
#include "xf_sensor_light.h"
#ifdef CC2540
  #include "iocc2540.h"
#else
  #include "iocc2530.h"
#endif
#include "xf_adc.h"
#include "xf_uart0_driver.h"

/* ADC采集光照强度模块模拟输出端电压 */
/**
  * @brief  ADC采集  
  * @details 光照传感器模块模拟输出端的电压值
  * @param  NULL
  * @param  NULL
  * @return Vcc 返回采集到的电压值
 **/
float AdcLightAoutVoltage(void)
{
    float Vcc;
    Vcc = XfZstackAdcChangeVcc(SEL_ADC_CHANNEL_0, SEL_ADC_RESOLUTION_14, SEL_ADC_REF_AVDD);
    return Vcc;
}

/* 把对应的电压转换成光照强度 */
/**
  * @brief  电压值转换光照值  
  * @details 光照传感器模块模拟输出端的电压值
  * @param  NULL
  * @param  NULL
  * @return LightStreng 返回光照值
 **/
uint16 VccChangeLightStrength(void)
{
    float LightStreng;
    LightStreng = AdcLightAoutVoltage();
    LightStreng =1500 - (float)1500*(LightStreng/3.3);
    return (uint16)LightStreng;
}

/**
  * @brief  获取光照值函数  
  * @details 
  * @param  NULL
  * @param  NULL
  * @return LightValue 返回光照值
 **/
uint16 XfGetLight(void)
{
  uint16 LightValue;
  LightValue = VccChangeLightStrength();
#ifdef UART0_DEBUG  
  uart0_printf("光照值：%d\r\n",LightValue);
#endif
  return LightValue;
}