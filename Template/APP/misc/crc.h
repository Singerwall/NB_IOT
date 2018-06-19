#ifndef crc_crc_h_included_
#define crc_crc_h_included_

#include <stdint.h>
#include <stddef.h> // for size_t

#ifdef __CC_ARM
#define inline __inline
#endif

// G(x) = x^7+x^3+1
uint8_t get_crc7(const uint8_t* buffer, size_t length, uint8_t crc);

uint8_t get_crc8(const uint8_t *buffer, uint8_t length, uint8_t crc);

// G(x) = x^16+x^15+x^2+1
uint16_t get_crc16(const uint8_t* buffer, size_t length, uint16_t crc);

// G(x) = x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1
uint32_t get_crc32(const uint8_t* buffer, size_t length, uint32_t crc);

#endif // crc_crc_h_included_
