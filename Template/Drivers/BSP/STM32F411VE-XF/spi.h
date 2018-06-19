#ifndef __SPI_H
#define __SPI_H
#include <delay.h>
	


/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_BUS STM32F4XX NUCLEO LOW LEVEL BUS
  * @{
  */
/*############################### SPI1 #######################################*/


#define RF_SPI5                                  SPI5
#define SPI5_CLK_ENABLE()                        __HAL_RCC_SPI5_CLK_ENABLE()

#define SPI5_SCK_AF                              GPIO_AF6_SPI5
#define SPI5_SCK_GPIO_PORT                       GPIOE
#define SPI5_SCK_PIN                             GPIO_PIN_12
#define SPI5_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI5_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOE_CLK_DISABLE()

#define SPI5_MISO_MOSI_AF                        GPIO_AF6_SPI5
#define SPI5_MISO_MOSI_GPIO_PORT                 GPIOE
#define SPI5_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI5_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define SPI5_MISO_PIN                            GPIO_PIN_13
#define SPI5_MOSI_PIN                            GPIO_PIN_14
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPI5_TIMEOUT_MAX                   1000




extern SPI_HandleTypeDef SPI5_Handler;  //SPI¾ä±ú

void SPI5_Init(void);
void SPI5_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI5_ReadWriteByte(u8 TxData);
#endif
