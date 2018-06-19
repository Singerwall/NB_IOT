/** Copyright(c) 2015, XunFang Tech Limited. All rights reserved.
  * @file		RF2P4G_Driver.c
  * @author		qyz
  * @version	V1.0.0
  * @date		07/21/2015
  * @brief		RF2P4G_Driver����������
  */ 


/**
  * @����ͷ�ļ�
  */

#include "RF2P4G_Driver.h"

/**
  * @��������
  */


static _SIM_CARD_ SIM_RX_DATA ;                /* �������ݻ����� */
static _SIM_CARD_ SIM_TX_DATA ;                /* �������ݻ����� */


OS_EVENT *SIMIRQSem;				    /* SIMˢ���ж����� */

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
  * @brief		2P4G��ʼ������
  * @details	
  * @param 	NULL ��
  * @retval 	NULL ��
  */
void rf2p4g_init(void)
{
    usart2_init();
    SIMIRQSem = OSSemCreate(0);
}


/**
  * @brief		����2��ʼ������
  * @details	
  * @param 	NULL ��
  * @retval 	NULL ��
  */
static void usart2_init(void)
{
    GPIO_InitTypeDef gpio;
    USART_InitTypeDef usart;
    USART_ClockInitTypeDef usart_clock;
	NVIC_InitTypeDef nvic;
		
    /* ʹ��PB��AFIO���� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO ,ENABLE);	
    /* ʹ�ܴ���2ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
    /* A2 ��USART2_Tx */
    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);

    /* A3 ��USART2_Rx */ 
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

    OFF_SIM_RX_DATA;/* ����2�رս����ж� */
	OFF_SIM_TX_DATA;

	/* UART2 IRQ */
	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 3;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}


/**
  * @brief		����2���ݽ����жϺ���	   
  * @details	�����ͨ�õĺ���
  * @param 	NULL	��
  * @retval 	NULL	��
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
  * @brief		�������ݽ��պ���
  * @details	
  * @param 	NULL	��
  * @retval 	NULL	��
  */
static void sim_resive_pasv(void)
{
	u8 ResiveData;
	u8 checksum = 0;
	ResiveData = USART_ReceiveData(USART2);
	if(0 == SIM_RX_DATA.SIM_INDEX_STR)		/* �ж��Ƿ��յ�֡ͷ */
	{                                                                   
		if(SIM_SOF_DATA == ResiveData)			/* �յ�֡ͷ�Ĵ��� */
		{
			SIM_RX_DATA.SIM_INDEX_STR = 1;
			SIM_RX_DATA.SIM_NUM_STR   = 1;
			SIM_RX_DATA.SIM_SOP       = SIM_SOF_DATA; 
		}
	}
	else										/* �յ�֡ͷ�Ժ�Ĵ��� */ 
	{
		/* ��û���յ�����λ�Ĵ��� */
		if(SIM_RX_DATA.SIM_NUM_STR < 4)
		{
			if(SIM_RX_DATA.SIM_NUM_STR == 1)	/* �յ�CMD */
			{                                                                   
				SIM_RX_DATA.SIM_CMD = ResiveData;                                  
			}                                                                   
			else															/* �յ����ݵĳ��� */                                                  
			{                                                                   
				if(SIM_RX_DATA.SIM_NUM_STR == 2)							/* �յ����ݳ��ȵĸ�λ */
				{                                                               
					SIM_RX_DATA.SIM_DATA_LEN = (ResiveData << 8)&0XFF00;           
				}                                                               
				else if(SIM_RX_DATA.SIM_NUM_STR == 3)						/* �յ����ݳ��ȵĵ�λ */
				{
					SIM_RX_DATA.SIM_DATA_LEN += ResiveData;				 
				}
			}
			SIM_RX_DATA.SIM_NUM_STR ++;
		}
		else																/* �յ�����λ��Ĵ��� */
		{
			if((SIM_RX_DATA.SIM_NUM_STR-4) == (SIM_RX_DATA.SIM_DATA_LEN+1))	/* �ﵽ���ݸ���--���н�����ƥ�� */
			{
				if(ResiveData == SIM_END_DATA)								/* ����һ֡��� */
				{
					SIM_RX_DATA.SIM_END = ResiveData;                       /* �洢������---�����ж� */
					/* У��λ�Ƚ� */
					checksum = SIM_RX_DATA.SIM_CMD ^ ((SIM_RX_DATA.SIM_DATA_LEN)&0x00ff) ^(((SIM_RX_DATA.SIM_DATA_LEN)>>8)&0x00ff);
					for(ResiveData=0;ResiveData<(SIM_RX_DATA.SIM_DATA_LEN);ResiveData++)
					{
						checksum ^= SIM_RX_DATA.SIM_BUF[ResiveData];
					}
					if(checksum == SIM_RX_DATA.SIM_BUF[SIM_RX_DATA.SIM_DATA_LEN])/* �ɹ�����һ֡ ---У���ͨ�� */
					{
						OFF_SIM_RX_DATA;		                                /* �رմ��ڽ����ж� */
						OSSemPost(SIMIRQSem);	                                /* �����յ�һ֡�����ź��� */
					}
					else														/* ���յ�������֡������ */
					{
						SIM_RX_DATA.SIM_END = 0;		                        /* ��������� */
					}									
				}
				else															/* ���յ�������֡������ */
				{
					SIM_RX_DATA.SIM_END = 0;		                            /* ��������� */
				}                                                                
				SIM_RX_DATA.SIM_INDEX_STR = 0;                               	/* ������յ�֡ͷ�ı�־ */
				SIM_RX_DATA.SIM_NUM_STR   = 0;                               	/* ������ݵĳ��� */		                        
			}                                                                   
			else																/* û�дﵽ���ݸ��� */                                               
			{                                                                   
				SIM_RX_DATA.SIM_BUF[SIM_RX_DATA.SIM_NUM_STR - 4]  = ResiveData;/* �洢���� */
				SIM_RX_DATA.SIM_NUM_STR ++;
			}
		}	
	}
}


