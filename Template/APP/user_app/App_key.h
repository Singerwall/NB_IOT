#ifndef _app_key_h_include_
#define _app_key_h_include_

#include "includes.h"

#define USER_KEY   1    /*�û�����*/
#define RESET_KEY  0    /*��λ����*/    

void create_key_task(void);
uint8_t is_key_pressure(uint8_t key_index);
uint32_t get_key_press_time(void);
#endif