/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  �û�������
  */ 
#include <includes.h>
#include "main.h"

typedef struct
{
  u16 value;/*��ֵ*/
  u8 device_type;/*�豸����*/
  u8 action ;/*ִ�п���*/
  u8 mask;
  void (*callback)(u8 s)  ;/**/
} rule_t;

typedef struct
{
  rule_t max;
  rule_t min;
}smart_rule_t;
  
smart_rule_t smart_rule;

UINT32 dwp_uwQueue=0;
static void temper_to_dwp(void);
void uid_state_to_dwp(void);

static void * Task_DWP(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief ����DWP����
  * @retval ��
 */
uint32_t create_Task_DWP(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_DWP_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*��������:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_DWP;
    stInitParam1.pcName = "Task_DWP";
    stInitParam1.usTaskPrio = Prio_Task_DWP;
    stInitParam1.uwStackSize =StackSize_Task_DWP;
  
    LOS_TaskLock();//��ס���񣬷�ֹ�´���������ȱ�����߶���������
    uwRet = LOS_TaskCreate(&Task_DWP_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_DWP failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//��������ֻ�ж��д�����ſ�ʼ�������
    
    return LOS_OK;
}

/**
  * @brief 
  * @details ����������
  * @param  pdata ����
  * @retval ��
 */
static void * Task_DWP(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
  UINT32 uwRet = 0; 
  int screen_struct_p;
  u8 screen_rev_buff[40]={0};
  u16 dwp_cmd = 0; 
  u16 time= 0;
  int8_t temper = 0;
    /*��������*/
    uwRet = LOS_QueueCreate("dwp_queue", 3, &dwp_uwQueue, 0, 40);
    if(uwRet != LOS_OK) debug_print_string("create dwp_queue failure!,error:%x\n",uwRet);
    else debug_print_string("create the dwp_queue success!\n");
    
    
    debug_printf("[%s] enter.\r\n", __func__);
    
    u8 temp_data[6]={0};
    
    for(u8 i=0; i<20;i++)
    {
      DS18B20_Get_Temp(temp_data); 
      OSTimeDly (200);
    }
#if 1         
    sgn_GPS_stars_1_to_dwp( 50 );
    sgn_GPS_stars_2_to_dwp( 50 );
    sgn_GPS_stars_3_to_dwp( 50 );
    sgn_GPS_stars_4_to_dwp( 50 );
    sgn_BD_stars_1_to_dwp(  50 );
    sgn_BD_stars_2_to_dwp(  50 );
    sgn_BD_stars_3_to_dwp(  50 );
    sgn_BD_stars_4_to_dwp(  50 );
#endif


    for(;;)
    {  
        uwRet = LOS_QueueRead(dwp_uwQueue, &screen_struct_p, 10, 10);
        if(uwRet != LOS_OK)
        {
          if(time %20 == 0)
          {
              temper_to_dwp();
              pwoer_to_dwp(50);
              relay_state_to_dwp( get_RELAY());
              elelock_state_to_dwp(Electronic_Lock_get_state());
              //set_APP_STATE(3);
              temper = (int8_t)DS18B20_Get_Tempter();
              if(smart_rule.max.mask == 0x07)
              {
                if(temper > smart_rule.max.value)
                {
                  smart_rule.max.callback(smart_rule.max.action);
                }
                else
                {
                  smart_rule.max.callback(!smart_rule.max.action);
                }
              }
              if(smart_rule.min.mask == 0x07)
              {
                if(temper < smart_rule.min.value)
                {
                  smart_rule.min.callback(smart_rule.min.action);
                }
                else
                {
                  smart_rule.min.callback(!smart_rule.min.action);
                }
              }
          }
          delay_ms(100);
          time++;
        }
        else   //��Ч����
        {
          memcpy(screen_rev_buff, ((rfid_struct_t  * )screen_struct_p)->data, ((rfid_struct_t  * )screen_struct_p)->size);
          if(screen_rev_buff[0]==0x5a && screen_rev_buff[1]==0xa5)
          {
             dwp_cmd = screen_rev_buff[4]*256+screen_rev_buff[5];
             switch(dwp_cmd)
             {
                 case 4096://�^��� 0x1000
                   set_RELAY(screen_rev_buff[8]);
                   break;
                 case 4097://늴��i0x1001
                   if(screen_rev_buff[8]){ Electronic_Lock_Open();}
                   else{Electronic_Lock_Close();}  
                   break; 
                 case 4098:/*����0x1002*/
                   uid_state_to_dwp();  
                   break;
                 case 4099:/*����0x1003*/
                   //uid_state_to_dwp();  
                   break;    
                 case 4100:/*ȷ��0x1004*/
                   //uid_state_to_dwp();  
                   break; 
                 case 4128:/*������Ϣ��ȡ0x1020 Ҫ��������Ϣ�ϴ���dwp*/
                   //uid_state_to_dwp();  
                     break;
//                     case 4101:/*������ȡmac��ַ 0x1005*/
//                       //uid_state_to_dwp();  
//                       break; 
//                     case 4102:/*������ȡ�豸����0x1006*/
//                       //uid_state_to_dwp();  
//                       break; 
                     case 0x1007:/*������0x1007*/
                        set_BL_POW_Ctrl(1);  ;  
                       break; 
                     case 0x1008:/*������0x1008*/
                        set_BL_POW_Ctrl(0);  
                       break; 
//                     case 0x1009:/*����״̬�򿪡��رա�����0x1009*/
//                       //uid_state_to_dwp();  
//                       break; 
 
                 case 0x1540:/*Gps�·���ȡ��Ϣ  ��GPS ��Դ���� 0x1540*/
                   set_GPS_POW_Ctrl(1);
                   set_flag_gps_update_to_dwp(1);
                   break; 
                case 0x1545:/*Gps�·���ȡ��Ϣ  ��GPS ��Դ���� 0x1540*/
                   set_GPS_POW_Ctrl(0);
                   set_flag_gps_update_to_dwp(0);
                   break; 
                 case 0x1800:/*NB��ȡ��Ϣ*/
                   set_flag_nb_update_to_dwp(1);
                   set_APP_STATE(3);
                   break;  
                /*���ܹ���*/      
                case 0x2300:/*�����¶���ֵ*/
                  smart_rule.max.value =  (screen_rev_buff[7]-0x30)*10+(screen_rev_buff[8]-0x30);
                   smart_rule.max.mask |= 0x01;
                         break; 
                case 0x2350:/*ִ���豸 �̵���*/
                  smart_rule.max.device_type = DEVICE_RELAY;
                  smart_rule.max.callback = set_RELAY;
                  smart_rule.max.mask |= 0x02;
                         break;  
                case 0x2351:/*ִ���豸 �������*/
                  smart_rule.max.device_type = DEVICE_STEPPER_MOTOR;
                  smart_rule.max.callback = set_moto_turn;
                  smart_rule.max.mask |= 0x02;
                         break;                           
                case 0x2352:/*��*/
                  smart_rule.max.action= 1;
                  smart_rule.max.mask |= 0x04;
                         break; 
                case 0x2353:/*�ر�*/
                  smart_rule.max.action= 0;
                  smart_rule.max.mask |= 0x04;
                         break;           
                case 0x2400:/*�����¶���ֵ*/
                  smart_rule.min.value = (screen_rev_buff[7]-'0')*10+(screen_rev_buff[8]-'0');
                  smart_rule.min.mask |= 0x01;
                         break; 
                case 0x2450:/*ִ���豸 �̵���*/
                  smart_rule.min.device_type = DEVICE_RELAY;
                  smart_rule.min.callback = set_RELAY;
                  smart_rule.min.mask |= 0x02;
                         break;  
                case 0x2451:/*ִ���豸 �������*/
                   smart_rule.min.device_type = DEVICE_STEPPER_MOTOR;
                   smart_rule.min.callback = set_moto_turn;
                   smart_rule.min.mask |= 0x02;
                         break;                           
                case 0x2452:/*��*/
                  smart_rule.min.action=1;
                   smart_rule.min.mask |= 0x04;
                         break; 
                case 0x2453:/*�ر�*/
                  smart_rule.min.action=0; 
                   smart_rule.min.mask |= 0x04;
                         break;                         
                      
                 default:
                   break;
             }
          } 
        }
        delay_ms(100);
    }
}



     
/**
  * @brief 
  * @details �����¶ȸ�������
  * @param  
  * @retval ��
 */
void date_time_to_dwp(u8 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec)
{
                            //5A   A5    0B    82   00         9C          5A      A5   12    05    0A    11   0C   10
                         /*֡ͷ1��֡ͷ2 ���ȣ�ָ�� ��ַ���ֽ� ��ַ���ֽ�  �̶�1 �̶�2  �ꡪ�������¡��������ա�������ʱ���������֡���������*/
      u8 lcd_time_data[14]={0x5A, 0xA5, 0x0B, 0x82, 0x00,     0x9C,       0x5A, 0xA5,  0x12, 0x05, 0x0A, 0x11, 0x1B, 0x10};
      lcd_time_data[8]=year;
      lcd_time_data[9]=mon;
      lcd_time_data[10]=day;
      lcd_time_data[11]=hour;
      lcd_time_data[12]=min;
      lcd_time_data[13]=sec;
     SCREEN_USART_Handle_Transmit(lcd_time_data, 14);

}
/**
  * @brief 
  * @details �����¶ȸ�������
  * @param  
  * @retval ��
 */
void temper_to_dwp(void)
{
     u8 temp_data[6]={0};
     u8 lcd_temp_data[14]={0x5A,0xA5,0x0B,0x82,0x03,0x10,0x5A,0xA5,0x01,0x00,0x00,0x01,0x00,0x14};
     DS18B20_Get_Temp(temp_data); 
     lcd_temp_data[13]=temp_data[2];
     SCREEN_USART_Handle_Transmit(lcd_temp_data, 14);
}
/**
  * @brief 
  * @details ���ͼ̵���״̬��������
  * @param  
  * @retval ��
 */
void relay_state_to_dwp(u8 state)
{
     u8 lcd_temp_data[]={0x5A, 0xA5, 0x05, 0x82, 0x10, 0x00, 00, 01};
     if(state)
     {
      lcd_temp_data[7] = 1;
     }
     else
     {
      lcd_temp_data[7] = 0;
     }
     SCREEN_USART_Handle_Transmit(lcd_temp_data, 8);
}
/**
  * @brief 
  * @details ���͵����״̬��������
  * @param  
  * @retval ��
 */
void elelock_state_to_dwp(u8 state)
{
     u8 lcd_temp_data[]={0x5A, 0xA5, 0x05, 0x82, 0x10, 0x01, 00, 01};
     if(state)
     {
      lcd_temp_data[7] = 1;
     }
     else
     {
      lcd_temp_data[7] = 0;
     }
     SCREEN_USART_Handle_Transmit(lcd_temp_data, 8);
}


/**
  * @brief 
  * @details ���͵����״̬��������
  * @param  
  * @retval ��
 */
void uid_state_to_dwp(void)
{ 
     u8 uid[4]= {1,2,3,4};
     u8 time = 0;
     u8 lcd_temp_data[]={0x5A, 0xA5, 0x0b, 0x82, 0x13, 0x00, 01,01,01,01, 01,01,01,01};
//     while(1)
//     {
//       if(!PN532_InListPassiveTarget(uid)) break;
//       else
//       {
//         delay_ms(100);
//         time ++;
//         if(time == 50)break;
//         
//       }
//       
//     }
     
#if 0 
    OSTimeDly (1000); 
    if(1)
    {
      packet.data[0]=0x02;
      _cmd_13p56m_read_card(&packet);
      if(packet.ack == ACK_OK)
      {
        Electronic_Lock_Open();
        app_transmit_packet(10001,3,4,5,6,packet.data_size,packet.data);
       
          
        OSTimeDly (2000);
        Electronic_Lock_Close();
      }
    }
#endif
    
        for(u8 i = 0; i<4; i++)
        {
          hex_to_ascii( (uint8_t *)&lcd_temp_data[6+2*i],uid[i]);
        }
     SCREEN_USART_Handle_Transmit(lcd_temp_data, 14);
}


/**
  * @brief 
  * @details ����γ�ȸ�������
  * @param  lat γ��
  * @retval ��
 */
void lat_to_dwp(double lat)
{
  char fstr[10]={0};
    u8 size=0;
    u8 lcd_temp_data[]={0x5A, 0xA5, 0x0b,    0x82,  0x16,0x00,  01,01,01,01, 01,01,01,01, 01,01};
    size=sprintf(fstr,"%f",lat);
    lcd_temp_data[2]=size+3;
    memcpy(&lcd_temp_data[6],fstr,size);
    SCREEN_USART_Handle_Transmit(lcd_temp_data, size+3+3);
}
/**
  * @brief 
  * @details ���;��ȸ�������
  * @param  lon ����
  * @retval ��
 */
void lon_to_dwp(double lon)
{
  char fstr[10]={0};
  
    u8 size=0;
    u8 lcd_temp_data[]={0x5A, 0xA5, 0x0b,    0x82,  0x15,0x50,  01,01,01,01, 01,01,01,01, 01,01};
    memset(fstr,0,10);
    size=sprintf(fstr,"%f",lon);
    lcd_temp_data[2]=size+3;
    memcpy(&lcd_temp_data[6],fstr,size);
    SCREEN_USART_Handle_Transmit(lcd_temp_data, size+3+3);
}
/**
  * @brief 
  * @details ��u8�������� ���͵���Ӧ��ַ
  * @param  value ��ֵ
  * @param  adress ��ַ
  * @retval ��
 */
void u8_to_dwp(u8 value,u16 adress)
{

    u8 lcd_temp_data[]={0x5A, 0xA5, 0x05,    0x82,  0x17,0x10,  00,00};

    lcd_temp_data[4]=(adress>>8)&0xff;
    lcd_temp_data[5]=adress&0xff;
    lcd_temp_data[7]=value;
  
    SCREEN_USART_Handle_Transmit(lcd_temp_data,8);
}
/**
  * @brief 
  * @details ���Ϳ���ʹ�õ�������������
  * @param  count ����
  * @retval ��
 */
void stars_inuse_to_dwp(u8 value)
{
  char fstr[10]={0};
    u8 size=0;
    u8 lcd_temp_data[]={0x5A, 0xA5, 0x0b,    0x82,  0x16,0x50,  01,01,01,01, 01,01,01,01, 01,01};
    size=sprintf(fstr,"%d",value);
    lcd_temp_data[2]=size+3;
    memcpy(&lcd_temp_data[6],fstr,size);
    SCREEN_USART_Handle_Transmit(lcd_temp_data, size+3+3);
}

/**
  * @brief 
  * @details GPS��1�ź�ǿ��
  * @param  count ����
  * @retval ��
 */
void sgn_GPS_stars_1_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1740);
}
/**
  * @brief 
  * @details GPS��2�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_GPS_stars_2_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1750);
}
/**
  * @brief 
  * @details GPS��3�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_GPS_stars_3_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1760);
}
/**
  * @brief 
  * @details GPS��4�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_GPS_stars_4_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1770);
}
/**
  * @brief 
  * @details BD��5�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_BD_stars_1_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1700);
}
/**
  * @brief 
  * @details BD��5�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_BD_stars_2_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1710);
}
/**
  * @brief 
  * @details BD��5�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_BD_stars_3_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1720);
}
/**
  * @brief 
  * @details ��5�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_BD_stars_4_to_dwp(u8 value)
{
  u8_to_dwp( value,0x1730);
}
/**
  * @brief 
  * @details ���ַ������͸�DWP�����20���ֽ�
  * @param  string �ַ���
  * @param  adress ��ַ
  * @retval ��
 */