/**
  * @brief		�������ݷ��ͺ���
  * @details	
  * @param 	NULL	��
  * @retval 	NULL	��
  */
static void sim_send_pasv(void)
{
	u8 checksum = 0;
	u8 i;
	if(0 != SIM_TX_DATA.SIM_NUM_STR)											/* ����û�з����� */
	{                                                                               
		if(SIM_TX_DATA.SIM_NUM_STR > (SIM_TX_DATA.SIM_DATA_LEN+2))				/* ���ͷ����ݶ� */
		{                                                                           
			if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 5)		/* ����SOP */
			{                                                                       
				USART_SendData(USART2, SIM_SOF_DATA);                                  
			}                                                                       
			else if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 4)/* ����CMD */
			{                                                                       
				USART_SendData(USART2, SIM_TX_DATA.SIM_CMD);                           
			}                                                                       
			else if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 3)/* ���͸�����λ */
			{                                                                       
				USART_SendData(USART2, (SIM_TX_DATA.SIM_DATA_LEN>>8)&0X00FF);          
			}                                                                       
			else if((SIM_TX_DATA.SIM_NUM_STR - SIM_TX_DATA.SIM_DATA_LEN-1) == 2)/* ���͵�����λ */
			{
				USART_SendData(USART2, SIM_TX_DATA.SIM_DATA_LEN&0X00FF);
				SIM_TX_DATA.SIM_DATA_LEN += 1;
			}
		}
		else/* �������ݶ� */
		{
			if(SIM_TX_DATA.SIM_NUM_STR == 1)							/* ���ͽ����� */
			{
				USART_SendData(USART2, SIM_END_DATA);
				OFF_SIM_TX_DATA;										/* �رմ��ڷ����ж� */
			}
			else if(SIM_TX_DATA.SIM_NUM_STR == 2)						/* ����У��� */
			{
				checksum = 0;
				checksum = SIM_TX_DATA.SIM_CMD ^ ((SIM_TX_DATA.SIM_DATA_LEN-1)&0x00ff) ^(((SIM_TX_DATA.SIM_DATA_LEN-1)>>8)&0x00ff);
				for(i=0;i<(SIM_TX_DATA.SIM_DATA_LEN-1);i++)
				{
					checksum ^= SIM_TX_DATA.SIM_BUF[i];
				}
				USART_SendData(USART2, checksum);
			}
			else/* �������� */
			{
				USART_SendData(USART2, SIM_TX_DATA.SIM_BUF[(SIM_TX_DATA.SIM_DATA_LEN+1)-SIM_TX_DATA.SIM_NUM_STR]);
			}
		}
		SIM_TX_DATA.SIM_NUM_STR --;
	}
	else/* ���ݷ������ */
	{
		SIM_TX_DATA.SIM_NUM_STR = 0;
		OFF_SIM_TX_DATA;			                                                 /* �رմ��ڷ����ж� */
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}


