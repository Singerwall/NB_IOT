#ifndef _gpio_iic_driver_h_include_
#define _gpio_iic_driver_h_include_
#include <delay.h>



/** @defgroup  xf_bsp_iic引脚分配
  * @{
  */
/*############################### iic #######################################*/



#define   SDA_PORT                      GPIOB
#define   SDA_PIN                       GPIO_PIN_9

#define   SDA_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   SDA_PULL                      GPIO_PULLUP/*上拉*/

#define   SCL_PORT                      GPIOB
#define   SCL_PIN                       GPIO_PIN_8

#define   SCL_MODE                      GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   SCL_PULL                      GPIO_PULLUP/*上拉*/



//IO方向设置
#define SDA_IN()  {SDA_PORT->MODER&=~(3<<(9*2));SDA_PORT->MODER|=0<<9*2;}//PB9输入模式
#define SDA_OUT() {SDA_PORT->MODER&=~(3<<(9*2));SDA_PORT->MODER|=1<<9*2;} //PB9输出模式


//IO操作
#define IIC_SCL   PBout(8) //SCL
#define IIC_SDA   PBout(9) //SDA
#define READ_SDA  PHin(9)  //输入SDA





//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
/**
  * @}
  */ 
#endif/*_gpio_iic_driver_h_include_*/

