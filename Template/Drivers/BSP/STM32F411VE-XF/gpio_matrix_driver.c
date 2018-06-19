/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û����󰴼�xxxx�ײ�����
  */ 

#include <includes.h>
#include "main.h"



uint8_t Key_Value = 0;  /*�����ļ�ֵ*/



/**
  * @brief       ���󰴼���ʼ������
  * @details	���þ��󰴼�ʱ�ӡ�IO��ģʽ�Ȳ���
  * @param a	��
  * @param b	��
  * @retval 	��
  */
void MATRIX_KEY_Init(void)
{
  BSP_GPIO_Init(KEYS_PORT, KEY0_PIN|KEY1_PIN|KEY2_PIN|KEY3_PIN, ROW_MODE, ROW_PULL);
  BSP_GPIO_Init(KEYS_PORT, KEY4_PIN|KEY5_PIN|KEY6_PIN|KEY7_PIN, COL_MODE, COL_PULL); 
}

uint8_t Keyscan(void)
{
	uint16_t temp, ReadValue, WriteValue, Buffer[4] = {0xfeff, 0xfdff, 0xfbff, 0xf7ff};//�þ�����̵�ÿ�зֱ�Ϊ�͵�ƽ
	uint8_t i,j;		
	for(j=0; j<4; j++)
	{
		ReadValue = GPIO_ReadInputData(KEYS_PORT);
		WriteValue = (ReadValue | 0xff00) & (Buffer[j]); 
		GPIO_Write(KEYS_PORT, WriteValue);  /*����GPIOA��ֵ*/
		temp = 0x10;
		for (i=0; i<4; i++)
		{
			if(!((GPIO_ReadInputData(KEYS_PORT)&0xff00) & temp))	   /*��ȡKEYS_PORT��ֵ*/
			{
				 return (i+j*4);		//���ؼ���
			}
			temp <<= 1;
		}
                
	}
}
/**
  * @brief		ɨ�����
  * @details	��
  * @param a	��
  * @param b	��
  * @retval 	��
  */
void show_key(void)
{
         HAL_GPIO_WritePin(KEYS_PORT, KEY0_PIN|KEY1_PIN|KEY2_PIN|KEY3_PIN,GPIO_PIN_RESET);/*���ð�������λΪ0*/
	 HAL_GPIO_WritePin(KEYS_PORT, KEY4_PIN|KEY5_PIN|KEY6_PIN|KEY7_PIN,GPIO_PIN_SET);  /*���ð�������λΪ1*/ 
         if ((GPIO_ReadInputData(KEYS_PORT)&0xff00) != 0xf000)     /*��ȡ����ֵ*/
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