/**
  * @brief		�ر��߼�ͨ��0-1-2-3
  * @details	4�ر�����ͨ��--����1-2-3��Ӧ��ͨ��
  * @param 	channel	ͨ��
  * @retval 	0���ɹ���1��ʧ��
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
	/* 5A 20 00 05 00 70 80 01 00 00 CA��б�岿�ֿ���Ϊ01��02��03��*/
	for(;i<j;i++)
	{
		SIM_TX_DATA.SIM_CMD 	    = SIMSCCMD;/* CMD */
		SIM_TX_DATA.SIM_DATA_LEN = 0x05;/* LEN */
		/* 40 08 00 01 06 FF FF FF FF FF FF */
		SIM_TX_DATA.SIM_BUF[0] 	= 0X00;	  
		SIM_TX_DATA.SIM_BUF[1] 	= 0X70;	  
		SIM_TX_DATA.SIM_BUF[2] 	= 0X80;	  
		SIM_TX_DATA.SIM_BUF[3] 	= i;/* �߼�ͨ��1-2-3 */
		SIM_TX_DATA.SIM_BUF[4] 	= 0X00;	  
		SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
		ON_SIM_TX_DATA;/* �򿪴��ڷ����� */
		sim_send_pasv();
		while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
		/* �ȴ���֤ACK */
		ON_SIM_RX_DATA;/* �򿪴��ڽ����ж� */
		OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);/* �ȴ���ȡһ֡�ź��� */ 
		if(OS_ERR_NONE == err)/* �ɹ��ȵ��ź��� */
		{                                                             	
			if(SIM_RX_DATA.SIM_CMD == SIMSCCMD)/* ���η��͵�����ظ� */
			{
				if((SIM_RX_DATA.SIM_BUF[0] == 0x6a && SIM_RX_DATA.SIM_BUF[1] == 0x86)||SIM_RX_DATA.SIM_BUF[0] ==0x90);													//	�����ɹ�--ɶҲ����
				else/* �ر�ͨ������ʧ�� */
					return SIMRC_SHUTCHANNEL_FAIL;                        	
			}                                                         	
			else/* �ظ�������� */
				return SIMRC_CMD_FAIL ;
		}
		else 
			return SIMRC_TIME_OUT;					   					 					   				
	}
	return  SIMRC_OK;
}

/**
  * @brief		���߼�ͨ��1-2-3
  * @details	
  * @param 	channel	ͨ��
  * @retval 	0���ɹ���1��ʧ��
  */
static s8 SimOpenChannel(u8 channel)
{
	#define SIMOCCMD 0x20
	u8 err;
	
	/* ���߼�ͨ��1 */
	/* 5A 20 00 05 00 70 00 00 01 CA */
	SIM_TX_DATA.SIM_CMD 	    = SIMOCCMD;/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x05;/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_BUF[0] 	= 0X00;	  
	SIM_TX_DATA.SIM_BUF[1] 	= 0X70;	  
	SIM_TX_DATA.SIM_BUF[2] 	= 0X00;	  
	SIM_TX_DATA.SIM_BUF[3] 	= 0x00;	  
	SIM_TX_DATA.SIM_BUF[4] 	= channel;/* �߼�ͨ��1-2-3 */  
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;/* �򿪴��ڷ����� */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* �ȴ���֤ACK */
	ON_SIM_RX_DATA;/* �򿪴��ڽ����ж�--æ */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);/* �ȴ���ȡһ֡�ź��� */ 
	if(OS_ERR_NONE == err)/* �ɹ��ȵ��ź��� */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMOCCMD)/* ���η��͵�����ظ� */
		{
			/* У�鷵����--��ƥ��Ҫ�򿪵�ͨ�� */
			if(SIM_RX_DATA.SIM_BUF[0] == channel && SIM_RX_DATA.SIM_BUF[1] == 0x90 && SIM_RX_DATA.SIM_BUF[2] == 0x00 && SIM_RX_DATA.SIM_BUF[3] == 0x00)
				return SIMRC_OK;
			else/* ��ͨ��ʧ�� */
				return SIMRC_OPENCHANNEL_FAIL;
		}
		else/* �ظ�������� */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}

