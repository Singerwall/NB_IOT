#ifndef _LOS_BSP_ADAPTER_H
#define _LOS_BSP_ADAPTER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "stm32f411xe.h"

typedef void (* _IRQHandler)(void);


extern void bsp_Init(void);
extern void LOS_EvbTrace(const char *str);
extern uint32_t AppSetIrq(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority, _IRQHandler IRQHandler);

extern void OSTimeDly (uint32_t ticks);
extern void OS_ENTER_CRITICAL(void);
extern void OS_EXIT_CRITICAL(void);


#endif
