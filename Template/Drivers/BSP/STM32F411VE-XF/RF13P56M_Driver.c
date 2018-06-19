/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		RF13P56M_Driver.c
  * @author		zp.h
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		RF13P56M_Driver����������
  */ 


/**
  * @����ͷ�ļ�
  */

#include "RF13P56M_Driver.h"

/**
  * @��������
  */

#define rfid_buff_size	200
static u8 rfid_rev_buff[rfid_buff_size];
static uint8_t is_operate_state = 0;//PN532�Ƿ��ڈ���ing
static void CleanBuffer(u16 num);
u8 send_to_rfid_buff(u8 *data,u8 len);









/**
  * @brief	PN532_WakeUp
  * @details	����PN532ģ�飬�����������⣺55 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF 03 FD D4 14 01 17 00
  * @param a	��
  * @param b	��
  * @retval 	��
  */ 
int PN532_WakeUp(void)
{
    u8 data[24];
    u8 i,time = 5;
    u8 CheckCode=0; /*����У���� */
    u8 temp=0;
    u32 uwRet = 0; 
    int rfid_struct_p;
    while(time--)
    { 
        data[0]=0x55;
        data[1]=0x55;
        data[2]=0x00;
        data[3]=0x00;
        data[4]=0x00;
        data[5]=0x00;
        data[6]=0x00;
        data[7]=0x00;
        data[8]=0x00;
        data[9]=0x00;
        data[10]=0x00;
        data[11]=0x00;
        data[12]=0x00;
        data[13]=0x00;
        data[14]=0x00;
        data[15]=0x00;       
        data[16]=0xFF;
        data[17]=0x03; /*�� ���� */
        data[18]=0xFD; /*�� ���� У��  0x100-data[3] */
        data[19]=0xD4; /*�����ʶ��*/
        data[20]=0x14; /*�����ʶ�� */
        data[21]=0x01;
        data[22]=0x17; /*���� У��   0x100- */
        data[23]=0x00; 
        send_to_rfid_buff(data,24);/*���� length���ȵ�����data*/
     
        uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 1000);
        if(uwRet != LOS_OK)
        {
          //debug_printf("\r\nPN532 wake up failure,error:%x\r\n",uwRet);
        }
        else
        {  
          memcpy(rfid_rev_buff, ((rfid_struct_t  * )rfid_struct_p)->data, ((rfid_struct_t  * )rfid_struct_p)->size);   
          for(i=11;i<13;i++)
          {
            temp+=rfid_rev_buff[i];
          }
          CheckCode=0x100-temp;
          if(CheckCode==rfid_rev_buff[13])
          {
            debug_printf("\r\n PN532 wake up ok \r\n");
            CheckCode=0x100-temp;
            CleanBuffer(40);/*��� ���ڽ��ջ�����ǰ30 ���ֽ����� */
            return 0;
          }
          delay_ms(10);
          
        }
	delay_ms(2000);
    }
    return -1;
}


/**
  * @brief	PN532_InListPassiveTarget
  * @details	Ѱ��
  * @param a	��
  * @param b	��
  * @retval 	
  */
