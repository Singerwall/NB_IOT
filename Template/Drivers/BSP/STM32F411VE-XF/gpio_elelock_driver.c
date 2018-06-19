/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  
  * @author  zp.h
  * @version  V3.0.0
  * @date  05/16/2018
  * @brief  ����������
  */ 

#include <includes.h>
/**
  * @brief	��������ʼ������
  * @details	���ö˿�ʱ�ӡ�IO��ģʽ�Ȳ���
  * @param NULL	��
  * @retval 	��
  */
void Electronic_lock_init(void)
{
  BSP_GPIO_Init(LOCK_PORT, LOCK_PIN, LOCK_MODE, LOCK_PULL);	/*��ʼ״̬�������ر�*/

}


/**
  * @brief	�������������
  * @details	��������λ�򿪵����
  * @param NULL	��
  * @retval  NULL	��
  */
void Electronic_Lock_Open(void)
{
 HAL_GPIO_WritePin(LOCK_PORT, LOCK_PIN, GPIO_PIN_SET);
  OSTimeDly (1000); 
 HAL_GPIO_WritePin(LOCK_PORT, LOCK_PIN, GPIO_PIN_RESET);
}

/**
  * @brief	������رպ���
  * @details	�����Ÿ�λ�رյ����
  * @param NULL	��
  * @param NULL	��
  * @retval 	��
  */
void Electronic_Lock_Close(void)
{
   HAL_GPIO_WritePin(LOCK_PORT, LOCK_PIN, GPIO_PIN_RESET);
}
/**
  * @brief	��ȡ�����״̬
  * @details	��ȡ�����״̬
  * @param NULL	��
  * @param NULL	��
  * @retval 	��
  */
u8 Electronic_Lock_get_state(void)
{
   return HAL_GPIO_ReadPin(LOCK_PORT, LOCK_PIN);
}