void string_to_dwp(char * string ,u16 adress)
{
  char fstr[20]={0};
    u8 size=0;
    memset(fstr,0,10);
    u8 lcd_temp_data[]={0x5A, 0xA5, 0x0b,    0x82,  0x17,0x10,  01,01,01,01, 01,01,01,01, 01,01,01,01,01,01, 01,01,01,01, 01,01};
    size=sprintf(fstr,"%s",string);
    lcd_temp_data[2]=size+3;
    lcd_temp_data[4]=(adress>>8)&0xff;
    lcd_temp_data[5]=adress&0xff;
    memcpy(&lcd_temp_data[6],fstr,size);
    SCREEN_USART_Handle_Transmit(lcd_temp_data, size+3+3);
}
/**
  * @brief 
  * @details NB��������
  * @param  statu ����״̬
  * @retval ��
 */
void net_nb_connect_statu_to_dwp(u8 statu )
{
  char *string =null;
  if(statu == 1)
  {
   string = " ON"; 
  }
  else if(statu == 0)
  {
   string = "OFF"; 
  }
  string_to_dwp( string ,0x2000);
}

/**
  * @brief 
  * @details NB��������
  * @param  count ǿ��
  * @retval ��
 */
void net_type_nb_to_dwp(u8 type)
{
   char *string =null;
  if(type == 1)
  {     
    string = "DianXing";
  }
  else if(type == 2)
  {     
    string = "YiDong";
  }
  else if(type == 3)
  {     
    string = "LianTong";
  }
  string_to_dwp( string ,0x2050);
}
/**
  * @brief 
  * @details NB�ź�ǿ��
  * @param  count ǿ��
  * @retval ��
 */
