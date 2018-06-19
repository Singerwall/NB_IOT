/*
    here we can include some standard header file
*/
#include <stdio.h>
#include <string.h>
#include "los_bsp_adapter.h"
#include "los_bsp_key.h"
#include "los_bsp_led.h"
#include <includes.h>

/* while use bsp code to start system tick, don't use LOS header */
#define INCLUDE_LOS_HEADER
#ifdef INCLUDE_LOS_HEADER
#include "los_tick.ph"
#include "los_base.h"
#include "los_task.ph"
#include "los_swtmr.h"
#include "los_hwi.h"
#endif
#include "los_bsp_key.h"
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/


/*****************************************************************************
    global var
 *****************************************************************************/
/* current system Freq , should be set according to the microchip */
const unsigned int sys_clk_freq = 100000000;//100 000,000 Hz

/* tick count per second , don't change this */
const unsigned int tick_per_second = 1000;
static unsigned int g_ucycle_per_tick = 0;

/*
    if g_use_ram_vect == 1, we should use sct file STM32F429I-LiteOS.sct
    and we can use LOS_HwiCreate(), LOS_HwiDelete() dynamically regist a irq func
    if g_use_ram_vect == 0, we use default vector table in rom start at address 0x00000000
*/
const unsigned char g_use_ram_vect = 1;

/*****************************************************************************
    LOS function extern
 *****************************************************************************/
extern void LOS_SetTickSycle(unsigned int);
extern void LOS_TickHandler(void);
extern unsigned int osGetVectorAddr(void);

/*****************************************************************************
 Function    : osTickStart
 Description : Set system tick reqister, and start system tick exception
              Note that This function is called by LOS_Start() In LiteOS kernel.
              so, please don't init system tick before LiteOS kernel Inited
              in your own system.
 Input       : None
 Output      : None
 Return      : LOS_OK
 *****************************************************************************/
unsigned int osTickStart(void)
{
    unsigned int uwRet = 0;
    
    /* This code section LOS need, so don't change it */
    g_ucycle_per_tick = sys_clk_freq / tick_per_second;
    LOS_SetTickSycle(g_ucycle_per_tick);
    
    /* 
      Set system tick relaod register valude, current register valude and start
      system tick exception.
      Note: here can be replaced by some function , for example in Stm32 bsp
      you can just call SysTick_Config(sys_clk_freq/tick_per_second);
    */

#ifdef LOS_STM32F411RE
    SysTick_Config(g_ucycle_per_tick);
#else
    *(volatile UINT32 *)OS_SYSTICK_RELOAD_REG = g_ucycle_per_tick - 1;
    *((volatile UINT8 *)OS_NVIC_EXCPRI_BASE + (((UINT32)(-1) & 0xF) - 4)) = ((7 << 4) & 0xff);
    *(volatile UINT32 *)OS_SYSTICK_CURRENT_REG = 0;
    *(volatile UINT32 *)OS_SYSTICK_CONTROL_REG = (1 << 2) | (1 << 1) | (1 << 0);
#endif
    
    return uwRet;

}

/*****************************************************************************
 Function    : SysTick_Handler
 Description : system tick interrupt handler.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void SysTick_Handler(void)
{
    /*
        LOS need call LOS_TickHandler() in SysTick_Handler, don't change it,
        otherwise, LiteOS will not work.
    */
    LOS_TickHandler();
    
    /*add your systick handler code here */
    
    
    return;
}

/*****************************************************************************
 Function    : LosAdapIntInit
 Description : config vector table in ram. please see g_use_ram_vect illustration
               this is used for arm cortex-M3/M4
 Input       : None
 Output      : None
 Return      : LOS_OK
 *****************************************************************************/
void LosAdapIntInit(void)
{
    /*
        1:ReLoad vector table address at ram . if do nothing , vector table is
        located in rom 0x00000000
        2:set nvic irq priority group
        
        Note: here can be replaced by some function , for example in Stm32 bsp
        you can just call SCB->VTOR = osGetVectorAddr(); and
        NVIC_SetPriorityGrouping(OS_NVIC_AIRCR_PRIGROUP);
    */
    if (g_use_ram_vect)
    {
        *(volatile UINT32 *)OS_NVIC_VTOR = osGetVectorAddr();
        *(volatile UINT32 *)OS_NVIC_AIRCR = (0x05FA0000 | OS_NVIC_AIRCR_PRIGROUP << 8);
        
         NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);//modify by huanghzipeng
    }

    return;
}


