/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		RF2P4G_Driver.c
  * @author		qyz
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		RF2P4G_Driver驱动函数。
  */ 


/**
  * @包含头文件
  */

#include "RF2P4G_Driver.h"

/**
  * @变量定义
  */


static _SIM_CARD_ SIM_RX_DATA ;                /* 接收数据缓冲区 */
static _SIM_CARD_ SIM_TX_DATA ;                /* 发送数据缓冲区 */


OS_EVENT *SIMIRQSem;				    /* SIM刷卡中断邮箱 */

static u8 SIM_ICID_BAK_LAST[8];

static void usart2_init(void);
static void sim_resive_pasv(void);
static void sim_send_pasv(void);
static s8 SimAuthenticKey(u8 section);
static s8 SimSelectAid(void);
static s8 SimOpenChannel(u8 channel);
static s8 SimShutChannel(u8 channel);
static s8 SearchSimCard (u8* bak_id);
static s8 sim_read_block(u8 block,u8* data);
static s8 sim_write_block(u8 block,u8* data);

/**
  * @brief		2P4G初始化函数
  * @details	
  * @param 	NULL 无
  * @retval 	NULL 无
  */
void rf2p4g_init(void)
{
    usart2_init();
    SIMIRQSem = OSSemCreate(0);
}


/**
  * @brief		串口2初始化函数
  * @details	
  * @param 	NULL 无
  * @retval 	NULL 无
  */
static void usart2_init(void)
{
    GPIO_InitTypeDef gpio;
    USART_InitTypeDef usart;
    USART_ClockInitTypeDef usart_clock;
	NVIC_InitTypeDef nvic;
		
    /* 使能PB，AFIO总线 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO ,ENABLE);	
    /* 使能串口2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
    /* A2 做USART2_Tx */
    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);

    /* A3 做USART2_Rx */ 
    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);
	
    usart.USART_BaudRate = 115200;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
         
    usart_clock.USART_Clock = USART_Clock_Disable;
    usart_clock.USART_CPOL = USART_CPOL_Low;
    usart_clock.USART_CPHA = USART_CPHA_2Edge;
    usart_clock.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART2, &usart_clock);
    USART_Init(USART2, &usart);
    
    USART_Cmd(USART2, ENABLE);

    OFF_SIM_RX_DATA;/* 串口2关闭接收中断 */
	OFF_SIM_TX_DATA;

	/* UART2 IRQ */
	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 3;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}


/**
  * @brief		串口2数据接收中断函数	   
  * @details	这个是通用的函数
  * @param 	NULL	无
  * @retval 	NULL	无
  */

void BSP_IntHandlerUSART2(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
    OSIntNesting++;
    OS_EXIT_CRITICAL();
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		sim_resive_pasv();
	}	
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);	
		USART_ReceiveData(USART2);			
	}
    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {   
		sim_send_pasv();
    }
    OSIntExit();
}  

/**
  * @brief		串口数据接收函数
  * @details	
  * @param 	NULL	无
  * @retval 	NULL	无
  */
