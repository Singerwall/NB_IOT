/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		xor.h
  * @author		lwl
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		异或校验驱动头文件。
  */ 

/**
  * @Multi-Include-Prevent Section
  */
#ifndef __XOR_H__
#define __XOR_H__

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
  * @Struct Define Section
  * @结构体定义
  */


/**
  * @Prototype Declare Section
  * @内部函数原型声明
  */
u8 get_xor_value(const u8 *Data ,u16 len);

#endif	/* XOR_H*/