void sgn_nb_to_dwp(char *string )
{
  u8 temp =0;
  char fstr[20]={0};
  temp=atoi(string);
  if(temp <10)
  {
    strcat(fstr, "0");
    strcat(fstr, string);
      string_to_dwp( fstr,0x2100);
  }
  else
  {
    string_to_dwp( string,0x2100);
  }

}
/**
  * @brief 
  * @details NB-IMEI
  * @param  count ǿ��
  * @retval ��
 */
void IMEI_nb_to_dwp(char * IMEI)
{
  string_to_dwp( IMEI ,0x2150);
}
/**
  * @brief 
  * @details NB-ͼ���ź�ǿ��logo
  * @param  count ǿ��
  * @retval ��
 */
void sgn_nb_logo_to_dwp(u8 value)
{
  u8 temp = 0;
  if( value <1)temp = 6;
  else if(value >=1 && value <5)temp = 1;
  else if(value >=5 && value <10)temp = 2;
  else if(value >=10 && value <15)temp = 3;
  else if(value >=15 && value <20)temp = 4;
  else if(value >=20 && value <25)temp = 4;
  else if(value >=25 && value <=33)temp = 5;
 
    u8_to_dwp( temp,0x2200);
}
/**
  * @brief 
  * @details ��ص������͸�DWP
  * @param  value ǿ��
  * @retval ��
 */
void pwoer_to_dwp(u8 value)
{
  u8_to_dwp( value,0x2250);
}






