/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û�xxxx�ײ�����
  */  
#include <includes.h>

TIM_HandleTypeDef SERVO_TIM_Handler;         /*��ʱ��2PWM���*/ 
TIM_OC_InitTypeDef SERVO_TIM_CHxHandler;     /*��ʱ��2ͨ��3���*/

/*     -------                         --------
       |      |                        |       |
       |      |                        |       |
-------|1.5ms |------------------------|       |-------
       |-----------> 20ms<-------------|
1ms ---------1.5ms ---------- 2.0ms
     -90��             90��
*/
/**
  * @brief ��������PWMƵ�� 50 ��λ Hz
 */
static const int frequence = 50;  /*50Hz*/
/**
  * @brief ��ʱ����װ��ֵ��PWM�����ֵ����Ӧ(1s/50)
 */
static const int period = 60000;  /*20ms*/
/**
  * @brief ��������ĵ� 1.5ms����ֵ  
 */
#define preiod_middle_pwm  (period / 20 * 15 / 10)
/**
  * @brief ���������ٶ�ֵ 0.5ms��
 */

#define max_time_offset    5  /* ʵ���� 0.5ms * 10 */
#define max_angle          90  /* ʵ���� 0.5ms��ƫ������Ӧ ���ת��  90�� */

#define max_value_pwm  (period / 20 * max_time_offset / 10)  

#define pwm_per_degree  (max_value_pwm / max_angle)    /*ÿһ�� ��Ӧ pwm ֵ*/ 



/**
  * @brief SERVO_TIM PWM���ֳ�ʼ��
  * @details PWM�����ʼ��

  * @retval ��
 */
void SERVO_TIM_PWM_Init(void)
{ 
    uint16_t prescaler = 100000000 / frequence / period;
    
    SERVO_TIM_Handler.Instance=SERVO_TIM;                  /*��ʱ��2*/
    SERVO_TIM_Handler.Init.Prescaler=prescaler-1;            /*��ʱ����Ƶ*/
   
    SERVO_TIM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;/*���ϼ���ģʽ*/
    SERVO_TIM_Handler.Init.Period=period-1;              /*�Զ���װ��ֵ*/
   
    SERVO_TIM_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&SERVO_TIM_Handler);           /*��ʼ��PWM*/
    
    SERVO_TIM_CHxHandler.OCMode=TIM_OCMODE_PWM1;   /*ģʽѡ��PWM1*/
    SERVO_TIM_CHxHandler.Pulse=preiod_middle_pwm; /*���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%*/  
    SERVO_TIM_CHxHandler.OCPolarity=TIM_OCPOLARITY_LOW; /*����Ƚϼ���Ϊ�� */
    SERVO_TIM_CHxHandler.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&SERVO_TIM_Handler,&SERVO_TIM_CHxHandler,SERVO_CHANNEL_X);/*����SERVO_TIMͨ��x*/
    
    HAL_TIM_PWM_Start(&SERVO_TIM_Handler,SERVO_CHANNEL_X);/*����PWMͨ��x*/
}


