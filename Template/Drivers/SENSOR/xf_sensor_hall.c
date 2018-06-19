/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_hall.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 霍尔传感器驱动程序
  */

#if 1
#define BIT(X)                (1 << X)
#define HALL_OUT_DATA_PIN_BIT BIT(7)
#define HALL_PIN              P1_7
#define HALL_DIR              P1DIR
#define HALL_IO               P1
#endif

#if 0
#define BIT(X)                (1 << X)
#define HALL_OUT_DATA_PIN_BIT BIT(4)
#define HALL_DIR              P0DIR
#define HALL_IO               P0
#endif


enum
{
    HALL_NO = 0,
    HALL_YES
};
/**
  * @brief  枚举类型  
  * @details NO：无电磁场
             YES:有电磁场
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
enum
{
  NO,
  YES
};

/* 霍尔传感器检测引脚配置为下拉输入 */
/**
  * @brief  霍尔传感器初始化 
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
void HallPinInit(void)
{
    P2INP |= BIT(6);  //端口1下拉
    HALL_DIR &= ~HALL_OUT_DATA_PIN_BIT;
}

/** 
  *@fn: 检测霍尔传感器数字引脚电平 
  *@param: 无
  *@return:HALL_YES - 有磁场
  *@        HALL_NO - 无磁场      
  */
uint8 DetectionHallDoutLevel(void)
{
    if(HALL_PIN)
    {
        return HALL_YES;
    }
    else
    {
        return HALL_NO;
    }
}
/** 
  *@fn: 判断有无磁场并上传上位机 
  *@param: 无
  *@return:YES - 有磁场
  *@        NO - 无磁场       
*/
uint8 XfGetHall(void)
{
    if(DetectionHallDoutLevel())
     {
#ifdef UART0_DEBUG
         uart0_printf("感应到磁场\r\n");
#endif
         return YES;
     }    
     else
     {     
#ifdef UART0_DEBUG
        uart0_printf("没感应到磁场\r\n");
#endif
        return NO;
     }
}