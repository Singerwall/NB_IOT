#ifndef __XF_SENSOR_HALL_H
#define __XF_SENSOR_HALL_H

#include "hal_types.h"

/* ���������������������Ϊ�������� */
void HallPinInit(void);
/* �������������������ŵ�ƽ */
uint8 DetectionHallDoutLevel(void);
uint8 XfGetHall(void);

#endif