#ifndef __AXIS3MOD
#define __AXIS3MOD

#include "hal_types.h"

enum
{
    X = 0,
    Y    ,
    Z 
};

void Axis3Mma7600Init(void);/*��ʼ�����ᴫ����*/

int8 *XfGetAxis(void);/*��ȡ������ֵ*/

#endif
