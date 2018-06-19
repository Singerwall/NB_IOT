/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  Electronic_Lock_Dirver.c
  * @author  qyz
  * @version  V2.0.0
  * @date  08/01/2016
  * @brief  电子锁驱动
  */ 

#include <stm32f10x.h>
#include "Electronic_Lock_Dirver.h"

/**
  * @brief		电子锁初始化函数
  * @details	配置端口时钟、IO、模式等参数
  * @param NULL	无
  * @retval 	无
  */
void Electronic_lock_init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_9;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;       
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);		/*初始状态电子锁关闭*/
}


/**
  * @brief		电磁锁开启函数
  * @details	对引脚置位打开电磁锁
  * @param NULL	无
  * @retval  NULL	无
  */
void Electronic_Lock_Open(void)
{
    GPIO_SetBits(GPIOC,GPIO_Pin_9);
}

/**
  * @brief		电磁锁关闭函数
  * @details	对引脚复位关闭电磁锁
  * @param NULL	无
  * @param NULL	无
  * @retval 	无
  */
void Electronic_Lock_Close(void)
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}