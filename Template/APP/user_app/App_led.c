/** <h2><center>&copy; COPYRIGHT 2013 XunFang </center></h2>
  * @file  can_server.c
  * @author  qyz
  * @version  V1.0.0
  * @date  09/05/2016
  * @brief  LED
  */
#define App_BSP_int_MODULE
#include "sys.h"
#include "delay.h"  
#include "led_driver.h"
#include "App_led.h"
#include "App_BSP_int.h"

/* LED control structure */
typedef struct {
    uint8_t device;
    uint8_t mode;       /* Operation mode */
    uint8_t number_blinks;       /* Blink cycles left */
    uint8_t stop_blinks_flag;
    uint8_t percent;      /* On cycle percentage */
    uint16_t period;      /* On/off cycle time (msec) */
    uint32_t next;      /* Time for next change */
    uint8_t status;
    
} Led_control_t;

typedef struct
{
    Led_control_t frame[LED_DEFAULT_MAX_LEDS];
} Led_status_t;
static Led_status_t led_status;
static void led_task(void *pdata);
static void _led_set_status (uint8_t leds, uint8_t mode);
static void update_led_mode_blink(Led_control_t *led);
static void update_led_mode_on_off(Led_control_t *led, uint8_t on_off);
/**
  * @brief  创建LED任务
 */
void create_led_task(void)
{
    OSTaskCreate(led_task,
                 (void *)0,
                 (OS_STK*)&Stack_Task_led[StackSize_Task_led-1],
                 Prio_Task_led);
}

static void led_task(void *pdata)
{
    uint8_t i;
    for(;;)
    {
        feed_watchdog();
        for(i = 0; i < LED_DEFAULT_MAX_LEDS; ++i)
        {
            switch(led_status.frame[i].mode)
            {
            case LED_MODE_BLINK:
                update_led_mode_blink(&led_status.frame[i]);
                break;
            case LED_MODE_ON:
                update_led_mode_on_off(&led_status.frame[i], 1);
                break;
            case LED_MODE_OFF:
                update_led_mode_on_off(&led_status.frame[i], 0);
                break;
            }
        }
        delay_ms(10);
    }									 
}

static void update_led_mode_blink(Led_control_t *led)
{
    uint32_t now_time = OSTimeGet();
    uint32_t on_time;

    if (now_time < led->next)
        return;
    
    if(led->stop_blinks_flag)
    {
        led->mode = LED_MODE_IDLE;
        return;
    }
    if(led->number_blinks)
    {
        led->number_blinks--;
        if(!led->number_blinks)
            led->stop_blinks_flag = 1;
    }
    led->status = !led->status;
    _led_set_status(led->device, led->status);
    on_time = led->percent * led->period / 100;
    if (led->status == 1)
        led->next = now_time + on_time;
    else
        led->next = now_time + led->period - on_time; 
}

static void update_led_mode_on_off(Led_control_t *led, uint8_t on_off)
{
    led->mode = LED_MODE_IDLE;
    _led_set_status(led->device, on_off);
}

void app_led_blink (uint8_t leds, uint8_t number_blinks, uint8_t percent, uint16_t period)
{
    uint8_t led_index;
    
    switch(leds)
    {
    case LED_1:
        led_index = 0;
        break;
    case LED_2:
        led_index = 1;
        break;
    case LED_3:
        led_index = 2;
        break;
    case LED_4:
        led_index = 3;
        break;
	default:
		return;
    }        
    
    led_status.frame[led_index].device = leds;
    led_status.frame[led_index].mode = LED_MODE_BLINK;
    led_status.frame[led_index].number_blinks = number_blinks * 2;
    led_status.frame[led_index].stop_blinks_flag = 0;
    led_status.frame[led_index].percent = percent;
    led_status.frame[led_index].period = period;
    led_status.frame[led_index].next = OSTimeGet();
    led_status.frame[led_index].status = 0;
}

void app_led_set(uint8_t leds, uint8_t mode)
{
    
}
void _led_set_status (uint8_t leds, uint8_t on_off)
{
    if (leds & LED_1)
    {
        if (on_off)
            Light_control(0, 1);
        else
            Light_control(0, 0);
    }
    if (leds & LED_2)
    {
        if (on_off)
            Light_control(1, 1);
        else
            Light_control(1, 0);
    }
    if (leds & LED_3)
    {
        if (on_off)
            Light_control(2, 1);
        else
            Light_control(2, 0);
    }
    if (leds & LED_4)
    {
        if (on_off)
            Light_control(3, 1);
        else
            Light_control(3, 0);
    }
}

