#ifndef switch_control_h_include_
#define switch_control_h_include_
#include <stdint.h>
#include "los_task.h"


typedef void (*sw_set_status_backfunc)(uint8_t on_off) ;

typedef enum
{
  SW_MODE_OFF   =  0x00,   //״̬:��
  SW_MODE_ON    =  0x01,   //״̬:��
  SW_MODE_BLINK=   0x02,   //״̬:��˸
  SW_MODE_FLASH=   0x04,   //״̬:
  SW_MODE_TOGGLE=  0x08,   //ר��:ȡ��
  SW_MODE_IDLE=    0x10,   //״̬:����
}switch_mode;


/**
  * @brief SW���ƽṹ��
  */
typedef struct {
    uint8_t device;
    uint8_t status;
    switch_mode mode;          /* Operation mode */
    uint8_t number_blinks; /* Blink cycles left */
    uint8_t percent;      /* On cycle percentage */
    uint16_t period;      /* On/off cycle time (msec) */
    uint64_t next;      /* Time for next change */
    sw_set_status_backfunc  sw_set_status;   /*�ײ���ƻص����� Ҫ��ʼ��*/
    
} switch_control_t;

/* Defaults */
#define SW_DEFAULT_MAX_SWS      4  //�����û���Ҫ�����豸����������




                       
/**
  * @brief  ��������豸�ṹ��  
  */
typedef struct
{
    switch_control_t frame[SW_DEFAULT_MAX_SWS];
    
} sw_status_t;






void sw_control_center(sw_status_t * sw_status);
void app_sw_onoff (sw_status_t * sw_status, uint8_t index, uint8_t on_off);
void app_sw_blink (sw_status_t * sw_status, uint8_t index, uint16_t number_blinks, uint8_t percent, uint16_t period);
void InitT_sw_set_statusbackcall(sw_status_t * sw_status, uint8_t index, sw_set_status_backfunc callback);
void InitT_ALL_sw_set_statusbackcall(sw_status_t * sw_status);
#endif /* switch_control_h_include_ */
