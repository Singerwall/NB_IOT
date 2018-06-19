#ifndef _usart_serialscreen_driver_h_include_
#define _usart_serialscreen_driver_h_include_


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/****************************************************************************/
/* Definition for SCREEN_USART clock resources */
#define SCREEN_USART                           USART2
#define SCREEN_USART_BAUDRATE                  115200  
#define SCREEN_USART_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define SCREEN_USART_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SCREEN_USART_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define SCREEN_USART_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define SCREEN_USART_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for SCREEN_USART Pins */
#define SCREEN_USART_TX_PIN                    GPIO_PIN_2
#define SCREEN_USART_TX_GPIO_PORT              GPIOA  
#define SCREEN_USART_TX_AF                     GPIO_AF7_USART2

#define SCREEN_USART_RX_PIN                    GPIO_PIN_3
#define SCREEN_USART_RX_GPIO_PORT              GPIOA 
#define SCREEN_USART_RX_AF                     GPIO_AF7_USART2

#define SCREEN_USART_IRQHandler                USART2_IRQHandler
#define SCREEN_USART_IRQ                       USART2_IRQn


#define SCREEN_USART_REC_LEN 200



void SCREEN_USART_Handle_Transmit(const uint8_t *pData, uint16_t Size);
void SCREEN_UART_MspInit(UART_HandleTypeDef *huart);
void SCREEN_USART_Init(void);
void SCREEN_USART_IRQHandler(void);



#endif/* _usart_serialscreen_driver_h_include_*/