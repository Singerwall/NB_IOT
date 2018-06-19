/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û�xxxx�ײ�����
  */ 
#include <includes.h>
#include "main.h"




static void * Task_Show_74HC595(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief �������ڵ�������
  * @retval ��
 */
uint32_t create_Task_Show_74HC595(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_Show_74HC595_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*��������:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_Show_74HC595;
    stInitParam1.pcName = "Task_Show_74HC595";
    stInitParam1.usTaskPrio = Prio_Task_Show_74HC595;
    stInitParam1.uwStackSize =StackSize_Task_Show_74HC595;
  
    LOS_TaskLock();//��ס���񣬷�ֹ�´���������ȱ�����߶���������
    uwRet = LOS_TaskCreate(&Task_Show_74HC595_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_Show_74HC595 failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//��������ֻ�ж��д�����ſ�ʼ�������
    
    return LOS_OK;
}

/**
  * @brief ���ڷ��ͷ������
  * @details �Ӷ�����ȡ�����ݣ���ʹ��DMA���з��͡�Ȼ��ȴ�������ɡ�
  * @param  pdata ����
  * @retval ��
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




