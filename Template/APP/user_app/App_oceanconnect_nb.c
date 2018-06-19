/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  NBģ���oceanconnectƽ̨ͨ�ŶԽ�ҵ��
  */ 
#include <includes.h>
#include "main.h"


  
UINT32 OceanConnect=0;

static void * Task_OceanConnect(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief ���� NBģ���oceanconnectƽ̨ͨ������
  * @retval ��
 */
uint32_t create_Task_OceanConnect(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_OceanConnect_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*��������:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_OceanConnect;
    stInitParam1.pcName = "Task_OceanConnect";
    stInitParam1.usTaskPrio = Prio_Task_OceanConnect;
    stInitParam1.uwStackSize =StackSize_Task_OceanConnect;
  
    LOS_TaskLock();//��ס���񣬷�ֹ�´���������ȱ�����߶���������
    uwRet = LOS_TaskCreate(&Task_OceanConnect_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_OceanConnect failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//��������ֻ�ж��д�����ſ�ʼ�������
    
    return LOS_OK;
}

/**
  * @brief 
  * @details 
  * @param  pdata ����
  * @retval ��
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
    /*��������*/
    uwRet = LOS_QueueCreate("OceanConnect_queue", 3, &OceanConnect, 0, 40);
    if(uwRet != LOS_OK) debug_print_string("create OceanConnect_queue failure!,error:%x\n",uwRet);
    else debug_print_string("create the OceanConnect_queue success!\n");
    
    debug_printf("[%s] enter.\r\n", __func__);
    
    for(;;)
    {  
        uwRet = LOS_QueueRead(OceanConnect, &Device_packet_t_p, 10, 10);
        if(uwRet != LOS_OK)/*δ���ܵ�oceanconnect����*/
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
        else/*���յ���������*/
        {
          
          
        }   
    }
}






