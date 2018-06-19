
/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  switch.c
  * @author  zp.h
  * @version 1.0.0 
  * @date  08/01/2018
  * @brief 常用开关控制
  *@instruction 本文件具有控制开关量闪烁、次数、占空比设置的功能。
  *
  * @bug 1 由于管理设备结构体数组长度定了所以当存在多个不同的设备时，
           以做大的设备数开辟空间，所以会浪费内存
          
             
  * @attention  1 backcallfunc函数要记得出初始化 
                  本文件与硬件相关的地方---就是设备控制的回调函数 


    @attention  2 可以将多个不同的设备在同一个设备管理数组中，
                 通过引进backcallfunc函数来操作不同的设置

    @attention  3 如何利用本文件快速开发            
                   1.首先根据系统中 类似于开关控制的设备的总数 来设置 SW_DEFAULT_MAX_SWS 的初始值
                   2.初始化所有控制设备的驱动回调函数 InitT_sw_set_statusbackcall
                   3.开辟管理设置控制结构体
    @attention 4 这里的延时 基于uc/os函数， 所以本文件配合uc/os或者常用RT-OS都是很好的
    
    @attention 5  使用接口之前必须先初始化，否则会出现硬件错误警告
  */ 


#include "los_bsp_adapter.h"
#include <gpio_leds_driver.h>
#include <gpio_beep_driver.h>
#include "switch_control.h"


 uint64_t OSTimeGet( void)
 {
  return LOS_TickCountGet(); 
 }              

///**
//  * @brief 开辟 SW 控制结构体的数组   这个由使用设备开辟
//*/
// 
//static sw_status_t sw_status;

///* SWS 
//#define SW_1     0x00  -- 对应一个管理控制结构体 ---对应一个回调函数
//#define SW_2     0x01     
//#define SW_3     0X02
 



static void update_sw_mode_blink(switch_control_t *sw);
static void update_sw_mode_on_off(switch_control_t *sw, uint8_t on_off);



/**
  * @brief   需要实时刷新该函数（放在任务中循环处理）
 */
void sw_control_center(sw_status_t * sw_status)
{
  
    for(uint8_t i = 0; i < SW_DEFAULT_MAX_SWS; ++i)
    {
         
        switch(sw_status->frame[i].mode)
        {
        case SW_MODE_BLINK:
            update_sw_mode_blink(&sw_status->frame[i]);
            break;
        case SW_MODE_ON:
            update_sw_mode_on_off(&sw_status->frame[i], 1);
            break;
        case SW_MODE_OFF:
            update_sw_mode_on_off(&sw_status->frame[i], 0);
            break;
        }
    }
}

/**
  * @brief  更新控制器的状态 
 */
static void update_sw_mode_blink(switch_control_t *sw)
{
    uint64_t now_time = OSTimeGet();
    uint32_t on_time;

    if (now_time < sw->next)
        return;
    
    if(sw->sw_set_status == NULL)
        return ;

    if(sw->number_blinks)
    {
        sw->number_blinks--;
        if(sw->number_blinks == 1)
            sw->mode = SW_MODE_IDLE;
    }
    
    sw->status = !sw->status;
    sw->sw_set_status( sw->status);
    on_time = sw->percent * sw->period / 100;
    if (sw->status == 1)
        sw->next = now_time + on_time;
    else
        sw->next = now_time + sw->period - on_time; 
}

/**
  * @brief  更新LED开关模式
 */
static void update_sw_mode_on_off(switch_control_t *sw, uint8_t on_off)
{
    
    if(sw->sw_set_status == NULL)
       return ;
    sw->mode = SW_MODE_IDLE;
    sw->sw_set_status(on_off);
}






/**
  * @brief  sw 开关
  * @sw_status 管理设备结构体数组指针
  * @param sw 控制的设备序号
  * @param   on_off   0 :关    1:开
 */
