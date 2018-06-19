/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  algorithm.c
  * @author  netfire.liang
  * @version  V2.0.0
  * @date  08/01/2013
  * @brief  ���ݶ���ӿڡ���С��ת����
  */ 
#include "algorithm.h"
#include "string.h"
/**
  * @brief  ��32λ���ݰ�8λ���ڴ��ȡ��ʽ����
  * @details  ���ڴ�С��ת��
  * @param  value �����32λ����
  * @return �������32λ����
 */
uint32_t swap_uint8_t_in_uint32_t(uint32_t value)
{
    return ((value & 0x000000FFUL) << 24) |
            ((value & 0x0000FF00UL) <<  8) |
            ((value & 0x00FF0000UL) >>  8) |
            ((value & 0xFF000000UL) >> 24);
}

/**
  * @brief  ��16λ���ݰ�8λ���ڴ��ȡ��ʽ����
  * @details  ���ڴ�С��ת��
  * @param  value �����16λ����
  * @return �������16λ����
 */
uint16_t swap_uint8_t_in_uint16_t(uint16_t value)
{
    return (value << 8) | (value >> 8);
}

/**
  * @brief  ��32λ����װ�ؽ�������
  * @details  װ����ɺ󣬽���ռ�û�����4���ֽ�
  * @param   value ��װ�ص�33λ����
  * @param   buffer Ŀ�Ļ�������ַ
  * @return ��
 */
void put_uint32_t_value_to_buffer(uint32_t value, uint8_t *buffer)
{
    *(uint32_t*)buffer = value;
}

/**
  * @brief  ��16λ����װ�ؽ�������
  * @details  װ����ɺ󣬽���ռ�û�����2���ֽ�
  * @param   value ��װ�ص�16λ����
  * @param   buffer Ŀ�Ļ�������ַ
  * @return ��
 */
void put_uint16_t_value_to_buffer(uint16_t value, uint8_t *buffer)
{
    *(uint16_t*)buffer = value;
}

/**
  * @brief  �ӻ�������ȡ��16λ����
  * @details  2���ֽ�
  * @param  buffer ���ݻ�������ַ
  * @return ����ȡ����16λ����
 */
uint16_t get_uint16_value_from_buffer(const uint8_t *buffer)
{
    return *(uint16_t *)buffer;
}

/**
  * @brief  �ӻ�������ȡ��32λ����
  * @details  4���ֽ�
  * @param  buffer ���ݻ�������ַ
  * @return ����ȡ����32λ����
 */
uint32_t get_uint32_value_from_buffer(const uint8_t *buffer)
{
    return *(uint32_t *)buffer;
}

/**
  * @brief  ��16λ����������ת��Ϊ����ģʽ��������
  * @details  ���ｫ�ᷢ����С��ת����2���ֽ�
  * @param  buffer �����������ݻ�������ַ
  * @return  ����16λ��������
 */
uint16_t network_buffer_to_host16(const uint8_t *buffer)
{
    return swap_uint8_t_in_uint16_t(get_uint16_value_from_buffer(buffer));
}

/**
  * @brief  ��32λ����������ת��Ϊ����ģʽ��������
  * @details   ���ｫ�ᷢ����С��ת����4���ֽ�
  * @param  buffer �����������ݻ�������ַ
  * @return  ����32λ��������
 */
uint32_t network_buffer_to_host32(const uint8_t *buffer)
{
    return swap_uint8_t_in_uint32_t(get_uint32_value_from_buffer(buffer));
}

/**
  * @brief  ��16λ������������ת��Ϊ������
  * @details  ���ｫ�ᷢ����С��ת����2���ֽ�
  * @param  value  16λ��������
  * @param  buffer �����������ݻ�������ַ
  * @return ��
 */
void host16_to_network_buffer(uint16_t value, uint8_t *buffer)
{
    put_uint16_t_value_to_buffer(swap_uint8_t_in_uint16_t(value), buffer);
}

/**
  * @brief  ��32λ������������ת��Ϊ������
  * @details  ���ｫ�ᷢ����С��ת����4���ֽ�
  * @param  value  32λ��������
  * @param  buffer �����������ݻ�������ַ
  * @return ��
 */
void host32_to_network_buffer(uint32_t value, uint8_t *buffer)
{
    put_uint32_t_value_to_buffer(swap_uint8_t_in_uint32_t(value), buffer);
}
/**
  * @brief  ��8λ����װ�ؽ�������
  * @details  װ����ɺ󣬽���ռ�û�����1���ֽ�
  * @param   value ��װ�ص�8λ����
  * @param   buffer Ŀ�Ļ�������ַ
  * @return ��
 */
void put_uint8_t_value_to_buffer(uint8_t value, uint8_t *buffer)
{
    *buffer = value;
}
/**
  * @brief  �ӻ�������ȡ��8λ����
  * @details  1���ֽ�
  * @param  buffer ���ݻ�������ַ
  * @return ����ȡ����8λ����
 */
uint8_t get_uint8_value_from_buffer(const uint8_t *buffer)
{
    return *buffer;
}
/**
  * @brief  ��2��ASCII��ת��Ϊ1��HEX��
  * @details  
  * @param  ascii Դ��ַ
  * @return 8λ��hex��
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
  * @brief  ��1�ֽڵ�HEX��ת��Ϊ2�ֽڵ�ASCII�ַ���
  * @details  
  * @param   dst ����ת�����Ŀ�ĵ�ַ
  * @param   src hex��Դ��ַ
  * @return ��
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
  * @brief HEX����תascii����
  * @param data ��Ҫת��������
  * @param data_size ����
  * @retval ����ascii�����ַ.
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
  * @brief  ��0~9��ASCII�ַ�ת��ʮ���Ƶ�����
  * @details  �����ַ��ĳ��Ȳ��ܴ���32λʮ���Ƶ�����
  * @param   ch ASCII�ַ�
  * @return ת�����ʮ������
 */
int a_to_dec(const char ch)
{
    int dec = -1;
    
    if(ch >= '0' && ch <= '9')
        dec = ch - '0';
    
    return dec;
}

/**
  * @brief  �������ŵ�ASCII�ַ���ת��Ϊʮ��������
  * @details �����ַ��ĳ��Ȳ��ܴ���32λʮ���Ƶ�����
  * @param  string ASCII�ַ�
  * @return ת�����ʮ������
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

