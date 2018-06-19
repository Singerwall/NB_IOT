/** <h2><center>&copy; COPYRIGHT 2013 XunFang </center></h2>
  * @file  app_usmart.c
  * @author  qyz
  * @version  V1.0.0
  * @date  09/05/2016
  * @brief  串口调试任务
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

/*系统命令*/
u8 *sys_cmd_tab[]=
{
	"help",         /*0 获取帮助信息*/
	"list",         /*1 查询可用的函数列表*/
	"id",           /*2 可用函数的ID列表*/
	"runtime",	    /*3 开启函数运行计时,1;关闭函数运行计时,0*/
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
    debug_printf("XF-USMART是物联网商用智能网关串口调试交互组件，用户可以通过此\r\n");
    debug_printf("组件对网关进行相应的调试及参数设置.具体命令如下:\r\n\r\n");
    debug_printf("输入“help”可以获取获取帮助信息.\r\n");
    debug_printf("输入“list”可以查看可使用的API接口列表.\r\n");
    debug_printf("输入“id”  可以查看可使用的API的内存地址.\r\n");
    debug_printf("输入“runtime x”,可以开启或关闭函数运行计时功能，x=1/0(开启/关闭)\r\n");
    debug_printf("输入对应的API函数，并设置好参数，可以直接运行相应的API--->例如:\r\n");
    debug_printf("\r\n输入“reboot_system()”则可以重启网关！\r\n\r\n");
    debug_printf("#############################################################\r\n");
    debug_printf("##                      www.xunfang.com                    ##\r\n");
    debug_printf("#############################################################\r\n");
}


/**
  * @brief usmart_sys_cmd_exe
  * @details 处理系统指令
  * @param  str 接收到的指令
  * @retval 0,成功处理;其他,错误代码;
 */
u8 usmart_sys_cmd_exe(u8 *str)
{
	u8 i;
	u8 sfname[MAX_FNAME_LEN];/*存放本地函数名*/
	u8 pnum;
	u8 rval;
	u32 res;  
	res=usmart_get_cmdname(str,sfname,&i,MAX_FNAME_LEN);/*得到指令及指令长度*/
	if(res)return USMART_FUNCERR;/*错误的指令*/
	str+=i;	 	 			    
	for(i=0;i<sizeof(sys_cmd_tab)/4;i++)/*支持的系统指令*/
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i])==0)break;
	}
	switch(i)
	{					   
		case 0:/*帮助指令*/
			debug_printf("\r\n");
#if USMART_USE_HELP
            print_help();
#else
			debug_printf("指令失效\r\n");
#endif
			break;
		case 1:/*查询指令*/
			debug_printf("\r\n");
			debug_printf("############## 函数清单 ##############\r\n");
			for(i=0;i<usmart_dev.fnum;i++)debug_printf("%s\r\n",usmart_dev.funs[i].name);
			debug_printf("\r\n");
			break;	 
		case 2:/*查询ID*/
			debug_printf("\r\n");
			debug_printf("############## 函数 ID  ##############\r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&pnum,&rval);/*得到本地函数名*/ 
				debug_printf("%s id is:\r\n0X%08X\r\n",sfname,usmart_dev.funs[i].func);/*显示ID*/
			}
			debug_printf("\r\n");
			break;	 
		case 3:/*runtime指令,设置是否显示函数执行时间*/
			debug_printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)/*参数正常*/
			{
				i=usmart_str2num(sfname,&res);	   		/*记录该参数*/	
				if(i==0)						   		/*读取指定地址数据功能*/
				{
                    usmart_dev.runtimeflag=res;
                    if(usmart_dev.runtimeflag)debug_printf("Run Time Calculation ON\r\n");
                    else debug_printf("Run Time Calculation OFF\r\n"); 
				}else return USMART_PARMERR;   			/*未带参数,或者参数错误*/	 
 			}else return USMART_PARMERR;				/*参数错误*/ 
			debug_printf("\r\n"); 
			break;	    
		default:/*非法指令*/
			return USMART_FUNCERR;
	}
	return 0;
}