/**
  * @brief ��ʱ���ײ�������ʱ��ʹ�ܣ���������
  * @details �˺����ᱻHAL_TIM_PWM_Init()����
  * @param   htim:��ʱ�����
  * @retval ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  
  if(htim->Instance == SERVO_TIM)
  {
    GPIO_InitTypeDef GPIO_Initure;
    
    SERVO_CLK_ENABLE();			   /*ʹ�ܶ�ʱ��2*/
    SERVO_PIN_CLK_ENABLE();		   /*����GPIOBʱ��*/
	
    GPIO_Initure.Pin=SERVO_PIN;            /*PB10*/
    GPIO_Initure.Mode=SERVO_MODE;  	   /*�����������*/
    GPIO_Initure.Pull=SERVO_PULL;          /*����*/
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    /*����*/
    GPIO_Initure.Alternate= SERVO_PIN_AF;   /*PB10����ΪTIM2_CH2*/
    HAL_GPIO_Init(SERVO_PORT,&GPIO_Initure);
  }
}
/**
  * @brief ����TIMͨ��x ��ռ�ձ�
  * @details 
  * @param   compare:�Ƚ�ֵ
  * @retval ��
 */
 void TIM_Set_SERVO_TIMCompare(u32 compare)
{
  switch(SERVO_CHANNEL_X )
  {
     case TIM_CHANNEL_1:
       SERVO_TIM->CCR1=compare; 
       break;
     case TIM_CHANNEL_2:
       SERVO_TIM->CCR2=compare;
       break;
     case TIM_CHANNEL_3:
       SERVO_TIM->CCR3=compare;
       break;
     case TIM_CHANNEL_4:
       SERVO_TIM->CCR4=compare;
       break;
     default:
       break;
  }
}
/**
  * @brief ��ȡTIMͨ��x ��ռ�ձ�
  * @details 
  * @param   compare:�Ƚ�ֵ
  * @retval ��
 */
 int32_t TIM_Get_SERVO_TIMCompare(void)
{
  int32_t compare = 0;
  
  switch(SERVO_CHANNEL_X )
  {
     case TIM_CHANNEL_1:
       compare = SERVO_TIM->CCR1; 
       break;
     case TIM_CHANNEL_2:
       compare = SERVO_TIM->CCR2;
       break;
     case TIM_CHANNEL_3:
       compare = SERVO_TIM->CCR3;
       break;
     case TIM_CHANNEL_4:
       compare =  SERVO_TIM->CCR4;
       break;
     default:
       break;
  }
  
   return compare;
}
/**
  * @brief ��ȡ�����ת��Ƕ�pwm
  * @retval angle ʵ�ʵ�ת��Ƕ� �������ĵĽǶ�
  * @note 
 */
 int get_servo_offset(void)
{
    int value;
    
    value = TIM_Get_SERVO_TIMCompare() - preiod_middle_pwm;
  
    return value;
}

/**
  * @brief ���ö����ת��Ƕ�
  * @retval pwm �����preiod_middle_pwm��ƫ����
  * @note 
 */
void set_servo_offset(int pwm)
{
     if(pwm > max_value_pwm)
        pwm = max_value_pwm;
    else if(pwm < -max_value_pwm)
        pwm = -max_value_pwm;
    
    TIM_Set_SERVO_TIMCompare(preiod_middle_pwm + pwm);  
}


/**
* @brief ��pwmת��Ϊ�Ƕ�
* @retval angle ʵ�ʵ�ת��Ƕ� �������ĵĽǶ�  ��Χ��-90 ----+90��
* @return   angle 
*/
 int pwm_to_angle( int pwm)
{
  return  (pwm/pwm_per_degree);   
}

/**
  * @brief �Ƕ�ת��ΪPWMֵ 
  * @retval angle ʵ�ʵ�ת��Ƕ� �������ĵĽǶ�  ��Χ��-90 ----+90��
  * @return   pwm 
 */
int angle_to_pwm( int angle)
{
  return (angle*pwm_per_degree);
}


/**
* @brief read ��ȡpwmת��Ϊ�Ƕ�
* @retval 
* @return   angle 
*/
int get_servo_angle(void)
{
  int pwm_now = 0;
 
  pwm_now= get_servo_offset();
  
  return  pwm_to_angle(pwm_now);  
}

/**
  * @brief ���ö����ת��Ƕ�
  * @retval angle ʵ�ʵ�ת��Ƕ� �������ĵĽǶ�
  * @note 
 */
void set_servo_angle(int angle)
{
   int pwm = 0;
   int now_angle=get_servo_angle();

    if(angle > max_angle)
        angle = max_angle;
    else if(angle < -max_angle)
        angle = -max_angle;
   
    OSTimeDly (abs(angle - now_angle)/4);
    pwm =angle_to_pwm(angle);
    set_servo_offset(pwm);

}













