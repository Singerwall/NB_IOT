/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户矩阵按键xxxx底层驱动
  */ 

#include <includes.h>
#include "main.h"



uint8_t Key_Value = 0;  /*读出的键值*/



/**
  * @brief       矩阵按键初始化函数
  * @details	配置矩阵按键时钟、IO、模式等参数
  * @param a	无
  * @param b	无
  * @retval 	无
  */
void MATRIX_KEY_Init(void)
{
  BSP_GPIO_Init(KEYS_PORT, KEY0_PIN|KEY1_PIN|KEY2_PIN|KEY3_PIN, ROW_MODE, ROW_PULL);
  BSP_GPIO_Init(KEYS_PORT, KEY4_PIN|KEY5_PIN|KEY6_PIN|KEY7_PIN, COL_MODE, COL_PULL); 
}

uint8_t Keyscan(void)
{
	uint16_t temp, ReadValue, WriteValue, Buffer[4] = {0xfeff, 0xfdff, 0xfbff, 0xf7ff};//让矩阵键盘的每行分别为低电平
	uint8_t i,j;		
	for(j=0; j<4; j++)
	{
		ReadValue = GPIO_ReadInputData(KEYS_PORT);
		WriteValue = (ReadValue | 0xff00) & (Buffer[j]); 
		GPIO_Write(KEYS_PORT, WriteValue);  /*设置GPIOA的值*/
		temp = 0x10;
		for (i=0; i<4; i++)
		{
			if(!((GPIO_ReadInputData(KEYS_PORT)&0xff00) & temp))	   /*读取KEYS_PORT的值*/
			{
				 return (i+j*4);		//返回键码
			}
			temp <<= 1;
		}
                
	}
}
/**
  * @brief		扫描键盘
  * @details	无
  * @param a	无
  * @param b	无
  * @retval 	无
  */
void show_key(void)
{
         HAL_GPIO_WritePin(KEYS_PORT, KEY0_PIN|KEY1_PIN|KEY2_PIN|KEY3_PIN,GPIO_PIN_RESET);/*设置按键低四位为0*/
	 HAL_GPIO_WritePin(KEYS_PORT, KEY4_PIN|KEY5_PIN|KEY6_PIN|KEY7_PIN,GPIO_PIN_SET);  /*设置按键高四位为1*/ 
         if ((GPIO_ReadInputData(KEYS_PORT)&0xff00) != 0xf000)     /*读取按键值*/
	 {
	 	 OSTimeDly (5);
		 if ((GPIO_ReadInputData(KEYS_PORT)&0xff00) != 0xf000)
	 	 {
		 	  OSTimeDly (5);
			 if ((GPIO_ReadInputData(KEYS_PORT)&&0xff00) != 0xf000)
	 	 	 {
		 		 Key_Value = Keyscan();
				
		 	 }
		 }
	 }
}

