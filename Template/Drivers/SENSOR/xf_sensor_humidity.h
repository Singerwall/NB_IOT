#ifndef __XF_SENSOR_HUMIDITY_H
#define __XF_SENSOR_HUMIDITY_H

#include "hal_types.h"

/* 检测湿度传感器数字引脚电平 */
uint8 DetectionHumidDoutPinLevel(void);
float HumidChange(void);
float XfGetHumid(void);/*获取湿度值*/

#endif