#ifndef _app_dwp_control_h_include_
#define _app_dwp_control_h_include_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>



/** @defgroup  xf_bsp_xxxx“˝Ω≈∑÷≈‰
  * @{
  */
/*############################### xxxxx #######################################*/
void date_time_to_dwp(u8 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec);
void lat_to_dwp(double lat);
void lon_to_dwp(double lon);
void u8_to_dwp(u8 value,u16 adress);
void stars_inuse_to_dwp(u8 value);
void sgn_GPS_stars_1_to_dwp(u8 value);
void sgn_GPS_stars_2_to_dwp(u8 value);
void sgn_GPS_stars_3_to_dwp(u8 value);
void sgn_GPS_stars_4_to_dwp(u8 value);
void sgn_BD_stars_1_to_dwp(u8 value);
void sgn_BD_stars_2_to_dwp(u8 value);
void sgn_BD_stars_3_to_dwp(u8 value);
void sgn_BD_stars_4_to_dwp(u8 value);

void net_nb_connect_statu_to_dwp(u8 statu );
void net_type_nb_to_dwp(u8 type);
void sgn_nb_to_dwp(char *string );
void IMEI_nb_to_dwp(char * IMEI);
void sgn_nb_logo_to_dwp(u8 value);
void pwoer_to_dwp(u8 value);
void relay_state_to_dwp(u8 state);
void elelock_state_to_dwp(u8 state);


/**
  * @}
  */ 



#endif/*_app_dwp_control_h_include_*/