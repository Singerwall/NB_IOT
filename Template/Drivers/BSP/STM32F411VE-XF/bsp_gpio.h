#ifndef _bsp_gpio_h_include_
#define _bsp_gpio_h_include_
#include <stdint.h>

#include <stm32f4xx.h>
#include "los_bsp_adapter.h"

void BSP_GPIO_Init(GPIO_TypeDef  *GPIOx, uint32_t PINx, uint32_t MODE, uint32_t PULL);
void BSP_GPIO_IT_Init(GPIO_TypeDef  *GPIOx, uint32_t PINx, uint32_t MODE, uint32_t PULL,
                      IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority, 
                      _IRQHandler IRQHandler);

void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint16_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx);
#endif /* _bsp_gpio_h_include_ */