
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include "stm32f4xx_hal.h"

#include "los_bsp_key.h"

#include "bsp_gpio.h"







/*****************************************************************************
 Function    : BSP_GetKeyVal
 Description : Get GIOP Key value
 Input       : None
 Output      : None
 Return      : 0 or 1 ,0xff(error)(按下还是弹起)
 *****************************************************************************/
unsigned int BSP_GetKeyVal(Key_Index_typedef KeyNum)
{
  unsigned int KeyVal=  0xff;
   switch(KeyNum)
    {
        case KEY_1_PIN:
            KeyVal = HAL_GPIO_ReadPin(KEY_1_GPIO_PORT, KEY_1_PIN);
            break;     
        default:
          
              break;
      
    }
  return KeyVal;
}






