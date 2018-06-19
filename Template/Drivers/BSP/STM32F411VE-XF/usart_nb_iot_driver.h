#ifndef _usart_nb_iot_driver_h_include_
#define _usart_nb_iot_driver_h_include_



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <NB_BC95.h>
/****************************************************************************/

#define NB_USART_DMA_CLK_ENABLE()          __HAL_RCC_DMA2_CLK_ENABLE()
/* Definition for NB_USART clock resources */
#define NB_USART                           USART6
#define NB_USART_BAUDRATE                  9600  
#define NB_USART_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE();
#define NB_USART_CLK_DISABLE()             __HAL_RCC_USART6_CLK_DISABLE();
#define NB_USART_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define NB_USART_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#define NB_USART_FORCE_RESET()             __HAL_RCC_USART6_FORCE_RESET()
#define NB_USART_RELEASE_RESET()           __HAL_RCC_USART6_RELEASE_RESET()

/* Definition for NB_USART Pins */
#define NB_USART_TX_PIN                    GPIO_PIN_6
#define NB_USART_TX_GPIO_PORT              GPIOC  
#define NB_USART_TX_AF                     GPIO_AF8_USART6

#define NB_USART_RX_PIN                    GPIO_PIN_7
#define NB_USART_RX_GPIO_PORT              GPIOC 
#define NB_USART_RX_AF                     GPIO_AF8_USART6

#define NB_USART_IRQHandler                USART6_IRQHandler
#define NB_USART_IRQ                       USART6_IRQn


#define NB_USART_REC_MAX_LEN 300







extern uint8_t NB_USART_RX_BUF[NB_USART_REC_MAX_LEN];   
extern uint16_t NB_USART_Rxlen ;

void NB_HAL_UARTDMA_Init(bc95_receive_cb cb,uint32_t baud);
void NB_HAL_UART_Write(uint8_t*buf,uint16_t len);
void NB_HAL_UART_Close(void);

void NB_USART_Init(uint32_t baud);

void NB_UART_MspInit(UART_HandleTypeDef *huart);
void NB_USART_Handle_Transmit(const uint8_t *pData, uint16_t Size);

void DMA2_Stream1_IRQn_IRQHandler(void);
void DMA2_Stream6_IRQn_IRQHandler(void);
void NB_USART_IRQHandler(void);

#endif/* _usart_nb_iot_driver_h_include_*/