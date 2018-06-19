#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H




typedef enum 
{  
  KEY_1 = 0,
  KEY_2 = 1,
  KEY_3 = 2,
  Key_4 = 3,
} Key_Index_typedef;

typedef enum 
{  
  KEY_MODE_GPIO = 0,
  KEY_MODE_EXTI = 1
}KeyMode_TypeDef;

typedef enum 
{ 
  JOY_NONE  = 0,
  JOY_SEL   = 1,
  JOY_DOWN  = 2,
  JOY_LEFT  = 3,
  JOY_RIGHT = 4,
  JOY_UP    = 5
}JOYState_TypeDef;

/**
  * @}
  */ 



/**
  * @brief Key push-button 用户按键宏定义
  */
#define KEY_1_PIN                         GPIO_PIN_13
#define KEY_1_GPIO_PORT                   GPIOC

#define KEY_1_EXTI_LINE                   GPIO_PIN_13
#define KEY_1_EXTI_IRQn                   EXTI15_10_IRQn
#define KEY_1_EXTI_IRQn_PreemptPriority   2 
#define KEY_1_EXTI_IRQn_SubPriority       3
    

/**
  * @}
  */ 


#define LOS_KEY_PRESS   0

#define USER_KEY        0

#define LOS_GPIO_ERR    0xFF;


unsigned int LOS_EvbGetKeyVal(int KeyNum);

void BSP_Key_IT_Init(void);

#endif

