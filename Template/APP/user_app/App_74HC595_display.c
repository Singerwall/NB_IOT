/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户xxxx底层驱动
  */ 
#include <includes.h>
#include "main.h"




static void * Task_Show_74HC595(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief 创建串口调试任务
  * @retval 无
 */
uint32_t create_Task_Show_74HC595(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_Show_74HC595_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*创建任务:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_Show_74HC595;
    stInitParam1.pcName = "Task_Show_74HC595";
    stInitParam1.usTaskPrio = Prio_Task_Show_74HC595;
    stInitParam1.uwStackSize =StackSize_Task_Show_74HC595;
  
    LOS_TaskLock();//锁住任务，防止新创建的任务比本任务高而发生调度
    uwRet = LOS_TaskCreate(&Task_Show_74HC595_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_Show_74HC595 failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//解锁任务，只有队列创建后才开始任务调度
    
    return LOS_OK;
}

/**
  * @brief 串口发送服务程序
  * @details 从队列中取出数据，并使用DMA进行发送。然后等待发送完成。
  * @param  pdata 无用
  * @retval 无
 */
static void * Task_Show_74HC595(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
  
    debug_printf("[%s] enter.\r\n", __func__);
    for(;;)
    {
       SHOW_595_SCAN();      
    
    }
}




