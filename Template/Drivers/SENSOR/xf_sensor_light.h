#ifndef __XF_SENSOR_LIGHT_H
#define __XF_SENSOR_LIGHT_H

#include "hal_types.h"

/* ADC采集光照强度模块模拟输出端电压 */
float AdcLightAoutVoltage(void);
/* 把对应的电压转换成光照强度 */
uint16 VccChangeLightStrength(void);

uint16 XfGetLight(void);

#endif