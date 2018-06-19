/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  bsp_gpio.c
  * @author  huangzhipeng
  * @version  V1.0.0
  * @date  03/30/2018
  * @brief  ADC初始化
  */ 

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include <includes.h>
    	
/*** @brief  ADC句柄. */
ADC_HandleTypeDef ADC1_Handler;


/**
  * @brief  Initializes ADC HAL.
  * @通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
  */
void MY_ADC_Init(void)
{ 
  if(HAL_ADC_GetState(&ADC1_Handler) == HAL_ADC_STATE_RESET)
  {
    ADC1_Handler.Instance=ADCx;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
  }
}
/**
  * @brief 空气湿度 引脚配置为ADC 模式
  */
void HR31_ADCx_GPIO_Init(void)
{
//    GPIO_InitTypeDef GPIO_Initure;
//    
//    HR31_ADCx_GPIO_CLK_ENABLE();            //开启GPIOA时钟
//    
//    GPIO_Initure.Pin=HR31_ADCx_GPIO_PIN;        
//    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
//    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
//    HAL_GPIO_Init(HR31_ADCx_GPIO_PORT,&GPIO_Initure);  
    
      BSP_GPIO_Init(HR31_ADCx_GPIO_PORT, HR31_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}
/**
  * @brief 系统电压 引脚配置为ADC 模式
  */
void MCU_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(MCU_ADCx_GPIO_PORT, MCU_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}
/**
  * @brief 系统电压 引脚配置为ADC 模式
  */
void BAT_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(BAT_ADCx_GPIO_PORT, BAT_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}
/**
  * @brief CURRENT_ADC 系统电流 引脚配置为ADC 模式
  */
void CURRENT_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(CURRENT_ADCx_GPIO_PORT, CURRENT_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}

/**
  * @brief 保留 引脚配置为ADC 模式
  */
void xxxx_ADCx_GPIO_Init(void)
{
   BSP_GPIO_Init(xxxx_ADCx_GPIO_PORT, xxxx_ADCx_GPIO_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL);
}

/**
  * @brief ADC底层驱动，引脚配置，时钟使能
  * @details 此函数会被HAL_ADC_Init()调用
  * @param  hadc:ADC句柄
  * @retval 无
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

    ADCx_CLK_ENABLE();            //使能ADC1时钟
   
#ifdef  HR31  //防止IO冲突
    HR31_ADCx_GPIO_Init();
#endif
#ifdef  xxxx 
       xxxx_ADCx_GPIO_Init();
#endif
    
   MCU_ADCx_GPIO_Init(); 
   BAT_ADCx_GPIO_Init(); 
   CURRENT_ADCx_GPIO_Init();

}

/**
  * @brief 获得ADC值
  * @details 此函数会被HAL_ADC_Init()调用
  * @param  ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
  * @retval 转换结果
 */
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //返回最近一次ADC1规则组的转换结果
}

/**
  * @brief 
  * @details 获取指定通道的转换值，取times次,然后平均 
  * @param  times:获取次数
  * @retval 转换结果
 */
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