static unsigned short  Preempt_Sub_Priority2Lite_Prior(uint32_t PreemptPriority, uint32_t SubPriority)
{
  unsigned short prior =0;
  uint32_t prioritygroup = 0x00; 
  prioritygroup = NVIC_GetPriorityGrouping();
  prior=NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority);
  
  return prior; 
}

/*****************************************************************************
 Function    : AppSetIrq  使用该函数替换LOS_HwiCreate()函数
 Description : 配置中断：使能中断，设置中断优先级 注册中断向量到lite_os系统中
            
 
 Output      : None
 Return      : None
 note        : add by huangzhipeng
 *****************************************************************************/
uint32_t AppSetIrq(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority, _IRQHandler IRQHandler)
{
  unsigned short  prior = 0;
#ifndef LOS_STM32F411RE
  /* Enable and set Button EXTI Interrupt to the  priority */
  HAL_NVIC_SetPriority(IRQn, PreemptPriority, SubPriority);
  HAL_NVIC_EnableIRQ(IRQn);
  return 0;//no use
#else
  prior= Preempt_Sub_Priority2Lite_Prior(PreemptPriority,SubPriority);
   
  return LOS_HwiCreate(IRQn, prior, 0, IRQHandler, 0);
 
#endif
  
}


/*****************************************************************************
 Function    : LosAdapIrqEnable
 Description : external interrupt enable, and set priority
               this function is called by LOS_HwiCreate(),
               so here can use bsp func to inplemente it 
               Note : if don't use LOS_HwiCreate(), leave it empty
 Input       : irqnum: external interrupt number
               prior: priority of this interrupt
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrqEnable(unsigned int irqnum, unsigned short prior)
{
    /*
        enable irq , for example in stm32 bsp you can use
        NVIC_EnableIRQ((IRQn_Type)irqnum);modify depend on your cpu
    */
    nvicSetIRQ(irqnum);
    /*
        set irq priority , for example in stm32 bsp you can use
        NVIC_SetPriority((IRQn_Type)irqnum, prior);
    */
    nvicSetIrqPRI(irqnum, prior << 4);
    return;
}

/*****************************************************************************
 Function    : LosAdapIrqDisable
 Description : external interrupt disable
               this function is called by LOS_HwiDelete(), so use bsp func
               to inplemente it
               Note : if don't use LOS_HwiDelete(), leave it empty
 Input       : irqnum: external interrupt number
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrqDisable(unsigned int irqnum)
{
    /*
        disable irq, for example in stm32 bsp you can use
        NVIC_DisableIRQ((IRQn_Type)irqnum);
    */
    nvicClrIRQ(irqnum);
     NVIC_DisableIRQ((IRQn_Type)irqnum);
    return;
}

 
/*****************************************************************************
 Function    : bsp_Init
 Description : enable the device on the dev baord  硬件初始化
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void bsp_Init(void)
{
  
  
   // USER_KEY_IT_Init();/*用戶按键初始化*/
   // LEDs_Init();/*LED初始化*/
 
  
  PowCtrl_Init(); 
    DEBUG_USART_Init();/*调试串口初始化*/

    SCREEN_USART_Init();/*屏幕连接串口初始化*/
    return;
}

/*****************************************************************************
 Function    : LOS_EvbTrace
 Description : trace printf
 Input       : const char *str
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbTrace(const char *str)
{
    LOS_EvbUartWriteStr(str);
    
    return;
}

/**
  * @brief 延时函数 uc -to--lite_os
  * @retval 无
 */
void OSTimeDly (uint32_t ticks)//延时时间决定了节拍精度
{
  LOS_TaskDelay(ticks);
}
/**
  * @brief 关任务调度 uc -to--lite_os
  * @retval 无
 */
void OS_ENTER_CRITICAL(void)
{
  LOS_TaskLock() ;
}
/**
  * @brief 开任务调度 uc -to--lite_os
  * @retval 无
 */
void OS_EXIT_CRITICAL(void)
{
  LOS_TaskUnlock();
}