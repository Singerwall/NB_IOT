/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  Electronic_Lock_Dirver.c
  * @author  qyz
  * @version  V2.0.0
  * @date  08/01/2016
  * @brief  ����������
  */ 

#include <stm32f10x.h>
#include "Electronic_Lock_Dirver.h"

/**
  * @brief		��������ʼ������
  * @details	���ö˿�ʱ�ӡ�IO��ģʽ�Ȳ���
  * @param NULL	��
  * @retval 	��
  */
void Electronic_lock_init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_9;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;       
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);		/*��ʼ״̬�������ر�*/
}


/**
  * @brief		�������������
  * @details	��������λ�򿪵����
  * @param NULL	��
  * @retval  NULL	��
  */
void Electronic_Lock_Open(void)
{
    GPIO_SetBits(GPIOC,GPIO_Pin_9);
}

/**
  * @brief		������رպ���
  * @details	�����Ÿ�λ�رյ����
  * @param NULL	��
  * @param NULL	��
  * @retval 	��
  */
void Electronic_Lock_Close(void)
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}