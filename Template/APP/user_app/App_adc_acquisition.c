/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û�xxxx�ײ�����
  */ 
#include <includes.h>
#include "main.h"




static void * Task_ADC(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief ����ADC�������
  * @retval ��
 */
uint32_t create_Task_ADC(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_ADC_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*��������:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_ADC;
    stInitParam1.pcName = "Task_ADC";
    stInitParam1.usTaskPrio = Prio_Task_ADC;
    stInitParam1.uwStackSize =StackSize_Task_ADC;
  
    LOS_TaskLock();//��ס���񣬷�ֹ�´���������ȱ�����߶���������
    uwRet = LOS_TaskCreate(&Task_ADC_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_ADC failed!,error:%x\n",uwRet);
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
static void * Task_ADC(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
    Device_packet_t packet;
    debug_printf("[%s] enter.\r\n", __func__);
     PN532_WakeUp();
    for(;;)
    {
#ifdef HARD_WARE_TEST
      debug_printf("HR31[%f]lux.\r\n", Get_HR31_Value() ); 
      debug_printf("MCU [%f]v.\r\n", Get_MCU_ADC_Value() ); 
      debug_printf("BAT [%f]v.\r\n", Get_BAT_Value() ); 
      debug_printf("CUR [%f]mA.\r\n",Get_CURRENT_Value() ); 
   
      Electronic_Lock_Close();
      control_moto_turn(1,5);
      OSTimeDly (1000);
      control_moto_turn(0,5.5);
      Electronic_Lock_Open();
     for(int i= -100;i<100;i=i+10)
     {
        set_servo_angle(i);
        OSTimeDly (1000);
     }
#endif   
          
#if 0
    OSTimeDly (1000); 
    if(1)
    {
      packet.data[0]=0x02;
      _cmd_13p56m_read_card(&packet);
      if(packet.ack == ACK_OK)
      {
        Electronic_Lock_Open();
        app_transmit_packet(10001,3,4,5,6,packet.data_size,packet.data);
         
        OSTimeDly (2000);
        Electronic_Lock_Close();
      }
    }
#endif

  
    OSTimeDly (1000);
    }
}