static void sim_resive_pasv(void)
{
	u8 ResiveData;
	u8 checksum = 0;
	ResiveData = USART_ReceiveData(USART2);
	if(0 == SIM_RX_DATA.SIM_INDEX_STR)		/* 判断是否收到帧头 */
	{                                                                   
		if(SIM_SOF_DATA == ResiveData)			/* 收到帧头的处理 */
		{
			SIM_RX_DATA.SIM_INDEX_STR = 1;
			SIM_RX_DATA.SIM_NUM_STR   = 1;
			SIM_RX_DATA.SIM_SOP       = SIM_SOF_DATA; 
		}
	}
	else										/* 收到帧头以后的处理 */ 
	{
		/* 还没有收到数据位的处理 */
		if(SIM_RX_DATA.SIM_NUM_STR < 4)
		{
			if(SIM_RX_DATA.SIM_NUM_STR == 1)	/* 收到CMD */
			{                                                                   
				SIM_RX_DATA.SIM_CMD = ResiveData;                                  
			}                                                                   
			else															/* 收到数据的长度 */                                                  
			{                                                                   
				if(SIM_RX_DATA.SIM_NUM_STR == 2)							/* 收到数据长度的高位 */
				{                                                               
					SIM_RX_DATA.SIM_DATA_LEN = (ResiveData << 8)&0XFF00;           
				}                                                               
				else if(SIM_RX_DATA.SIM_NUM_STR == 3)						/* 收到数据长度的低位 */
				{
					SIM_RX_DATA.SIM_DATA_LEN += ResiveData;				 
				}
			}
			SIM_RX_DATA.SIM_NUM_STR ++;
		}
		else																/* 收到数据位后的处理 */
		{
			if((SIM_RX_DATA.SIM_NUM_STR-4) == (SIM_RX_DATA.SIM_DATA_LEN+1))	/* 达到数据个数--进行结束符匹配 */
			{
				if(ResiveData == SIM_END_DATA)								/* 接收一帧完成 */
				{
					SIM_RX_DATA.SIM_END = ResiveData;                       /* 存储结束符---用于判断 */
					/* 校验位比较 */
					checksum = SIM_RX_DATA.SIM_CMD ^ ((SIM_RX_DATA.SIM_DATA_LEN)&0x00ff) ^(((SIM_RX_DATA.SIM_DATA_LEN)>>8)&0x00ff);
					for(ResiveData=0;ResiveData<(SIM_RX_DATA.SIM_DATA_LEN);ResiveData++)
					{
						checksum ^= SIM_RX_DATA.SIM_BUF[ResiveData];
					}
					if(checksum == SIM_RX_DATA.SIM_BUF[SIM_RX_DATA.SIM_DATA_LEN])/* 成功接收一帧 ---校验和通过 */
					{
						OFF_SIM_RX_DATA;		                                /* 关闭串口接收中断 */
						OSSemPost(SIMIRQSem);	                                /* 发送收到一帧数据信号量 */
					}
					else														/* 接收到的数据帧有问题 */
					{
						SIM_RX_DATA.SIM_END = 0;		                        /* 清除结束符 */
					}									
				}
				else															/* 接收到的数据帧有问题 */
				{
					SIM_RX_DATA.SIM_END = 0;		                            /* 清除结束符 */
				}                                                                
				SIM_RX_DATA.SIM_INDEX_STR = 0;                               	/* 清除接收到帧头的标志 */
				SIM_RX_DATA.SIM_NUM_STR   = 0;                               	/* 清除数据的长度 */		                        
			}                                                                   
			else																/* 没有达到数据个数 */                                               
			{                                                                   
				SIM_RX_DATA.SIM_BUF[SIM_RX_DATA.SIM_NUM_STR - 4]  = ResiveData;/* 存储数据 */
				SIM_RX_DATA.SIM_NUM_STR ++;
			}
		}	
	}
}


/**
  * @brief		串口数据发送函数
  * @details	
  * @param 	NULL	无
  * @retval 	NULL	无
  */
