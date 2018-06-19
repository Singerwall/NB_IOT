/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  用户xxxx底层驱动
  */  
#include <includes.h>

TIM_HandleTypeDef SERVO_TIM_Handler;         /*定时器2PWM句柄*/ 
TIM_OC_InitTypeDef SERVO_TIM_CHxHandler;     /*定时器2通道3句柄*/

/*     -------                         --------
       |      |                        |       |
       |      |                        |       |
-------|1.5ms |------------------------|       |-------
       |-----------> 20ms<-------------|
1ms ---------1.5ms ---------- 2.0ms
     -90°             90°
*/
/**
  * @brief 输出到电调PWM频率 50 单位 Hz
 */
static const int frequence = 50;  /*50Hz*/
/**
  * @brief 定时器的装载值，PWM的最大值：对应(1s/50)
 */
static const int period = 60000;  /*20ms*/
/**
  * @brief 电调的中心点 1.5ms计数值  
 */
#define preiod_middle_pwm  (period / 20 * 15 / 10)
/**
  * @brief 电调的最大速度值 0.5ms，
 */

#define max_time_offset    5  /* 实际上 0.5ms * 10 */
#define max_angle          90  /* 实际上 0.5ms的偏移量对应 舵机转动  90° */

#define max_value_pwm  (period / 20 * max_time_offset / 10)  

#define pwm_per_degree  (max_value_pwm / max_angle)    /*每一° 对应 pwm 值*/ 



/**
  * @brief SERVO_TIM PWM部分初始化
  * @details PWM输出初始化

  * @retval 无
 */
void SERVO_TIM_PWM_Init(void)
{ 
    uint16_t prescaler = 100000000 / frequence / period;
    
    SERVO_TIM_Handler.Instance=SERVO_TIM;                  /*定时器2*/
    SERVO_TIM_Handler.Init.Prescaler=prescaler-1;            /*定时器分频*/
   
    SERVO_TIM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;/*向上计数模式*/
    SERVO_TIM_Handler.Init.Period=period-1;              /*自动重装载值*/
   
    SERVO_TIM_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&SERVO_TIM_Handler);           /*初始化PWM*/
    
    SERVO_TIM_CHxHandler.OCMode=TIM_OCMODE_PWM1;   /*模式选择PWM1*/
    SERVO_TIM_CHxHandler.Pulse=preiod_middle_pwm; /*设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%*/  
    SERVO_TIM_CHxHandler.OCPolarity=TIM_OCPOLARITY_LOW; /*输出比较极性为低 */
    SERVO_TIM_CHxHandler.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&SERVO_TIM_Handler,&SERVO_TIM_CHxHandler,SERVO_CHANNEL_X);/*配置SERVO_TIM通道x*/
    
    HAL_TIM_PWM_Start(&SERVO_TIM_Handler,SERVO_CHANNEL_X);/*开启PWM通道x*/
}


/**
  * @brief 定时器底层驱动，时钟使能，引脚配置
  * @details 此函数会被HAL_TIM_PWM_Init()调用
  * @param   htim:定时器句柄
  * @retval 无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  
  if(htim->Instance == SERVO_TIM)
  {
    GPIO_InitTypeDef GPIO_Initure;
    
    SERVO_CLK_ENABLE();			   /*使能定时器2*/
    SERVO_PIN_CLK_ENABLE();		   /*开启GPIOB时钟*/
	
    GPIO_Initure.Pin=SERVO_PIN;            /*PB10*/
    GPIO_Initure.Mode=SERVO_MODE;  	   /*复用推挽输出*/
    GPIO_Initure.Pull=SERVO_PULL;          /*上拉*/
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    /*高速*/
    GPIO_Initure.Alternate= SERVO_PIN_AF;   /*PB10复用为TIM2_CH2*/
    HAL_GPIO_Init(SERVO_PORT,&GPIO_Initure);
  }
}
/**
  * @brief 设置TIM通道x 的占空比
  * @details 
  * @param   compare:比较值
  * @retval 无
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
  * @brief 获取TIM通道x 的占空比
  * @details 
  * @param   compare:比较值
  * @retval 无
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
  * @brief 获取舵机的转向角度pwm
  * @retval angle 实际的转向角度 距离中心的角度
  * @note 
 */
 int get_servo_offset(void)
{
    int value;
    
    value = TIM_Get_SERVO_TIMCompare() - preiod_middle_pwm;
  
    return value;
}

/**
  * @brief 设置舵机的转向角度
  * @retval pwm 相对于preiod_middle_pwm的偏移量
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
* @brief 将pwm转化为角度
* @retval angle 实际的转向角度 距离中心的角度  范围（-90 ----+90）
* @return   angle 
*/
 int pwm_to_angle( int pwm)
{
  return  (pwm/pwm_per_degree);   
}

/**
  * @brief 角度转化为PWM值 
  * @retval angle 实际的转向角度 距离中心的角度  范围（-90 ----+90）
  * @return   pwm 
 */
int angle_to_pwm( int angle)
{
  return (angle*pwm_per_degree);
}


/**
* @brief read 读取pwm转化为角度
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
  * @brief 设置舵机的转向角度
  * @retval angle 实际的转向角度 距离中心的角度
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













