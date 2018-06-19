/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		PN532.h
  * @author		qyz
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		PN532驱动头文件。
  */ 

/**
  * @Multi-Include-Prevent Section
  */


#ifndef __RF2P4G_DRIVER_H__
#define __RF2P4G_DRIVER_H__ 
/**
  * @Include File Section
  * @包含头文件
  */
#include "Define_ALL.h"
#include "delay.h"
/**
  * @Macro Define Section
  * @宏定义
  */
#define SIM_BUF_NUM  256                                                //  缓冲区数据个数
#define SIM_SOF_DATA 0x5a                                               //  起始符号
#define SIM_END_DATA 0xCA                                               //  结束符号
#define OFF_SIM_RX_DATA USART_ITConfig(USART2, USART_IT_RXNE, DISABLE)  //  关闭接收中断
#define ON_SIM_RX_DATA  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE)   //  打开接收中断
#define OFF_SIM_TX_DATA USART_ITConfig(USART2, USART_IT_TXE, DISABLE)   //  关闭发送中断
#define ON_SIM_TX_DATA  USART_ITConfig(USART2, USART_IT_TXE, ENABLE)    //  打开发送中断
                                                                            
#define SIM_TRIG_ONTIME  350											//  等待收取一帧信号量的时间 MS
#define SEARCHTIME  	 0 												//  查找ID卡的命令返回时间
#define SIM_READER_MODEL_LEN	  6										//  主动值为5 ，被动值为6

/*  error code  */
#define SIMRC_OK                0
#define SIMRC_TIME_OUT         -1		  //    等待超时
#define SIMRC_CMD_FAIL    	   -2		  //    非本次命令返回结果
#define SIMRC_GETUID_FAIL      -3		  //    获取ID失败
#define SIMRC_SAMEID_FAIL 	   -4		  //    重复卡号
#define SIMRC_SHUTCHANNEL_FAIL -5		  //    关闭通道失败
#define SIMRC_OPENCHANNEL_FAIL -6		  //    打开通道失败
#define SIMRC_SELECTAID_FAIL   -7		  //    选择AID失败
#define SIMRC_KEY_FAIL   	   -8		  //    认证KEY失败
#define SIMRC_READDATA_FAIL    -9		  //    读数据失败
#define SIMRC_WRITEDATA_FAIL   -10		  //    写数据失败
#define SIMRC_HALTCARD_FAIL    -11		  //    休眠卡失败
/**
  * @brief SIM卡结构体定义
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
  * @内部函数原型声明
  */
void rf2p4g_init(void); /*2.4G读写器初始化*/
u8 Sim_InListPassiveTarget(u8 *card_uid);
u8 Sim_read_press(u8 SimBlock_File,u8 *SimBlock_data);
u8 Sim_write_press(u8 SimBlock_File,u8 *SimBlock_data);

#endif /* __RF2P4G_DRIVER_H__ */

