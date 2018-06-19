/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  Gate_Dirver.c
  * @author  qyz
  * @version  V2.0.0
  * @date  08/01/2016
  * @brief  门闸驱动
  */ 

#include <stm32f10x.h>
#include "Gate_Dirver.h"

/**
  * @brief	闸机初始化
  * @details	PWM输出初始化
  * @retval 	无
  */
void Gate_init(void)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef  time;
	TIM_OCInitTypeDef  oc;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/*使能定时器3时钟*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  /*使能GPIO外设和AFIO复用功能模块时钟 */
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); /*Timer3部分重映射  TIM3_CH2->PB5 */   
 
   /*设置该引脚为复用输出功能,输出TIM3 CH3的PWM脉冲波形	   */
	gpio.GPIO_Pin = GPIO_Pin_8; /*TIM_CH3  */
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;  /*复用推挽输出*/
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);/*初始化GPIO */
 
   /*初始化TIM3 */
	time.TIM_Period = 7199; /*设置在下一个更新事件装入活动的自动重装载寄存器周期的值*/
	time.TIM_Prescaler =99; /*设置用来作为TIMx时钟频率除数的预分频值*/ 
	time.TIM_ClockDivision = 0; /*设置时钟分割:TDTS = Tck_tim  */
	time.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式*/
	TIM_TimeBaseInit(TIM3, &time); /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/
	
	/*初始化TIM3 Channel2 PWM模式*/	 
	oc.TIM_OCMode = TIM_OCMode_PWM1; /*选择定时器模式:TIM脉冲宽度调制模式1*/
 	oc.TIM_OutputState = TIM_OutputState_Enable; /*比较输出使能*/
	oc.TIM_OCPolarity = TIM_OCPolarity_High; /*输出极性:TIM输出比较极性高 */
	TIM_OC3Init(TIM3, &oc);  /*根据T指定的参数初始化外设TIM3 OC3	*/

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  /*使能TIM3在CCR3上的预装载寄存器*/
 
	TIM_Cmd(TIM3, ENABLE);  /*使能TIM3 */
}


/**
  * @brief		闸机打开函数
  * @details	无
  * @param NULL	无
  * @retval 	无
  */
void Gate_Open(void)
{
    TIM_SetCompare3(TIM3,open_door_duty);
}

/**
  * @brief		闸机关闭函数
  * @details	无
  * @param NULL	无
  * @retval 	无
  */
void Gate_Close(void)
{
    TIM_SetCompare3(TIM3,close_door_duty);
}