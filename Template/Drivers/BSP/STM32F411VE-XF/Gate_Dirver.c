/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  Gate_Dirver.c
  * @author  qyz
  * @version  V2.0.0
  * @date  08/01/2016
  * @brief  ��բ����
  */ 

#include <stm32f10x.h>
#include "Gate_Dirver.h"

/**
  * @brief	բ����ʼ��
  * @details	PWM�����ʼ��
  * @retval 	��
  */
void Gate_init(void)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef  time;
	TIM_OCInitTypeDef  oc;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/*ʹ�ܶ�ʱ��3ʱ��*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  /*ʹ��GPIO�����AFIO���ù���ģ��ʱ�� */
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); /*Timer3������ӳ��  TIM3_CH2->PB5 */   
 
   /*���ø�����Ϊ�����������,���TIM3 CH3��PWM���岨��	   */
	gpio.GPIO_Pin = GPIO_Pin_8; /*TIM_CH3  */
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;  /*�����������*/
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);/*��ʼ��GPIO */
 
   /*��ʼ��TIM3 */
	time.TIM_Period = 7199; /*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ*/
	time.TIM_Prescaler =99; /*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ*/ 
	time.TIM_ClockDivision = 0; /*����ʱ�ӷָ�:TDTS = Tck_tim  */
	time.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ*/
	TIM_TimeBaseInit(TIM3, &time); /*����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/
	
	/*��ʼ��TIM3 Channel2 PWMģʽ*/	 
	oc.TIM_OCMode = TIM_OCMode_PWM1; /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1*/
 	oc.TIM_OutputState = TIM_OutputState_Enable; /*�Ƚ����ʹ��*/
	oc.TIM_OCPolarity = TIM_OCPolarity_High; /*�������:TIM����Ƚϼ��Ը� */
	TIM_OC3Init(TIM3, &oc);  /*����Tָ���Ĳ�����ʼ������TIM3 OC3	*/

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  /*ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���*/
 
	TIM_Cmd(TIM3, ENABLE);  /*ʹ��TIM3 */
}


/**
  * @brief		բ���򿪺���
  * @details	��
  * @param NULL	��
  * @retval 	��
  */
void Gate_Open(void)
{
    TIM_SetCompare3(TIM3,open_door_duty);
}

/**
  * @brief		բ���رպ���
  * @details	��
  * @param NULL	��
  * @retval 	��
  */
void Gate_Close(void)
{
    TIM_SetCompare3(TIM3,close_door_duty);
}