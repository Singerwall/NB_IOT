#ifndef __spi3_stc_driver_H
#define __spi3_stc_driver_H
#include <delay.h>
	


/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_BUS STM32F4XX NUCLEO LOW LEVEL BUS
  * @{
  */
/*############################### SPI1 #######################################*/


#define RF_SPI3                                  SPI3
#define SPI3_CLK_ENABLE()                        __HAL_RCC_SPI3_CLK_ENABLE()

#define SPI3_SCK_AF                              GPIO_AF6_SPI3
#define SPI3_SCK_GPIO_PORT                       GPIOC
#define SPI3_SCK_PIN                             GPIO_PIN_10
#define SPI3_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI3_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOC_CLK_DISABLE()

#define SPI3_MISO_MOSI_AF                        GPIO_AF6_SPI3
#define SPI3_MISO_MOSI_GPIO_PORT                 GPIOC
#define SPI3_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI3_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define SPI3_MISO_PIN                            GPIO_PIN_11
#define SPI3_MOSI_PIN                            GPIO_PIN_12
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPI3_TIMEOUT_MAX                   1000




extern SPI_HandleTypeDef SPI3_Handler;  //SPI¾ä±ú
extern void SPI3_MspInit(SPI_HandleTypeDef *hspi);
void SPI3_Init(void);
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI3_ReadWriteByte(u8 TxData);
#endif/*__spi3_stc_driver_H*/
