/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_electricity.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 电流传感器驱动程序
  */
#include "xf_sensor_electricity.h"
#include "xf_uart0_driver.h"
#include "xf_adc.h"

/* Adc采集电流传感器电压 */
/**
  * @brief ADC采集电流传感器电压函数
  * @details 
  * @param NULL
  * @param NULL
  * @param NULL
  * @return vcc 电压值
 **/
float AdcElectricitySenorVolt(void)
{
    float Vcc;
    Vcc = XfZstackAdcChangeVcc(SEL_ADC_CHANNEL_0, SEL_ADC_RESOLUTION_14, SEL_ADC_REF_AVDD);
    return Vcc;
}
/**
  * @brief 获取电流值
  * @details 
  * @param NULL
  * @param NULL
  * @param NULL
  * @return value 电流值
 **/
float XfGetElectricValue(void)
{
    float value;
    value = AdcElectricitySenorVolt();
#ifdef UART0_DEBUG
    uart0_printf("电压 = %.3fV\r\n",value);//串口打印数据
#endif   
    return value;
}