/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  bsp_gpio.c
  * @author  huangzhipeng
  * @version  V1.0.0
  * @date  03/30/2018
  * @brief  ADC��ʼ��
  */ 

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include <includes.h>
    	
/*** @brief  ADC���. */
ADC_HandleTypeDef ADC1_Handler;


/**
  * @brief  Initializes ADC HAL.
  * @ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
  */
void MY_ADC_Init(void)
{ 
  if(HAL_ADC_GetState(&ADC1_Handler) == HAL_ADC_STATE_RESET)
  {
    ADC1_Handler.Instance=ADCx;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
  }
}
/**
  * @brief ����ʪ�� ��������ΪADC ģʽ
  */
void HR31_ADCx_GPIO_Init(void)
{
//    GPIO_InitTypeDef GPIO_Initure;
//    
//    HR31_ADCx_GPIO_CLK_ENABLE();            //����GPIOAʱ��
//    
//    GPIO_Initure.Pin=HR31_ADCx_GPIO_PIN;        
//    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
//    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
//    HAL_GPIO_Init(HR31_ADCx_GPIO_PORT,&GPIO_Initure);  
    
      BSP_GPIO_Init(HR31_ADCx_GPIO_PORT, HR31_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}
/**
  * @brief ϵͳ��ѹ ��������ΪADC ģʽ
  */
void MCU_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(MCU_ADCx_GPIO_PORT, MCU_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}
/**
  * @brief ϵͳ��ѹ ��������ΪADC ģʽ
  */
void BAT_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(BAT_ADCx_GPIO_PORT, BAT_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}
/**
  * @brief CURRENT_ADC ϵͳ���� ��������ΪADC ģʽ
  */
void CURRENT_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(CURRENT_ADCx_GPIO_PORT, CURRENT_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}

/**
  * @brief ���� ��������ΪADC ģʽ
  */
void xxxx_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(xxxx_ADCx_GPIO_PORT, xxxx_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}

/**
  * @brief ADC�ײ��������������ã�ʱ��ʹ��
  * @details �˺����ᱻHAL_ADC_Init()����
  * @param  hadc:ADC���
  * @retval ��
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

    ADCx_CLK_ENABLE();            //ʹ��ADC1ʱ��
   
#ifdef  HR31  //��ֹIO��ͻ
    HR31_ADCx_GPIO_Init();
#endif
#ifdef  xxxx 
       xxxx_ADCx_GPIO_Init();
#endif
    
   MCU_ADCx_GPIO_Init(); 
   BAT_ADCx_GPIO_Init(); 
   CURRENT_ADCx_GPIO_Init();

}

/**
  * @brief ���ADCֵ
  * @details �˺����ᱻHAL_ADC_Init()����
  * @param  ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
  * @retval ת�����
 */
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //�������һ��ADC1�������ת�����
}

/**
  * @brief 
  * @details ��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
  * @param  times:��ȡ����
  * @retval ת�����
 */
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
