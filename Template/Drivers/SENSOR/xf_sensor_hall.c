/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_hall.c
  * @author  wuyijun
  * @version  V1.0.1
  * @date  14/07/2016
  * @brief  cc2530 ������������������
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
  * @brief  ö������  
  * @details NO���޵�ų�
             YES:�е�ų�
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
enum
{
  NO,
  YES
};

/* ���������������������Ϊ�������� */
/**
  * @brief  ������������ʼ�� 
  * @details 
  * @param  NULL
  * @param  NULL
  * @return NULL
 **/
void HallPinInit(void)
{
    P2INP |= BIT(6);  //�˿�1����
    HALL_DIR &= ~HALL_OUT_DATA_PIN_BIT;
}

/** 
  *@fn: �������������������ŵ�ƽ 
  *@param: ��
  *@return:HALL_YES - �дų�
  *@        HALL_NO - �޴ų�      
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
  *@fn: �ж����޴ų����ϴ���λ�� 
  *@param: ��
  *@return:YES - �дų�
  *@        NO - �޴ų�       
*/
uint8 XfGetHall(void)
{
    if(DetectionHallDoutLevel())
     {
#ifdef UART0_DEBUG
         uart0_printf("��Ӧ���ų�\r\n");
#endif
         return YES;
     }    
     else
     {     
#ifdef UART0_DEBUG
        uart0_printf("û��Ӧ���ų�\r\n");
#endif
        return NO;
     }
}