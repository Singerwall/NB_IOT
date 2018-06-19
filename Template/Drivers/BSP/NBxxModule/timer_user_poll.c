/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file 
  * @author  hzp
  * @version  V1.0.0
  * @date  04/26/2018
  * @brief BC95系列NB-IOT模块的AT指令产生及对指令响应进行解析
  */ 
//******************************************************************************
#include "timer_user_poll.h"
#include "nb_bc95.h"
extern uint64_t OSTimeGet( void);
bc95_timeout_cb  nb_timeoutCb;

uint32_t  gTickStart = 0;
uint32_t  gTickDelta = 0;
uint8_t   gTimeOpenFlag = 0;

 uint32_t HAL_GetTick(void)
 {
  return (uint32_t)OSTimeGet();
 }
void MX_TIM_Set(bc95_timeout_cb cb)
{
  nb_timeoutCb = cb;
}

void MX_TIM_Start(uint32_t ms)
{
  gTickDelta = ms;
  gTickStart = HAL_GetTick();
  gTimeOpenFlag = 1;
}

void MX_TIM_Stop(void)
{
  gTimeOpenFlag = 0;
}


void MX_TimerPoll()
{
  uint32_t tick;
  if(gTimeOpenFlag)
  {
    tick = HAL_GetTick();
    uint32_t de = tick >= gTickStart ? tick - gTickStart : tick + UINT32_MAX - gTickStart;
    if(gTickDelta > de)
    {
      gTickDelta -= de;
      gTickStart = tick;
    }
    else
    {
      gTickDelta = 0;
      gTimeOpenFlag = 0;
      if(nb_timeoutCb)
      {
        nb_timeoutCb();
      }
    }
    
  }
}