/**
  * @brief		AIDѡ��---Ӧ������ѡ��
  * @details	
  * @param 	NULL	��
  * @retval 	0���ɹ���1��ʧ��
  */
static s8 SimSelectAid(void)
{
	#define SIMSAID 0x20
	u8 err;

	/* AIDѡ�� */
	/* 5A 20 00 0F 91 A4 04F0 0B 4D 69 66 61 72 65 20 41 70 70 31 CA */			    
	SIM_TX_DATA.SIM_CMD 	    = SIMSAID;	  	/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x10;	  		/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */ 		  	
	SIM_TX_DATA.SIM_BUF[0] 	= 0X91;	  		/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0Xa4;	  		/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= 0X04;	  		/* P1 */
	SIM_TX_DATA.SIM_BUF[3] 	= 0Xf0;	  		/* P2 */     
	SIM_TX_DATA.SIM_BUF[4] 	= 0X0B;	  		/* LC	 �ն˷������ݵ�ʵ�ʳ��ȡ�XX ���ܴ���0x10 */ 
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
	ON_SIM_TX_DATA;							/* �򿪴��ڷ����� */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* �ȴ���֤ACK */
	ON_SIM_RX_DATA;/* �򿪴��ڽ����ж�--æ */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);               	/* �ȴ���ȡһ֡�ź��� */ 
	if(OS_ERR_NONE == err)										/* �ɹ��ȵ��ź��� */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMSAID) 						/* ���η��͵�����ظ� */
		{
			/* У�鷵���� */
			if(SIM_RX_DATA.SIM_BUF[0] == 0x90 && SIM_RX_DATA.SIM_BUF[1] == 0x00 && SIM_RX_DATA.SIM_BUF[2] == 0x00)
				return SIMRC_OK;
			else  													/* ѡ��AIDʧ�� */
				return SIMRC_SELECTAID_FAIL;
		}
		else 														/* �ظ�������� */
			return SIMRC_CMD_FAIL ;
	}
	else return  SIMRC_TIME_OUT;
}

/**
  * @brief		��֤���� --ѡ�� A key
  * @details	������Ϊ0~15--��ʵ��ѡ0�㹻
  * @param 	section	������
  * @retval 	0���ɹ���1��ʧ��
  */
static s8 SimAuthenticKey(u8 section)
{
	#define SIMAKEYCMD 0X20
	u8 err;

	/* ��֤���� --ѡ�� A key */
	/* 5A 20 00 0B   41 08 00 00     06    FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_CMD 	    = SIMAKEYCMD;	  /* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x0b;	  /* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_BUF[0] 	= 0X41;	  			/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0X08;	  			/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= section;			/* P1������ */
	SIM_TX_DATA.SIM_BUF[3] 	= 0X00;	  			/* P2    00��B, 01��A */
	SIM_TX_DATA.SIM_BUF[4] 	= 0X06;	  			/* LC	  00��ʹ��POS �ڲ���Կ��֤(RFU)   06��ʹ��ָ��������Կ��֤ */
	SIM_TX_DATA.SIM_BUF[5] 	= 0Xff;   			/* DATA  Lc Ϊ6 ʱ������ԿA ��B ��ֵ */
	SIM_TX_DATA.SIM_BUF[6] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[7] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[8] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[9] 	= 0Xff;
	SIM_TX_DATA.SIM_BUF[10]	= 0Xff;
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;								/* �򿪴��ڷ����� */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);			   
	/* �ȴ���֤ACK */
	ON_SIM_RX_DATA;/* �򿪴��ڽ����ж�--æ */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);   /* �ȴ���ȡһ֡�ź��� */ 
	if(OS_ERR_NONE == err)					/* �ɹ��ȵ��ź��� */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMAKEYCMD) 	/* ���η��͵�����ظ� */
		{
			/* У�鷵���� */
			if(SIM_RX_DATA.SIM_BUF[0] == 0x90 && SIM_RX_DATA.SIM_BUF[1] == 0x00 && SIM_RX_DATA.SIM_BUF[2] == 0x00)
				return SIMRC_OK;
			else  								/* ����KEYʧ�� */
				return SIMRC_KEY_FAIL;
		}
		else 									/* �ظ�������� */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}

