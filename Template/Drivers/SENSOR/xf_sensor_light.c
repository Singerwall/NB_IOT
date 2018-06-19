/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_light.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 ��紫������������
  */
#include "xf_sensor_light.h"
#ifdef CC2540
  #include "iocc2540.h"
#else
  #include "iocc2530.h"
#endif
#include "xf_adc.h"
#include "xf_uart0_driver.h"

/* ADC�ɼ�����ǿ��ģ��ģ������˵�ѹ */
/**
  * @brief  ADC�ɼ�  
  * @details ���մ�����ģ��ģ������˵ĵ�ѹֵ
  * @param  NULL
  * @param  NULL
  * @return Vcc ���زɼ����ĵ�ѹֵ
 **/
float AdcLightAoutVoltage(void)
{
    float Vcc;
    Vcc = XfZstackAdcChangeVcc(SEL_ADC_CHANNEL_0, SEL_ADC_RESOLUTION_14, SEL_ADC_REF_AVDD);
    return Vcc;
}

/* �Ѷ�Ӧ�ĵ�ѹת���ɹ���ǿ�� */
/**
  * @brief  ��ѹֵת������ֵ  
  * @details ���մ�����ģ��ģ������˵ĵ�ѹֵ
  * @param  NULL
  * @param  NULL
  * @return LightStreng ���ع���ֵ
 **/
uint16 VccChangeLightStrength(void)
{
    float LightStreng;
    LightStreng = AdcLightAoutVoltage();
    LightStreng =1500 - (float)1500*(LightStreng/3.3);
    return (uint16)LightStreng;
}

/**
  * @brief  ��ȡ����ֵ����  
  * @details 
  * @param  NULL
  * @param  NULL
  * @return LightValue ���ع���ֵ
 **/
uint16 XfGetLight(void)
{
  uint16 LightValue;
  LightValue = VccChangeLightStrength();
#ifdef UART0_DEBUG  
  uart0_printf("����ֵ��%d\r\n",LightValue);
#endif
  return LightValue;
}