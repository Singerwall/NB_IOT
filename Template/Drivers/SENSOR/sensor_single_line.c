/*******************************************************************
*
*    FILENAME(�ļ���)     :    sensor_single_line.c
*
*    COPYRIGHT(����Ȩ)    :    ������Ѷ��ͨ�ż������޹�˾
*
*    MODULE NAME(�����)  :
*
*    CPU                  :    stm32F103ZET6
*
*    RTOS(����ϵͳ)       :    
*
*    CreateDate(����ʱ��) :
*
*    AUTHOR(����)         :
*
*    Abstract Description 
*    ժҪ����             :	  DS18B20����������
*
*--------------------------Revision History------------------------
*
*    No    Version    Date    Revise By    Item    Description
*
*******************************************************************/

/*******************************************************************
*    Debug switch Section
*    ���Կ���
*******************************************************************/

/*******************************************************************
*    Include File Section
*    ����ͷ�ļ�
*******************************************************************/
#include "sensor_single_line.h"
/*******************************************************************
*    Macro Define Section
*    �궨��
*******************************************************************/

/*******************************************************************
*    Struct Define Section
*    �ṹ�嶨��
*******************************************************************/

/*******************************************************************
*    Global variables defined
*    ȫ�ֱ�������
*******************************************************************/

/*******************************************************************
*    External Varible Declare
*    �ⲿ��������
*******************************************************************/

/*******************************************************************
*    Function Define Section
*    �������岿��
*******************************************************************/


/*****************************************************************
*    Function Name(������)   :   DS18B20_Rst()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 ��λDS18B20
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void DS18B20_Rst(void)	   
{ 	               
	DS18B20_IO_OUT(); 		//SET PA0 OUTPUT
	DS18B20_DQ_SET;			//���ݶ˿�	PB7 
	delay_us(10);			//5US
    DS18B20_DQ_CLR; 		//����DQ
    delay_us(480);    		//��ʱ400us-960us
    DS18B20_DQ_SET; 		//DQ=1 
	delay_us(30);     		//15US
}


/*****************************************************************
*    Function Name(������)   :   DS18B20_Check()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 �ȴ�DS18B20�Ļ�Ӧ
*
*    Returns(����ֵ)         :	 //����1:δ��⵽DS18B20�Ĵ���    ����0:����
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	 
	return 0;
}
/*****************************************************************
*    Function Name(������)   :   DS18B20_Read_Byte()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 ��DS18B20��ȡһ���ֽ�
*
*    Returns(����ֵ)         :	 ����ֵ������������
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,rdat;

	DS18B20_IO_OUT();
	DS18B20_DQ_SET;
	delay_us(5);
	for (i=1;i<=8;i++) 
	{
		rdat>>=1;
		DS18B20_DQ_CLR;
		delay_us(5);
		DS18B20_DQ_SET;
		delay_us(5);
		DS18B20_IO_IN();
		if(DS18B20_DQ_IN)
		{
			rdat|=0x80;
		}
		else
		{
			rdat&=0x7f;
		}

		DS18B20_IO_OUT();
		delay_us(35);
		DS18B20_DQ_SET;
    }
						    
    return rdat;
}
/*****************************************************************
*    Function Name(������)   :   DS18B20_Write_Byte()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 дһ���ֽڵ�DS18B20
*
*    Returns(����ֵ)         :	 dat��Ҫд����ֽ�
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;

	//TICK_DISABLE();
	DS18B20_IO_OUT();//SET PB7 OUTPUT;
	DS18B20_DQ_SET;
    for (j=1;j<=8;j++) 
	{
		DS18B20_DQ_CLR;
		delay_us(5);
        if (dat&0x01) 
        {                           
            DS18B20_DQ_SET;            
        }
        else 
        {
            DS18B20_DQ_CLR;// Write 0                         
        }
		delay_us(35);
		DS18B20_DQ_SET;
		delay_us(2);
		dat>>=1;
    }
}
/*****************************************************************
*    Function Name(������)   :   DS18B20_Start()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 ��ʼ�¶�ת��
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 
/*****************************************************************
*    Function Name(������)   :   DS18B20_Init()
*
*    Parameters(����)        :   
*
*    Description(����)       : 	 ��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
*
*    Returns(����ֵ)         :	 ����1:������ ����0:���� 
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
u8 DS18B20_Init(void)
{
    BSP_GPIO_Init(DS18B20_PORT, DS18B20_PIN, DS18B20_MODE, DS18B20_PULL);
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN,GPIO_PIN_SET);
    DS18B20_Rst();
    return DS18B20_Check();
}  
/*****************************************************************
*    Function Name(������)   :   DS18B20_Get_Temp()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 ��ds18b20�õ��¶�ֵ
*
*    Returns(����ֵ)         :	 ���ȣ�0.1C ����ֵ���¶�ֵ ��-550~1250��
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void DS18B20_Get_Temp(u8* data)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();                    	// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);				// skip rom
    DS18B20_Write_Byte(0xbe);				// readTemp	 
    TL=DS18B20_Read_Byte(); 				// LSB   
    TH=DS18B20_Read_Byte(); 				// MSB  

	//��ʼ����ֵ
 	data[0] = 0;
	data[1] = 0;
        data[2] = 0;							//
	data[3] = 0;
	data[4] = 0;	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;								//�¶�Ϊ��  
    }else temp=1;							//�¶�Ϊ��	  	  
    tem=TH; 								//��ø߰�λ
    tem<<=8;    
    tem+=TL;								//��õװ�λ
    tem=(float)tem*0.625;					//ת��  
//ת���ɱ���������ʾ������	
	if(temp == 0)
		  data[0] = 1;

	data[1] = 0; 
	data[2] = ((tem/10)%0XFF);
	data[3] = 0; 
	data[4] = (tem%10);

}


 
