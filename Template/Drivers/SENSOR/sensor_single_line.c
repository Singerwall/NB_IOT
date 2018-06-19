/*******************************************************************
*
*    FILENAME(文件名)     :    sensor_single_line.c
*
*    COPYRIGHT(著作权)    :    深圳市讯方通信技术有限公司
*
*    MODULE NAME(组件名)  :
*
*    CPU                  :    stm32F103ZET6
*
*    RTOS(操作系统)       :    
*
*    CreateDate(创建时间) :
*
*    AUTHOR(作者)         :
*
*    Abstract Description 
*    摘要描述             :	  DS18B20传感器函数
*
*--------------------------Revision History------------------------
*
*    No    Version    Date    Revise By    Item    Description
*
*******************************************************************/

/*******************************************************************
*    Debug switch Section
*    调试开关
*******************************************************************/

/*******************************************************************
*    Include File Section
*    包含头文件
*******************************************************************/
#include "sensor_single_line.h"
/*******************************************************************
*    Macro Define Section
*    宏定义
*******************************************************************/

/*******************************************************************
*    Struct Define Section
*    结构体定义
*******************************************************************/

/*******************************************************************
*    Global variables defined
*    全局变量定义
*******************************************************************/

/*******************************************************************
*    External Varible Declare
*    外部变量声明
*******************************************************************/

/*******************************************************************
*    Function Define Section
*    函数定义部分
*******************************************************************/


/*****************************************************************
*    Function Name(函数名)   :   DS18B20_Rst()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :	 复位DS18B20
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void DS18B20_Rst(void)	   
{ 	               
	DS18B20_IO_OUT(); 		//SET PA0 OUTPUT
	DS18B20_DQ_SET;			//数据端口	PB7 
	delay_us(10);			//5US
    DS18B20_DQ_CLR; 		//拉低DQ
    delay_us(480);    		//延时400us-960us
    DS18B20_DQ_SET; 		//DQ=1 
	delay_us(30);     		//15US
}


/*****************************************************************
*    Function Name(函数名)   :   DS18B20_Check()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :	 等待DS18B20的回应
*
*    Returns(返回值)         :	 //返回1:未检测到DS18B20的存在    返回0:存在
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   DS18B20_Read_Byte()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :	 从DS18B20读取一个字节
*
*    Returns(返回值)         :	 返回值：读到的数据
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   DS18B20_Write_Byte()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :	 写一个字节到DS18B20
*
*    Returns(返回值)         :	 dat：要写入的字节
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   DS18B20_Start()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :	 开始温度转换
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   DS18B20_Init()
*
*    Parameters(参数)        :   
*
*    Description(描述)       : 	 初始化DS18B20的IO口 DQ 同时检测DS的存在
*
*    Returns(返回值)         :	 返回1:不存在 返回0:存在 
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   DS18B20_Get_Temp()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :	 从ds18b20得到温度值
*
*    Returns(返回值)         :	 精度：0.1C 返回值：温度值 （-550~1250）
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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

	//初始化数值
 	data[0] = 0;
	data[1] = 0;
        data[2] = 0;							//
	data[3] = 0;
	data[4] = 0;	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;								//温度为负  
    }else temp=1;							//温度为正	  	  
    tem=TH; 								//获得高八位
    tem<<=8;    
    tem+=TL;								//获得底八位
    tem=(float)tem*0.625;					//转换  
//转化成便于网关显示的数据	
	if(temp == 0)
		  data[0] = 1;

	data[1] = 0; 
	data[2] = ((tem/10)%0XFF);
	data[3] = 0; 
	data[4] = (tem%10);

}


 
