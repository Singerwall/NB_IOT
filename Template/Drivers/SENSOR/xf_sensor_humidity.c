/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_humidity.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 湿度传感器驱动程序
  */
#include "xf_sensor_humidity.h"
#ifdef CC2540
  #include "iocc2540.h"
#else
  #include "iocc2530.h"
#endif
#include "xf_adc.h"
#include "xf_uart0_driver.h"

#define BIT(X)                   (1 << X)
#define HUMIDITY_OUT_DATA_PIN_BIT BIT(7)
#define HUMIDITY_DIR              P1DIR
#define HUMIDITY_IO               P1
#define HUMIDITY_PIN              P1_7


enum
{
    HUMIDITY_NO = 0,
    HUMIDITY_YES
};

/* 检测湿度传感器数字引脚电平 */
/**
  * @brief  检测湿度传感器的数字引脚的电平  
  * @details 
  * @param  NULL
  * @param  NULL
  * @return HUMIDITY_YES：引脚为高
            HUMIDITY_NO：引脚为低
 **/
uint8 DetectionHumidDoutPinLevel(void)
{
    HUMIDITY_DIR &= ~HUMIDITY_OUT_DATA_PIN_BIT;
    if(HUMIDITY_PIN)
    {
        return HUMIDITY_YES;
    }
    else
    {
        return HUMIDITY_NO;
    }
}


/* 湿度转换 */
/**
  * @brief  湿度转换  
  * @details 将采集到的电压值转换为湿度值
  * @param  NULL
  * @param  NULL
  * @return humid 返回湿度值
 **/
float HumidChange(void)
{
    float vcc;
    float r_val;
    float humid;
    
    /* 把电压值转换成相应的电阻值 */
    vcc = XfZstackAdcChangeVcc(SEL_ADC_CHANNEL_0, SEL_ADC_RESOLUTION_14, SEL_ADC_REF_AVDD);
    
    /* 说明：根据电阻串联分压算出电阻值 */
    r_val = (100*vcc)/(3.4 - vcc);
    
    /* 把电阻值转换成对应的湿度 */
    humid = 100 - 0.01*r_val;
    
    if(humid < 0)
       humid = 0;
    
    return humid;
}
/**
  * @brief  获取湿度值
  * @details 
  * @param  NULL
  * @param  NULL
  * @return humid 返回湿度值
 **/
float XfGetHumid(void)
{
  float humid;
  humid=HumidChange();
#ifdef UART0_DEBUG
  uart0_printf("humid = %0.2fRH\r\n",humid);
#endif
  return humid;
}