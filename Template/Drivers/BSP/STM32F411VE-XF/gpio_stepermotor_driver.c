/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û�xxxx�ײ�����
  */ 

#include <includes.h>
#include "main.h"

#define EIGHT_SHOOT_MAX_LEN  8      //����

/**************int1-int2-int3-int4*/
/**************    A-    B-   C - D*/
#define PHASE_A   1<<5|0<<4|0<<7|0<<6
#define PHASE_AB  1<<5|1<<4|0<<7|0<<6
#define PHASE_B   0<<5|1<<4|0<<7|0<<6
#define PHASE_BC  0<<5|1<<4|1<<7|0<<6
#define PHASE_C   0<<5|0<<4|1<<7|0<<6
#define PHASE_CD  0<<5|0<<4|1<<7|1<<6
#define PHASE_D   0<<5|0<<4|0<<7|1<<6
#define PHASE_DA  1<<5|0<<4|0<<7|1<<6

#define PHASE_AD  1<<5|0<<4|0<<7|1<<6
#define PHASE_BD  0<<5|1<<4|0<<7|1<<6

#define PHASE_C  0<<5|0<<4|1<<7|0<<6


static const uint16  TwoWayEightShoot[EIGHT_SHOOT_MAX_LEN] = {PHASE_A, PHASE_AB, PHASE_B, PHASE_BC, PHASE_C, PHASE_CD, PHASE_D, PHASE_DA};                           //˫������





/**
  * @brief  Configures steper_motor GPIO.
  */
void steper_motor_Init(void)
{

  BSP_GPIO_Init(INT1_PORT, INT1_PIN|INT2_PIN|INT3_PIN|INT4_PIN, INT1_MODE, INT1_PULL);

}

/**
  * @brief  ˫�����ģʽ˳ʱ��תһ��.
  */
 void OwfsMotorCwTurnOneShoot(void)
{
   uint16_t  ReadValue,WriteValue; 
   static uint8 Pos = EIGHT_SHOOT_MAX_LEN - 1;
    

    ReadValue = GPIO_ReadInputData(INT1_PORT);
    ReadValue &=   (~(0x000f << 4));
    WriteValue = (ReadValue | TwoWayEightShoot[Pos]) ; 
    GPIO_Write(INT1_PORT, WriteValue);  /*����GPIOA��ֵ*/

    if( Pos )
    {
        Pos--;
    }
    else
    {
        Pos = EIGHT_SHOOT_MAX_LEN -1;
    }
}


/**
  * @brief  ˫�����ģʽ��ʱ��תһ��.
  * @brief  �����5.625  ,8�ļ�5.625*8=45 ���Ƴ�תһȦ 360 ��Ҫ8��8�ļ�64���� 
  */
 void OwfsMotorCcwTurnOneShoot(void)
{
    static uint8 Pos = 0;
    uint16_t  ReadValue,WriteValue; 
 
    ReadValue = GPIO_ReadInputData(INT1_PORT);
    ReadValue &=   (~(0x000f << 4));
    WriteValue = (ReadValue | TwoWayEightShoot[Pos]) ; 
    GPIO_Write(INT1_PORT, WriteValue);  /*����GPIOA��ֵ*/
    
    if(++Pos == EIGHT_SHOOT_MAX_LEN)
    {
        Pos = 0;
    }
}
/**
  * @brief  ���Ʋ������������ת���Ƕ� 
  * @brief  �����5.625  ,8�ļ�5.625*8=45 ���Ƴ�תһȦ 360 ��Ҫ8��8�ļ�64���� 
  * @param  mode 1:��ת; 0:��ת
  * @param  Coil ת���Ƕ�
  */
void control_moto_turn( u8 mode  , float Coil )
{
    int count = 0 ;
    while(  count  < Coil * 64*8*8)
    {
      if( mode == 1) //��ת
      {
        OwfsMotorCcwTurnOneShoot();
      }
      else//��ת
      {
        OwfsMotorCwTurnOneShoot();
      }
      OSTimeDly (1);
        count++;

    }
}
/**
  * @brief  ���Ʋ��������ת360�� 
  * @brief  
  * @param  on_off 1:����
  * @param 
  */
void set_moto_turn( u8 on_off )
{
  if(on_off == 1)
  control_moto_turn( 1, 1);
}