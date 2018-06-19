#ifndef __AXIS3MOD
#define __AXIS3MOD

#include "hal_types.h"

enum
{
    X = 0,
    Y    ,
    Z 
};

void Axis3Mma7600Init(void);/*初始化三轴传感器*/

int8 *XfGetAxis(void);/*获取传感器值*/

#endif
