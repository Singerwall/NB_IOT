#ifndef   _app_config_h_include_
#define   _app_config_h_include_


/*
********************************************************************************
*                             EXTERNS
********************************************************************************
*/
#ifdef   USER_GLOBALS
#define  USER_EXT
#else
#define  USER_EXT extern
#endif
/*
********************************************************************************
*                             TASK PRIORITIES
********************************************************************************
*/     
/* ---------10以下为互斥量使用 ------------------- */
/* -------- 10~20为驱动级任务优先级 -------------- */
/* -------- 20以上为任务用 ----------------------- */
#define  Prio_NB_Task       	        18
#define  Prio_Switch_Task       	19 
#define  Prio_Task_netif                20 
#define  Prio_Task_server               21 
#define  Prio_Task_client_recv          22
#define  Prio_Task_client_heart         23
#define  Prio_Task_ethernet_client      24
#define  Prio_Task_Zigbee               25
#define  Prio_Task_led       	        26
#define  Prio_Task_Beep                 27   
#define  Prio_Task_start      		28
#define  Prio_Task_usmart               29
#define  Prio_Task_Debug                30 
/*
********************************************************************************
*                                            TASK STACK SIZES
********************************************************************************
*/
#define StackSize_NB_Task               512*3
#define StackSize_Switch_Task           512
#define StackSize_Task_Debug            512
#define StackSize_Task_start	        128
#define StackSize_Task_led	        128
#define StackSize_Task_key  		128
#define StackSize_Task_Beep             128
#define StackSize_Task_Debug            512
#define StackSize_Task_usmart           384
#define StackSize_Task_netif            256
#define StackSize_Task_ethernet_client  1024
#define StackSize_Task_server           512
#define StackSize_Task_client_recv      512
#define StackSize_Task_client_heart     384
#define StackSize_Task_Zigbee           512
/*
********************************************************************************
*                                            APPLICATION TASK STACKS
********************************************************************************
*/       
#ifndef LOS_STM32F411RE
USER_EXT OS_STK Stack_Task_start[StackSize_Task_start];
USER_EXT OS_STK Stack_Task_led[StackSize_Task_led];
USER_EXT OS_STK Stack_Task_key[StackSize_Task_key];
USER_EXT OS_STK Stack_Task_Beep[StackSize_Task_Beep];
USER_EXT OS_STK Stack_Task_Debug[StackSize_Task_Debug];
USER_EXT OS_STK Stack_Task_usmart[StackSize_Task_usmart];
USER_EXT OS_STK Stack_Task_netif[StackSize_Task_netif];
USER_EXT OS_STK Stack_Task_server[StackSize_Task_server];
USER_EXT OS_STK Stack_Task_client_recv[StackSize_Task_client_recv];
USER_EXT OS_STK Stack_Task_client_heart[StackSize_Task_client_heart];
USER_EXT OS_STK Stack_Task_Zigbee[StackSize_Task_Zigbee];
USER_EXT OS_STK Stack_Task_ethernet_client[StackSize_Task_ethernet_client];
#endif

#endif