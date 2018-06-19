/*
************************************************************************************************
主要的包含文件

文 件: INCLUDES.C ucos包含文件
作 者: Jean J. Labrosse
************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#include <stm32f411xe.h>
#include <core_cm4.h>
#include "stm32f4xx_hal.h"
#include <stm32f4xx.h>
#include "stm32f4xx_hal_def.h"

/*los*/
#include "los_task.h"
#include "los_queue.h"
#include "los_membox.h"
#include "los_typedef.h"
#include "los_sys.h"
#include "los_tick.h"

#include "los_config.h"
#include "main.h"
#include "los_bsp_adapter.h"

/*bsp*/
#include <delay.h>
#include <bsp_gpio.h>
#include <gpio_iic_driver.h>
#include <demo_xx_driver.h>

#include <usart_debug_driver.h>
#include <usart_nb_iot_driver.h>
#include <usart_serialscreen_driver.h>
#include <gpio_leds_driver.h>
#include <gpio_userkey_driver.h>
#include <gpio_matrix_driver.h>
#include <gpio_powctrl_driver.h>
#include <gpio_beep_driver.h>
#include <gpio_74hc595_driver.h>
#include "sensor_single_line.h"
#include "sensor_hall_driver.h"
#include "sensor_light5537_driver.h"
#include "adc_driver.h"
#include "oled_driver.h"


#include <timer_user_poll.h>
#include <nb_types.h>
#include <NB_BC95.h>
#include <NB_Board.h>
#include <NB_Board_Cfg.h>

#include "usmart.h"		
#include "usmart_str.h"

#include <spi.h>
#include <spi1_wsn2_driver.h>
#include <spi4_wsn1_driver.h>

#include <spi2_rfid2_driver.h>
#include <spi5_rfid1_driver.h>

#include <spi3_stc_driver.h>

/*APP*/
#include <App_Confg.h>

#include "App_Debug.h"
#include <App_switch_control.h>
#include <App_NB_control.h>


/*misc*/

#include "switch_control.h"
#include "device_protocol.h"
#include "algorithm.h"
#endif































