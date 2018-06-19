#ifndef __spi1_rfid2_driver_H
#define __spi1_rfid2_driver_H
#include <delay.h>
	


/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_BUS STM32F4XX NUCLEO LOW LEVEL BUS
  * @{
  */
/*############################### SPI2 #######################################*/


#define RF_SPI2                                  SPI2
#define SPI2_CLK_ENABLE()                        __HAL_RCC_SPI2_CLK_ENABLE()

#define SPI2_SCK_AF                              GPIO_AF5_SPI2
#define SPI2_SCK_GPIO_PORT                       GPIOB
#define SPI2_SCK_PIN                             GPIO_PIN_13
#define SPI2_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI2_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPI2_MISO_MOSI_AF                        GPIO_AF5_SPI2
#define SPI2_MISO_MOSI_GPIO_PORT                 GPIOB
#define SPI2_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI2_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define SPI2_MISO_PIN                            GPIO_PIN_14
#define SPI2_MOSI_PIN                            GPIO_PIN_15
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPI2_TIMEOUT_MAX                   1000




extern SPI_HandleTypeDef SPI2_Handler;  //SPI¾ä±ú
void SPI2_MspInit(SPI_HandleTypeDef *hspi);
void SPI2_Init(void);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI2_ReadWriteByte(u8 TxData);
#endif/*__spi1_rfid2_driver_H*/
