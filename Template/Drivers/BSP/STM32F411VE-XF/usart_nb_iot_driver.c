/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  与NB_IOT接口底层驱动
  */ 

#include <includes.h>
#include "main.h"



static bc95_receive_cb  nb_receCb = NULL;
static uint8_t msgBuf[NB_USART_REC_MAX_LEN];

/**
  * @brief 串口配置句柄
 */
UART_HandleTypeDef NB_USART_Handle;  
/*接收存储池，最大NB_USART_REC_LEN个字节*/
uint8_t NB_USART_RX_BUF[NB_USART_REC_MAX_LEN];   
uint16_t NB_USART_Rxlen = 0;


DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;

/**
 * @brief       : Init uart device
 * @param       : None
 * @param       : None
 * @Return      : None
 */
void NB_USART_Init(uint32_t baud)
{
    

   NB_USART_Handle.Instance          = NB_USART;

   NB_USART_Handle.Init.BaudRate     = baud;
   NB_USART_Handle.Init.WordLength   = UART_WORDLENGTH_8B;
   NB_USART_Handle.Init.StopBits     = UART_STOPBITS_1;
   NB_USART_Handle.Init.Parity       = UART_PARITY_NONE;
   NB_USART_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
   NB_USART_Handle.Init.Mode         = UART_MODE_TX_RX;
   NB_USART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&NB_USART_Handle) != HAL_OK)
    {
        /* Initialization Error */
        _Error_Handler(__FILE__, __LINE__);
    }
   

   /*enable dma received*/
    HAL_UART_Receive_DMA(&NB_USART_Handle,NB_USART_RX_BUF,NB_USART_REC_MAX_LEN); 

    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : 会被HAL_UART_Init()调用 
 Input       : huart 串口句柄
 Output      : None
 Return      : None
 *****************************************************************************/
/**
  * @brief UART MSP Initialization 
              会被HAL_UART_Init()调用 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void NB_UART_MspInit(UART_HandleTypeDef *huart)
{  
  
  if(huart->Instance==NB_USART)
  {
    GPIO_InitTypeDef  GPIO_InitStruct;
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    NB_USART_TX_GPIO_CLK_ENABLE();
    NB_USART_RX_GPIO_CLK_ENABLE();
    
    /* Enable NB_USART clock */
    NB_USART_CLK_ENABLE();
    
    /* Enable NB_USART clock */
   NB_USART_DMA_CLK_ENABLE();
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = NB_USART_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = NB_USART_TX_AF;
    
    HAL_GPIO_Init(NB_USART_TX_GPIO_PORT, &GPIO_InitStruct);
      
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = NB_USART_RX_PIN;
    GPIO_InitStruct.Alternate = NB_USART_RX_AF;
      
    HAL_GPIO_Init(NB_USART_RX_GPIO_PORT, &GPIO_InitStruct);
        /* USART6 DMA Init */
    /* USART6_RX Init */
    hdma_usart6_rx.Instance = DMA2_Stream1;
    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_NORMAL;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(huart,hdmarx,hdma_usart6_rx);

        /* USART6_TX Init */
    hdma_usart6_tx.Instance = DMA2_Stream6;
    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(huart,hdmatx,hdma_usart6_tx);
    
    
    
//    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 1, 1);
//    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
//    AppSetIrq(DMA2_Stream1_IRQn,  1,  1,  DMA2_Stream1_IRQn_IRQHandler);  /*在中断向量表中注册中断号  */
//  

    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
    AppSetIrq(DMA2_Stream6_IRQn,  0,  1,  DMA2_Stream6_IRQn_IRQHandler);  /*在中断向量表中注册中断号  */

    
   
    /*使能串口接收空闲中断*/
    __HAL_UART_ENABLE_IT(&NB_USART_Handle, UART_IT_IDLE);	   
    HAL_NVIC_SetPriority(NB_USART_IRQ,2,2);
    HAL_NVIC_EnableIRQ(NB_USART_IRQ);
    AppSetIrq(NB_USART_IRQ,  2,  2,  NB_USART_IRQHandler);  /*在中断向量表中注册中断号  */
  }
}


/**
  * @brief 串口打印数据,硬打印
  * @details 
  * @param  pData 指向要发送的缓冲区
  * @param  Size  数据长度
  * @retval 无
 */
 void NB_USART_Handle_Transmit(const uint8_t *pData, uint16_t Size)
 {
   
#define DMA_NB
 #ifndef DMA_NB  
        if(HAL_UART_Transmit(&NB_USART_Handle, (uint8_t*)pData, Size, 1000)!= HAL_OK)
        {
               _Error_Handler(__FILE__, __LINE__);
        }
    
#else  
        if(HAL_UART_Transmit_DMA(&NB_USART_Handle, (uint8_t*)pData, Size)!= HAL_OK)
        {
               _Error_Handler(__FILE__, __LINE__);
        } 
 #endif 
        
 }


 
