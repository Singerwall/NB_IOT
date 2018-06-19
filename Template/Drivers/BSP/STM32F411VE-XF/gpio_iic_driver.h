#ifndef _gpio_iic_driver_h_include_
#define _gpio_iic_driver_h_include_
#include <delay.h>



/** @defgroup  xf_bsp_iic���ŷ���
  * @{
  */
/*############################### iic #######################################*/



#define   SDA_PORT                      GPIOB
#define   SDA_PIN                       GPIO_PIN_9

#define   SDA_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   SDA_PULL                      GPIO_PULLUP/*����*/

#define   SCL_PORT                      GPIOB
#define   SCL_PIN                       GPIO_PIN_8

#define   SCL_MODE                      GPIO_MODE_OUTPUT_PP/*�������*/
#define   SCL_PULL                      GPIO_PULLUP/*����*/



//IO��������
#define SDA_IN()  {SDA_PORT->MODER&=~(3<<(9*2));SDA_PORT->MODER|=0<<9*2;}//PB9����ģʽ
#define SDA_OUT() {SDA_PORT->MODER&=~(3<<(9*2));SDA_PORT->MODER|=1<<9*2;} //PB9���ģʽ


//IO����
#define IIC_SCL   PBout(8) //SCL
#define IIC_SDA   PBout(9) //SDA
#define READ_SDA  PHin(9)  //����SDA





//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
/**
  * @}
  */ 
#endif/*_gpio_iic_driver_h_include_*/

