#ifndef __XF_SENSOR_IR_THERMOMETRY_H
#define __XF_SENSOR_IR_THERMOMETRY_H

#include "hal_types.h"

//Define I/O port and the direction for SMBus signals

#define  BITY(X) (1<<X)
#define _SDA_OUTPUT  P0DIR |= BITY(5)            //Set SDA as Open-drain Output 
#define _SDA_INPUT   P0DIR &= ~BITY(5)		//Set SDA as Input
#define _SCL_IO      P0DIR |= BITY(4)	        //Set SCL as Open-drain I/O

#define SDA  P0_5   //Assign P14 as SDA line
#define SCL  P0_4   //Assign P13 as SCL line

extern uint16 *CALTEMP(uint32 TEMP);

void SMBus_Init(void);
void start_bit(void);
void stop_bit(void);
void send_bit(uint8 bit_out);
uint8 receive_bit(void);
uint8 slave_ack(void);
void TX_byte(uint8 TX_buffer);
uint8 RX_byte(uint8 ack_nack);
uint8 receive_bit(void);
uint8 PEC_cal(uint8 pec[],int n);
uint32 MEM_READ(uint8 slave_addR,uint8 cmdR);
void EEPROM_WRITE(uint8 slave_addW,uint8 cmdW,uint8 DataL,uint8 DataH);

void MLX90614_Init(void);/*初始化红外测温传感器*/
uint16 MLX90614_TemperRead(void);
uint8 *XfGetIRTempValue(void);/*获取温度值*/
#endif