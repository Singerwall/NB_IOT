/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �봮�����ӿڵײ�����
  */ 



#include <includes.h>
#include "main.h"




/**
  * @brief �������þ��
 */
UART_HandleTypeDef SCREEN_USART_Handle;  
/*���մ洢�أ����SCREEN_USART_REC_LEN���ֽ�*/
uint8_t SCREEN_USART_RX_BUF[SCREEN_USART_REC_LEN];   



/**
 * @brief       : Init uart device
 * @param       : None
 * @param       : None
 * @Return      : None
 */
void SCREEN_USART_Init(void)
{
    

   SCREEN_USART_Handle.Instance          = SCREEN_USART;

   SCREEN_USART_Handle.Init.BaudRate     = SCREEN_USART_BAUDRATE;
   SCREEN_USART_Handle.Init.WordLength   = UART_WORDLENGTH_8B;
   SCREEN_USART_Handle.Init.StopBits     = UART_STOPBITS_1;
   SCREEN_USART_Handle.Init.Parity       = UART_PARITY_NONE;
   SCREEN_USART_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
   SCREEN_USART_Handle.Init.Mode         = UART_MODE_TX_RX;
   SCREEN_USART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&SCREEN_USART_Handle) != HAL_OK)
    {
        /* Initialization Error */
        _Error_Handler(__FILE__, __LINE__);
    }
   
    __HAL_UART_ENABLE_IT(&SCREEN_USART_Handle, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&SCREEN_USART_Handle, UART_IT_IDLE);
    

    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartInit
 Description : �ᱻHAL_UART_Init()���� 
 Input       : huart ���ھ��
 Output      : None
 Return      : None
 *****************************************************************************/
/**
  * @brief UART MSP Initialization 
              �ᱻHAL_UART_Init()���� 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void SCREEN_UART_MspInit(UART_HandleTypeDef *huart)
{  
  
  if(huart->Instance==SCREEN_USART)
  {
    GPIO_InitTypeDef  GPIO_InitStruct;
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SCREEN_USART_TX_GPIO_CLK_ENABLE();
    SCREEN_USART_RX_GPIO_CLK_ENABLE();
    
    /* Enable SCREEN_USART clock */
    SCREEN_USART_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SCREEN_USART_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = SCREEN_USART_TX_AF;
    HAL_GPIO_Init(SCREEN_USART_TX_GPIO_PORT, &GPIO_InitStruct);      
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = SCREEN_USART_RX_PIN;
    GPIO_InitStruct.Alternate = SCREEN_USART_RX_AF;
      
    HAL_GPIO_Init(SCREEN_USART_RX_GPIO_PORT, &GPIO_InitStruct);
    
    
    
    
        /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);
    
        /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Stream6;
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);
    
    
    /*ʹ�ܴ��ڽ��տ����ж�*/
    __HAL_UART_ENABLE_IT(&SCREEN_USART_Handle, UART_IT_IDLE);	
    HAL_NVIC_EnableIRQ(SCREEN_USART_IRQ);		 
    HAL_NVIC_SetPriority(SCREEN_USART_IRQ,2,2);	
    AppSetIrq(SCREEN_USART_IRQ,  2,  2,  SCREEN_USART_IRQHandler);  /*���ж���������ע���жϺ�  */
  }
}


/**
  * @brief ���ڴ�ӡ����,Ӳ��ӡ
  * @details 
  * @param  pData ָ��Ҫ���͵Ļ�����
  * @param  Size  ���ݳ���
  * @retval ��
 */
 void SCREEN_USART_Handle_Transmit(const uint8_t *pData, uint16_t Size)
 {
        if(HAL_UART_Transmit(&SCREEN_USART_Handle, (uint8_t*)pData, Size, 1000)!= HAL_OK)
        {
               _Error_Handler(__FILE__, __LINE__);
        }
 }
 

/**
  * @brief SCREEN_USART_IRQHandler�жϽ��մ�����
  * @retval ��
 */

void SCREEN_USART_IRQHandler(void)
{
  uint8_t Res;
  if((__HAL_UART_GET_FLAG(&SCREEN_USART_Handle,UART_FLAG_RXNE)!=RESET))  
  {
    HAL_UART_Receive(&SCREEN_USART_Handle,&Res,1,1000); 
    SCREEN_USART_Handle_Transmit(&Res, 1);   
  }
  if((__HAL_UART_GET_FLAG(&SCREEN_USART_Handle,UART_FLAG_IDLE)!=RESET))  
  {
    Res = (uint8_t)((&SCREEN_USART_Handle)->Instance->DR & (uint8_t)0x00FF);//clear IT_UART_FLAG_IDLE
      
  }
  
  HAL_UART_IRQHandler(&SCREEN_USART_Handle);	    

}