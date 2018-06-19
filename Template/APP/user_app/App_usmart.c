/** <h2><center>&copy; COPYRIGHT 2013 XunFang </center></h2>
  * @file  app_usmart.c
  * @author  qyz
  * @version  V1.0.0
  * @date  09/05/2016
  * @brief  ���ڵ�������
  */
#define App_BSP_int_MODULE
#include "includes.h"
#include "App_usmart.h"
#include "App_debug.h"
#include "usart_debug_driver.h"
#include "esp8266_driver.h"
#include "sys.h" 
#include "delay.h"
#include "malloc.h"
#include "App_BSP_int.h"
#include "App_netif.h"

/*ϵͳ����*/
u8 *sys_cmd_tab[]=
{
	"help",         /*0 ��ȡ������Ϣ*/
	"list",         /*1 ��ѯ���õĺ����б�*/
	"id",           /*2 ���ú�����ID�б�*/
	"runtime",	    /*3 �����������м�ʱ,1;�رպ������м�ʱ,0*/
};	 

static void task_usmart(void *pdata);

void create_usmart_task(void)
{
    OSTaskDel(Prio_Task_usmart);
    
    OSTaskCreateExt(task_usmart,                              
                    (void *)0,                             
                    &Stack_Task_usmart[StackSize_Task_usmart - 1],   
                    (INT8U)(Prio_Task_usmart),                  
                    (INT16U)(Prio_Task_usmart),                    
                    &Stack_Task_usmart[0],                       
                    (INT32U)StackSize_Task_usmart,             
                    (void *)0,                                         
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}

static void task_usmart(void *pdata)
{
    usmart_dev.init();	
    for(;;)
    {
        feed_watchdog();
        if(get_igw_work_state())
            usmart_dev.scan();
        delay_ms(20);
    }
}
static void print_help(void)
{
    debug_printf("#############################################################\r\n");
    debug_printf("##                       XF-IGW-V1.00                      ##\r\n");    
    debug_printf("#############################################################\r\n\r\n");    
    debug_printf("XF-USMART�������������������ش��ڵ��Խ���������û�����ͨ����\r\n");
    debug_printf("��������ؽ�����Ӧ�ĵ��Լ���������.������������:\r\n\r\n");
    debug_printf("���롰help�����Ի�ȡ��ȡ������Ϣ.\r\n");
    debug_printf("���롰list�����Բ鿴��ʹ�õ�API�ӿ��б�.\r\n");
    debug_printf("���롰id��  ���Բ鿴��ʹ�õ�API���ڴ��ַ.\r\n");
    debug_printf("���롰runtime x��,���Կ�����رպ������м�ʱ���ܣ�x=1/0(����/�ر�)\r\n");
    debug_printf("�����Ӧ��API�����������úò���������ֱ��������Ӧ��API--->����:\r\n");
    debug_printf("\r\n���롰reboot_system()��������������أ�\r\n\r\n");
    debug_printf("#############################################################\r\n");
    debug_printf("##                      www.xunfang.com                    ##\r\n");
    debug_printf("#############################################################\r\n");
}


/**
  * @brief usmart_sys_cmd_exe
  * @details ����ϵͳָ��
  * @param  str ���յ���ָ��
  * @retval 0,�ɹ�����;����,�������;
 */
u8 usmart_sys_cmd_exe(u8 *str)
{
	u8 i;
	u8 sfname[MAX_FNAME_LEN];/*��ű��غ�����*/
	u8 pnum;
	u8 rval;
	u32 res;  
	res=usmart_get_cmdname(str,sfname,&i,MAX_FNAME_LEN);/*�õ�ָ�ָ���*/
	if(res)return USMART_FUNCERR;/*�����ָ��*/
	str+=i;	 	 			    
	for(i=0;i<sizeof(sys_cmd_tab)/4;i++)/*֧�ֵ�ϵͳָ��*/
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i])==0)break;
	}
	switch(i)
	{					   
		case 0:/*����ָ��*/
			debug_printf("\r\n");
#if USMART_USE_HELP
            print_help();
#else
			debug_printf("ָ��ʧЧ\r\n");
#endif
			break;
		case 1:/*��ѯָ��*/
			debug_printf("\r\n");
			debug_printf("############## �����嵥 ##############\r\n");
			for(i=0;i<usmart_dev.fnum;i++)debug_printf("%s\r\n",usmart_dev.funs[i].name);
			debug_printf("\r\n");
			break;	 
		case 2:/*��ѯID*/
			debug_printf("\r\n");
			debug_printf("############## ���� ID  ##############\r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&pnum,&rval);/*�õ����غ�����*/ 
				debug_printf("%s id is:\r\n0X%08X\r\n",sfname,usmart_dev.funs[i].func);/*��ʾID*/
			}
			debug_printf("\r\n");
			break;	 
		case 3:/*runtimeָ��,�����Ƿ���ʾ����ִ��ʱ��*/
			debug_printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)/*��������*/
			{
				i=usmart_str2num(sfname,&res);	   		/*��¼�ò���*/	
				if(i==0)						   		/*��ȡָ����ַ���ݹ���*/
				{
                    usmart_dev.runtimeflag=res;
                    if(usmart_dev.runtimeflag)debug_printf("Run Time Calculation ON\r\n");
                    else debug_printf("Run Time Calculation OFF\r\n"); 
				}else return USMART_PARMERR;   			/*δ������,���߲�������*/	 
 			}else return USMART_PARMERR;				/*��������*/ 
			debug_printf("\r\n"); 
			break;	    
		default:/*�Ƿ�ָ��*/
			return USMART_FUNCERR;
	}
	return 0;
}


