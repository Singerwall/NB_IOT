#include "device_protocol.h"
#include "algorithm.h"
#include "string.h"
#include <crc.h>
#include <xor.h>

#define PACKET_START 0x5A

#define PACKET_END   0xCA


uint8_t device_buffer_to_packet(const uint8_t *buffer, Device_packet_t *packet)
{
    const uint8_t *p = buffer;
    uint8_t check,xor;
    int i;
    
    if(get_uint8_value_from_buffer(p) != PACKET_START)/* got start flag error */
        return 0;
    p ++;  
    
    packet->msg_num = swap_uint8_t_in_uint16_t(get_uint16_value_from_buffer(p));
    p += 2;
    
//    memcpy(packet->igw_mac_addr,p,MAC_ADDR_LEN);            
//    p += MAC_ADDR_LEN;
    
    packet->node_type = get_uint8_value_from_buffer(p);
    p ++;
    
    packet->device_type = get_uint8_value_from_buffer(p);
    p++;
    
//    memcpy(packet->mac_addr,p,MAC_ADDR_LEN);            
//    p += MAC_ADDR_LEN;
       
    packet->cmd = get_uint8_value_from_buffer(p);;
    p ++;   
    
    packet->ack = get_uint8_value_from_buffer(p);
    p ++; 
    
    packet->data_size = get_uint8_value_from_buffer(p);
    p ++;
    
    if(packet->data_size > device_data_max_size)/* got pakcet data size error */
        return 0;
    
    for(i = 0; i < packet->data_size; ++i)
    {
        packet->data[i] = get_uint8_value_from_buffer(p);
        p++;
    }
    
    check = get_xor_value(buffer, p - buffer);
    xor = get_uint8_value_from_buffer(p);
    p ++;    
    if(check != xor)
    {
        return 0;
    }
    if(get_uint8_value_from_buffer(p) != PACKET_END)/* got end flag error */
        return 0;
    p++;
    
    return (p - buffer);/* return the number of buffer size */
}

uint8_t device_packet_to_buffer(const Device_packet_t *packet, uint8_t *buffer)
{
    uint8_t xor;
    uint8_t *p = buffer;
    int i;
    
    if(packet->data_size > device_data_max_size) /* got pakcet data size error */
        return 0;
    
    /* start flag 0x5A */
    put_uint8_t_value_to_buffer(PACKET_START, p);
    p ++; 
    
    put_uint16_t_value_to_buffer(swap_uint8_t_in_uint16_t(packet->msg_num), p);
    p +=2; 
    
//    memcpy(p,packet->igw_mac_addr,MAC_ADDR_LEN);
//    p += MAC_ADDR_LEN;
    
    put_uint8_t_value_to_buffer(packet->node_type, p);
    p ++;
    
    put_uint8_t_value_to_buffer(packet->device_type, p);
    p ++;
        
//    memcpy(p,packet->mac_addr,MAC_ADDR_LEN);            
//    p += MAC_ADDR_LEN;    
    
    put_uint8_t_value_to_buffer(packet->cmd, p);
    p ++; 

    put_uint8_t_value_to_buffer(packet->ack, p);
    p ++; 
        
    put_uint8_t_value_to_buffer(packet->data_size, p);
    p ++;
    
    for(i = 0; i < packet->data_size; ++i)
    {
        put_uint8_t_value_to_buffer(packet->data[i], p);
        p++; 
    }
    
    xor = get_xor_value(buffer, p - buffer);
    put_uint8_t_value_to_buffer(xor, p);
    p ++;
    
    /* packet end flag 0xCA*/
    put_uint8_t_value_to_buffer(PACKET_END, p);
    p ++; 
    
    return (p - buffer);/* return the number of buffer size */
}


















