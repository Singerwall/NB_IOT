/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_electricity.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 ������������������
  */
#include "xf_sensor_electricity.h"
#include "xf_uart0_driver.h"
#include "xf_adc.h"

/* Adc�ɼ�������������ѹ */
/**
  * @brief ADC�ɼ�������������ѹ����
  * @details 
  * @param NULL
  * @param NULL
  * @param NULL
  * @return vcc ��ѹֵ
 **/
float AdcElectricitySenorVolt(void)
{
    float Vcc;
    Vcc = XfZstackAdcChangeVcc(SEL_ADC_CHANNEL_0, SEL_ADC_RESOLUTION_14, SEL_ADC_REF_AVDD);
    return Vcc;
}
/**
  * @brief ��ȡ����ֵ
  * @details 
  * @param NULL
  * @param NULL
  * @param NULL
  * @return value ����ֵ
 **/
float XfGetElectricValue(void)
{
    float value;
    value = AdcElectricitySenorVolt();
#ifdef UART0_DEBUG
    uart0_printf("��ѹ = %.3fV\r\n",value);//���ڴ�ӡ����
#endif   
    return value;
}