/**
  * @brief		Ѱ��--��ȡ��ID
  * @details	
  * @param bak_id	��ID
  * @retval 	0���ɹ���1��ʧ��
  */
static u8 SIM_SearchCardCt = 0;  											/* Ѱ����ʱ�ƴ� */
static s8 SearchSimCard (u8* bak_id)
{
	#define SIMGIDCMD 0x10
	u8 err;

	/* ����Ѱ�� */
	/* 5A 10 00 01 0A 1B CA */
	/* Ѱ����Ƭ���������أ�5A 10 00 09 11 22 33 44 55 66 77 88 00 91 CA */
	/* û��Ѱ����Ƭ���������أ�5A 10 00 01 05 14 CA */
	SIM_TX_DATA.SIM_CMD = SIMGIDCMD;
	SIM_TX_DATA.SIM_DATA_LEN = 1;
	SIM_TX_DATA.SIM_BUF[0] = SEARCHTIME;/* POS �ն˷��ʹ򿪶�����Ѱ������������յ������ʼѰ��������DATA*10ms ʱ */
										/* ���ڷ��ؽ������DATA ֵΪ0x00����ʾ��������һֱѰ������״̬һֱ���ֵ��п����� */
										/* ��Ϊֹ */
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;/* �򿪴��ڷ����� */
	sim_send_pasv();                          			  	
	while(SIM_TX_DATA.SIM_NUM_STR != 0);/* �ȴ����ݷ������ */
	/* �ȴ���֤ACK */                            			  	
	ON_SIM_RX_DATA;/* �򿪴��ڽ����ж�--æ */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);/* �ȴ���ȡһ֡�ź��� SIM_TRIG_ONTIME */	 
	if(OS_ERR_NONE == err)/* �ɹ��ȵ��ź��� */
	{ 	
		SIM_SearchCardCt=0;
		if(SIM_RX_DATA.SIM_CMD == SIMGIDCMD)/* ���η��͵�����ظ� */
		{
			if(SIM_RX_DATA.SIM_DATA_LEN == 9)
			{ 
				return  SIMRC_OK;						 
			}	
			else/* ���������ΪSIM���Ѿ��ÿ� */
			{
				if(SIM_SearchCardCt>5)
				{
					SIM_SearchCardCt=0;
					memset(bak_id,0,8);/* ������һ��ID�洢 */
				}
				else 
					SIM_SearchCardCt++;
				return SIMRC_GETUID_FAIL;/* ��ȡIDʧ��--Ҳ����Ѱ��ʧ�� */
			}
		}
		else/* �ظ�������� */
			return SIMRC_CMD_FAIL ;
	}
	else 
	{
		return  SIMRC_TIME_OUT;
	}
}




/**
  * @brief		�����ݿ�
  * @details	0~2 ��������3---KEY��
  * @param 	block	������
  * @param 	data	����
  * @retval 	0���ɹ���1��ʧ��
  */