static void sim_send_pasv(void)
{
	u8 checksum = 0;
	u8 i;
	if(0 != SIM_TX_DATA.SIM_NUM_STR)											/* 数据没有发送完 */
	{                                                                               
		if(SIM_TX_DATA.SIM_NUM_STR > (SIM_TX_DATA.SIM_DATA_LEN+2))				/* 发送非数据段 */
		{                                                                           
			if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 5)		/* 发送SOP */
			{                                                                       
				USART_SendData(USART2, SIM_SOF_DATA);                                  
			}                                                                       
			else if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 4)/* 发送CMD */
			{                                                                       
				USART_SendData(USART2, SIM_TX_DATA.SIM_CMD);                           
			}                                                                       
			else if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 3)/* 发送高数据位 */
			{                                                                       
				USART_SendData(USART2, (SIM_TX_DATA.SIM_DATA_LEN>>8)&0X00FF);          
			}                                                                       
			else if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 2)/* 发送低数据位 */
			{
				USART_SendData(USART2, SIM_TX_DATA.SIM_DATA_LEN&0X00FF);
				SIM_TX_DATA.SIM_DATA_LEN += 1;
			}
		}
		else/* 发送数据段 */
		{
			if(SIM_TX_DATA.SIM_NUM_STR == 1)							/* 发送结束符 */
			{
				USART_SendData(USART2, SIM_END_DATA);
				OFF_SIM_TX_DATA;										/* 关闭串口发送中断 */
			}
			else if(SIM_TX_DATA.SIM_NUM_STR == 2)						/* 发送校验和 */
			{
				checksum = 0;
				checksum = SIM_TX_DATA.SIM_CMD ^ ((SIM_TX_DATA.SIM_DATA_LEN-1)&0x00ff) ^(((SIM_TX_DATA.SIM_DATA_LEN-1)>>8)&0x00ff);
				for(i=0;i<(SIM_TX_DATA.SIM_DATA_LEN-1);i++)
				{
					checksum ^= SIM_TX_DATA.SIM_BUF[i];
				}
				USART_SendData(USART2, checksum);
			}
			else/* 发送数据 */
			{
				USART_SendData(USART2, SIM_TX_DATA.SIM_BUF[(SIM_TX_DATA.SIM_DATA_LEN+1)-SIM_TX_DATA.SIM_NUM_STR]);
			}
		}
		SIM_TX_DATA.SIM_NUM_STR --;
	}
	else/* 数据发送完成 */
	{
		SIM_TX_DATA.SIM_NUM_STR = 0;
		OFF_SIM_TX_DATA;			                                                 /* 关闭串口发送中断 */
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}


/**
  * @brief		关闭逻辑通道0-1-2-3
  * @details	4关闭所有通道--否则1-2-3对应其通道
  * @param 	channel	通道
  * @retval 	0：成功；1：失败
  */
static s8 SimShutChannel(u8 channel)
{
	#define SIMSCCMD 0x20
	u8 i,j;
	u8 err;
	if(channel == 4)
	{
		i = 1;
		j = 4;
	}
	else 
	{
		i = channel;
		j = i+1;
	}
	/* 5A 20 00 05 00 70 80 01 00 00 CA（斜体部分可以为01、02、03）*/
	for(;i<j;i++)
	{
		SIM_TX_DATA.SIM_CMD 	    = SIMSCCMD;/* CMD */
		SIM_TX_DATA.SIM_DATA_LEN = 0x05;/* LEN */
		/* 40 08 00 01 06 FF FF FF FF FF FF */
		SIM_TX_DATA.SIM_BUF[0] 	= 0X00;	  
		SIM_TX_DATA.SIM_BUF[1] 	= 0X70;	  
		SIM_TX_DATA.SIM_BUF[2] 	= 0X80;	  
		SIM_TX_DATA.SIM_BUF[3] 	= i;/* 逻辑通道1-2-3 */
		SIM_TX_DATA.SIM_BUF[4] 	= 0X00;	  
		SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
		ON_SIM_TX_DATA;/* 打开串口发送中 */
		sim_send_pasv();
		while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
		/* 等待认证ACK */
		ON_SIM_RX_DATA;/* 打开串口接收中断 */
		OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);/* 等待收取一帧信号量 */ 
		if(OS_ERR_NONE == err)/* 成功等到信号量 */
		{                                                             	
			if(SIM_RX_DATA.SIM_CMD == SIMSCCMD)/* 本次发送的命令回复 */
			{
				if((SIM_RX_DATA.SIM_BUF[0] == 0x6a && SIM_RX_DATA.SIM_BUF[1] == 0x86)||SIM_RX_DATA.SIM_BUF[0] ==0x90);													//	操作成功--啥也不做
				else/* 关闭通道操作失败 */
					return SIMRC_SHUTCHANNEL_FAIL;                        	
			}                                                         	
			else/* 回复命令错误 */
				return SIMRC_CMD_FAIL ;
		}
		else 
			return SIMRC_TIME_OUT;					   					 					   				
	}
	return  SIMRC_OK;
}

/**
  * @brief		打开逻辑通道1-2-3
  * @details	
  * @param 	channel	通道
  * @retval 	0：成功；1：失败
  */
