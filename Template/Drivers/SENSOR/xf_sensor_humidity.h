#ifndef __XF_SENSOR_HUMIDITY_H
#define __XF_SENSOR_HUMIDITY_H

#include "hal_types.h"

/* ���ʪ�ȴ������������ŵ�ƽ */
uint8 DetectionHumidDoutPinLevel(void);
float HumidChange(void);
float XfGetHumid(void);/*��ȡʪ��ֵ*/

#endif