static s8 sim_read_block(u8 block,u8* data)
{
	#define SIMRBLCCMD 0X20
	u8 err;

	/* �����ݿ�-0 */
	/* 5A 20 00 05    4102 0100 10  					read block */
	SIM_TX_DATA.SIM_CMD 	    = SIMRBLCCMD;	  	/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x05;	  			/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */ 			
	SIM_TX_DATA.SIM_BUF[0] 	= 0X41;	  			/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0X02;	  			/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= 0X01;	  			/* P1 */
	SIM_TX_DATA.SIM_BUF[3] 	= block;  			/* P2 ��ţ�����֤��������Կ�ţ����ܿ�ţ� */
	SIM_TX_DATA.SIM_BUF[4] 	= 0X10;	  			/* LE  ��Ӧ������������󳤶� */
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;								/* �򿪴��ڷ����� */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* �ȴ���֤ACK */
	SIM_RX_DATA.SIM_BUF[16] = 0;
	ON_SIM_RX_DATA;/* �򿪴��ڽ����ж�--æ */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);               	/* �ȴ���ȡһ֡�ź��� */ 
	if(OS_ERR_NONE == err)									/* �ɹ��ȵ��ź��� */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMRBLCCMD) 					/* ���η��͵�����ظ� */
		{
			/* У�鷵���� */
			if(SIM_RX_DATA.SIM_BUF[16] == 0x90 && SIM_RX_DATA.SIM_BUF[17] == 0x00 && SIM_RX_DATA.SIM_BUF[18] == 0x00)
			{
				memcpy(data,SIM_RX_DATA.SIM_BUF,16);			/* �������� */
				return SIMRC_OK;
			}
			else  												/* �����ݿ�ʧ�� */
				return SIMRC_READDATA_FAIL;
		}
		else 													/* �ظ�������� */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}


/**
  * @brief		д���ݿ�
  * @details	0~2 ��������3---KEY��
  * @param 	block	������
  * @param 	data	����
  * @retval 	0���ɹ���1��ʧ��
  */
static s8 sim_write_block(u8 block,u8* data)
{
	#define SIMWBLCCMD 0X20
	u8 err;

	/* д���ݿ� */
	/* 5A 20 00 15  41 02 02 00 10 00001122 FFFFEEDD 00001122 00FF00FF CA */
	/* 			  41 02 02 00 10 00001122 FFFFEEDD 00001122 00FF00FF  write block */
	SIM_TX_DATA.SIM_CMD 	    = SIMWBLCCMD;	  	/* CMD */
	SIM_TX_DATA.SIM_DATA_LEN = 0x15;	  			/* LEN */
	/* 40 08 00 01 06 FF FF FF FF FF FF */
	SIM_TX_DATA.SIM_BUF[0] 	= 0X41;	  			/* CLA */
	SIM_TX_DATA.SIM_BUF[1] 	= 0X02;	  			/* INS */
	SIM_TX_DATA.SIM_BUF[2] 	= 0X02;	  			/* P1 ��дģʽ  1 ��  2д */
	SIM_TX_DATA.SIM_BUF[3] 	= block;  			/* P2 ��ţ�����֤��������Կ�ţ����ܿ�ţ� */
	SIM_TX_DATA.SIM_BUF[4] 	= 0X10;	  			/* LE  ��Ӧ������������󳤶� */
	memcpy(&SIM_TX_DATA.SIM_BUF[5],data,16);
	SIM_TX_DATA.SIM_NUM_STR = SIM_TX_DATA.SIM_DATA_LEN + SIM_READER_MODEL_LEN;				   
	ON_SIM_TX_DATA;								/* �򿪴��ڷ����� */
	sim_send_pasv();
	while(SIM_TX_DATA.SIM_NUM_STR != 0);				   
	/* �ȴ���֤ACK */
	SIM_RX_DATA.SIM_BUF[0] = 0;
	ON_SIM_RX_DATA;								/* �򿪴��ڽ����ж� */
	OSSemPend(SIMIRQSem,SIM_TRIG_ONTIME,&err);   /* �ȴ���ȡһ֡�ź��� */ 
	if(OS_ERR_NONE == err)					/* �ɹ��ȵ��ź��� */
	{
		if(SIM_RX_DATA.SIM_CMD == SIMWBLCCMD)  /* ���η��͵�����ظ� */
		{										/* У�鷵���� */
			if(SIM_RX_DATA.SIM_BUF[0] == 0x90 && SIM_RX_DATA.SIM_BUF[1] == 0x00 && SIM_RX_DATA.SIM_BUF[2] == 0x00)
				return SIMRC_OK;
			else  								/* д���ݿ�ʧ�� */
				return SIMRC_WRITEDATA_FAIL;
		}
		else 									/* �ظ�������� */
			return SIMRC_CMD_FAIL ;
	}
	else 
		return SIMRC_TIME_OUT;
}
/**
  * @brief		Ѱ��
  * @details	0~2 ��������3---KEY��
  * @param 	card_uid	UID��
  * @retval 	0���ɹ���1��ʧ��
  */