static s8 SimOpenChannel(u8 channel)
{
	#define SIMOCCMD 0x20
	u8 err;
	
	/* 打开逻辑通道1 */
	/* 5A 20 00 05 00 70 00 00 01 CA */
	SIM_TX_DATA.SIM_CMD 	    = SIMOCCMD;/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x05;/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_BUF[0] 	= 0X00;	  
	SIM_TX_DATA.SIM_BUF[1] 	= 0X70;	  
	SIM_TX_DATA.SIM_BUF[2] 	= 0X00;	  
	SIM_TX_DATA.SIM_BUF[3] 	= 0x00;	  
	SIM_TX_DATA.SIM_BUF[4] 	= channel;/* 逻辑通道1-2-3 */  
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;/* 打开串口发送中 */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* 等待认证ACK */
	ON_SIM_RX_DATA;/* 打开串口接收中断--忙 */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);/* 等待收取一帧信号量 */ 
	if(OS_ERR_NONE == err)/* 成功等到信号量 */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMOCCMD)/* 本次发送的命令回复 */
		{
			/* 校验返回码--和匹配要打开的通道 */
			if(SIM_RX_DATA.SIM_BUF[0] == channel && SIM_RX_DATA.SIM_BUF[1] == 0x90 && SIM_RX_DATA.SIM_BUF[2] == 0x00 && SIM_RX_DATA.SIM_BUF[3] == 0x00)
				return SIMRC_OK;
			else/* 打开通道失败 */
				return SIMRC_OPENCHANNEL_FAIL;
		}
		else/* 回复命令错误 */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}

/**
  * @brief		AID选择---应用命令选择
  * @details	
  * @param 	NULL	无
  * @retval 	0：成功；1：失败
  */
static s8 SimSelectAid(void)
{
	#define SIMSAID 0x20
	u8 err;

	/* AID选择 */
	/* 5A 20 00 0F 91 A4 04F0 0B 4D 69 66 61 72 65 20 41 70 70 31 CA */			    
	SIM_TX_DATA.SIM_CMD 	    = SIMSAID;	  	/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x10;	  		/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */ 		  	
	SIM_TX_DATA.SIM_BUF[0] 	= 0X91;	  		/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0Xa4;	  		/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= 0X04;	  		/* P1 */
	SIM_TX_DATA.SIM_BUF[3] 	= 0Xf0;	  		/* P2 */     
	SIM_TX_DATA.SIM_BUF[4] 	= 0X0B;	  		/* LC	 终端发出数据的实际长度。XX 不能大于0x10 */ 
	SIM_TX_DATA.SIM_BUF[5] 	= 0X4D;   		/* DATA  AID */
	SIM_TX_DATA.SIM_BUF[6] 	= 0X69;
	SIM_TX_DATA.SIM_BUF[7] 	= 0X66;
	SIM_TX_DATA.SIM_BUF[8] 	= 0X61;
	SIM_TX_DATA.SIM_BUF[9] 	= 0X72;
	SIM_TX_DATA.SIM_BUF[10] = 0X65;
	SIM_TX_DATA.SIM_BUF[11]	= 0X20;
	SIM_TX_DATA.SIM_BUF[12]	= 0X41;
	SIM_TX_DATA.SIM_BUF[13]	= 0X70;
	SIM_TX_DATA.SIM_BUF[14]	= 0X70;
	SIM_TX_DATA.SIM_BUF[15]	= 0X31;
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;							/* 打开串口发送中 */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* 等待认证ACK */
	ON_SIM_RX_DATA;/* 打开串口接收中断--忙 */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);               	/* 等待收取一帧信号量 */ 
	if(OS_ERR_NONE == err)										/* 成功等到信号量 */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMSAID) 						/* 本次发送的命令回复 */
		{
			/* 校验返回码 */
			if(SIM_RX_DATA.SIM_BUF[0] == 0x90 && SIM_RX_DATA.SIM_BUF[1] == 0x00 && SIM_RX_DATA.SIM_BUF[2] == 0x00)
				return SIMRC_OK;
			else  													/* 选择AID失败 */
				return SIMRC_SELECTAID_FAIL;
		}
		else 														/* 回复命令错误 */
			return SIMRC_CMD_FAIL ;
	}
	else return  SIMRC_TIME_OUT;
}

