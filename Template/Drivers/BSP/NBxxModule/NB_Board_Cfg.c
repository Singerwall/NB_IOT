//******************************************************************************

/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file NB_Board_Cfg.c
  * @author  hzp
  * @version  V1.0.0
  * @date  04/26/2018
  * @brief NB_IOT与平台相关的配置变量定义
  */ 
//******************************************************************************
#include <usart_nb_iot_driver.h>

#include "timer_user_poll.h"

#include "NB_Board.h"
#include "NB_BC95.h"

extern int NB_MsgreportCb(msg_types_t,int ,char*);

com_fxnTable  com_fxn = 
{
  .openFxn = NB_HAL_UARTDMA_Init,
  .sendFxn = NB_HAL_UART_Write,
  .closeFxn = NB_HAL_UART_Close
};

time_fxnTable time_fxn = 
{
  .initTimerFxn = MX_TIM_Set,
  .startTimerFxn = MX_TIM_Start,
  .stopTimerFxn = MX_TIM_Stop
};

hw_object_t  HWAtrrs_object = 
{
  .baudrate = 9600,
  .uart_fxnTable = &com_fxn,
  .timer_fxnTable = &time_fxn
};

NB_Config  nb_config = 
{
  .fxnTablePtr = NULL,
  .object = (void*)&HWAtrrs_object,
  .AppReceCB = NB_MsgreportCb,
  .log = NULL
};
