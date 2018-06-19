/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  08/01/2013
  * @brief  用户switch 控制应用
  */ 
#include <includes.h>



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#include <includes.h>
  
  
 /**
  * @brief 开辟 SW 控制结构体的数组 1
  */
static sw_status_t sw_status;   

static void * Switch_Task(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);


/**
  * @brief 创建开关量管理控制任务
  * @retval 无
 */
uint32_t create_Switch_Task(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Switch_Task_Index = 0;
    TSK_INIT_PARAM_S stTaskInitParam;
    
    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    /*创建任务:Switch_Task*/
    stTaskInitParam.pfnTaskEntry = Switch_Task;
    stTaskInitParam.pcName = "Switch_Task";
    stTaskInitParam.usTaskPrio = Prio_Switch_Task;
    stTaskInitParam.uwStackSize =StackSize_Switch_Task;

    LOS_TaskLock();//锁住任务，防止新创建的任务比本任务高而发生调度
    uwRet = LOS_TaskCreate(&Switch_Task_Index, &stTaskInitParam);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Switch_Task failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//解锁任务，
    
    return LOS_OK;
}

extern void leds_config(void);

u8 SPI5RX[100]={0};
u8 SPI5RX_COUNT =0;
/**
  * @brief 串口发送服务程序
  * @details 从队列中取出数据，并使用DMA进行发送。然后等待发送完成。
  * @param  pdata 无用
  * @retval 无
 */
static void * Switch_Task(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
  LEDs_Init();/*LED初始化*/
  BEEP_Init();/*蜂鳴器初始化*/
 
  InitT_ALL_sw_set_statusbackcall(&sw_status); //2.初始化所有接口回调函数
  debug_printf("[%s] enter.\r\n", __func__);
  
  SPI3_Init();
  SPI5_Init(); 
  
  OLED_Init(); 
//#define test_leds
    
#ifdef test_leds   
    while(1)
    { 
      set_Led1(1);
      set_Led2(1);
      set_Led3(1);
      off_all_POW_Ctrl();
     

 delay_us(1000);

      set_Led1(0);
      set_Led2(0);
      set_Led3(0);
      on_all_POW_Ctrl();
 //  delay_ms(1000);;
       delay_ms(1000);
       delay_us(1000);
    }
#else
    
    app_sw_blink (&sw_status, LED_1, 10, 50, 1000);
    app_sw_blink (&sw_status, LED_2, 10, 50, 1000);
    app_sw_blink (&sw_status, LED_3, 10, 50, 1000);
  
    //app_sw_blink (&sw_status, BEEP_1,   5, 50, 1000); 

    
    for(;;)
    {  
        OS_ENTER_CRITICAL();
//        for(u8 i=0;i<100;i++)
//        {        
//         SPI5RX[i] = SPI5_ReadWriteByte(i);
//          if(i==99)
//            i=0;
//            OSTimeDly (100);
//        }
        
        

        sw_control_center(&sw_status);
        OS_EXIT_CRITICAL();  
        OSTimeDly (10); //延时时间决定了节拍精度10
      
    }
#endif
}



void app_sw_blink_interface(uint8_t index, uint16_t number_blinks, uint8_t percent, uint16_t period)
{

    app_sw_blink (&sw_status, index, number_blinks, number_blinks, period); 
}

void app_sw_onoff_interface(uint8_t index, uint8_t on_off)
{
    app_sw_onoff (&sw_status,  index,  on_off);
}






#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


