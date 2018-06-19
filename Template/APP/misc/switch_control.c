
/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  switch.c
  * @author  zp.h
  * @version 1.0.0 
  * @date  08/01/2018
  * @brief ���ÿ��ؿ���
  *@instruction ���ļ����п��ƿ�������˸��������ռ�ձ����õĹ��ܡ�
  *
  * @bug 1 ���ڹ����豸�ṹ�����鳤�ȶ������Ե����ڶ����ͬ���豸ʱ��
           ��������豸�����ٿռ䣬���Ի��˷��ڴ�
          
             
  * @attention  1 backcallfunc����Ҫ�ǵó���ʼ�� 
                  ���ļ���Ӳ����صĵط�---�����豸���ƵĻص����� 


    @attention  2 ���Խ������ͬ���豸��ͬһ���豸���������У�
                 ͨ������backcallfunc������������ͬ������

    @attention  3 ������ñ��ļ����ٿ���            
                   1.���ȸ���ϵͳ�� �����ڿ��ؿ��Ƶ��豸������ ������ SW_DEFAULT_MAX_SWS �ĳ�ʼֵ
                   2.��ʼ�����п����豸�������ص����� InitT_sw_set_statusbackcall
                   3.���ٹ������ÿ��ƽṹ��
    @attention 4 �������ʱ ����uc/os������ ���Ա��ļ����uc/os���߳���RT-OS���Ǻܺõ�
    
    @attention 5  ʹ�ýӿ�֮ǰ�����ȳ�ʼ������������Ӳ�����󾯸�
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
//  * @brief ���� SW ���ƽṹ�������   �����ʹ���豸����
//*/
// 
//static sw_status_t sw_status;

///* SWS 
//#define SW_1     0x00  -- ��Ӧһ��������ƽṹ�� ---��Ӧһ���ص�����
//#define SW_2     0x01     
//#define SW_3     0X02
 



static void update_sw_mode_blink(switch_control_t *sw);
static void update_sw_mode_on_off(switch_control_t *sw, uint8_t on_off);



/**
  * @brief   ��Ҫʵʱˢ�¸ú���������������ѭ������
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
  * @brief  ���¿�������״̬ 
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
  * @brief  ����LED����ģʽ
 */
static void update_sw_mode_on_off(switch_control_t *sw, uint8_t on_off)
{
    
    if(sw->sw_set_status == NULL)
       return ;
    sw->mode = SW_MODE_IDLE;
    sw->sw_set_status(on_off);
}






/**
  * @brief  sw ����
  * @sw_status �����豸�ṹ������ָ��
  * @param sw ���Ƶ��豸���
  * @param   on_off   0 :��    1:��
 */
void app_sw_onoff (sw_status_t * sw_status, uint8_t index, uint8_t on_off)
{

  
    uint8_t sw_index;
    
   
    if(index >= SW_DEFAULT_MAX_SWS) /*��ֹ�������кŲ���ȷ���¹������*/
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
  * @brief   sw ��˸
  * @sw_status �����豸�ṹ������ָ��
  * @param sw ���Ƶ��豸���
  * @param  number_blinks   ��˸����
      percent
      |-------|        |--------|
      |   ��  |   ��   |  ��    | ��
------|       |--------|        |------
  * @param  percent   �ٷֱ� 0-100
  * @param  period  ���� ms
 */
void app_sw_blink (sw_status_t * sw_status, uint8_t index, uint16_t number_blinks, uint8_t percent, uint16_t period)
{
    uint8_t sw_index;
  
    uint32_t on_time;
   
    if(index >= SW_DEFAULT_MAX_SWS) /*��ֹ�������кŲ���ȷ���¹������*/
    {
    return ;
    }
    
	sw_index=index;
    
    OS_ENTER_CRITICAL();
    on_time = percent * period / 100;	 /*һ������������ʱ��*/
    sw_status->frame[sw_index].device = sw_index;
    sw_status->frame[sw_index].status = 1;  /*Ĭ���ȿ�-��-��-��*/
    sw_status->frame[sw_index].mode = SW_MODE_BLINK;
    sw_status->frame[sw_index].number_blinks = number_blinks * 2;
    sw_status->frame[sw_index].percent = percent;
    sw_status->frame[sw_index].period = period;
    sw_status->frame[sw_index].next = OSTimeGet()+ on_time ;  /*ʱ������ǻ���uc/os ����*/
    sw_status->frame[sw_index].sw_set_status( sw_status->frame[sw_index].status);
    OS_EXIT_CRITICAL();
}


/**
  * @brief switch���ƽӿ� �ص�������ʼ��

  * @param  ָ��ִ�к�����ָ��
  * @example  ��led1�Ŀ���������ֵ�� �ص�ָ��

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



/*����ʵ���޸�����ָ���ָ��*/
sw_set_status_backfunc callback1 = set_Led1;  
sw_set_status_backfunc callback2 = set_Led2;
sw_set_status_backfunc callback3 = set_Led3; 
sw_set_status_backfunc callback4 = set_Beep;



/**
  * @brief switch���ƽӿ� ��ʼ�����еĽӿں���   ��������豸ʱ���������case

 */
void InitT_ALL_sw_set_statusbackcall(sw_status_t * sw_status)
{
  for(uint8_t i=0; i<SW_DEFAULT_MAX_SWS; i++)
  {
    
    switch(i)
    {
      case 0:
        InitT_sw_set_statusbackcall( sw_status, i, callback1); //�����豸���޸�callback-->led1
          break;
      case 1:
         InitT_sw_set_statusbackcall( sw_status, i, callback2); //�����豸���޸�callback-->led2
          break;
      case 2:
         InitT_sw_set_statusbackcall( sw_status, i, callback3); //�����豸���޸�callback-->led3
          break;
      case 3:
          InitT_sw_set_statusbackcall( sw_status, i, callback4); //�����豸���޸�callback-->beep
          break;
      default:
        
        break;
    }
  
  }
}




