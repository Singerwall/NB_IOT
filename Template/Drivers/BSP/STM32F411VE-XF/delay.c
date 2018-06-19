/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file  delay.c
  * @author  qiuyongzheng
  * @version  V1.0.0
  * @date  08/05/2016
  * @brief  ϵͳ��ʱ�����ӿ�
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

#ifdef 	OS_CRITICAL_METHOD                              /*OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII*/				
#define delay_osrunning		OSRunning		/*OS�Ƿ����б��,0,������;1,������*/
#define delay_ostickspersec	OS_TICKS_PER_SEC	/*OSʱ�ӽ���,��ÿ����ȴ���*/
#define delay_osintnesting 	OSIntNesting		/*�ж�Ƕ�׼���,���ж�Ƕ�״���*/
#endif

/*֧��UCOSIII*/
#ifdef 	CPU_CFG_CRITICAL_METHOD				/*CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII*/	
#define delay_osrunning		OSRunning		/*OS�Ƿ����б��,0,������;1,������*/
#define delay_ostickspersec	OSCfg_TickRate_Hz	/*OSʱ�ӽ���,��ÿ����ȴ���*/
#define delay_osintnesting 	OSIntNestingCtr		/*�ж�Ƕ�׼���,���ж�Ƕ�״���*/
#endif


/*֧��lite_os*/
#ifdef 	LOS_STM32F411RE				                        /*CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��lite_os*/	
#define delay_osrunning		OSRunning		                /*OS�Ƿ����б��,0,������;1,������*/
#define delay_ostickspersec	LOSCFG_BASE_CORE_TICK_PER_SECOND	/*OSʱ�ӽ���,��ÿ����ȴ���*/
#define delay_osintnesting 	g_vuwIntCount		                /*�ж�Ƕ�׼���,���ж�Ƕ�״���*/
#endif

void OSSchedLock(void)
{}

void OSSchedUnlock(void)
{

}

		
/**
  * @brief  us����ʱʱ�ر��������
  * @details ��ֹ���us���ӳ�
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
  * @brief  us����ʱʱ,�ָ��������
  * @details ��ֹ���us���ӳ�
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
  * @brief  ����OS�Դ�����ʱ������ʱ
  * @details  ������ʱ
  * @param  ticks:��ʱ�Ľ�����
 */
static void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
    OS_ERR err; 
    OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);/*UCOSIII��ʱ��������ģʽ*/
#else
    OSTimeDly(ticks);		/*UCOSII��ʱ*/
#endif 
}
 
#endif



#if SYSTEM_SUPPORT_OS 						
 		
/**
  * @brief  ��ʱnus
  * @details ��ʱ����
  * @param  nus:Ҫ��ʱ��us��.0~204522252(���ֵ��2^32/fac_us@fac_us=21)	 	
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
  * @brief  ��ʱnms
  * @details ��ʱ����
  * @param  nms:Ҫ��ʱ��ms����0~65535��
 */
void delay_ms(u16 nms)
{	
    if(delay_osrunning&&delay_osintnesting==0)/*���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)*/	    
    {		 
        if(nms>=fac_ms)						
        { 
            delay_ostimedly(nms/fac_ms);	
        }
        nms%=fac_ms;						
    }
    delay_us((u32)(nms*1000));				
}
#else  /*����osʱ*/
/**
  * @brief  ��ʱnus
  * @details ��ʱ����
  * @param  nus:Ҫ��ʱ��us��.0~204522252(���ֵ��2^32/fac_us@fac_us=21)	 	
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
  * @brief  ��ʱnms
  * @details ��100M������,nms<=480ms 
  * @param  nms:Ҫ��ʱ��ms����0~65535��
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
  * @brief  ��ʱnms
  * @details ��ʱ����
  * @param  nms:Ҫ��ʱ��ms����0~65535��
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
//ʹ��Ӳ����ʱ			 
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


































