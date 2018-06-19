#ifndef __XF_SENSOR_HALL_H
#define __XF_SENSOR_HALL_H

#include "hal_types.h"

/* 霍尔传感器检测引脚配置为下拉输入 */
void HallPinInit(void);
/* 检测霍尔传感器数字引脚电平 */
uint8 DetectionHallDoutLevel(void);
uint8 XfGetHall(void);

#endif