/**
  * @brief		认证命令 --选择 A key
  * @details	扇区号为0~15--本实验选0足够
  * @param 	section	扇区号
  * @retval 	0：成功；1：失败
  */
static s8 SimAuthenticKey(u8 section)
{
	#define SIMAKEYCMD 0X20
	u8 err;

	/* 认证命令 --选择 A key */
	/* 5A 20 00 0B   41 08 00 00     06    FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_CMD 	    = SIMAKEYCMD;	  /* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x0b;	  /* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_BUF[0] 	= 0X41;	  			/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0X08;	  			/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= section;			/* P1扇区号 */
	SIM_TX_DATA.SIM_BUF[3] 	= 0X00;	  			/* P2    00：B, 01：A */
	SIM_TX_DATA.SIM_BUF[4] 	= 0X06;	  			/* LC	  00：使用POS 内部密钥认证(RFU)   06：使用指令所含密钥认证 */
	SIM_TX_DATA.SIM_BUF[5] 	= 0Xff;   			/* DATA  Lc 为6 时，填密钥A 或B 的值 */
	SIM_TX_DATA.SIM_BUF[6] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[7] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[8] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[9] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[10]	= 0Xff;
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;								/* 打开串口发送中 */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);			   
	/* 等待认证ACK */
	ON_SIM_RX_DATA;/* 打开串口接收中断--忙 */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);   /* 等待收取一帧信号量 */ 
	if(OS_ERR_NONE == err)					/* 成功等到信号量 */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMAKEYCMD) 	/* 本次发送的命令回复 */
		{
			/* 校验返回码 */
			if(SIM_RX_DATA.SIM_BUF[0] == 0x90 && SIM_RX_DATA.SIM_BUF[1] == 0x00 && SIM_RX_DATA.SIM_BUF[2] == 0x00)
				return SIMRC_OK;
			else  								/* 认真KEY失败 */
				return SIMRC_KEY_FAIL;
		}
		else 									/* 回复命令错误 */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}

/**
  * @brief		寻卡--获取卡ID
  * @details	
  * @param bak_id	卡ID
  * @retval 	0：成功；1：失败
  */
static u8 SIM_SearchCardCt = 0;  											/* 寻卡超时计次 */
static s8 SearchSimCard (u8* bak_id)
{
	#define SIMGIDCMD 0x10
	u8 err;

	/* 被动寻卡 */
	/* 5A 10 00 01 0A 1B CA */
	/* 寻到卡片读卡器返回：5A 10 00 09 11 22 33 44 55 66 77 88 00 91 CA */
	/* 没有寻到卡片读卡器返回：5A 10 00 01 05 14 CA */
	SIM_TX_DATA.SIM_CMD = SIMGIDCMD;
	SIM_TX_DATA.SIM_DATA_LEN = 1;
	SIM_TX_DATA.SIM_BUF[0] = SEARCHTIME;/* POS 终端发送打开读卡器寻卡命令，读卡器收到命令后开始寻卡，并在DATA*10ms 时 */
										/* 间内返回结果，若DATA 值为0x00，表示读卡器须一直寻卡，此状态一直保持到有卡号上 */
										/* 报为止 */
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;/* 打开串口发送中 */
	sim_send_pasv();                          			  	
	while(SIM_TX_DATA.SIM_NUM_STR != 0);/* 等待数据发送完成 */
	/* 等待认证ACK */                            			  	
	ON_SIM_RX_DATA;/* 打开串口接收中断--忙 */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);/* 等待收取一帧信号量 SIM_TRIG_ONTIME */	 
	if(OS_ERR_NONE == err)/* 成功等到信号量 */
	{ 	
		SIM_SearchCardCt=0;
		if(SIM_RX_DATA.SIM_CMD == SIMGIDCMD)/* 本次发送的命令回复 */
		{
			if(SIM_RX_DATA.SIM_DATA_LEN == 9)
			{ 
				return  SIMRC_OK;						 
			}	
			else/* 这里可以认为SIM卡已经拿开 */
			{
				if(SIM_SearchCardCt>5)
				{
					SIM_SearchCardCt=0;
					memset(bak_id,0,8);/* 清除最后一次ID存储 */
				}
				else 
					SIM_SearchCardCt++;
				return SIMRC_GETUID_FAIL;/* 获取ID失败--也就是寻卡失败 */
			}
		}
		else/* 回复命令错误 */
			return SIMRC_CMD_FAIL ;
	}
	else 
	{
		return  SIMRC_TIME_OUT;
	}
}




