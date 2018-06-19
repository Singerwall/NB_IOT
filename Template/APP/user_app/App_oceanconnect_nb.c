/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  NB模块和oceanconnect平台通信对接业务
  */ 
#include <includes.h>
#include "main.h"


  
UINT32 OceanConnect=0;

static void * Task_OceanConnect(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief 创建 NB模块和oceanconnect平台通信任务
  * @retval 无
 */
uint32_t create_Task_OceanConnect(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_OceanConnect_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*创建任务:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_OceanConnect;
    stInitParam1.pcName = "Task_OceanConnect";
    stInitParam1.usTaskPrio = Prio_Task_OceanConnect;
    stInitParam1.uwStackSize =StackSize_Task_OceanConnect;
  
    LOS_TaskLock();//锁住任务，防止新创建的任务比本任务高而发生调度
    uwRet = LOS_TaskCreate(&Task_OceanConnect_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_OceanConnect failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//解锁任务，只有队列创建后才开始任务调度
    
    return LOS_OK;
}

/**
  * @brief 
  * @details 
  * @param  pdata 无用
  * @retval 无
 */
static void * Task_OceanConnect(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
  UINT32 uwRet = 0; 
  int Device_packet_t_p;
 __IO UINT32 sec_count = 0; 
  u8 temp_data[6]={0};
    /*创建队列*/
    uwRet = LOS_QueueCreate("OceanConnect_queue", 3, &OceanConnect, 0, 40);
    if(uwRet != LOS_OK) debug_print_string("create OceanConnect_queue failure!,error:%x\n",uwRet);
    else debug_print_string("create the OceanConnect_queue success!\n");
    
    debug_printf("[%s] enter.\r\n", __func__);
    
    for(;;)
    {  
        uwRet = LOS_QueueRead(OceanConnect, &Device_packet_t_p, 10, 10);
        if(uwRet != LOS_OK)/*未接受到oceanconnect数据*/
        {    
         
           OSTimeDly (1000);
          sec_count++;
          if(sec_count%18 == 0)
          {
             sec_count =0;
             DS18B20_Get_Temp(temp_data); 
             app_transmit_packet(10000,07,01,0x14,0,5,temp_data);     
          }

        }
        else/*接收到控制命令*/
        {
          
          
        }   
    }
}






