/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  设备电源底层驱动
  */ 

#include <includes.h>
#include "main.h"




void PowCtrl_Init(void)
{
  BSP_GPIO_Init(WSN1_POW_Ctrl_PORT, WSN1_POW_Ctrl_PIN, POW_Ctrl_MODE, POW_Ctrl_PULL);
  BSP_GPIO_Init(WSN2_POW_Ctrl_PORT, WSN2_POW_Ctrl_PIN, POW_Ctrl_MODE, POW_Ctrl_PULL);
  
  BSP_GPIO_Init(RFID1_POW_Ctrl_PORT, RFID1_POW_Ctrl_PIN, POW_Ctrl_MODE, POW_Ctrl_PULL);
  BSP_GPIO_Init(RFID2_POW_Ctrl_PORT, RFID2_POW_Ctrl_PIN, POW_Ctrl_MODE, POW_Ctrl_PULL);
  
  BSP_GPIO_Init(LCD_POW_Ctrl_PORT, LCD_POW_Ctrl_PIN, POW_Ctrl_MODE, POW_Ctrl_PULL);
  BSP_GPIO_Init(GPS_POW_Ctrl_PORT, GPS_POW_Ctrl_PIN, POW_Ctrl_MODE, POW_Ctrl_PULL);
  BSP_GPIO_Init(BL_POW_Ctrl_PORT, BL_POW_Ctrl_PIN, POW_Ctrl_MODE, POW_Ctrl_PULL);
  
  
}


/**
  * @brief  WSN1_POW_Ctrl_PIN开关设置.
  */
 void set_WSN1_POW_Ctrl(uint8_t state)
{ 
  HAL_GPIO_WritePin(WSN1_POW_Ctrl_PORT, WSN1_POW_Ctrl_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}

/**
  * @brief  WSN2_POW_Ctrl_PIN开关设置.
  */
 void set_WSN2_POW_Ctrl(uint8_t state)
{ 
  HAL_GPIO_WritePin(WSN2_POW_Ctrl_PORT, WSN2_POW_Ctrl_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}

/**
  * @brief  RFID1_POW_Ctrl_PIN开关设置.
  */
 void set_RFID1_POW_Ctrl(uint8_t state)
{ 
  HAL_GPIO_WritePin(RFID1_POW_Ctrl_PORT, RFID1_POW_Ctrl_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}

/**
  * @brief  RFID2_POW_Ctrl_PIN开关设置.
  */
 void set_RFID2_POW_Ctrl(uint8_t state)
{ 
  HAL_GPIO_WritePin(RFID2_POW_Ctrl_PORT, RFID2_POW_Ctrl_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}

/**
  * @brief  LCD_POW_Ctrl_PIN开关设置.
  */
 void set_LCD_POW_Ctrl(uint8_t state)
{ 
  HAL_GPIO_WritePin(LCD_POW_Ctrl_PORT, LCD_POW_Ctrl_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}

/**
  * @brief  GPS_POW_Ctrl开关设置.
  */
 void set_GPS_POW_Ctrl(uint8_t state)
{ 
  HAL_GPIO_WritePin(GPS_POW_Ctrl_PORT, GPS_POW_Ctrl_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}
/**
  * @brief  BL_POW_Ctrl开关设置.
  */
 void set_BL_POW_Ctrl(uint8_t state)
{ 
  HAL_GPIO_WritePin(BL_POW_Ctrl_PORT, BL_POW_Ctrl_PIN,((state == ON )? GPIO_PIN_SET:GPIO_PIN_RESET));
}


/**
  * @brief 外设设备电源控制接口
  * @details 
  * @param  index 设备类型 
* @retval state 0:关，1：开
 */
void control_device_pow(Pow_Ctrl_Device_TypeDef index, uint8_t state)
{
    switch(index)
    {
    case WSN1:
        set_WSN1_POW_Ctrl( state );
        break;
    case WSN2:
        set_WSN2_POW_Ctrl( state );
        break;   
    case RFID1:
        set_RFID1_POW_Ctrl( state );
        break;  
    case RFID2:
        set_RFID2_POW_Ctrl( state );
        break;
    case LCD:
        set_LCD_POW_Ctrl( state );
        break;   
    case GPS:
        set_GPS_POW_Ctrl( state );
        break;  
     case BL:
        set_BL_POW_Ctrl( state );
        break;  
    default:
        assert_param(0);
    }
}

/**
  * @brief 关闭所有设备电源
 */
void off_all_POW_Ctrl(void)
{
  set_WSN1_POW_Ctrl(0);
  set_WSN2_POW_Ctrl(0);
  set_RFID1_POW_Ctrl(0);
  set_RFID2_POW_Ctrl(0);
  set_LCD_POW_Ctrl(0);
  set_GPS_POW_Ctrl(0);
  set_BL_POW_Ctrl(0); 
}

/**
  * @brief 打开所有设备电源
 */
void on_all_POW_Ctrl(void)
{
  set_WSN1_POW_Ctrl(1);
  set_WSN2_POW_Ctrl(1);
  set_RFID1_POW_Ctrl(1);
  set_RFID2_POW_Ctrl(1);
  set_LCD_POW_Ctrl(1);
  set_GPS_POW_Ctrl(1);
  set_BL_POW_Ctrl(1); 
}


