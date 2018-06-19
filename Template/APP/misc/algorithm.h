
#ifndef _algorithm_h_include_
#define _algorithm_h_include_
#include <stdint.h>
uint32_t swap_uint8_t_in_uint32_t(uint32_t value);
uint16_t swap_uint8_t_in_uint16_t(uint16_t value);
void put_uint32_t_value_to_buffer(uint32_t value, uint8_t *buffer);
void put_uint16_t_value_to_buffer(uint16_t value, uint8_t *buffer);
uint16_t get_uint16_value_from_buffer(const uint8_t *buffer);
uint32_t get_uint32_value_from_buffer(const uint8_t *buffer);
uint8_t network_buffer_to_host8(const uint8_t *buffer);
uint16_t network_buffer_to_host16(const uint8_t *buffer);
uint32_t network_buffer_to_host32(const uint8_t *buffer);
void host16_to_network_buffer(uint16_t value, uint8_t *buffer);
void host32_to_network_buffer(uint32_t value, uint8_t *buffer);
uint8_t get_uint8_value_from_buffer(const uint8_t *buffer);
void put_uint8_t_value_to_buffer(uint8_t value, uint8_t *buffer);
uint8_t *data_to_datasting(const uint8_t *data,const uint16_t data_size);
void hex_to_ascii(uint8_t *dst, uint8_t src);
uint8_t ascii_to_hex(const uint8_t *ascii);
int a_to_dec(const char ch);
int string_to_dec(const char *string);
#endif /* _algorithm_h_include_ */