/**
  * @brief		读数据块
  * @details	0~2 数据区，3---KEY区
  * @param 	block	扇区号
  * @param 	data	数据
  * @retval 	0：成功；1：失败
  */
static s8 sim_read_block(u8 block,u8* data)
{
	#define SIMRBLCCMD 0X20
	u8 err;

	/* 读数据块-0 */
	/* 5A 20 00 05    4102 0100 10  					read block */
	SIM_TX_DATA.SIM_CMD 	    = SIMRBLCCMD;	  	/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x05;	  			/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */ 			
	SIM_TX_DATA.SIM_BUF[0] 	= 0X41;	  			/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0X02;	  			/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= 0X01;	  			/* P1 */
	SIM_TX_DATA.SIM_BUF[3] 	= block;  			/* P2 块号（已认证扇区内相对块号，非总块号） */
	SIM_TX_DATA.SIM_BUF[4] 	= 0X10;	  			/* LE  响应数据期望的最大长度 */
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;								/* 打开串口发送中 */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* 等待认证ACK */
	SIM_RX_DATA.SIM_BUF[16] = 0;
	ON_SIM_RX_DATA;/* 打开串口接收中断--忙 */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);               	/* 等待收取一帧信号量 */ 
	if(OS_ERR_NONE == err)									/* 成功等到信号量 */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMRBLCCMD) 					/* 本次发送的命令回复 */
		{
			/* 校验返回码 */
			if(SIM_RX_DATA.SIM_BUF[16] == 0x90 && SIM_RX_DATA.SIM_BUF[17] == 0x00 && SIM_RX_DATA.SIM_BUF[18] == 0x00)
			{
				memcpy(data,SIM_RX_DATA.SIM_BUF,16);			/* 复制数据 */
				return SIMRC_OK;
			}
			else  												/* 读数据块失败 */
				return SIMRC_READDATA_FAIL;
		}
		else 													/* 回复命令错误 */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}


/**
  * @brief		写数据块
  * @details	0~2 数据区，3---KEY区
  * @param 	block	扇区号
  * @param 	data	数据
  * @retval 	0：成功；1：失败
  */
static s8 sim_write_block(u8 block,u8* data)
{
	#define SIMWBLCCMD 0X20
	u8 err;

	/* 写数据块 */
	/* 5A 20 00 15  41 02 02 00 10 00001122 FFFFEEDD 00001122 00FF00FF CA */
	/* 			  41 02 02 00 10 00001122 FFFFEEDD 00001122 00FF00FF  write block */
	SIM_TX_DATA.SIM_CMD 	    = SIMWBLCCMD;	  	/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x15;	  			/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_BUF[0] 	= 0X41;	  			/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0X02;	  			/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= 0X02;	  			/* P1 读写模式  1 读  2写 */
	SIM_TX_DATA.SIM_BUF[3] 	= block;  			/* P2 块号（已认证扇区内相对块号，非总块号） */
	SIM_TX_DATA.SIM_BUF[4] 	= 0X10;	  			/* LE  响应数据期望的最大长度 */
	memcpy(&SIM_TX_DATA.SIM_BUF[5],data,16);
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;								/* 打开串口发送中 */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* 等待认证ACK */
	SIM_RX_DATA.SIM_BUF[0] = 0;
	ON_SIM_RX_DATA;								/* 打开串口接收中断 */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);   /* 等待收取一帧信号量 */ 
	if(OS_ERR_NONE == err)					/* 成功等到信号量 */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMWBLCCMD)  /* 本次发送的命令回复 */
		{										/* 校验返回码 */
			if(SIM_RX_DATA.SIM_BUF[0] == 0x90 && SIM_RX_DATA.SIM_BUF[1] == 0x00 && SIM_RX_DATA.SIM_BUF[2] == 0x00)
				return SIMRC_OK;
			else  								/* 写数据块失败 */
				return SIMRC_WRITEDATA_FAIL;
		}
		else 									/* 回复命令错误 */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}
