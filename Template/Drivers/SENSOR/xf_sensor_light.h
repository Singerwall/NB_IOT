#ifndef __XF_SENSOR_LIGHT_H
#define __XF_SENSOR_LIGHT_H

#include "hal_types.h"

/* ADC�ɼ�����ǿ��ģ��ģ������˵�ѹ */
float AdcLightAoutVoltage(void);
/* �Ѷ�Ӧ�ĵ�ѹת���ɹ���ǿ�� */
uint16 VccChangeLightStrength(void);

uint16 XfGetLight(void);

#endif