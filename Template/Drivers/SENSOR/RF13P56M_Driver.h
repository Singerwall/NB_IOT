/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		PN532.h
  * @author		qyz
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
#include "Define_ALL.h"
#include "delay.h"
/**
  * @Macro Define Section
  * @�궨��
  */

#define Default_Baud           115200
#define UART2BUFFSIZE		   200


				   
/**
  * @Prototype Declare Section
  * @�ڲ�����ԭ������
  */
void PN532_init(void);                      /*13.56Mģ���ʼ��*/
int PN532_InListPassiveTarget(u8 *uid);     /*Ѱ��*/
int PN532_PsdVerifyKeyA(u8 block,u8 *uid);  /*������Ȩ����֤KeyA*/
int PN532_read(u8 block,u8 *block_data);    /*��16���ֽ� */
int PN532_write(u8 block,u8 *block_data);   /*д16���ֽڵĵ�һ���ֽ�*/

#endif /* __RF_13P56M_DRIVER_H__ */

