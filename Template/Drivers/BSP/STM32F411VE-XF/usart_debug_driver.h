#ifndef _usart_debug_driver_h_include_
#define _usart_debug_driver_h_include_


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/****************************************************************************/
/* Definition for DEBUG_USART clock resources */
#define DEBUG_USART                           USART1
#define DEBUG_USART_BAUDRATE                  115200  
#define DEBUG_USART_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define DEBUG_USART_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define DEBUG_USART_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for DEBUG_USART Pins */
#define DEBUG_USART_TX_PIN                    GPIO_PIN_9
#define DEBUG_USART_TX_GPIO_PORT              GPIOA  
#define DEBUG_USART_TX_AF                     GPIO_AF7_USART1

#define DEBUG_USART_RX_PIN                    GPIO_PIN_10
#define DEBUG_USART_RX_GPIO_PORT              GPIOA 
#define DEBUG_USART_RX_AF                     GPIO_AF7_USART1

#define DEBUG_USART_IRQHandler                USART1_IRQHandler
#define DEBUG_USART_IRQ                       USART1_IRQn


#define DEBUG_USART_REC_LEN 200


/****************************************************************************/

void LOS_EvbUartWriteByte(const char c);
void LOS_EvbUartReadByte(char* c);
void LOS_EvbUartPrintf(char* fmt, ...);
void LOS_EvbUartWriteStr(const char* str);

void reset_uart_rx_len(void);
uint8_t * get_uart_rx_buffer(void);
uint16_t * get_uart_rx_len(void);

void DEBUG_USART_Handle_Transmit(const uint8_t *pData, uint16_t Size);
void DEBUG_USART_Init(void);
void DEBUG_USART_IRQHandler(void);
#endif/*_usart_debug_driver_h_include_*/