/**
  * @brief usmart_init
  * @details ��ʼ�����ڿ�����
 */
void usmart_init(void)
{
	usmart_dev.sptype=0;
}		

/**
  * @brief usmart_cmd_rec
  * @details ��str�л�ȡ������,id,��������Ϣ
  * @param  *str ���յ���ָ��
  * @retval 0,ʶ��ɹ�;����,�������.
 */
u8 usmart_cmd_rec(u8*str) 
{
	u8 sta,i,rval; 
	u8 rpnum,spnum;
	u8 rfname[MAX_FNAME_LEN];
	u8 sfname[MAX_FNAME_LEN];
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);  
	if(sta)return sta;
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&spnum,&rval);/*�õ����غ���������������*/
		if(sta)return sta; 
		if(usmart_strcmp(sfname,rfname)==0)
		{
			if(spnum>rpnum)return USMART_PARMERR;
			usmart_dev.id=i;
			break;
		}	
	}
	if(i==usmart_dev.fnum)return USMART_NOFUNCFIND;	
 	sta=usmart_get_fparam(str,&i);					
	if(sta)return sta;								
	usmart_dev.pnum=i;			/*����������¼*/
    return USMART_OK;
}

/**
  * @brief usmart_exe
  * @details usamrtִ�к��� �ú�����������ִ�дӴ����յ�����Ч����
 */
void usmart_exe(void)
{
    u8 *p;
	u8 id,i;
	u32 res;		   
	u32 temp[MAX_PARM];/*����ת��,ʹ֧֮�����ַ���*/ 
	u8 sfname[MAX_FNAME_LEN];/*��ű��غ�����*/
	u8 pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)return;
	usmart_get_fname((u8*)usmart_dev.funs[id].name,sfname,&pnum,&rval);/*�õ����غ�����,����������*/ 
	debug_printf("\r\n%s(",sfname);
	for(i=0;i<pnum;i++)/*�������*/
	{
		if(usmart_dev.parmtype&(1<<i))/*�������ַ���*/
		{
			debug_printf("%c",'"');			 
			debug_printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			debug_printf("%c",'"');
			temp[i]=(u32)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  /*����������*/
		{
			temp[i]=*(u32*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)debug_printf("%lu",temp[i]);/*10���Ʋ�����ʾ*/
			else debug_printf("0X%X",temp[i]);/*16���Ʋ�����ʾ*/ 	   
		}
		if(i!=pnum-1)debug_printf(",");
	}
	debug_printf(")");
	usmart_dev.runtime = OSTimeGet();
	switch(usmart_dev.pnum)
	{
		case 0:									  
			res=(*(u32(*)())usmart_dev.funs[id].func)();
			break;
	    case 1:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
	    case 2:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1]);
			break;
	    case 3:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
	usmart_dev.runtime = OSTimeGet() - usmart_dev.runtime;
	if(rval==1)/*��Ҫ����ֵ.*/
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)debug_printf("=%lu;\r\n",res);
		else debug_printf("=0X%X;\r\n",res);  
            /*���ʹ����wifi����ģʽ����͸����wifi*/
        if(get_igw_work_state() && get_config_connect_flag())
        {
            p=mymalloc(SRAMIN,32);
            sprintf((char*)p,"%d",res);
            esp_8266_cipsend(0,strlen((char*)p),p);
            myfree(SRAMIN,p);
        }
	}else debug_printf(";\r\n");
	if(usmart_dev.runtimeflag)	/*��Ҫ��ʾ����ִ��ʱ��*/
	{ 
		debug_printf("Function Run Time:%d%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);
	}	
}

/**
  * @brief usmart_scan
  * @details usmartɨ�躯��
 */
void usmart_scan(void)
{
	u8 sta,len;  
    u8 temp_buff[uart_rx_buffer_size] = {0};

	if(get_uart_rx_sta()&0x8000)/*���ڽ������*/
	{					   
		len=get_uart_rx_sta()&0x3fff;	/*�õ��˴ν��յ������ݳ���*/
        memcpy(temp_buff,get_uart_rx_buffer(),len);        
		temp_buff[len]='\0';	/*��ĩβ���������.*/ 
		sta=usmart_dev.cmd_rec(temp_buff);/*�õ�����������Ϣ*/
		if(sta==0)usmart_dev.exe();	/*ִ�к���*/ 
		else 
		{  
			len=usmart_sys_cmd_exe(temp_buff);
			if(len!=USMART_FUNCERR)sta=len;
			if(sta)
			{
				switch(sta)
				{
					case USMART_FUNCERR:
						debug_printf("��������!\r\n");   			
						break;	
					case USMART_PARMERR:
						debug_printf("��������!\r\n");   			
						break;				
					case USMART_PARMOVER:
						debug_printf("����̫��!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						debug_printf("δ�ҵ�ƥ��ĺ���!\r\n");   			
						break;		
				}
			}
		}
		set_uart_rx_sta(0);/*״̬�Ĵ������	*/    
	}
}

#if USMART_USE_WRFUNS==1 	/*���ʹ���˶�д����*/
	 
u32 read_addr(u32 addr)
{
	return *(u32*)addr;
}
		 
void write_addr(u32 addr,u32 val)
{
	*(u32*)addr=val; 	
}
#endif













