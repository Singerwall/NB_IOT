/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  algorithm.c
  * @author  netfire.liang
  * @version  V2.0.0
  * @date  08/01/2013
  * @brief  数据对齐接口。大小端转换。
  */ 
#include "algorithm.h"
#include "string.h"
/**
  * @brief  将32位数据按8位的内存存取方式交换
  * @details  用于大小端转换
  * @param  value 输入的32位数据
  * @return 交换后的32位数据
 */
uint32_t swap_uint8_t_in_uint32_t(uint32_t value)
{
    return ((value & 0x000000FFUL) << 24) |
            ((value & 0x0000FF00UL) <<  8) |
            ((value & 0x00FF0000UL) >>  8) |
            ((value & 0xFF000000UL) >> 24);
}

/**
  * @brief  将16位数据按8位的内存存取方式交换
  * @details  用于大小端转换
  * @param  value 输入的16位数据
  * @return 交换后的16位数据
 */
uint16_t swap_uint8_t_in_uint16_t(uint16_t value)
{
    return (value << 8) | (value >> 8);
}

/**
  * @brief  将32位数据装载进缓冲区
  * @details  装载完成后，将会占用缓冲区4个字节
  * @param   value 是装载的33位数据
  * @param   buffer 目的缓冲区地址
  * @return 无
 */
void put_uint32_t_value_to_buffer(uint32_t value, uint8_t *buffer)
{
    *(uint32_t*)buffer = value;
}

/**
  * @brief  将16位数据装载进缓冲区
  * @details  装载完成后，将会占用缓冲区2个字节
  * @param   value 是装载的16位数据
  * @param   buffer 目的缓冲区地址
  * @return 无
 */
void put_uint16_t_value_to_buffer(uint16_t value, uint8_t *buffer)
{
    *(uint16_t*)buffer = value;
}

/**
  * @brief  从缓冲区中取出16位数据
  * @details  2个字节
  * @param  buffer 数据缓冲区地址
  * @return 返回取出的16位数据
 */
uint16_t get_uint16_value_from_buffer(const uint8_t *buffer)
{
    return *(uint16_t *)buffer;
}

/**
  * @brief  从缓冲区中取出32位数据
  * @details  4个字节
  * @param  buffer 数据缓冲区地址
  * @return 返回取出的32位数据
 */
uint32_t get_uint32_value_from_buffer(const uint8_t *buffer)
{
    return *(uint32_t *)buffer;
}

/**
  * @brief  将16位网络数据流转换为主机模式的数据流
  * @details  这里将会发生大小端转换，2个字节
  * @param  buffer 网络流的数据缓冲区地址
  * @return  返回16位主机数据
 */
uint16_t network_buffer_to_host16(const uint8_t *buffer)
{
    return swap_uint8_t_in_uint16_t(get_uint16_value_from_buffer(buffer));
}

/**
  * @brief  将32位网络数据流转换为主机模式的数据流
  * @details   这里将会发生大小端转换，4个字节
  * @param  buffer 网络流的数据缓冲区地址
  * @return  返回32位主机数据
 */
uint32_t network_buffer_to_host32(const uint8_t *buffer)
{
    return swap_uint8_t_in_uint32_t(get_uint32_value_from_buffer(buffer));
}

/**
  * @brief  将16位的主机数据流转换为网络流
  * @details  这里将会发生大小端转换，2个字节
  * @param  value  16位主机数据
  * @param  buffer 网络流的数据缓冲区地址
  * @return 无
 */
void host16_to_network_buffer(uint16_t value, uint8_t *buffer)
{
    put_uint16_t_value_to_buffer(swap_uint8_t_in_uint16_t(value), buffer);
}

/**
  * @brief  将32位的主机数据流转换为网络流
  * @details  这里将会发生大小端转换，4个字节
  * @param  value  32位主机数据
  * @param  buffer 网络流的数据缓冲区地址
  * @return 无
 */
void host32_to_network_buffer(uint32_t value, uint8_t *buffer)
{
    put_uint32_t_value_to_buffer(swap_uint8_t_in_uint32_t(value), buffer);
}
/**
  * @brief  将8位数据装载进缓冲区
  * @details  装载完成后，将会占用缓冲区1个字节
  * @param   value 是装载的8位数据
  * @param   buffer 目的缓冲区地址
  * @return 无
 */
void put_uint8_t_value_to_buffer(uint8_t value, uint8_t *buffer)
{
    *buffer = value;
}
/**
  * @brief  从缓冲区中取出8位数据
  * @details  1个字节
  * @param  buffer 数据缓冲区地址
  * @return 返回取出的8位数据
 */
uint8_t get_uint8_value_from_buffer(const uint8_t *buffer)
{
    return *buffer;
}
/**
  * @brief  将2个ASCII码转换为1个HEX码
  * @details  
  * @param  ascii 源地址
  * @return 8位的hex码
 */
uint8_t ascii_to_hex(const uint8_t *ascii)
{
    uint8_t temp;
    
    if(*ascii >= '0' && *ascii <= '9')
        temp = ((*ascii) - '0') << 4;
    else
        temp = ((*ascii) - 'A' + 10) << 4;
    
    ascii++;
    if(*ascii >= '0' && *ascii <= '9')
        temp |= ((*ascii) - '0') & 0x0f;
    else
        temp |= ((*ascii) - 'A' + 10) & 0x0f;
    
    return temp;
}


/**
  * @brief  将1字节的HEX码转换为2字节的ASCII字符码
  * @details  
  * @param   dst 存在转换后的目的地址
  * @param   src hex码源地址
  * @return 无
 */
void hex_to_ascii(uint8_t *dst, uint8_t src)
{
    uint8_t temp = src >> 4;
    if(temp <= 0x09)
        *dst++ = temp + '0';
    else
        *dst++ = temp - 0x0a + 'A';
    
    temp = src & 0x0f;
    if(temp <= 0x09)
        *dst++ = temp + '0';
    else
        *dst++ = temp - 0x0a + 'A';
}

/**
  * @brief HEX数组转ascii数组
  * @param data 需要转换的数组
  * @param data_size 长度
  * @retval 返回ascii数组地址.
 */
uint8_t *data_to_datasting(const uint8_t *data,const uint16_t data_size)
{
	int i,j=0;
	static uint8_t ascii_string[200] = {0};	
	memset(ascii_string,0,200);
	for (i=0;i<data_size;i++)
	{
		hex_to_ascii(&ascii_string[j],data[i]);
		j += 2;
	}
	ascii_string[j++] = '\0';
	return ascii_string;
}


/**
  * @brief  将0~9的ASCII字符转换十进制的数据
  * @details  数据字符的长度不能大于32位十进制的数。
  * @param   ch ASCII字符
  * @return 转换后的十进制数
 */
int a_to_dec(const char ch)
{
    int dec = -1;
    
    if(ch >= '0' && ch <= '9')
        dec = ch - '0';
    
    return dec;
}

/**
  * @brief  将带符号的ASCII字符串转换为十进制数据
  * @details 数据字符的长度不能大于32位十进制的数。
  * @param  string ASCII字符
  * @return 转换后的十进制数
 */
int string_to_dec(const char *string)
{
    int i = -1;
    int dd = 0, sign = 1;
    
    if(*string == '-')
    {
        ++string;
        sign = -1;
    }
    
    while(*string)
    {
        i = a_to_dec(*string ++);
        if(i < 0)
            break;
        dd = dd * 10 + i;
    }
    
    return dd * sign;
}

