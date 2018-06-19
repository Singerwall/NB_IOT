/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		xor.c
  * @author		lwl
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		���У������������
  */ 

/**
  * @Include File Section
  * @����ͷ�ļ�
  */

#include "xor.h"

/**
  * @Global Varible Declare
  * @ȫ�ֱ�������
  */


/**
  * @brief		���У����㺯��
  * @details	
  * @param Data	���ݻ���
  * @param len	���ݳ���
  * @retval 	У��ֵ
  */
u8 get_xor_value(const u8 *Data ,u16 len)
{
  u8 xorvalue =0;
  u16 i;

  for( i=0;i<len;i++)
	xorvalue ^=Data[i];

  return xorvalue;
}
