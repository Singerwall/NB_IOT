/** <h2><center>&copy; COPYRIGHT 2013 XunFang </center></h2>
  * @file  can_server.c
  * @author  qyz
  * @version  V1.0.0
  * @date  09/05/2016
  * @brief  KEY
  */
#define App_BSP_int_MODULE
#include "key_driver.h"
#include "sys.h"
#include "delay.h"  
#include "App_key.h"
#include "App_BSP_int.h"
#include "App_Beep.h"
#include "App_Debug.h"

#define KEY_FLAG_PRESSURE_1     0x4000     

static OS_FLAG_GRP *key_flag;

static void key_task(void *pdata);

static uint32_t key_press_time;
/**
  * @brief  创建KEY任务
 */
void create_key_task(void)
{
    OSTaskCreate(key_task,
                 (void *)0,
                 (OS_STK*)&Stack_Task_key[StackSize_Task_key-1],
                 Prio_Task_key);
}

static void key_task(void *pdata)
{
    int index;
    int key_state;
    uint8_t err;
    KEY_Init();
    key_flag = OSFlagCreate(0, &err);
    assert_param(key_flag);
    
    while(1)
    {
        feed_watchdog();
        for(index = 0; index < KEY_NUMBER; ++index)
        {
            key_state = InputKey_GetKey(index);
            if(key_state)/*按下*/
            {   
                key_press_time = OSTimeGet();
                
                delay_ms(10);
                while(InputKey_GetKey(index))/*等待弹起*/
                {
                   delay_ms(10);
                }
                OSFlagPost(key_flag, KEY_FLAG_PRESSURE_1 << index, OS_FLAG_SET, &err);
                key_press_time = OSTimeGet() - key_press_time;     
                Beep_output_time(30);
                //debug_printf("key press time [%d]\r\n",key_press_time);
            }  
        }
        delay_ms(10);
    }
}

/**
  * @brief 判断是否有按键按下
  * @param key_index 键位索引值
  * @retval 如果该按键被按下，则返回1，否则返回0
 */
uint8_t is_key_pressure(uint8_t key_index)
{
    uint8_t err;
    OS_FLAGS ret_flag;
    
    ret_flag = OSFlagAccept(key_flag, KEY_FLAG_PRESSURE_1 << key_index, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, &err);
    
    if((err == OS_ERR_NONE) && (ret_flag & (KEY_FLAG_PRESSURE_1 << key_index)))
        return 1;

    return 0;
}

/**
  * @brief 获得按键按下时间
  * @retval key_press_time  键按下时间
 */
uint32_t get_key_press_time(void)
{
    return key_press_time;
}



