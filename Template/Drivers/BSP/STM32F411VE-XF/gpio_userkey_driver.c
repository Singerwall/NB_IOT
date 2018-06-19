/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户按键底层驱动
  */ 

#include <includes.h>
#include "main.h"


/**
  * @brief   USER_KEY1_EXTI_IRQHandler-> EXTI15_10_IRQHandler 
  * @NOTE  外部中断硬件入口函数，也就是注册到向量表中的
  */
void USER_KEY1_EXTI_IRQHandler(void)
{
    /*GPIO中断处理函数入口-->HAL_GPIO_EXTI_Callback(GPIO_Pin);  在bsp_gpio.c 文件中定义*/
    HAL_GPIO_EXTI_IRQHandler(USER_KEY1_PIN); 
    
    return;
}

/**
  * @brief  Configures USER KEY GPIO.
  */
void USER_KEY_IT_Init(void)
{
  BSP_GPIO_IT_Init(USER_KEY1_GPIO_PORT, USER_KEY1_PIN, USER_KEY1_GPIO_IT_MODE, USER_KEY1_GPIO_PULL,
                      USER_KEY1_EXTI_IRQ, USER_KEY1_EXTI_IRQn_PreemptPriority, USER_KEY1_EXTI_IRQn_SubPriority, 
                      USER_KEY1_EXTI_IRQHandler);
}

