#ifndef _app_led_h_include_
#define _app_led_h_include_

#include "includes.h" 

/* Modes */
#define LED_MODE_OFF     0x00
#define LED_MODE_ON      0x01
#define LED_MODE_BLINK   0x02
#define LED_MODE_FLASH   0x04
#define LED_MODE_TOGGLE  0x08
#define LED_MODE_IDLE    0x10

/* Defaults */
#define LED_DEFAULT_MAX_LEDS      4
#define LED_DEFAULT_DUTY_CYCLE    5
#define LED_DEFAULT_FLASH_COUNT   50
#define LED_DEFAULT_FLASH_TIME    1000

/* LEDS - The LED number is the same as the bit position */
#define LED_1     0x01
#define LED_2     0x02
#define LED_3     0x04
#define LED_4     0x08
#define LED_ALL   (LED_1 | LED_2 | LED_3 | LED_4)

void create_led_task(void);
void app_led_blink (uint8_t leds, uint8_t numBlinks, uint8_t percent, uint16_t period);

#endif /* _app_led_h_include_ */
