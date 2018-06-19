/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_humidity.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 ʪ�ȴ�������������
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

/* ���ʪ�ȴ������������ŵ�ƽ */
/**
  * @brief  ���ʪ�ȴ��������������ŵĵ�ƽ  
  * @details 
  * @param  NULL
  * @param  NULL
  * @return HUMIDITY_YES������Ϊ��
            HUMIDITY_NO������Ϊ��
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


/* ʪ��ת�� */
/**
  * @brief  ʪ��ת��  
  * @details ���ɼ����ĵ�ѹֵת��Ϊʪ��ֵ
  * @param  NULL
  * @param  NULL
  * @return humid ����ʪ��ֵ
 **/
float HumidChange(void)
{
    float vcc;
    float r_val;
    float humid;
    
    /* �ѵ�ѹֵת������Ӧ�ĵ���ֵ */
    vcc = XfZstackAdcChangeVcc(SEL_ADC_CHANNEL_0, SEL_ADC_RESOLUTION_14, SEL_ADC_REF_AVDD);
    
    /* ˵�������ݵ��贮����ѹ�������ֵ */
    r_val = (100*vcc)/(3.4 - vcc);
    
    /* �ѵ���ֵת���ɶ�Ӧ��ʪ�� */
    humid = 100 - 0.01*r_val;
    
    if(humid < 0)
       humid = 0;
    
    return humid;
}
/**
  * @brief  ��ȡʪ��ֵ
  * @details 
  * @param  NULL
  * @param  NULL
  * @return humid ����ʪ��ֵ
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