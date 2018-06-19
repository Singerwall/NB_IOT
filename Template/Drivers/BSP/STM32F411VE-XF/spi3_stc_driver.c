/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  bsp_gpio.c
  * @author  huangzhipeng
  * @version  V1.0.0
  * @date  03/30/2018
  * @brief  ADC初始化
  */ 
#include <includes.h>

SPI_HandleTypeDef SPI3_Handler;  //SPI句柄

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
//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI3的初始化
void SPI3_Init(void)
{
    SPI3_Handler.Instance=RF_SPI3;                         //SP3
    SPI3_Handler.Init.Mode=SPI_MODE_SLAVE;             //设置SPI工作模式，设置为从模式
    SPI3_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI3_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI3_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI3_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI3_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI3_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI3_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI3_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI3_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI3_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI3_Handler);//初始化
    
    __HAL_SPI_ENABLE_IT(&SPI3_Handler,SPI_IT_RXNE);
   
    __HAL_SPI_ENABLE(&SPI3_Handler);                    //使能SPI3
	
    //SPI3_ReadWriteByte(0Xff);                           //启动传输
}

//SPI3底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
void SPI3_MspInit(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance==RF_SPI3)
    {
    GPIO_InitTypeDef GPIO_Initure;
    
    SPI3_SCK_GPIO_CLK_ENABLE();       //使能GPIOF时钟
    SPI3_CLK_ENABLE();        //使能SPI3时钟
    
    //PC10,11,12
    GPIO_Initure.Pin=SPI3_SCK_PIN|SPI3_MISO_PIN|SPI3_MOSI_PIN;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速            
    GPIO_Initure.Alternate=GPIO_AF6_SPI3;           //复用为SPI3
    HAL_GPIO_Init(SPI3_SCK_GPIO_PORT,&GPIO_Initure);
   
    
    HAL_NVIC_EnableIRQ(SPI3_IRQn);		
    
    HAL_NVIC_SetPriority(SPI3_IRQn,3,3);	
    AppSetIrq(SPI3_IRQn,  3,  3,  SPI3_IRQHandler);
    }
}

//SPI速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为45Mhz：
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI3_Handler);            //关闭SPI
    SPI3_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI3_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI3_Handler);             //使能SPI
    
}

//SPI3 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI3_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}
