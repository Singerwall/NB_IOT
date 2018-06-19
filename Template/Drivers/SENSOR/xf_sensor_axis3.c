/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_axis3.c
  * @author  wuyijun
  * @version  V1.0.0
  * @date  30/03/2015
  * @brief  cc2530���� ������ٶ�����
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
/***************������ֲ�������Ƭ����Ҫ�޸ĵĵط�***************/
/*����IO����,out����IO����Ϊ���*/
/**
  * @brief ����SDAΪ���
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
  * @brief ����SCLΪ���
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

//����IO����,in�豸IO����Ϊ����
/**
  * @brief ����SDAΪ����
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
  * @brief  nop��ʱ
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
/*                        ��ʼ��IIC                          */
/*************************************************************/
/**
  * @brief  IIC��ʼ��
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
/*                         ����IIC                           */
/*************************************************************/
/**
  * @brief  ����IIC 
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
/*                       IIC��������                         */
/*************************************************************/
/**
  * @brief  IIC��������  
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
/*                       IIC��������                         */
/*************************************************************/
/**
  * @brief  IIC��������  
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
/*                      IIC��ȡӦ��λ                        */
/*************************************************************/
/**
  * @brief  IIC��ȡӦ��λ    
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
/*                      IIC����Ӧ��λ                        */
/*************************************************************/
/**
  * @brief  IIC����Ӧ��λ    
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
/*                         ֹͣIIC                           */
/*************************************************************/
/**
  * @brief  ֹͣIIC    
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
/*                    ��ʼ�����ٶȴ�����                     */
/*************************************************************/
/**
  * @brief  ��ʼ�����ٶȴ�����    
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static void INIT_MMA7660(void) 
{
    I2Cstart();
    I2Csend(0x98);    //���ʹӻ���ַ��д�����
    I2Creadack();
    I2Csend(0x07);          //д��Ĵ�����ַ
    I2Creadack();
    I2Csend(0x01);          //���üĴ�����ֵ,��������2g�����ò���ģʽ
    I2Creadack();
    I2Cstop();
}

/*************************************************************/
/*                       ��ȡ3����ٶ�                       */
/*************************************************************/
/**
  * @brief  ��ȡ3����ٶ�    
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
static int8 *Read3Axle(void) 
{
    static int8 Val[3];
    I2Cstart();
    I2Csend(0x98);               //���ʹӻ���ַ��д�����
    I2Creadack();
    I2Csend(0x00);               //д�����ȡ�ĵ�һ���Ĵ�����ַ
    I2Creadack();
    I2Cstart();
    I2Csend(0x99);               //���ʹӻ���ַ����ȡ����
    I2Creadack();
    Val[X] = I2Creceive();      //��ȡX����ٶ�
    I2Csendack();
    Val[Y] = I2Creceive();      //��ȡY����ٶ�
    I2Csendack();
    Val[Z] = I2Creceive();      //��ȡZ����ٶ�
    I2Cstop();
   
    /* �Ѷ��������ļĴ�����ֵת���ɼ��ٶ� */
    Val[X] = (Val[X] << 2);
    Val[Y] = (Val[Y] << 2);
    Val[Z] = (Val[Z] << 2);
    
    Val[X] = Val[X]/8;
    Val[Y] = Val[Y]/8;
    Val[Z] = Val[Z]/8;

    return Val;
}

/* MMA7600(���ٶȴ�����)��ʼ�� */
/**
  * @brief  MMA7600(���ٶȴ�����)��ʼ��   
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
  * @brief  ��ȡ���ٶȴ�������ֵ   
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
    uart0_printf("X-����ٶ�%d\r\n",*(AxisValue+X));
    uart0_printf("Y-����ٶ�%d\r\n",*(AxisValue+Y));
    uart0_printf("Z-����ٶ�%d\r\n",*(AxisValue+Z));
#endif
    return AxisValue;
}