int PN532_InListPassiveTarget(u8 *uid)
{
    u8 data[11];
    u8 i;
    u8 time = 2;
    u8 CheckCode=0; /*����У����	*/
    u8 temp=0;
    u8 chekdata[4] = {0};
    u32 uwRet = 0; 
    int rfid_struct_p;
     
    while(time--)
    {   
        data[0]=0x00;
        data[1]=0x00;
        data[2]=0xFF;
        data[3]=0x04; /*�� ����	*/
        data[4]=0xFC; /*�� ���� У��  0x100-data[3]*/
        data[5]=0xD4; /*�����ʶ�� */
        data[6]=0x4A; /*�����ʶ��	*/
        data[7]=0x01;
        data[8]=0x00;
        data[9]=0xE1; /*���� У��   0x100-*/
        data[10]=0x00;
        send_to_rfid_buff(data,11);

        uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 500);
        if(uwRet != LOS_OK)
        {
            // debug_printf("PN532 ListPassiveTarget  ack failure,error:%x\n",uwRet);  
             delay_ms(10);
        }
        else
        {
            if( ((rfid_struct_t  * )rfid_struct_p)->size == 0x06 )/*Ѱ���ظ�ok*/
            {
                  debug_printf("\r\nPN532 ListPassiveTarget ack ok \r\n");
                  memset(((rfid_struct_t  * )rfid_struct_p)->data, 0, ((rfid_struct_t  * )rfid_struct_p)->size);
                  
                  uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 1000);/*����Ѱ���Ŀ���*/
                  if(uwRet != LOS_OK)
                  {
                      debug_printf("\r\nPN532 ListPassiveTarget ID failure \r\n ,error:%x\n",uwRet);
                  }
                  else
                  {  
                      memcpy(rfid_rev_buff, ((rfid_struct_t  * )rfid_struct_p)->data, ((rfid_struct_t  * )rfid_struct_p)->size);
                      temp=0;
                      for(i=(11-6);i<(23-6);i++)
                      {
                          temp+=rfid_rev_buff[i];
                      }
                      CheckCode=0x100-temp;
                      if(CheckCode==rfid_rev_buff[23-6])
                      {
                          memcpy(uid,&rfid_rev_buff[19-6],4);
                          if(memcmp(uid,chekdata,4) != 0)/*����Ƿ�Ѱ����*/
                          { 
                             debug_printf("\r\nPN532 ListPassiveTarget ID:");
                             debug_print_buffer(uid,4);
                             debug_printf("\r\n");
                             CleanBuffer(40);			 /*��� ���ڽ��ջ�����ǰ40 ���ֽ����� */
                             return 0;
                          }
                         
                      }            
                  }
            }
            else/*ack ok + ����*/
            {    
                  memcpy(rfid_rev_buff, ((rfid_struct_t  * )rfid_struct_p)->data, ((rfid_struct_t  * )rfid_struct_p)->size);
                  temp=0;
                  for(i=(11-6);i<(23-6);i++)
                  {
                    temp+=rfid_rev_buff[i];
                  }
                  CheckCode=0x100-temp;
      
                  if(CheckCode==rfid_rev_buff[23-6])
                  {
                    memcpy(uid,&rfid_rev_buff[19-6],4);
                    if(memcmp(uid,chekdata,4) != 0)/*����Ƿ�Ѱ����*/
                    { 
                      debug_printf("ListPassiveTarget ID ok \n");
                      CleanBuffer(40);			 /*��� ���ڽ��ջ�����ǰ40 ���ֽ����� */
                      return 0;
                    }
                    delay_ms(500);
                  }  
                   return -1;
             } 
                          
        }
          
    }
    return -1;
}


/**
  * @brief		PN532_PsdVerifyKeyA
  * @details	������Ȩ����֤KeyA
  * @param a	��
  * @param b	��
  * @retval 	00 00 FF 0F F1 D4 40 01 60 03 FF FF FF FF FF FF UID1 UID2 UID3 UID4 2A 00
  *             00 00 FF 0F F1 D4 40 01 60 03 FF FF FF FF FF FF 94 8A 3B 0B 2A 00
  */
int PN532_PsdVerifyKeyA(u8 block,u8 *uid)
{
    u8 data[22];
    u8 temp=0; 
    u8 i;
    u8 CheckCode=0; /*����У����*/
    u32 uwRet = 0; 
    int rfid_struct_p;
    
    data[0]=0x00;
    data[1]=0x00;
    data[2]=0xFF;
    
    data[3]=0x0F; /*�� ���� */
    data[4]=0xF1; /*�� ���� У��  0x100-data[3] */
    
    data[5]=0xD4; /*�����ʶ�� */
    data[6]=0x40; /*�����ʶ�� */
    
    data[7]=0x01;
    data[8]=0x60;
    data[9]=block; 
    
    data[10]=0xFF;                    /*KEY A ���� FF FF FF FF FF FF*/
    data[11]=0xFF;
    data[12]=0xFF;
    data[13]=0xFF;
    data[14]=0xFF;
    data[15]=0xFF;
    memcpy(&data[16],uid,4);
    
    for(i=5;i<20;i++)
    {
        temp+=data[i];
    }

    data[20]=0x100-temp;                /*���� У��   0x100- */
    
    data[21]=0x00;
    
    send_to_rfid_buff(data,22);                  /*��USART2������ length���ȵ�����data */
    
    uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 500);
    if(uwRet != LOS_OK)
    {
       //debug_printf("PN532_PsdVerifyKeyA ack failure,error:%x\n",uwRet);
    }
    else
    {
      //debug_printf("PN532_PsdVerifyKeyA  ack ok");
      
      uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 1000);
      if(uwRet != LOS_OK)
      {
        debug_printf("\r\nPN532_PsdVerifyKeyA Check failure,error:%x\r\n",uwRet);
      }
      else
      {    
          memcpy(rfid_rev_buff, ((rfid_struct_t  * )rfid_struct_p)->data, ((rfid_struct_t  * )rfid_struct_p)->size);
          
          temp=0;
          for(i=11-6;i<14-6;i++)
          {
            temp+=rfid_rev_buff[i];
          }
          CheckCode=0x100-temp;
          if(CheckCode==rfid_rev_buff[14-6])
          {
            if(rfid_rev_buff[13-6]==0x00)
            { 
              debug_printf("\r\nPN532_PsdVerifyKeyA Check ok \r\n");
              CleanBuffer(40);              /*��� ���ڽ��ջ�����ǰ40 ���ֽ�����*/
              return 0; /*��֤��Կ�ɹ�*/
            } 
          } 
        }
    }

  return -1;	   /*��֤��Կʧ��*/
}


