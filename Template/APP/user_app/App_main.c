/** <h2><center>&copy; COPYRIGHT 2013 XunFang </center></h2>
  * @file  can_server.c
  * @author  qyz
  * @version  V1.0.0
  * @date  09/05/2016
  * @brief  创建一个任务用于初始化CPU与建立应用任务
  */ 
#define USER_GLOBALS 
#include "sys.h" 
#include "includes.h" 
#include "App_BSP_int.h" 


int main(void)
{
      
#if (OS_TASK_NAME_EN > 0u)
    INT8U  err;
#endif
    

    Bsp_Chiplevel_Init();
    OSInit(); 
            
    OSTaskCreate(Task_StartTask,(void *)0,(OS_STK *)&Stack_Task_start[StackSize_Task_start-1],Prio_Task_start );

#if (OS_TASK_NAME_EN > 0u)
    OSTaskNameSet(Prio_Task_start, "Task_StartTask", &err);   
#endif                

    OSStart();
    
    return 0;
}


#ifdef USE_FULL_ASSERT
/******************************************************************************
*
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line 
number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) 
*/

  /* Infinite loop */
    while (1)
    {}
}
#endif