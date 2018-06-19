/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		xor.c
  * @author		lwl
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		异或校验驱动函数。
  */ 

/**
  * @Include File Section
  * @包含头文件
  */

#include "xor.h"

/**
  * @Global Varible Declare
  * @全局变量声明
  */


/**
  * @brief		异或校验计算函数
  * @details	
  * @param Data	数据缓存
  * @param len	数据长度
  * @retval 	校验值
  */
u8 get_xor_value(const u8 *Data ,u16 len)
{
  u8 xorvalue =0;
  u16 i;

  for( i=0;i<len;i++)
	xorvalue ^=Data[i];

  return xorvalue;
}