u8 Sim_InListPassiveTarget(u8 *card_uid)
{
    u8 statu_flag = 0;
    if(SearchSimCard(SIM_ICID_BAK_LAST) == SIMRC_OK)
    {
        memcpy(SIM_ICID_BAK_LAST,SIM_RX_DATA.SIM_BUF,8);	/* ���뱾��ID��ͬ�Ŀ����д洢--��ֹ�ظ�����*/
		memcpy(card_uid,SIM_RX_DATA.SIM_BUF,8);				/* ��ID���뷢�ͻ���*/
    }
    else 
	    statu_flag++;
    return statu_flag;
}
/**
  * @brief		�����ݿ�
  * @details	0~2 ��������3---KEY��
  * @param 	block	������
  * @param 	data	����
  * @retval 	0���ɹ���1��ʧ��
  */
u8 Sim_read_press(u8 SimBlock_File,u8 *SimBlock_data)
{
    u8 statu_flag = 0;
    if(SearchSimCard(SIM_ICID_BAK_LAST) == SIMRC_OK)
    {
        memcpy(SIM_ICID_BAK_LAST,SIM_RX_DATA.SIM_BUF,8);	/* ���뱾��ID��ͬ�Ŀ����д洢--��ֹ�ظ�����*/
        if(SimShutChannel(4) == SIMRC_OK)	   							/* �ر�����ͨ��1-2-3 */
		{	                                                          	
			if(SimOpenChannel(SimBlock_File) == SIMRC_OK) 			/* ���߼�ͨ��SimBlock_File */
			{                                                         	
				if(SimSelectAid() == SIMRC_OK)							/* AIDѡ��---Ӧ������ѡ�� */
				{                                                     	
					if(SimAuthenticKey(SimBlock_File ) == SIMRC_OK)  /* ��֤���� --ѡ�� A key  ----��ѡ������ΪSimBlock_File */
                    {
                        if(sim_read_block(SimBlock_File,SimBlock_data) == SIMRC_OK); /* ���� */
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
  * @brief		д���ݿ�
  * @details	0~2 ��������3---KEY��
  * @param 	block	������
  * @param 	data	����
  * @retval 	0���ɹ���1��ʧ��
  */
u8 Sim_write_press(u8 SimBlock_File,u8 *SimBlock_data)
{
    u8 statu_flag = 0;
    if(SearchSimCard(SIM_ICID_BAK_LAST) == SIMRC_OK)
    {
        memcpy(SIM_ICID_BAK_LAST,SIM_RX_DATA.SIM_BUF,8);	/* ���뱾��ID��ͬ�Ŀ����д洢--��ֹ�ظ�����*/
        if(SimShutChannel(4) == SIMRC_OK)	   							/* �ر�����ͨ��1-2-3 */
		{	                                                          	
			if(SimOpenChannel(SimBlock_File) == SIMRC_OK) 			/* ���߼�ͨ��SimBlock_File */
			{                                                         	
				if(SimSelectAid() == SIMRC_OK)							/* AIDѡ��---Ӧ������ѡ�� */
				{                                                     	
					if(SimAuthenticKey(SimBlock_File ) == SIMRC_OK)  /* ��֤���� --ѡ�� A key  ----��ѡ������ΪSimBlock_File */
                    {
                        if(sim_write_block(SimBlock_File,SimBlock_data) == SIMRC_OK); /* д�� */
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