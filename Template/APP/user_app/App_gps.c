/** @copyright XunFang Communication Tech Limited. All rights reserved. 2018.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  04/11/2018
  * @brief  GPS解析业务
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
  * @brief 创建 NB模块和oceanconnect平台通信任务
  * @retval 无
 */
uint32_t create_Task_GPS(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_GPS_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*创建任务:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_GPS;
    stInitParam1.pcName = "Task_GPS";
    stInitParam1.usTaskPrio = Prio_Task_GPS;
    stInitParam1.uwStackSize =StackSize_Task_GPS; 
  
    LOS_TaskLock();//锁住任务，防止新创建的任务比本任务高而发生调度
    uwRet = LOS_TaskCreate(&Task_GPS_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_GPS failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//解锁任务，只有队列创建后才开始任务调度
    
    return LOS_OK;
}

/**
  * @brief 
  * @details 
  * @param  pdata 无用
  * @retval 无
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
    double deg_lat;/*转换成[degree].[degree]格式的纬度*/
    double deg_lon;/*转换成[degree].[degree]格式的经度*/
    
    nmeaINFO info; /*GPS解码后得到的信息*/     
    nmeaPARSER parser;      /*解码时使用的数据结构  */
//    uint8_t new_parse=0;    /*是否有新的解码数据标志*/
    
    nmeaTIME beiJingTime;    /*北京时间*/ 
    
    /* 设置用于输出调试信息的函数 */
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;
    nmea_property()->info_func = &gps_info;
    
    /* 初始化GPS数据结构 */
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
    
    
    /*创建队列*/
    uwRet = LOS_QueueCreate("GPS_queue", 2, &gps_uwQueue, 0, 40);
    if(uwRet != LOS_OK) debug_print_string("create GPS_queue failure!,error:%x\n",uwRet);
    else debug_print_string("create the GPS_queue success!\n");
    debug_printf("[%s] enter.\r\n", __func__);
    
    for(;;)
    {  
        uwRet = LOS_QueueRead(gps_uwQueue, &gps_struct_p, 10, 10);
        if(uwRet != LOS_OK)/*未接受到gps数据*/
        {    
         // debug_printf("\n\r get gps_uwQueue message failure,error:%x\n\r",uwRet);
        }
        else/*接收到控制命令*/
        {
          LOS_TaskLock();
         // debug_printf("\n\r get gps_uwQueue message ok\n\r"); 
          
         nmea_parse(&parser, (const char*)((gps_struct_t  * )gps_struct_p)->data, 
                                         ((gps_struct_t  * )gps_struct_p)->size, &info);
          /* 对解码后的时间进行转换，转换成北京时间 */
          GMTconvert(&info.utc,&beiJingTime,8,1);
         
          deg_lat = nmea_ndeg2degree(info.lat);
          deg_lon = nmea_ndeg2degree(info.lon);
          LOS_TaskUnlock();          
          delay_ms(10);

#if 0
          /* 输出解码得到的信息 */
          debug_printf("\r\n时间%d-%02d-%02d,%d:%d:%d", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec); 
          debug_printf("\r\n纬度：%f,经度%f",deg_lat,deg_lon);
          debug_printf("\r\n海拔高度：%f 米 ", info.elv);
          debug_printf("\r\n速度：%f km/h ", info.speed);
          debug_printf("\r\n航向：%f 度", info.direction);			
          debug_printf("\r\n正在使用的GPS卫星：%d,可见GPS卫星：%d",info.satinfo.inuse,info.satinfo.inview);   
          for(u8 i=0;i<info.satinfo.inview;i++)
          {
             debug_printf("\r\n可见GPS卫星编号：%d,可见GPS卫星信号强度：%d ", info.satinfo.sat[i].id,info.satinfo.sat[i].sig);
          }
          debug_printf("\r\n正在使用的北斗卫星：%d,可见北斗卫星：%d",info.BDsatinfo.inuse,info.BDsatinfo.inview);
          for(u8 i=0;i<info.BDsatinfo.inview;i++)
          {
             debug_printf("\r\n可见BD卫星编号：%d,可见BD卫星信号强度：%d ", info.BDsatinfo.sat[i].id,info.BDsatinfo.sat[i].sig);
          }
          debug_printf("\r\nPDOP：%f,HDOP：%f，VDOP：%f",info.PDOP,info.HDOP,info.VDOP);
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
  * @brief  trace 在解码时输出捕获的GPS语句
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void trace(const char *str, int str_size)
{
  #ifdef __GPS_DEBUG    //在gps_config.h文件配置这个宏，是否输出调试信息
    uint16_t i;
    debug_printf("\r\nTrace: ");
    for(i=0;i<str_size;i++)
      debug_printf("%c",*(str+i));
  
    debug_printf("\n");
  #endif
}

/**
  * @brief  error 在解码出错时输出提示消息
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void error(const char *str, int str_size)
{
    #ifdef __GPS_DEBUG   //在gps_config.h文件配置这个宏，是否输出调试信息

    uint16_t i;
    debug_printf("\r\nError: ");
    for(i=0;i<str_size;i++)
      debug_printf("%c",*(str+i));
    debug_printf("\n");
    #endif
}

/**
  * @brief  error 在解码出错时输出提示消息
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
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
**     函数名称:            bit        IsLeapYear(uint8_t    iYear) 
**    功能描述:            判断闰年(仅针对于2000以后的年份) 
**    入口参数：            iYear    两位年数 
**    出口参数:            uint8_t        1:为闰年    0:为平年 
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
**     函数名称:            void    GMTconvert(uint8_t *DT,uint8_t GMT,uint8_t AREA) 
**    功能描述:            格林尼治时间换算世界各时区时间 
**    入口参数：            *DT:    表示日期时间的数组 格式 YY,MM,DD,HH,MM,SS 
**                        GMT:    时区数 
**                        AREA:    1(+)东区 W0(-)西区 
********************************************************************************************************/ 
void    GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) 
{ 
    uint32_t    YY,MM,DD,hh,mm,ss;        //年月日时分秒暂存变量 
     
    if(GMT==0)    return;                //如果处于0时区直接返回 
    if(GMT>12)    return;                //时区最大为12 超过则返回         

    YY    =    SourceTime->year;                //获取年 
    MM    =    SourceTime->mon;                 //获取月 
    DD    =    SourceTime->day;                 //获取日 
    hh    =    SourceTime->hour;                //获取时 
    mm    =    SourceTime->min;                 //获取分 
    ss    =    SourceTime->sec;                 //获取秒 

    if(AREA)                        //东(+)时区处理 
    { 
        if(hh+GMT<24)    hh    +=    GMT;//如果与格林尼治时间处于同一天则仅加小时即可 
        else                        //如果已经晚于格林尼治时间1天则进行日期处理 
        { 
            hh    =    hh+GMT-24;        //先得出时间 
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    //大月份(12月单独处理) 
            { 
                if(DD<31)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==4 || MM==6 || MM==9 || MM==11)                //小月份2月单独处理) 
            { 
                if(DD<30)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==2)    //处理2月份 
            { 
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))        //本来是闰年且是2月29日 或者不是闰年且是2月28日 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
                else    DD++; 
            } 
            else if(MM==12)    //处理12月份 
            { 
                if(DD<31)    DD++; 
                else        //跨年最后一天 
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
        if(hh>=GMT)    hh    -=    GMT;    //如果与格林尼治时间处于同一天则仅减小时即可 
        else                        //如果已经早于格林尼治时间1天则进行日期处理 
        { 
            hh    =    hh+24-GMT;        //先得出时间 
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    //上月是大月份(1月单独处理) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    31; 
                    MM    --; 
                } 
            } 
            else if(MM==5 || MM==7 || MM==10 || MM==12)                //上月是小月份2月单独处理) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    30; 
                    MM    --; 
                } 
            } 
            else if(MM==3)    //处理上个月是2月份 
            { 
                if((DD==1) && IsLeapYear(YY)==0)                    //不是闰年 
                { 
                    DD    =    28; 
                    MM    --; 
                } 
                else    DD--; 
            } 
            else if(MM==1)    //处理1月份 
            { 
                if(DD>1)    DD--; 
                else        //新年第一天 
                {               
                    DD    =    31; 
                    MM    =    12; 
                    YY    --; 
                } 
            } 
        } 
    }         

    ConvertTime->year   =    YY;                //更新年 
    ConvertTime->mon    =    MM;                //更新月 
    ConvertTime->day    =    DD;                //更新日 
    ConvertTime->hour   =    hh;                //更新时 
    ConvertTime->min    =    mm;                //更新分 
    ConvertTime->sec    =    ss;                //更新秒 
}  






