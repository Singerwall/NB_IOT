/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户Debug接口串口底层驱动
  */ 

#include <includes.h>
#include "main.h"



/******************************************************************************
    here include some special hearder file you need
******************************************************************************/

static u16 USART_RX_STA=0;
#define uart_rx_buffer_size 512
static uint8_t uart_rx_buffer[uart_rx_buffer_size]={0};
static uint16_t len;
  
uint8_t * get_uart_rx_buffer(void)
{
  return uart_rx_buffer;
}
uint16_t * get_uart_rx_len(void)
{
  return &len;
}
void reset_uart_rx_len(void)
{
  len = 0;
}

extern void _Error_Handler(char *file, int line);




/**
  * @brief 串口配置句柄
 */
UART_HandleTypeDef DEBUG_USART_Handle;  

/*重定向c库函数printf到串口，重定向后可使用printf函数*/

struct __FILE 
{
    int handle; /* Add whatever needed */ 
}; 

typedef struct __FILE FILE;
FILE __stdout; 

void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f) 
{
 
   while(( DEBUG_USART ->SR&0X40)==0);  
   DEBUG_USART->DR = (uint8_t) ch;      
   return ch;
 
}




/*接收存储池，最大DEBUG_USART_REC_LEN个字节*/
uint8_t DEBUG_USART_RX_BUF[DEBUG_USART_REC_LEN];   



/**
 * @brief       : Init uart device
 * @param       : None
 * @param       : None
 * @Return      : None
 */
void DEBUG_USART_Init(void)
{
    

   DEBUG_USART_Handle.Instance          = DEBUG_USART;

   DEBUG_USART_Handle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
   DEBUG_USART_Handle.Init.WordLength   = UART_WORDLENGTH_8B;
   DEBUG_USART_Handle.Init.StopBits     = UART_STOPBITS_1;
   DEBUG_USART_Handle.Init.Parity       = UART_PARITY_NONE;
   DEBUG_USART_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
   DEBUG_USART_Handle.Init.Mode         = UART_MODE_TX_RX;
   DEBUG_USART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&DEBUG_USART_Handle) != HAL_OK)
    {
        /* Initialization Error */
        _Error_Handler(__FILE__, __LINE__);
    }
   
    __HAL_UART_ENABLE_IT(&DEBUG_USART_Handle, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&DEBUG_USART_Handle, UART_IT_IDLE);
    

    return;
}

/**
  * @brief DEBUG_UART_MspInit
  * @details 
  * @param  huart 串口句柄
  * @retval 无
 */
void DEBUG_UART_MspInit(UART_HandleTypeDef *huart)
{
 if(huart->Instance==DEBUG_USART)
  {
     GPIO_InitTypeDef  GPIO_InitStruct;
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    DEBUG_USART_TX_GPIO_CLK_ENABLE();
    DEBUG_USART_RX_GPIO_CLK_ENABLE();
    
    /* Enable DEBUG_USART clock */
    DEBUG_USART_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = DEBUG_USART_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
    
    HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
      
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
    GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
      
    HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
    
    HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ);		
    
    HAL_NVIC_SetPriority(DEBUG_USART_IRQ,3,3);	
    AppSetIrq(DEBUG_USART_IRQ,  3,  3,  DEBUG_USART_IRQHandler);
  
  }
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
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  DEBUG_UART_MspInit(huart); 
  NB_UART_MspInit(huart);
  SCREEN_UART_MspInit(huart);
}


/**
  * @brief 串口打印数据,硬打印
  * @details 
  * @param  pData 指向要发送的缓冲区
  * @param  Size  数据长度
  * @retval 无
 */
 void DEBUG_USART_Handle_Transmit(const uint8_t *pData, uint16_t Size)
 {
        if(HAL_UART_Transmit(&DEBUG_USART_Handle, (uint8_t*)pData, Size, 1000)!= HAL_OK)
        {
               _Error_Handler(__FILE__, __LINE__);
        }
 }
 
 /*****************************************************************************
 Function    : LOS_EvbUartWriteByte
 Description : Uart write one byte
 Input       : const char c
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteByte(const char c)
{
    //add you code here.
#ifdef LOS_STM32F411RE
    HAL_UART_Transmit(&DEBUG_USART_Handle, (uint8_t *)&c, 1, 0xFFFF);
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartReadByte
 Description : Uart reaad one byte
 Input       : char* c
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
    //add you code here.    
#ifdef LOS_STM32F411RE
    if(HAL_UART_Receive(&DEBUG_USART_Handle, (uint8_t *)c, 1, 5000) != HAL_OK)
    {
        Error_Handler();
    }
#endif
    return ;
}

/*****************************************************************************
 Function    : LosUartPrintf
 Description : Los printf function
 Input       : char* fmt, ...
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartPrintf(char* fmt, ...)
{
  #define _buffer_size 200
    char _buffer[_buffer_size];
    //add you code here.
#ifdef LOS_STM32F411RE
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
#endif
    return;
}

/*****************************************************************************
 Function    : LOS_EvbUartWriteStr
 Description : Uart Write String function
 Input       : const char* str
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
    //add you code here.
#ifdef LOS_STM32F411RE
    while (*str)
    {
        if(HAL_UART_Transmit(&DEBUG_USART_Handle, (uint8_t*)str, 1, 5000)!= HAL_OK)
        {
            Error_Handler();
        }
        str++;
    }
#endif
    
    return;
}

/**
  * @brief DEBUG_USART_IRQHandler中断接收处理函数
  * @retval 无
 */

void DEBUG_USART_IRQHandler(void)
{
  uint8_t Res;
 
  if((__HAL_UART_GET_FLAG(&DEBUG_USART_Handle,UART_FLAG_RXNE)!=RESET))  
  {
    HAL_UART_Receive(&DEBUG_USART_Handle,&Res,1,1000); 
     
    //LOS_EvbUartWriteByte(Res);  
    
		
    if((USART_RX_STA&0x8000)==0)     //接收未完成   --  USART_RX_STA 等待用户使用接受到数据清零
    {
            if(USART_RX_STA&0x4000)     //接收到0x0d
            {
                    if(Res!=0x0a)USART_RX_STA=0;   /*接收错误*/
                    else 
                    {
                          USART_RX_STA|=0x8000;     /*接收完成*/                                   
                          len=USART_RX_STA&0x3fff;
                         
                          // usmart_scan(uart_rx_buffer, len );	//执行usmart扫描                            
                          USART_RX_STA=0;                                   
                     }
                    
            }
            else   /*还没有接收到0x0d*/
            {	
                    if(Res==0x0d)   //接收到0x0d
                    {
                        USART_RX_STA|=0x4000;
                    }
                    else        /* 接收到数据*/
                    {
                            uart_rx_buffer[USART_RX_STA&0X3FFF]=Res;
                            USART_RX_STA++;
                            if(USART_RX_STA>(uart_rx_buffer_size-1))USART_RX_STA=0;/*超出最大接受buff清零*/
                    }
            }
    }
    
    
  }
  if((__HAL_UART_GET_FLAG(&DEBUG_USART_Handle,UART_FLAG_IDLE)!=RESET))  
  {
    Res = (uint8_t)((&DEBUG_USART_Handle)->Instance->DR & (uint8_t)0x00FF);//clear IT_UART_FLAG_IDLE
      
  }
  
  
  
  
  HAL_UART_IRQHandler(&DEBUG_USART_Handle);	    

}






