/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		PN532.h
  * @author		zp.h
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		PN532����ͷ�ļ���
  */ 

/**
  * @Multi-Include-Prevent Section
  */


#ifndef __RF_13P56M_DRIVER_H__
#define __RF_13P56M_DRIVER_H__ 
/**
  * @Include File Section
  * @����ͷ�ļ�
  */

#include <includes.h>
/**
  * @Macro Define Section
  * @�궨��
  */




				   
/**
  * @Prototype Declare Section
  * @�ڲ�����ԭ������
  */
int PN532_WakeUp(void);
int PN532_InListPassiveTarget(u8 *uid);     /*Ѱ��*/
int PN532_PsdVerifyKeyA(u8 block,u8 *uid);  /*������Ȩ����֤KeyA*/
int PN532_read(u8 block,u8 *block_data);    /*��16���ֽ� */
int PN532_write(u8 block,u8 *block_data);   /*д16���ֽڵĵ�һ���ֽ�*/


int _cmd_13p56m_list_card(Device_packet_t *packet);
int _cmd_13p56m_read_card(Device_packet_t *packet);
int _cmd_13p56m_write_card(Device_packet_t *packet);

#endif /* __RF_13P56M_DRIVER_H__ */

