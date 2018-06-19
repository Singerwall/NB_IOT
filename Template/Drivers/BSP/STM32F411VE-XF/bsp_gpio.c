/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  bsp_gpio.c
  * @author  huangzhipeng
  * @version  V1.0.0
  * @date  03/30/2018
  * @brief  GPIO_PIN引脚配置 集成函数包含中断，普通IO配置
  */ 

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include <includes.h>
    

/**
  * @brief  CLK 使能
  */
static void GPIO_PORTx_CLK_ENABLE(GPIO_TypeDef  *GPIOx)
{

       if(GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE(); 
  else if(GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE(); 
  else if(GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE(); 
  else if(GPIOx == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE(); 
  else if(GPIOx == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE(); 
  else if(GPIOx == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE(); 
  
}

/**
  * @brief  关clk
  */
 void GPIO_PORTx_CLK_DISABLE(GPIO_TypeDef  *GPIOx)
{

       if(GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_DISABLE(); 
  else if(GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_DISABLE(); 
  else if(GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_DISABLE(); 
  else if(GPIOx == GPIOD) __HAL_RCC_GPIOD_CLK_DISABLE(); 
  else if(GPIOx == GPIOE) __HAL_RCC_GPIOE_CLK_DISABLE(); 
  else if(GPIOx == GPIOH) __HAL_RCC_GPIOH_CLK_DISABLE(); 
  
}


/**
  * @brief  配置为普通的IO口
  * @param  GPIOx
  * @param  PINx
  * @param  MODE
  * @param  PULL
  */
void BSP_GPIO_Init(GPIO_TypeDef  *GPIOx, uint32_t PINx, uint32_t MODE, uint32_t PULL)
{
 
  GPIO_InitTypeDef GPIO_InitStruct; 
  /* Enable the BUTTON Clock */
  GPIO_PORTx_CLK_ENABLE(GPIOx);
  
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = PINx;
    GPIO_InitStruct.Mode = MODE;
    GPIO_InitStruct.Pull = PULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
    

  
}
void leds_config(void)
{
      GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //?a??GPIOBê±?ó
	
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6; //PB1,0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //í?íìê?3?
    GPIO_Initure.Pull=GPIO_PULLUP;          //é?à-
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //???ù
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);	//PB0??1￡???è?3?ê??ˉoóμ??e
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);	//PB1??1￡???è?3?ê??ˉoóμ??e
}
/**
  * @brief  取消配置，但是没有关闭时钟
  * @param  GPIOx
  * @param  PINx
  * @param  MODE
  * @param  PULL
  */
void BSP_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t PINx,  IRQn_Type IRQn)
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin =  PINx;
    HAL_NVIC_DisableIRQ((IRQn_Type)(IRQn));
    HAL_GPIO_DeInit(GPIOx, gpio_init_structure.Pin);
}

/**
  * @brief GPIO中断配置函数
 
  * @param  GPIOx
  * @param  PINx
  * @param  MODE
  * @param  PULL

  * @param  IRQn 中断序号   参考 stm32f411xe.h
  * @param  PreemptPriority 抢占优先级
  * @param  SubPriority     响应优先级
  * @param  IRQHandler     中断处理函数   参考startup_xxxx.s启动文件的名称(为了兼容不带lite_os操作系统)
  *        
  * @retval None
  */
void BSP_GPIO_IT_Init(GPIO_TypeDef  *GPIOx, uint32_t PINx, uint32_t MODE, uint32_t PULL,
                      IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority, 
                      _IRQHandler IRQHandler)
{
  
  
  BSP_GPIO_Init(GPIOx, PINx, MODE, PULL);
  
 // HAL_NVIC_EnableIRQ(IRQn);
  
  AppSetIrq( IRQn,  PreemptPriority,  SubPriority,  IRQHandler);
  
  
}


/*****************************************************************************
 Function    : HAL_GPIO_EXTI_Callback
 Description : 被 HAL_GPIO_EXTI_IRQHandler 调用 ,  所有的GPIO中断 处理函数 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   switch(GPIO_Pin)
    {
        case USER_KEY1_PIN:
         printf("\r\n恭喜你按键中断实验成功。\r\n_xunfang_company");
            break;
        default:
          
              break;
      
    }
}


/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port output data register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/**
  * @brief  Reads the specified GPIO input data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @retval GPIO input data port value.
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

/**
  * @brief  Reads the specified GPIO input data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @retval GPIO input data port value.
  */
uint16_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx)
{

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

