/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  delay.c
  * @author  qiuyongzheng
  * @version  V1.0.0
  * @date  08/05/2016
  * @brief  系统延时函数接口
  */

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include <includes.h>

#if SYSTEM_SUPPORT_OS
#include "includes.h"			
#endif

extern UINT32  g_vuwIntCount ;
extern UINT32 OSRunning ;	
static u8  fac_us=100;	//100M   
static u16 fac_ms=1000/1000;
	
#if SYSTEM_SUPPORT_OS						

#ifdef 	OS_CRITICAL_METHOD                              /*OS_CRITICAL_METHOD定义了,说明要支持UCOSII*/				
#define delay_osrunning		OSRunning		/*OS是否运行标记,0,不运行;1,在运行*/
#define delay_ostickspersec	OS_TICKS_PER_SEC	/*OS时钟节拍,即每秒调度次数*/
#define delay_osintnesting 	OSIntNesting		/*中断嵌套级别,即中断嵌套次数*/
#endif

/*支持UCOSIII*/
#ifdef 	CPU_CFG_CRITICAL_METHOD				/*CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII*/	
#define delay_osrunning		OSRunning		/*OS是否运行标记,0,不运行;1,在运行*/
#define delay_ostickspersec	OSCfg_TickRate_Hz	/*OS时钟节拍,即每秒调度次数*/
#define delay_osintnesting 	OSIntNestingCtr		/*中断嵌套级别,即中断嵌套次数*/
#endif


/*支持lite_os*/
#ifdef 	LOS_STM32F411RE				                        /*CPU_CFG_CRITICAL_METHOD定义了,说明要支持lite_os*/	
#define delay_osrunning		OSRunning		                /*OS是否运行标记,0,不运行;1,在运行*/
#define delay_ostickspersec	LOSCFG_BASE_CORE_TICK_PER_SECOND	/*OS时钟节拍,即每秒调度次数*/
#define delay_osintnesting 	g_vuwIntCount		                /*中断嵌套级别,即中断嵌套次数*/
#endif

void OSSchedLock(void)
{}

void OSSchedUnlock(void)
{

}

		
/**
  * @brief  us级延时时关闭任务调度
  * @details 防止打断us级延迟
 */
static void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			
    OS_ERR err; 
    OSSchedLock(&err);	
#elifdef  LOS_STM32F411RE
    LOS_TaskLock();
#else										
    OSSchedLock();							
#endif
}

/**
  * @brief  us级延时时,恢复任务调度
  * @details 防止打断us级延迟
 */
static void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   			
    OS_ERR err; 
    OSSchedUnlock(&err);
#elifdef  LOS_STM32F411RE
    LOS_TaskUnlock();
#else										
    OSSchedUnlock();						
#endif
}

/**
  * @brief  调用OS自带的延时函数延时
  * @details  函数延时
  * @param  ticks:延时的节拍数
 */
static void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
    OS_ERR err; 
    OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);/*UCOSIII延时采用周期模式*/
#else
    OSTimeDly(ticks);		/*UCOSII延时*/
#endif 
}
 
#endif



#if SYSTEM_SUPPORT_OS 						
 		
/**
  * @brief  延时nus
  * @details 延时函数
  * @param  nus:要延时的us数.0~204522252(最大值即2^32/fac_us@fac_us=21)	 	
 */
void delay_us(u32 nus)
{		
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD;				   	 
    ticks=nus*fac_us; 						 
    delay_osschedlock();					
    told=SysTick->VAL;        				
    while(1)
    {
        tnow=SysTick->VAL;	
        if(tnow!=told)
        {	    
            if(tnow<told)tcnt+=told-tnow;	
            else tcnt+=reload-tnow+told;	    
            told=tnow;
            if(tcnt>=ticks)break;			
        }  
    };
    delay_osschedunlock();															    
}  

/**
  * @brief  延时nms
  * @details 延时函数
  * @param  nms:要延时的ms数（0~65535）
 */
void delay_ms(u16 nms)
{	
    if(delay_osrunning&&delay_osintnesting==0)/*如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)*/	    
    {		 
        if(nms>=fac_ms)						
        { 
            delay_ostimedly(nms/fac_ms);	
        }
        nms%=fac_ms;						
    }
    delay_us((u32)(nms*1000));				
}
#else  /*不用os时*/
/**
  * @brief  延时nus
  * @details 延时函数
  * @param  nus:要延时的us数.0~204522252(最大值即2^32/fac_us@fac_us=21)	 	
 */
void delay_us(u32 nus)
{		
    u32 temp;	    	 
    SysTick->LOAD=nus*fac_us; 					  		 
    SysTick->VAL=0x00;        				
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  	 
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));	   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 
    SysTick->VAL =0X00;       				 
}
/**
  * @brief  延时nms
  * @details 对100M条件下,nms<=480ms 
  * @param  nms:要延时的ms数（0~65535）
 */
void delay_xms(u16 nms)
{	 		  	  
    u32 temp;		   
    SysTick->LOAD=(u32)nms*fac_ms;			
    SysTick->VAL =0x00;           			
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));	
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;     
    SysTick->VAL =0X00;     		  		    
} 
/**
  * @brief  延时nms
  * @details 延时函数
  * @param  nms:要延时的ms数（0~65535）
 */
void delay_ms(u16 nms)
{	 	 
    u8 repeat=nms/540;						
                                                                                    
    u16 remain=nms%540;
    while(repeat)
    {
        delay_xms(540);
        repeat--;
    }
    if(remain)delay_xms(remain);
} 
#endif
//使用硬件延时			 
void delay_xms(u16 nms)
{	 		  	  
    u32 temp;		   
    SysTick->LOAD=(u32)nms*fac_ms;			
    SysTick->VAL =0x00;           			
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));	
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;     
    SysTick->VAL =0X00;     		  		    
} 


