/**
  * @brief		寻卡
  * @details	0~2 数据区，3---KEY区
  * @param 	card_uid	UID号
  * @retval 	0：成功；1：失败
  */
u8 Sim_InListPassiveTarget(u8 *card_uid)
{
    u8 statu_flag = 0;
    if(SearchSimCard(SIM_ICID_BAK_LAST) == SIMRC_OK)
    {
        memcpy(SIM_ICID_BAK_LAST,SIM_RX_DATA.SIM_BUF,8);	/* 把与本次ID不同的卡进行存储--防止重复操作*/
		memcpy(card_uid,SIM_RX_DATA.SIM_BUF,8);				/* 将ID放入发送缓存*/
    }
    else 
	    statu_flag++;
    return statu_flag;
}
/**
  * @brief		读数据块
  * @details	0~2 数据区，3---KEY区
  * @param 	block	扇区号
  * @param 	data	数据
  * @retval 	0：成功；1：失败
  */
u8 Sim_read_press(u8 SimBlock_File,u8 *SimBlock_data)
{
    u8 statu_flag = 0;
    if(SearchSimCard(SIM_ICID_BAK_LAST) == SIMRC_OK)
    {
        memcpy(SIM_ICID_BAK_LAST,SIM_RX_DATA.SIM_BUF,8);	/* 把与本次ID不同的卡进行存储--防止重复操作*/
        if(SimShutChannel(4) == SIMRC_OK)	   							/* 关闭所有通道1-2-3 */
		{	                                                          	
			if(SimOpenChannel(SimBlock_File) == SIMRC_OK) 			/* 打开逻辑通道SimBlock_File */
			{                                                         	
				if(SimSelectAid() == SIMRC_OK)							/* AID选择---应用命令选择 */
				{                                                     	
					if(SimAuthenticKey(SimBlock_File ) == SIMRC_OK)  /* 认证命令 --选择 A key  ----并选择扇区为SimBlock_File */
                    {
                        if(sim_read_block(SimBlock_File,SimBlock_data) == SIMRC_OK); /* 读卡 */
                        else 
						    statu_flag++;
                    }
					else 
						statu_flag++;
				}
				else 
					statu_flag++;
			}
			else 
				statu_flag++;
		}
		else 
			statu_flag++;        
    }
    else 
	    statu_flag++;
    return statu_flag;
}
/**
  * @brief		写数据块
  * @details	0~2 数据区，3---KEY区
  * @param 	block	扇区号
  * @param 	data	数据
  * @retval 	0：成功；1：失败
  */
u8 Sim_write_press(u8 SimBlock_File,u8 *SimBlock_data)
{
    u8 statu_flag = 0;
    if(SearchSimCard(SIM_ICID_BAK_LAST) == SIMRC_OK)
    {
        memcpy(SIM_ICID_BAK_LAST,SIM_RX_DATA.SIM_BUF,8);	/* 把与本次ID不同的卡进行存储--防止重复操作*/
        if(SimShutChannel(4) == SIMRC_OK)	   							/* 关闭所有通道1-2-3 */
		{	                                                          	
			if(SimOpenChannel(SimBlock_File) == SIMRC_OK) 			/* 打开逻辑通道SimBlock_File */
			{                                                         	
				if(SimSelectAid() == SIMRC_OK)							/* AID选择---应用命令选择 */
				{                                                     	
					if(SimAuthenticKey(SimBlock_File ) == SIMRC_OK)  /* 认证命令 --选择 A key  ----并选择扇区为SimBlock_File */
                    {
                        if(sim_write_block(SimBlock_File,SimBlock_data) == SIMRC_OK); /* 写卡 */
                        else 
						    statu_flag++;
                    }
					else 
						statu_flag++;
				}
				else 
					statu_flag++;
			}
			else 
				statu_flag++;
		}
		else 
			statu_flag++;        
    }
    else 
	    statu_flag++;
    return statu_flag;
}