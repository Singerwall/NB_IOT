/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  GPS����ҵ��
  */ 
#include <includes.h>
#include "main.h"
#include "stm32f4xx.h"



#include "nmea/nmea.h"


static __IO u8  flag_gps_update_to_dwp = 0;

void set_flag_gps_update_to_dwp(u8 on_off)
{
    flag_gps_update_to_dwp = on_off;
}

UINT32 gps_uwQueue=0;
void trace(const char *str, int str_size);
void error(const char *str, int str_size);
void gps_info(const char *str, int str_size);
void gps_info(const char *str, int str_size); 
void GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) ;


static void * Task_GPS(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief ���� NBģ���oceanconnectƽ̨ͨ������
  * @retval ��
 */
uint32_t create_Task_GPS(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_GPS_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*��������:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_GPS;
    stInitParam1.pcName = "Task_GPS";
    stInitParam1.usTaskPrio = Prio_Task_GPS;
    stInitParam1.uwStackSize =StackSize_Task_GPS; 
  
    LOS_TaskLock();//��ס���񣬷�ֹ�´���������ȱ�����߶���������
    uwRet = LOS_TaskCreate(&Task_GPS_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_GPS failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//��������ֻ�ж��д�����ſ�ʼ�������
    
    return LOS_OK;
}

/**
  * @brief 
  * @details 
  * @param  pdata ����
  * @retval ��
 */
static void * Task_GPS(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
    UINT32 uwRet = 0; 
    int gps_struct_p;
    u8 record[7]={0};
    u8 zero = 0;
    double deg_lat;/*ת����[degree].[degree]��ʽ��γ��*/
    double deg_lon;/*ת����[degree].[degree]��ʽ�ľ���*/
    
    nmeaINFO info; /*GPS�����õ�����Ϣ*/     
    nmeaPARSER parser;      /*����ʱʹ�õ����ݽṹ  */
//    uint8_t new_parse=0;    /*�Ƿ����µĽ������ݱ�־*/
    
    nmeaTIME beiJingTime;    /*����ʱ��*/ 
    
    /* �����������������Ϣ�ĺ��� */
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;
    nmea_property()->info_func = &gps_info;
    
    /* ��ʼ��GPS���ݽṹ */
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
    
    
    /*��������*/
    uwRet = LOS_QueueCreate("GPS_queue", 2, &gps_uwQueue, 0, 40);
    if(uwRet != LOS_OK) debug_print_string("create GPS_queue failure!,error:%x\n",uwRet);
    else debug_print_string("create the GPS_queue success!\n");
    debug_printf("[%s] enter.\r\n", __func__);
    
    for(;;)
    {  
        uwRet = LOS_QueueRead(gps_uwQueue, &gps_struct_p, 10, 10);
        if(uwRet != LOS_OK)/*δ���ܵ�gps����*/
        {    
         // debug_printf("\n\r get gps_uwQueue message failure,error:%x\n\r",uwRet);
        }
        else/*���յ���������*/
        {
          LOS_TaskLock();
         // debug_printf("\n\r get gps_uwQueue message ok\n\r"); 
          
         nmea_parse(&parser, (const char*)((gps_struct_t  * )gps_struct_p)->data, 
                                         ((gps_struct_t  * )gps_struct_p)->size, &info);
          /* �Խ�����ʱ�����ת����ת���ɱ���ʱ�� */
          GMTconvert(&info.utc,&beiJingTime,8,1);
         
          deg_lat = nmea_ndeg2degree(info.lat);
          deg_lon = nmea_ndeg2degree(info.lon);
          LOS_TaskUnlock();          
          delay_ms(10);

#if 0
          /* �������õ�����Ϣ */
          debug_printf("\r\nʱ��%d-%02d-%02d,%d:%d:%d", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec); 
          debug_printf("\r\nγ�ȣ�%f,����%f",deg_lat,deg_lon);
          debug_printf("\r\n���θ߶ȣ�%f �� ", info.elv);
          debug_printf("\r\n�ٶȣ�%f km/h ", info.speed);
          debug_printf("\r\n����%f ��", info.direction);			
          debug_printf("\r\n����ʹ�õ�GPS���ǣ�%d,�ɼ�GPS���ǣ�%d",info.satinfo.inuse,info.satinfo.inview);   
          for(u8 i=0;i<info.satinfo.inview;i++)
          {
             debug_printf("\r\n�ɼ�GPS���Ǳ�ţ�%d,�ɼ�GPS�����ź�ǿ�ȣ�%d ", info.satinfo.sat[i].id,info.satinfo.sat[i].sig);
          }
          debug_printf("\r\n����ʹ�õı������ǣ�%d,�ɼ��������ǣ�%d",info.BDsatinfo.inuse,info.BDsatinfo.inview);
          for(u8 i=0;i<info.BDsatinfo.inview;i++)
          {
             debug_printf("\r\n�ɼ�BD���Ǳ�ţ�%d,�ɼ�BD�����ź�ǿ�ȣ�%d ", info.BDsatinfo.sat[i].id,info.BDsatinfo.sat[i].sig);
          }
          debug_printf("\r\nPDOP��%f,HDOP��%f��VDOP��%f",info.PDOP,info.HDOP,info.VDOP);
#endif 
          if(flag_gps_update_to_dwp)
          {
            lat_to_dwp(deg_lat);
            lon_to_dwp(deg_lon);
           
            date_time_to_dwp(beiJingTime.year+1900-2000,beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
           
            stars_inuse_to_dwp(info.satinfo.inuse);
            if(info.satinfo.inuse == 0)
            {
#if 0          
              sgn_GPS_stars_1_to_dwp( 50 );
              sgn_GPS_stars_2_to_dwp( 50 );
              sgn_GPS_stars_3_to_dwp( 50 );
              sgn_GPS_stars_4_to_dwp( 50 );
              sgn_BD_stars_1_to_dwp(  50 );
              sgn_BD_stars_2_to_dwp(  50 );
              sgn_BD_stars_3_to_dwp(  50 );
              sgn_BD_stars_4_to_dwp(  50 );
#endif
            }
            else
            {
#if 1
              u8 j=0;
                for( u8 i=0;i<info.satinfo.inview;i++)
                {         
                  if(info.satinfo.sat[i].sig > 2)
                  {                 
                    j++;              
                    if(j == 1){ record[0]=i;}
                    else if(j == 2){ record[1]=i;}
                    else if(j == 3){ record[2]=i;}
                    else if(j == 4){ record[3]=i;break;}
                  }
                  else
                  {
                    zero = i;
                  }
                }
                memset(record+j,zero,4-(j%5)); 
                sgn_GPS_stars_1_to_dwp(50- info.satinfo.sat[ record[0]].sig );
                sgn_GPS_stars_2_to_dwp(50- info.satinfo.sat[ record[1]].sig );
                sgn_GPS_stars_3_to_dwp(50- info.satinfo.sat[ record[2]].sig );
                sgn_GPS_stars_4_to_dwp(50- info.satinfo.sat[ record[3]].sig );
                
                memset(record,0,4);
                j=0;
                for(u8 i=0;i<info.BDsatinfo.inview;i++)
                {
                  if(info.satinfo.sat[i].sig > 2)
                  {
                    j++;             
                    if(j == 1){ record[0]=i;}
                    else if(j == 2){ record[1]=i;}
                    else if(j == 3){ record[2]=i;}
                    else if(j == 4){ record[3]=i;}
                  }
                  else
                  {
                    zero = i;
                  }
                }
                memset(record+j,zero,4-(j%5));
                sgn_BD_stars_1_to_dwp(50- info.satinfo.sat[ record[0]].sig );
                    delay_ms(10);
                sgn_BD_stars_2_to_dwp(50- info.satinfo.sat[ record[1]].sig );
                    delay_ms(10);
                sgn_BD_stars_3_to_dwp(50- info.satinfo.sat[ record[2]].sig );
                    delay_ms(10);
                sgn_BD_stars_4_to_dwp(50- info.satinfo.sat[ record[3]].sig );
                memset(record,0,4);             
#endif 
            }        
   
          }
   
         ((gps_struct_t  * )gps_struct_p)->size = 0;
         ((gps_struct_t  * )gps_struct_p)->flag = 0;
      
          
        }
        delay_ms(300);
    }
}


/**
  * @brief  trace �ڽ���ʱ��������GPS���
  * @param  str: Ҫ������ַ�����str_size:���ݳ���
  * @retval ��
  */
void trace(const char *str, int str_size)
{
  #ifdef __GPS_DEBUG    //��gps_config.h�ļ���������꣬�Ƿ����������Ϣ
    uint16_t i;
    debug_printf("\r\nTrace: ");
    for(i=0;i<str_size;i++)
      debug_printf("%c",*(str+i));
  
    debug_printf("\n");
  #endif
}

/**
  * @brief  error �ڽ������ʱ�����ʾ��Ϣ
  * @param  str: Ҫ������ַ�����str_size:���ݳ���
  * @retval ��
  */
void error(const char *str, int str_size)
{
    #ifdef __GPS_DEBUG   //��gps_config.h�ļ���������꣬�Ƿ����������Ϣ

    uint16_t i;
    debug_printf("\r\nError: ");
    for(i=0;i<str_size;i++)
      debug_printf("%c",*(str+i));
    debug_printf("\n");
    #endif
}

/**
  * @brief  error �ڽ������ʱ�����ʾ��Ϣ
  * @param  str: Ҫ������ַ�����str_size:���ݳ���
  * @retval ��
  */
void gps_info(const char *str, int str_size)
{
  #ifdef __GPS_DEBUG
    uint16_t i;
    debug_printf("\r\nInfo: ");
    for(i=0;i<str_size;i++)
      debug_printf("%c",*(str+i));
    debug_printf("\n");
  #endif
}



/******************************************************************************************************** 
**     ��������:            bit        IsLeapYear(uint8_t    iYear) 
**    ��������:            �ж�����(�������2000�Ժ�����) 
**    ��ڲ�����            iYear    ��λ���� 
**    ���ڲ���:            uint8_t        1:Ϊ����    0:Ϊƽ�� 
********************************************************************************************************/ 
static uint8_t IsLeapYear(uint8_t iYear) 
{ 
    uint16_t    Year; 
    Year    =    2000+iYear; 
    if((Year&3)==0) 
    { 
        return ((Year%400==0) || (Year%100!=0)); 
    } 
     return 0; 
} 

/******************************************************************************************************** 
**     ��������:            void    GMTconvert(uint8_t *DT,uint8_t GMT,uint8_t AREA) 
**    ��������:            ��������ʱ�任�������ʱ��ʱ�� 
**    ��ڲ�����            *DT:    ��ʾ����ʱ������� ��ʽ YY,MM,DD,HH,MM,SS 
**                        GMT:    ʱ���� 
**                        AREA:    1(+)���� W0(-)���� 
********************************************************************************************************/ 
void    GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) 
{ 
    uint32_t    YY,MM,DD,hh,mm,ss;        //������ʱ�����ݴ���� 
     
    if(GMT==0)    return;                //�������0ʱ��ֱ�ӷ��� 
    if(GMT>12)    return;                //ʱ�����Ϊ12 �����򷵻�         

    YY    =    SourceTime->year;                //��ȡ�� 
    MM    =    SourceTime->mon;                 //��ȡ�� 
    DD    =    SourceTime->day;                 //��ȡ�� 
    hh    =    SourceTime->hour;                //��ȡʱ 
    mm    =    SourceTime->min;                 //��ȡ�� 
    ss    =    SourceTime->sec;                 //��ȡ�� 

    if(AREA)                        //��(+)ʱ������ 
    { 
        if(hh+GMT<24)    hh    +=    GMT;//������������ʱ�䴦��ͬһ�������Сʱ���� 
        else                        //����Ѿ����ڸ�������ʱ��1����������ڴ��� 
        { 
            hh    =    hh+GMT-24;        //�ȵó�ʱ�� 
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    //���·�(12�µ�������) 
            { 
                if(DD<31)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==4 || MM==6 || MM==9 || MM==11)                //С�·�2�µ�������) 
            { 
                if(DD<30)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==2)    //����2�·� 
            { 
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))        //��������������2��29�� ���߲�����������2��28�� 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
                else    DD++; 
            } 
            else if(MM==12)    //����12�·� 
            { 
                if(DD<31)    DD++; 
                else        //�������һ�� 
                {               
                    DD    =    1; 
                    MM    =    1; 
                    YY    ++; 
                } 
            } 
        } 
    } 
    else 
    {     
        if(hh>=GMT)    hh    -=    GMT;    //������������ʱ�䴦��ͬһ�������Сʱ���� 
        else                        //����Ѿ����ڸ�������ʱ��1����������ڴ��� 
        { 
            hh    =    hh+24-GMT;        //�ȵó�ʱ�� 
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    //�����Ǵ��·�(1�µ�������) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    31; 
                    MM    --; 
                } 
            } 
            else if(MM==5 || MM==7 || MM==10 || MM==12)                //������С�·�2�µ�������) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    30; 
                    MM    --; 
                } 
            } 
            else if(MM==3)    //�����ϸ�����2�·� 
            { 
                if((DD==1) && IsLeapYear(YY)==0)                    //�������� 
                { 
                    DD    =    28; 
                    MM    --; 
                } 
                else    DD--; 
            } 
            else if(MM==1)    //����1�·� 
            { 
                if(DD>1)    DD--; 
                else        //�����һ�� 
                {               
                    DD    =    31; 
                    MM    =    12; 
                    YY    --; 
                } 
            } 
        } 
    }         

    ConvertTime->year   =    YY;                //������ 
    ConvertTime->mon    =    MM;                //������ 
    ConvertTime->day    =    DD;                //������ 
    ConvertTime->hour   =    hh;                //����ʱ 
    ConvertTime->min    =    mm;                //���·� 
    ConvertTime->sec    =    ss;                //������ 
}  