/**
  * @brief		PN532_read
  * @details	block ��ȡ����
  * @param a	��
  * @param b	��
  * @retval 	00 00 FF 05 FB D4 40 01 30 02 B9 00
  */
int PN532_read(u8 block,u8 *block_data)
{
    u8 data[12];
    u8 temp=0; 
    u8 i;
    u8 time = 2;
    u8 CheckCode=0; /*����У����*/
    u32 uwRet = 0; 
    int rfid_struct_p;
    while(time--)
    {   
        data[0]=0x00;
        data[1]=0x00;
        data[2]=0xFF;
        
        data[3]=0x05; /*�� ���� */
        data[4]=0xFB; /*�� ���� У��  0x100-data[3]*/
        
        data[5]=0xD4; /*�����ʶ�� */
        data[6]=0x40; /*�����ʶ��  */
        
        data[7]=0x01;
        data[8]=0x30;
        data[9]=block; /*��16�ֽ�����*/ 
        temp=0;
        for(i=5;i<10;i++)
        {
            temp+=data[i];
        }    
        data[10]=0x100-temp; /*����У��	*/
        data[11]=0x00;

        send_to_rfid_buff(data,12);
        
        uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 1000);
        if(uwRet != LOS_OK)
        {
          // debug_printf("\r\nPN532_read ack failure,error:%x",uwRet); 
        }
        else
        {
          debug_printf("\r\nPN532_read ack ok");
          
          uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 1000); /*����ظ���������*/
          if(uwRet != LOS_OK)
          {
            debug_printf("PN532_read failure,error:%x\n",uwRet);
          }
          else
          {
              memcpy(rfid_rev_buff, ((rfid_struct_t  * )rfid_struct_p)->data, ((rfid_struct_t  * )rfid_struct_p)->size);
              
              temp=0;
              for(i=(11-6);i<(30-6);i++)
              {
                temp+=rfid_rev_buff[i];
              }
              CheckCode=0x100-temp;
              if(CheckCode==rfid_rev_buff[30-6])
              { 
                memcpy(block_data,&rfid_rev_buff[14-6],16);
               
                debug_printf("\r\nPN532_read data:");
                debug_print_buffer(&rfid_rev_buff[14-6],16);
                debug_printf("\r\n");
                
                CleanBuffer(40);/*��� ���ڽ��ջ�����ǰ40 ���ֽ�����*/
                return 0;     /*�������ݳɹ�*/
              }
              else
              {
                CleanBuffer(40);
              }          
          }
        }
        
        delay_ms(500);
   
  } 
    return -1;
}


/**
  * @brief		PN532_write
  * @details	Ĭ���� 02��д16���ֽ�
  * @param a	00 00 FF 15 EB D4 40 01 A0 02 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D1 00
  * @param b	��
  * @retval 	
  */
int PN532_write(u8 block,u8 *block_data)
{
    u8 data[28];
    u8 temp=0;
    u8 i;
    u8 time = 3;
    u8 CheckCode=0;
    u32 uwRet = 0; 
    int rfid_struct_p;
    while(time--)    
    {
        data[0]=0x00;
        data[1]=0x00;
        data[2]=0xFF;
        
        data[3]=0x15; /*�� ���� */
        data[4]=0xEB; /*�� ���� У��  0x100-data[3]*/
        
        data[5]=0xD4; /*�����ʶ�� */
        data[6]=0x40; /*�����ʶ��*/
        
        data[7]=0x01; /*��д����6�ֽ� ����1�����ֲ�*/
        data[8]=0xA0; /*д*/
        data[9]=block; /*д16�ֽ�����*/   
        
        memcpy(&data[10],block_data,16);
        for(i=5;i<26;i++)
        {
            temp+=data[i];
        }
        data[26]=0x100-temp; /*���� У����*/
        data[27]=0x00;
        send_to_rfid_buff(data,28);
        
        uwRet = LOS_QueueRead(rfid_uwQueue, &rfid_struct_p, 10, 5000);
        if(uwRet != LOS_OK)
        {
          debug_printf("\r\nPN532 Write failure,error:%x\r\n",uwRet);
        }
        else
        {
            debug_printf("recv message ok \n");
            memcpy(rfid_rev_buff, ((rfid_struct_t  * )rfid_struct_p)->data, ((rfid_struct_t  * )rfid_struct_p)->size);
            
                   
            /*����00 00 FF 00 FF 00 00 00 FF 03 FD D5 41 00 EA 00 */
            temp=0;
            for(i=11;i<14;i++)
            {
                temp+=rfid_rev_buff[i];
            }
            CheckCode=0x100-temp;
            if(CheckCode==0xEA)
            {
                CleanBuffer(40);/*��� ���ڽ��ջ�����ǰ40 ���ֽ����� */
                return 0;/*д�����ݳɹ�*/  
            }
            else
            {
                CleanBuffer(40);	
            }
        }
        OSTimeDly (500); 

    }		
    return -1;
}


