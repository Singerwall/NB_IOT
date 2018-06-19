/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/19/2018
  * @brief  IO --> 模拟 IIC
  */ 
#include <includes.h>




/**
  * @brief IIC初始化
  * @details 
  * @param  
  * @retval 无
 */
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOH_CLK_ENABLE();   //使能GPIOH时钟
    
    //PH4,5初始化设置
    GPIO_Initure.Pin=SDA_PIN|SCL_PIN;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(SDA_PORT,&GPIO_Initure);
    
    IIC_SDA=1;
    IIC_SCL=1;  
}

/**
  * @brief 产生IIC起始信号
  * @retval 无
 */
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
/**
  * @brief 产生IIC停止信号
  * @retval 无
 */
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	delay_us(4);			
	IIC_SDA=1;//发送I2C总线结束信号				   	
}       
/**
  * @brief 等待应答信号到来
  * @retval 1，接收应答失败
            0，接收应答成功
 */
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
/**
  * @brief 产生ACK应答
  * @retval 无
 */
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
/**
  * @brief 不产生ACK应答	
  * @retval 无
 */
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
/**
  * @brief IIC发送一个字节	
  * @retval 无
 */
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
/**
  * @brief 读1个字节，ack=1时，发送ACK，ack=0，发送nACK  	
  * @retval 无
 */
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


