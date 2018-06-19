/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  bsp_gpio.c
  * @author  huangzhipeng
  * @version  V1.0.0
  * @date  03/30/2018
  * @brief  ADC��ʼ��
  */ 
#include <includes.h>

SPI_HandleTypeDef SPI3_Handler;  //SPI���

u8 SPI3RX[100]={0};
u8 SPI3RX_COUNT =0;
void SPI3_IRQHandler(void)
{
  if(__HAL_SPI_GET_FLAG(&SPI3_Handler,SPI_FLAG_RXNE) != RESET)
  {
   
     SPI3RX[SPI3RX_COUNT++] = (u8)(SPI3_Handler.Instance->DR);
     if(SPI3RX_COUNT == 100)SPI3RX_COUNT=0;
  }
}
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI3�ĳ�ʼ��
void SPI3_Init(void)
{
    SPI3_Handler.Instance=RF_SPI3;                         //SP3
    SPI3_Handler.Init.Mode=SPI_MODE_SLAVE;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI3_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI3_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI3_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI3_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI3_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI3_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI3_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI3_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI3_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI3_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI3_Handler);//��ʼ��
    
    __HAL_SPI_ENABLE_IT(&SPI3_Handler,SPI_IT_RXNE);
   
    __HAL_SPI_ENABLE(&SPI3_Handler);                    //ʹ��SPI3
	
    //SPI3_ReadWriteByte(0Xff);                           //��������
}

//SPI3�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void SPI3_MspInit(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance==RF_SPI3)
    {
    GPIO_InitTypeDef GPIO_Initure;
    
    SPI3_SCK_GPIO_CLK_ENABLE();       //ʹ��GPIOFʱ��
    SPI3_CLK_ENABLE();        //ʹ��SPI3ʱ��
    
    //PC10,11,12
    GPIO_Initure.Pin=SPI3_SCK_PIN|SPI3_MISO_PIN|SPI3_MOSI_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
    GPIO_Initure.Alternate=GPIO_AF6_SPI3;           //����ΪSPI3
    HAL_GPIO_Init(SPI3_SCK_GPIO_PORT,&GPIO_Initure);
   
    
    HAL_NVIC_EnableIRQ(SPI3_IRQn);		
    
    HAL_NVIC_SetPriority(SPI3_IRQn,3,3);	
    AppSetIrq(SPI3_IRQn,  3,  3,  SPI3_IRQHandler);
    }
}

//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ45Mhz��
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI3_Handler);            //�ر�SPI
    SPI3_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI3_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI3_Handler);             //ʹ��SPI
    
}

//SPI3 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI3_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}
