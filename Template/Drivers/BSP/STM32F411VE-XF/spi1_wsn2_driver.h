#ifndef __spi1_wsn2_driver_H
#define __spi1_wsn2_driver_H
#include <delay.h>
	


/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_BUS STM32F4XX NUCLEO LOW LEVEL BUS
  * @{
  */
/*############################### SPI1 #######################################*/


#define RF_SPI1                                  SPI1
#define SPI1_CLK_ENABLE()                        __HAL_RCC_SPI1_CLK_ENABLE()

#define SPI1_SCK_AF                              GPIO_AF5_SPI1
#define SPI1_SCK_GPIO_PORT                       GPIOA
#define SPI1_SCK_PIN                             GPIO_PIN_5
#define SPI1_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPI1_MISO_MOSI_AF                        GPIO_AF5_SPI1
#define SPI1_MISO_MOSI_GPIO_PORT                 GPIOA
#define SPI1_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI1_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define SPI1_MISO_PIN                            GPIO_PIN_6
#define SPI1_MOSI_PIN                            GPIO_PIN_7
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPI1_TIMEOUT_MAX                   1000




extern SPI_HandleTypeDef SPI1_Handler;  //SPI¾ä±ú
void SPI1_MspInit(SPI_HandleTypeDef *hspi);
void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);
#endif/*__spi1_wsn2_driver_H*/