/**
  * @brief  ͨ��SPI3�����ݷ���stc15
  * @details	
  * @param Data	���ݻ���
  * @param len	���ݳ���
  * @retval 	��
  */
u8 send_to_rfid_buff(u8 *data,u8 len)
{
  return SPI3_Send_Packet(0x7,0x7,data,len);
}

/**
  * @brief		CleanBuffer
  * @details	����ǰ���ٸ��ֽڵ�����
  * @retval 	��
  */
static void CleanBuffer(u16 num)
{
    memset(rfid_rev_buff,0,num);
}



/**
  * @brief   13.56MѰ��ǩ
  * @param   packet ���ݰ�
  * @retval  Ѱ��ǩ���
 */
int _cmd_13p56m_list_card(Device_packet_t *packet)
{
    uint8_t card_uid[4] = {0};
    if(is_operate_state)
        return 0;
    is_operate_state = 1;
 //led�W�q
    if(!PN532_InListPassiveTarget(card_uid))
    {
        packet->ack = ACK_OK;
        packet->data_size = 4;
        memcpy(packet->data,card_uid,4);
        debug_printf("list 13.56m card uid:");
        debug_print_buffer(card_uid,4);  
      //  Beep_output_time(30);
    }
    else
    {
        packet->ack = ACK_CARD_HANDLE_ERR;
        packet->data_size = 0;
    }
    is_operate_state = 0;
   // _led_set_status(LED_YELLOW,LED_MODE_OFF);
    return  1;//zigbee_send_pack(packet);
}
 

/**
  * @brief   13.56M����ǩ
  * @param   packet ���ݰ�
  * @retval  Ѱ��ǩ���
 */
int _cmd_13p56m_read_card(Device_packet_t *packet)
{
    uint8_t card_uid[4] = {0};
    uint8_t block_data[16] = {0};
    if(is_operate_state)
        return 0;
    is_operate_state = 1;
    //app_led_blink(LED_YELLOW, 1, 100, 3000);
    if(!PN532_InListPassiveTarget(card_uid))/*Ѱ��*/
    {

        if(!PN532_PsdVerifyKeyA(packet->data[0],card_uid))/*��֤��Կ*/
        {
        
            if(!PN532_read(packet->data[0],block_data))/*����*/
            {
                packet->ack = ACK_OK;
                packet->data_size = 20;
                memcpy(packet->data,card_uid,4);
                memcpy(packet->data+4,block_data,16);
                debug_printf("read 13.56m card uid and block_data:");
                debug_print_buffer(card_uid,4);
                debug_print_buffer(block_data,16);
                //Beep_output_time(30);           
            }
            else
            {
                packet->ack = ACK_CARD_HANDLE_ERR;
                packet->data_size = 0;
            }
       
        }
        else
        {
            packet->ack = ACK_CARD_HANDLE_ERR;
            packet->data_size = 0;
        }

    }
    else
    {
        packet->ack = ACK_CARD_HANDLE_ERR;
        packet->data_size = 0;
    }    
    is_operate_state = 0;
    return 1;//zigbee_send_pack(packet);
}


/**
  * @brief   13.56Mд��ǩ
  * @param   packet ���ݰ�
  * @retval  Ѱ��ǩ���
 */
int _cmd_13p56m_write_card(Device_packet_t *packet)
{
    uint8_t card_uid[4] = {0};
    if(is_operate_state)
        return 0;
    is_operate_state = 1;
    //app_led_blink(LED_YELLOW, 1, 80, 1000);
    if(!PN532_InListPassiveTarget(card_uid))/*Ѱ��*/
    {
        if(!PN532_PsdVerifyKeyA(packet->data[0],card_uid))/*��֤��Կ*/
        {
            if(!PN532_write(packet->data[0],&packet->data[1]))/*д��*/
            {
                packet->ack = ACK_OK;
                packet->data_size = 0;
                debug_printf("write 13.56m card uid and block_data:");
                debug_print_buffer(&packet->data[1],16);
                //Beep_output_time(30);
            }
            else
            {
                packet->ack = ACK_CARD_HANDLE_ERR;
                packet->data_size = 0;
            }
        }
        else
        {
            packet->ack = ACK_CARD_HANDLE_ERR;
            packet->data_size = 0;
        }
    }
    else
    {
        packet->ack = ACK_CARD_HANDLE_ERR;
        packet->data_size = 0;
    }     
    is_operate_state = 0;
    return 1;//zigbee_send_pack(packet);
}