void app_sw_onoff (sw_status_t * sw_status, uint8_t index, uint8_t on_off)
{

  
    uint8_t sw_index;
    
   
    if(index >= SW_DEFAULT_MAX_SWS) /*防止设置序列号不正确导致过界访问*/
    {
    return ;
    }
    
	sw_index=index;
    
    OS_ENTER_CRITICAL();
  
    sw_status->frame[sw_index].device = sw_index;
    sw_status->frame[sw_index].status = on_off;
    if(on_off == 0)
    {
       sw_status->frame[sw_index].mode = SW_MODE_OFF; 
    }
    else
    {
        sw_status->frame[sw_index].mode = SW_MODE_ON;
    }
    
    OS_EXIT_CRITICAL();
}


/**
  * @brief   sw 闪烁
  * @sw_status 管理设备结构体数组指针
  * @param sw 控制的设备序号
  * @param  number_blinks   闪烁次数
      percent
      |-------|        |--------|
      |   亮  |   灭   |  亮    | 灭
------|       |--------|        |------
  * @param  percent   百分比 0-100
  * @param  period  周期 ms
 */
void app_sw_blink (sw_status_t * sw_status, uint8_t index, uint16_t number_blinks, uint8_t percent, uint16_t period)
{
    uint8_t sw_index;
  
    uint32_t on_time;
   
    if(index >= SW_DEFAULT_MAX_SWS) /*防止设置序列号不正确导致过界访问*/
    {
    return ;
    }
    
	sw_index=index;
    
    OS_ENTER_CRITICAL();
    on_time = percent * period / 100;	 /*一个周期中亮的时间*/
    sw_status->frame[sw_index].device = sw_index;
    sw_status->frame[sw_index].status = 1;  /*默认先开-关-开-关*/
    sw_status->frame[sw_index].mode = SW_MODE_BLINK;
    sw_status->frame[sw_index].number_blinks = number_blinks * 2;
    sw_status->frame[sw_index].percent = percent;
    sw_status->frame[sw_index].period = period;
    sw_status->frame[sw_index].next = OSTimeGet()+ on_time ;  /*时间计算是基于uc/os 节拍*/
    sw_status->frame[sw_index].sw_set_status( sw_status->frame[sw_index].status);
    OS_EXIT_CRITICAL();
}


/**
  * @brief switch控制接口 回调函数初始化

  * @param  指向执行函数的指针
  * @example  将led1的控制驱动赋值给 回调指针

            void led1( u8 mode)
            {
              GPIO_WriteBit(LED_PORT, LED_PIN_1, (on ? Bit_SET : Bit_RESET));
            }

            InitT_sw_set_statusbackcall(sw_status,1,led1)
 */
void InitT_sw_set_statusbackcall(sw_status_t * sw_status, uint8_t index, sw_set_status_backfunc callback)
{
   if(callback == NULL)
       return ;
   sw_status->frame[index].sw_set_status = callback; 
}



/*根据实际修改以下指针的指向*/
sw_set_status_backfunc callback1 = set_Led1;  
sw_set_status_backfunc callback2 = set_Led2;
sw_set_status_backfunc callback3 = set_Led3; 
sw_set_status_backfunc callback4 = set_Beep;



/**
  * @brief switch控制接口 初始化所有的接口函数   当新添加设备时，可以添加case

 */
void InitT_ALL_sw_set_statusbackcall(sw_status_t * sw_status)
{
  for(uint8_t i=0; i<SW_DEFAULT_MAX_SWS; i++)
  {
    
    switch(i)
    {
      case 0:
        InitT_sw_set_statusbackcall( sw_status, i, callback1); //根据设备来修改callback-->led1
          break;
      case 1:
         InitT_sw_set_statusbackcall( sw_status, i, callback2); //根据设备来修改callback-->led2
          break;
      case 2:
         InitT_sw_set_statusbackcall( sw_status, i, callback3); //根据设备来修改callback-->led3
          break;
      case 3:
          InitT_sw_set_statusbackcall( sw_status, i, callback4); //根据设备来修改callback-->beep
          break;
      default:
        
        break;
    }
  
  }
}




