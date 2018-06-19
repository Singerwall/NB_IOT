/*******************************************************************
*
*    FILENAME(�ļ���)     :    SENSOR.H
*
*    COPYRIGHT(����Ȩ)    :    ������Ѷ��ͨ�ż������޹�˾
*
*    MODULE NAME(�����)  :
*
*    CPU                  :    stm32F103ZET6
*
*    RTOS(����ϵͳ)       :    
*
*    CreateDate(����ʱ��) :
*
*    AUTHOR(����)         :    
*
*    Abstract Description 
*    ժҪ����             :
*
*--------------------------Revision History------------------------
*
*    No    Version    Date    Revise By    Item    Description
*
*******************************************************************/

/*******************************************************************
*    Multi-Include-Prevent Section
*******************************************************************/
#ifndef __SENSOR_H__
#define __SENSOR_H__
/*******************************************************************
*    Debug switch Section
*    ���Կ���
*******************************************************************/

/*******************************************************************
*    Include File Section
*    ����ͷ�ļ�
*******************************************************************/
#include "..\\include.h"
#include "sensor_single_line.h"
/*******************************************************************
*    Macro Define Section
*    �궨�� 
*******************************************************************/
//======================================================================
//BO-B1 --IO0-IO1
//B7 - TTL��ƽ	IO2
//C0 - SENSER-OUT		C1-�ο���ѹ
#define SENSOR_CLOCK  			RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
#define Sensor_IO_PORT   		GPIOB
#define Sensor_IO_PIN0   		GPIO_Pin_0
#define Sensor_IO_PIN1   		GPIO_Pin_1
#define Sensor_IO_PIN2   		GPIO_Pin_7
#define Sensor_IO_NUM0   		0
#define Sensor_IO_NUM1   		1
#define Sensor_IO_NUM2   		7

//�ɼ���IO�ж�����
#define Sensor_Exti_PORT    	GPIO_PortSourceGPIOB
#define Sensor_Exti_NUM			GPIO_PinSource7
#define Sensor_Exti_Line_NUM 	EXTI_Line7

#define SENSOR0_IN()	 		((Sensor_IO_PORT->IDR&Sensor_IO_PIN0)>>Sensor_IO_NUM0)		   		//��ӦIO-2
#define SENSOR1_IN()	 		((Sensor_IO_PORT->IDR&Sensor_IO_PIN1)>>Sensor_IO_NUM1)		   		//��ӦIO-1
#define SENSOR2_IN()	 		((Sensor_IO_PORT->IDR&Sensor_IO_PIN2)>>Sensor_IO_NUM2)		   		//��ӦADC


#define Sensor_ADC_PORT   		GPIOC
#define Sensor_ADC_PIN0   		GPIO_Pin_0
#define Sensor_ADC_NUM0   		0
#define SENSOR3_IN()	  		((Sensor_ADC_PORT->IDR&Sensor_ADC_PIN0)>>Sensor_ADC_NUM0)		   //��ӦTTL
/*******************************************************************
*    Struct Define Section
*    �ṹ�嶨��
*******************************************************************/

//���������Ͷ���
enum {
SENSOR_GBS,        	/* 	0�������鴫���� 			 */
SENSOR_DS18B20,	   	/* 	1�¶ȴ����� 				 */
SENSOR_SHT11,	   	/* 	2����ʪ�ȴ����� 			 */
SENSOR_HR31,	   	/* 	3����ʪ�ȴ����� 			 */
SENSOR_LIGHT5537,  	/*	4���������� 				 */
SENSOR_MQ7,        	/*	5һ����̼Ũ�Ȳ��������� 	 */
SENSOR_MQ2,        	/*	6ú����⴫����				 */
SENSOR_MQ3,       	/* 	7�ƾ����������� 			 */
SENSOR_MQ5,        	/*	8��ȼ������������� 		 */
SENSOR_FLAME,	   	/* 	9���洫���� 				 */
SENSOR_HDS10,      	/* 	10��¶������ 				 */
SENSOR_HALL3144,   	/* 	11����������				 */
SENSOR_SOUND,     	/* 	12����������				 */
SENSOR_801S,       	/* 	13�𶯴�����				 */
SENSOR_TILT,       	/* 	14��б������				 */
SENSOR_ME003,     	/* 	15���͵�����ߴ�����		 */
SENSOR_LM35,       	/*	16�¶ȴ����� 				 */
SENSOR_CCD,       	/*	17����ʽ������ 				 */
SENSOR_ASD       	/*	18���ٶȴ����� 				 */

};
/*******************************************************************
*    Global Varible Declare
*    ȫ�ֱ�������
*******************************************************************/
extern u8 sensor_exit_flag;
/*******************************************************************
*    Prototype Declare Section
*    �ڲ�����ԭ������
*******************************************************************/
void Sensor_init_TTL(void);
void Sensor_Exti_init(void);
void SensorADInit(void);
void GetSensorData(u8* data);
void senser_Init(u8 type);//���д�������ʼ��
u8 sensor_get_data(u8 type,u8* data);//��ȡ����������

#endif

