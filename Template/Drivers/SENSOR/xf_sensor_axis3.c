/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_axis3.c
  * @author  wuyijun
  * @version  V1.0.0
  * @date  30/03/2015
  * @brief  cc2530外设 三轴加速度驱动
  */ 

#include "xf_sensor_axis3.h"
#ifdef CC2540
  #include "iocc2540.h"
#else
  #include "iocc2530.h"
#endif
#include "xf_uart0_driver.h"

/* SCL */
#define BIT(X)       (1 << X)
#define TX_PIN       P0_4
#define SCL          TX_PIN
#define SCL_PIN_BIT  BIT(4) 
#define SCLDIR       P0DIR

/* SDA */
#define BIT(X)      (1 << X)
#define RX_PIN      P0_5
#define SDA         RX_PIN
#define SDA_PIN_BIT BIT(5) 
#define SDADIR      P0DIR


static void somenop(void);
static void INIT_IIC(void);
static void I2Cstart(void);
static void I2Csend(uint8 data);
static int8 I2Creceive(void);
static void I2Creadack(void);
static void I2Csendack(void);
static void I2Cstop(void);
static int8 *Read3Axle(void);
static void INIT_MMA7660(void);
/***************如需移植到其它款单片机需要修改的地方***************/
/*设置IO方向,out设置IO方向为输出*/
/**
  * @brief 设置SDA为输出
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void SDA_SET_OUT(void)
{
    uint8 temp;
    temp = SDADIR;
    temp &= ~(SDA_PIN_BIT | SCL_PIN_BIT);
    temp |=  (SDA_PIN_BIT | SCL_PIN_BIT);
    SDADIR = temp;
}
/**
  * @brief 设置SCL为输出
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void SCL_SET_OUT(void)
{
    uint8 temp;
    temp = SCLDIR;
    temp &= ~SCL_PIN_BIT;
    temp |= SCL_PIN_BIT;
    SCLDIR = temp;
}

//设置IO方向,in设备IO方向为输入
/**
  * @brief 设置SDA为输入
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void SDA_SET_IN(void)
{
    SDADIR &= ~SDA_PIN_BIT;
}
/**
  * @brief  nop延时
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void somenop()
{
    uint8 i;
    for(i=0; i<40; i++)
    {
        asm("nop");
    }
}


/*************************************************************/
/*                        初始化IIC                          */
/*************************************************************/
/**
  * @brief  IIC初始化
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void INIT_IIC(void) 
{
 SCL_SET_OUT();
 SDA_SET_OUT();
 SCL = 1;
 SDA = 1;
}


/*************************************************************/
/*                         启动IIC                           */
/*************************************************************/
/**
  * @brief  启动IIC 
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void I2Cstart(void) 
{
  SCL_SET_OUT();
  SDA_SET_OUT();
  SDA = 1;
  somenop();
  SCL = 1;
  somenop();
  somenop();
  SDA = 0;
  somenop();
  somenop();
  SCL = 0;
}


/*************************************************************/
/*                       IIC发送数据                         */
/*************************************************************/
/**
  * @brief  IIC发送数据  
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void I2Csend(uint8 data) 
{
   uint8 i=8;
   SCL_SET_OUT();
   SDA_SET_OUT();
   while(i) 
   {
        SCL = 0;  
        somenop();
        SDA=(data&0x80)>>7; 
        data<<=1; 
        somenop();
        SCL = 1;
        somenop();
        somenop();
        i--;     
   }
 SCL = 0;
}

/*************************************************************/
/*                       IIC接收数据                         */
/*************************************************************/
/**
  * @brief  IIC接收数据  
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static int8 I2Creceive(void) 
{
 uint8 i=8;
 uint8 data=0;

  SCL_SET_OUT();
  SDA_SET_IN();
 while(i) 
 {
      SCL = 0;  
      somenop();
      somenop();
      SCL = 1;
      somenop();
      data<<=1;
      if(SDA)
      {
           data |= 1;
      }
     
      somenop();
      i--;     
 }
 SCL = 0;
 return(data);
}

/*************************************************************/
/*                      IIC读取应答位                        */
/*************************************************************/
/**
  * @brief  IIC读取应答位    
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void I2Creadack(void) 
{
  SCL_SET_OUT();
  SDA_SET_IN();
  SCL = 0;
  somenop();
  somenop();
  SCL = 1;
  somenop();
  if(SDA == 1) 
  {
     for(;;) ;  
  }

  somenop();
  SCL = 0;
  somenop();
  somenop();
}

/*************************************************************/
/*                      IIC发送应答位                        */
/*************************************************************/
/**
  * @brief  IIC发送应答位    
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void I2Csendack(void) 
{
  SCL_SET_OUT();
  SDA_SET_OUT();
  SCL = 0;
  somenop();
  SDA = 0;
  somenop();
  SCL = 1;
  somenop();
  somenop();
  SCL = 0;
  somenop();
  somenop();
}

/*************************************************************/
/*                         停止IIC                           */
/*************************************************************/
/**
  * @brief  停止IIC    
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void I2Cstop(void) 
{
  SCL_SET_OUT();
  SDA_SET_OUT();
  SCL = 0;
  SDA = 0;
  somenop(); 
  somenop();
  SCL = 1;
  somenop();
  SDA = 1; 
  somenop(); 
  somenop();
}

/*************************************************************/
/*                    初始化加速度传感器                     */
/*************************************************************/
/**
  * @brief  初始化加速度传感器    
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void INIT_MMA7660(void) 
{
    I2Cstart();
    I2Csend(0x98);    //发送从机地址，写入操作
    I2Creadack();
    I2Csend(0x07);          //写入寄存器地址
    I2Creadack();
    I2Csend(0x01);          //设置寄存器的值,设置量程2g，设置测量模式
    I2Creadack();
    I2Cstop();
}

/*************************************************************/
/*                       读取3轴加速度                       */
/*************************************************************/
/**
  * @brief  读取3轴加速度    
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static int8 *Read3Axle(void) 
{
    static int8 Val[3];
    I2Cstart();
    I2Csend(0x98);               //发送从机地址，写入操作
    I2Creadack();
    I2Csend(0x00);               //写入待读取的第一个寄存器地址
    I2Creadack();
    I2Cstart();
    I2Csend(0x99);               //发送从机地址，读取操作
    I2Creadack();
    Val[X] = I2Creceive();      //读取X轴加速度
    I2Csendack();
    Val[Y] = I2Creceive();      //读取Y轴加速度
    I2Csendack();
    Val[Z] = I2Creceive();      //读取Z轴加速度
    I2Cstop();
   
    /* 把读出出来的寄存器的值转换成加速度 */
    Val[X] = (Val[X] << 2);
    Val[Y] = (Val[Y] << 2);
    Val[Z] = (Val[Z] << 2);
    
    Val[X] = Val[X]/8;
    Val[Y] = Val[Y]/8;
    Val[Z] = Val[Z]/8;

    return Val;
}

/* MMA7600(加速度传感器)初始化 */
/**
  * @brief  MMA7600(加速度传感器)初始化   
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
void Axis3Mma7600Init(void)
{
    INIT_IIC();
    INIT_MMA7660();
}
/**
  * @brief  获取加速度传感器的值   
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
int8 *XfGetAxis(void)
{
    int8 *AxisValue;
	  AxisValue = Read3Axle();
#ifdef UART0_DEBUG    
    uart0_printf("X-轴加速度%d\r\n",*(AxisValue+X));
    uart0_printf("Y-轴加速度%d\r\n",*(AxisValue+Y));
    uart0_printf("Z-轴加速度%d\r\n",*(AxisValue+Z));
#endif
    return AxisValue;
}
