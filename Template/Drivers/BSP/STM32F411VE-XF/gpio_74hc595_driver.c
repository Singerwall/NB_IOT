/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户数码管底层驱动
  */ 

#include <includes.h>
#include "main.h"



const unsigned char LED_0F[] = 
{
  0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
/*  0	1    2	   3	4    5	  6    7    8	 9    A    b	C    d	  E    F    -*/
};




void SDATA_595_SET(uint8_t state)
{
  HAL_GPIO_WritePin(DIO_PORT, DIO_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}
void RCLK_595_SET(uint8_t state)
{
  HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}
void SCLK_595_SET(uint8_t state)
{
  HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}
	



/**
  * @brief		初始化74HC595
  * @details	无
  * @param a	无
  * @param b	无
  * @retval 	无
  */
void Init_595(void)
{
  BSP_GPIO_Init(DIO_PORT, DIO_PIN|RCLK_PIN|SCLK_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN);
  SCLK_595_SET(0);
  RCLK_595_SET(0);
}



/**
  * @brief		输出数据
  * @details	无
  * @param a	无
  * @param b	无
  * @retval 	无
  */
void OUT_595(void)
{
    RCLK_595_SET(0);
    delay_us(2);
    RCLK_595_SET(1);  /*上升沿将数据送到输出锁存器*/
    delay_us(3);
    RCLK_595_SET(0);
}


/**
  * @brief		往74HC595中写数据
  * @details	无
  * @param a	要写入得数据
  * @param b	无
  * @retval 	无
  */
void WR_595(uint8_t temp)
{
     uint8_t j;
    for (j=0; j<8; j++)
    {
        SCLK_595_SET(0); 
		if ((temp<<j) & 0x80)
			SDATA_595_SET(1);
		else
			SDATA_595_SET(0);
        SCLK_595_SET(1);                     //上升沿发生移位
        delay_us(2);
        SCLK_595_SET(0);
     }   
}


/**
  * @brief	显示0--f
  * @details	无
  * @param a	要写入得数据
  * @param b	无
  * @retval 	无
  */
void SHOW_595(uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4)
{
  unsigned char i=0;
  
  if(n1>17|n2>17|n3>17|n4>17)
  {
    return;
  }
  i=LED_0F[n1];
  WR_595(i);
  OUT_595();
  
  i=LED_0F[n2];
  WR_595(i);
  OUT_595();
  
  i=LED_0F[n3];
  WR_595(i);
  OUT_595();
  
  i=LED_0F[n4];
  WR_595(i);
  OUT_595();
}
