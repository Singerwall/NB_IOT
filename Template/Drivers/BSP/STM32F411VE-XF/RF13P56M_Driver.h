/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		PN532.h
  * @author		zp.h
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		PN532驱动头文件。
  */ 

/**
  * @Multi-Include-Prevent Section
  */


#ifndef __RF_13P56M_DRIVER_H__
#define __RF_13P56M_DRIVER_H__ 
/**
  * @Include File Section
  * @包含头文件
  */

#include <includes.h>
/**
  * @Macro Define Section
  * @宏定义
  */




				   
/**
  * @Prototype Declare Section
  * @内部函数原型声明
  */
int PN532_WakeUp(void);
int PN532_InListPassiveTarget(u8 *uid);     /*寻卡*/
int PN532_PsdVerifyKeyA(u8 block,u8 *uid);  /*密码授权，验证KeyA*/
int PN532_read(u8 block,u8 *block_data);    /*读16个字节 */
int PN532_write(u8 block,u8 *block_data);   /*写16个字节的第一个字节*/


int _cmd_13p56m_list_card(Device_packet_t *packet);
int _cmd_13p56m_read_card(Device_packet_t *packet);
int _cmd_13p56m_write_card(Device_packet_t *packet);

#endif /* __RF_13P56M_DRIVER_H__ */

