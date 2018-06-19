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


#ifndef __RF2P4G_DRIVER_H__
#define __RF2P4G_DRIVER_H__ 
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
#define SIM_BUF_NUM  256                                                //  ���������ݸ���
#define SIM_SOF_DATA 0x5a                                               //  ��ʼ����
#define SIM_END_DATA 0xCA                                               //  ��������
#define OFF_SIM_RX_DATA USART_ITConfig(USART2, USART_IT_RXNE, DISABLE)  //  �رս����ж�
#define ON_SIM_RX_DATA  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE)   //  �򿪽����ж�
#define OFF_SIM_TX_DATA USART_ITConfig(USART2, USART_IT_TXE, DISABLE)   //  �رշ����ж�
#define ON_SIM_TX_DATA  USART_ITConfig(USART2, USART_IT_TXE, ENABLE)    //  �򿪷����ж�
                                                                            
#define SIM_TRIG_ONTIME  350											//  �ȴ���ȡһ֡�ź�����ʱ�� MS
#define SEARCHTIME  	 0 												//  ����ID���������ʱ��
#define SIM_READER_MODEL_LEN	  6										//  ����ֵΪ5 ������ֵΪ6

/*  error code  */
#define SIMRC_OK                0
#define SIMRC_TIME_OUT         -1		  //    �ȴ���ʱ
#define SIMRC_CMD_FAIL    	   -2		  //    �Ǳ�������ؽ��
#define SIMRC_GETUID_FAIL      -3		  //    ��ȡIDʧ��
#define SIMRC_SAMEID_FAIL 	   -4		  //    �ظ�����
#define SIMRC_SHUTCHANNEL_FAIL -5		  //    �ر�ͨ��ʧ��
#define SIMRC_OPENCHANNEL_FAIL -6		  //    ��ͨ��ʧ��
#define SIMRC_SELECTAID_FAIL   -7		  //    ѡ��AIDʧ��
#define SIMRC_KEY_FAIL   	   -8		  //    ��֤KEYʧ��
#define SIMRC_READDATA_FAIL    -9		  //    ������ʧ��
#define SIMRC_WRITEDATA_FAIL   -10		  //    д����ʧ��
#define SIMRC_HALTCARD_FAIL    -11		  //    ���߿�ʧ��
/**
  * @brief SIM���ṹ�嶨��
  */
typedef struct
{
 u8  SIM_INDEX_STR;
 u16  SIM_NUM_STR;
 u8  SIM_SOP;
 u8  SIM_CMD;
 u16 SIM_DATA_LEN;
 u8  SIM_BUF[SIM_BUF_NUM];
 u8  SIM_END;

}_SIM_CARD_;

/**
  * @Prototype Declare Section
  * @�ڲ�����ԭ������
  */
void rf2p4g_init(void); /*2.4G��д����ʼ��*/
u8 Sim_InListPassiveTarget(u8 *card_uid);
u8 Sim_read_press(u8 SimBlock_File,u8 *SimBlock_data);
u8 Sim_write_press(u8 SimBlock_File,u8 *SimBlock_data);

#endif /* __RF2P4G_DRIVER_H__ */

