/*******************************************************************
*
*    FILENAME(文件名)     :    SENSOR.H
*
*    COPYRIGHT(著作权)    :    深圳市讯方通信技术有限公司
*
*    MODULE NAME(组件名)  :
*
*    CPU                  :    stm32F103ZET6
*
*    RTOS(操作系统)       :    
*
*    CreateDate(创建时间) :
*
*    AUTHOR(作者)         :    
*
*    Abstract Description 
*    摘要描述             :
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
*    调试开关
*******************************************************************/

/*******************************************************************
*    Include File Section
*    包含头文件
*******************************************************************/
#include "..\\include.h"
#include "sensor_single_line.h"
/*******************************************************************
*    Macro Define Section
*    宏定义 
*******************************************************************/
//======================================================================
//BO-B1 --IO0-IO1
//B7 - TTL电平	IO2
//C0 - SENSER-OUT		C1-参考电压
#define SENSOR_CLOCK  			RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
#define Sensor_IO_PORT   		GPIOB
#define Sensor_IO_PIN0   		GPIO_Pin_0
#define Sensor_IO_PIN1   		GPIO_Pin_1
#define Sensor_IO_PIN2   		GPIO_Pin_7
#define Sensor_IO_NUM0   		0
#define Sensor_IO_NUM1   		1
#define Sensor_IO_NUM2   		7

//采集口IO中断配置
#define Sensor_Exti_PORT    	GPIO_PortSourceGPIOB
#define Sensor_Exti_NUM			GPIO_PinSource7
#define Sensor_Exti_Line_NUM 	EXTI_Line7

#define SENSOR0_IN()	 		((Sensor_IO_PORT->IDR&Sensor_IO_PIN0)>>Sensor_IO_NUM0)		   		//对应IO-2
#define SENSOR1_IN()	 		((Sensor_IO_PORT->IDR&Sensor_IO_PIN1)>>Sensor_IO_NUM1)		   		//对应IO-1
#define SENSOR2_IN()	 		((Sensor_IO_PORT->IDR&Sensor_IO_PIN2)>>Sensor_IO_NUM2)		   		//对应ADC


#define Sensor_ADC_PORT   		GPIOC
#define Sensor_ADC_PIN0   		GPIO_Pin_0
#define Sensor_ADC_NUM0   		0
#define SENSOR3_IN()	  		((Sensor_ADC_PORT->IDR&Sensor_ADC_PIN0)>>Sensor_ADC_NUM0)		   //对应TTL
/*******************************************************************
*    Struct Define Section
*    结构体定义
*******************************************************************/

//传感器类型定义
enum {
SENSOR_GBS,        	/* 	0玻璃破碎传感器 			 */
SENSOR_DS18B20,	   	/* 	1温度传感器 				 */
SENSOR_SHT11,	   	/* 	2土壤湿度传感器 			 */
SENSOR_HR31,	   	/* 	3环境湿度传感器 			 */
SENSOR_LIGHT5537,  	/*	4光敏传感器 				 */
SENSOR_MQ7,        	/*	5一氧化碳浓度测量传感器 	 */
SENSOR_MQ2,        	/*	6煤气检测传感器				 */
SENSOR_MQ3,       	/* 	7酒精测量传感器 			 */
SENSOR_MQ5,        	/*	8可燃气体测量传感器 		 */
SENSOR_FLAME,	   	/* 	9火焰传感器 				 */
SENSOR_HDS10,      	/* 	10凝露传感器 				 */
SENSOR_HALL3144,   	/* 	11霍尔传感器				 */
SENSOR_SOUND,     	/* 	12声音传感器				 */
SENSOR_801S,       	/* 	13震动传感器				 */
SENSOR_TILT,       	/* 	14倾斜传感器				 */
SENSOR_ME003,     	/* 	15热释电红外线传感器		 */
SENSOR_LM35,       	/*	16温度传感器 				 */
SENSOR_CCD,       	/*	17电容式传感器 				 */
SENSOR_ASD       	/*	18加速度传感器 				 */

};
/*******************************************************************
*    Global Varible Declare
*    全局变量声明
*******************************************************************/
extern u8 sensor_exit_flag;
/*******************************************************************
*    Prototype Declare Section
*    内部函数原型声明
*******************************************************************/
void Sensor_init_TTL(void);
void Sensor_Exti_init(void);
void SensorADInit(void);
void GetSensorData(u8* data);
void senser_Init(u8 type);//所有传感器初始化
u8 sensor_get_data(u8 type,u8* data);//获取传感器数据

#endif

