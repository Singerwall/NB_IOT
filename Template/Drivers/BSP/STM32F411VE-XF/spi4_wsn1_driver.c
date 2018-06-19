/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  bsp_gpio.c
  * @author  huangzhipeng
  * @version  V1.0.0
  * @date  03/30/2018
  * @brief  ADC��ʼ��
  */ 
#include <includes.h>

SPI_HandleTypeDef SPI4_Handler;  //SPI���

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI4�ĳ�ʼ��
void SPI4_Init(void)
{
    SPI4_Handler.Instance=RF_SPI4;                         //SPI4
    SPI4_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI4_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI4_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI4_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI4_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI4_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI4_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI4_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI4_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI4_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI4_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI4_Handler);//��ʼ��
    
   
    __HAL_SPI_ENABLE(&SPI4_Handler);                    //ʹ��SPI4
	
   // SPI4_ReadWriteByte(0Xff);                           //��������
}
void SPI4_MspInit(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance==RF_SPI4)
    {
    GPIO_InitTypeDef GPIO_Initure;
    
    SPI4_SCK_GPIO_CLK_ENABLE();       //ʹ��GPIOFʱ��
    SPI4_CLK_ENABLE();        //ʹ��SPI4ʱ��
    
    //PE2,5,6
    GPIO_Initure.Pin=SPI4_SCK_PIN|SPI4_MISO_PIN|SPI4_MOSI_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
    GPIO_Initure.Alternate=GPIO_AF6_SPI4;           //����ΪSPI4
    HAL_GPIO_Init(SPI4_SCK_GPIO_PORT,&GPIO_Initure);
    }
}


//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ45Mhz��
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI4_Handler);            //�ر�SPI
    SPI4_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI4_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI4_Handler);             //ʹ��SPI
    
}

//SPI4 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI4_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI4_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}
