#ifndef __spi4_wsn1_driver_H
#define __spi4_wsn1_driver_H
#include <delay.h>
	


/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_BUS STM32F4XX NUCLEO LOW LEVEL BUS
  * @{
  */
/*############################### SPI4 #######################################*/


#define RF_SPI4                                  SPI4
#define SPI4_CLK_ENABLE()                        __HAL_RCC_SPI4_CLK_ENABLE()

#define SPI4_SCK_AF                              GPIO_AF6_SPI4
#define SPI4_SCK_GPIO_PORT                       GPIOE
#define SPI4_SCK_PIN                             GPIO_PIN_2
#define SPI4_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOE_CLK_DISABLE()

#define SPI4_MISO_MOSI_AF                        GPIO_AF6_SPI4
#define SPI4_MISO_MOSI_GPIO_PORT                 GPIOE
#define SPI4_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define SPI4_MISO_PIN                            GPIO_PIN_5
#define SPI4_MOSI_PIN                            GPIO_PIN_6
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPI4_TIMEOUT_MAX                   1000




extern SPI_HandleTypeDef SPI4_Handler;  //SPI¾ä±ú
void SPI4_MspInit(SPI_HandleTypeDef *hspi);
void SPI4_Init(void);
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI4_ReadWriteByte(u8 TxData);
#endif/*__spi4_wsn1_driver_H*/