void DMA2_Stream1_IRQn_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}

void DMA2_Stream6_IRQn_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
}

/**
  * @brief NB_USART_IRQHandler空闲中断接收处理函数
  * @retval 统计接收的数据总的长度NB_USART_Rxlen，并再次开启接收
 */
void NB_USART_IDLE_Callback(UART_HandleTypeDef *huart)
{
    uint32_t tmp1 = 0, tmp2 = 0;

    tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
    tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);

    if((tmp2 != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
	/* set uart state  ready*/
	huart->gState = HAL_UART_STATE_READY;
          
	/* Disable the rx  DMA peripheral */
	__HAL_DMA_DISABLE(&hdma_usart6_rx);
			
	/*Clear the DMA Stream pending flags.*/
	__HAL_DMA_CLEAR_FLAG(huart->hdmarx,__HAL_DMA_GET_TC_FLAG_INDEX(huart->hdmarx));
			
	/* get rx data len */
	NB_USART_Rxlen = NB_USART_REC_MAX_LEN - huart->hdmarx->Instance->NDTR;
        // user deal with Cb 
        if(NB_USART_Rxlen)
        {
          memcpy(msgBuf, NB_USART_RX_BUF, NB_USART_Rxlen);
          //add debug_printf_buff
          memset(NB_USART_RX_BUF,0,NB_USART_Rxlen);
          msgBuf[NB_USART_Rxlen] = 0;
          printf("\r\n<-:%s",msgBuf);
          if(nb_receCb)
          {
            nb_receCb((char*)msgBuf,NB_USART_Rxlen);
          }
          NB_USART_Rxlen = 0;
        }	
        /* Process Unlocked */
	__HAL_UNLOCK(huart->hdmarx);
	HAL_UART_Receive_DMA(huart,NB_USART_RX_BUF,NB_USART_REC_MAX_LEN);	
       
        __HAL_DMA_ENABLE(&hdma_usart6_rx);
    }


}
/**
  * @brief NB_USART_IRQHandler中断接收处理函数
  * @retval 无
 */
void NB_USART_IRQHandler(void)
{

  NB_USART_IDLE_Callback(&NB_USART_Handle);
  HAL_UART_IRQHandler(&NB_USART_Handle);	    

}

/**
  * @brief NB_uart DMA 初始化函数
  * @param  cb  串口接收数据回调函数
  * @param  baud 波特率
  * @retval 无
 */
void NB_HAL_UARTDMA_Init(bc95_receive_cb cb,uint32_t baud)
{

  nb_receCb = cb;
  NB_USART_Init(baud);
 // HAL_UART_Receive_DMA(&hlpuart1,UartDma_Init(uart_dma_send,LPUART1),RECE_BUF_MAX_LEN);
}
/**
  * @brief NB_uart DMA 发送数据函数
  * @param  buf
  * @param  len
  * @retval 无
 */
void NB_HAL_UART_Write(uint8_t*buf,uint16_t len)
{
  buf[len] = 0;
  printf("\r\n->:%s",buf);
  NB_USART_Handle_Transmit(buf,len);
}


 void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  if(huart->Instance == NB_USART)
  {
     NB_USART_CLK_DISABLE();
     HAL_GPIO_DeInit(NB_USART_RX_GPIO_PORT, NB_USART_TX_PIN|NB_USART_RX_PIN);
     
     HAL_DMA_DeInit(huart->hdmarx);
     HAL_DMA_DeInit(huart->hdmatx);
     HAL_NVIC_DisableIRQ(NB_USART_IRQ);
  }
}

/**
  * @brief NB_uart DMA 关闭 函数
  * @param  buf
  * @param  len
  * @retval 无
 */
void NB_HAL_UART_Close(void)
{
  HAL_UART_MspDeInit( &NB_USART_Handle);
}
void test_rec(void)
{
        if(NB_USART_Rxlen)
        {
          memcpy(msgBuf, NB_USART_RX_BUF, NB_USART_Rxlen);
          //add debug_printf_buff
          memset(NB_USART_RX_BUF,0,NB_USART_Rxlen);
          msgBuf[NB_USART_Rxlen] = 0;
          printf("\r\n<-:%s",msgBuf);
//          if(nb_receCb)
//          {
//            nb_receCb((char*)msgBuf,NB_USART_Rxlen);
//          }
          NB_USART_Rxlen = 0;
        }
}