/**
  * @brief usmart_init
  * @details 初始化串口控制器
 */
void usmart_init(void)
{
	usmart_dev.sptype=0;
}		

/**
  * @brief usmart_cmd_rec
  * @details 从str中获取函数名,id,及参数信息
  * @param  *str 接收到的指令
  * @retval 0,识别成功;其他,错误代码.
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
		sta=usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&spnum,&rval);/*得到本地函数名及参数个数*/
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
	usmart_dev.pnum=i;			/*参数个数记录*/
    return USMART_OK;
}

/**
  * @brief usmart_exe
  * @details usamrt执行函数 该函数用于最终执行从串口收到的有效函数
 */
void usmart_exe(void)
{
    u8 *p;
	u8 id,i;
	u32 res;		   
	u32 temp[MAX_PARM];/*参数转换,使之支持了字符串*/ 
	u8 sfname[MAX_FNAME_LEN];/*存放本地函数名*/
	u8 pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)return;
	usmart_get_fname((u8*)usmart_dev.funs[id].name,sfname,&pnum,&rval);/*得到本地函数名,及参数个数*/ 
	debug_printf("\r\n%s(",sfname);
	for(i=0;i<pnum;i++)/*输出参数*/
	{
		if(usmart_dev.parmtype&(1<<i))/*参数是字符串*/
		{
			debug_printf("%c",'"');			 
			debug_printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			debug_printf("%c",'"');
			temp[i]=(u32)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  /*参数是数字*/
		{
			temp[i]=*(u32*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)debug_printf("%lu",temp[i]);/*10进制参数显示*/
			else debug_printf("0X%X",temp[i]);/*16进制参数显示*/ 	   
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
	if(rval==1)/*需要返回值.*/
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)debug_printf("=%lu;\r\n",res);
		else debug_printf("=0X%X;\r\n",res);  
            /*如果使用了wifi配置模式，则透传到wifi*/
        if(get_igw_work_state() && get_config_connect_flag())
        {
            p=mymalloc(SRAMIN,32);
            sprintf((char*)p,"%d",res);
            esp_8266_cipsend(0,strlen((char*)p),p);
            myfree(SRAMIN,p);
        }
	}else debug_printf(";\r\n");
	if(usmart_dev.runtimeflag)	/*需要显示函数执行时间*/
	{ 
		debug_printf("Function Run Time:%d%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);
	}	
}

/**
  * @brief usmart_scan
  * @details usmart扫描函数
 */
void usmart_scan(void)
{
	u8 sta,len;  
    u8 temp_buff[uart_rx_buffer_size] = {0};

	if(get_uart_rx_sta()&0x8000)/*串口接收完成*/
	{					   
		len=get_uart_rx_sta()&0x3fff;	/*得到此次接收到的数据长度*/
        memcpy(temp_buff,get_uart_rx_buffer(),len);        
		temp_buff[len]='\0';	/*在末尾加入结束符.*/ 
		sta=usmart_dev.cmd_rec(temp_buff);/*得到函数各个信息*/
		if(sta==0)usmart_dev.exe();	/*执行函数*/ 
		else 
		{  
			len=usmart_sys_cmd_exe(temp_buff);
			if(len!=USMART_FUNCERR)sta=len;
			if(sta)
			{
				switch(sta)
				{
					case USMART_FUNCERR:
						debug_printf("函数错误!\r\n");   			
						break;	
					case USMART_PARMERR:
						debug_printf("参数错误!\r\n");   			
						break;				
					case USMART_PARMOVER:
						debug_printf("参数太多!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						debug_printf("未找到匹配的函数!\r\n");   			
						break;		
				}
			}
		}
		set_uart_rx_sta(0);/*状态寄存器清空	*/    
	}
}

#if USMART_USE_WRFUNS==1 	/*如果使能了读写操作*/
	 
u32 read_addr(u32 addr)
{
	return *(u32*)addr;
}
		 
void write_addr(u32 addr,u32 val)
{
	*(u32*)addr=val; 	
}
#endif













