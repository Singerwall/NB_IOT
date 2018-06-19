/** @copyright XunFang Communication Tech Limited. All rights reserved. 2015.
  * @file  xf_sensor_ir_thermometry.c
  * @author wuyijun
  * @version  V1.0.0
  * @date  12/04/2015
  * @brief  MLX90614红外测温驱动
  */ 

#include "xf_sensor_ir_thermometry.h"
#ifdef CC2540
  #include "iocc2540.h"
#else
  #include "iocc2530.h"
#endif
#include "xf_uart0_driver.h"
#include "xf_sensor_ds18b20.h"

static void Delay(uint16 N);

#define _nop_() asm("nop")
/**
  * @brief  Delay延时函数  
  * @details 
  * @param  NULL
  * @param  N 为延时时间
  * @return NULL
 **/
static void Delay(uint16 N)
{ 
    uint16 i;
    for(i=0;i<N;i++) 
    _nop_();
}

/**
  * @brief  PEC_cal函数  
  * @details 
  * @param  pec[]
  * @param  n 
  * @return NULL
 **/
uint8 PEC_cal(uint8 pec[],int n)
{
    uint8 crc[6];
    uint8 Bitposition=47;
    uint8 shift;
    uint8 i;
    uint8 j;
    uint8 temp;

    do{
        crc[5]=0;           			        //Load CRC value 0x000000000107
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;
        Bitposition=47;     		                //Set maximum bit position at 47
        shift=0;
        i=5;                				//Set highest index (package byte index)
        j=0;                			        //Byte bit index, from lowest
        while((pec[i]&(0x80>>j))==0 && (i>0)) 
        {
            Bitposition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
    }//End of while, and the position of highest "1" bit in Bitposition is calculated 
       
    shift=Bitposition-8;                          //Get shift value for CRC value
    while(shift)
    {
        for(i=5;i<0xFF;i--)
        {  
            if((crc[i-1]&0x80) && (i>0))        //Check if the MSB of the byte lower is "1"
            {   		                //Yes - current byte + 1
                temp=1;			//No - current byte + 0
            }				//So that "1" can shift between bytes
            else
            {
                temp=0;
            }
                crc[i]<<=1;
                crc[i]+=temp;
            } 
        shift--;
        } 

           //Exclusive OR between pec and crc

        for(i=0;i<=5;i++)
        {
            pec[i]^=crc[i];
        }  

    }while(Bitposition>8); 

	return pec[0];
}
 
/**
  * @brief  从MLX90614读取数据 
  * @details 
  * @param  slave_addR 从地址
  * @param   cmdR   命令
  * @return  Data 返回读到的数据
 **/
uint32 MEM_READ(uint8 slave_addR, uint8 cmdR)
{
    uint8 DataL;		                 //
    uint8 DataH;				 //Data packets from MLX90614
    uint8 PEC;				 //
    uint32 Data;			 //Register value returned from MLX90614
    uint8 Pecreg;				 //Calculated PEC byte storage
    uint8 arr[6];				 //Buffer for the sent bytes
    uint8 ack_nack;
    uint8 SLA;
    uint8 i = 5;										 
    SLA=(slave_addR<<1);
    while(i--)
    {
         start_bit();                 			     //Send start bit
         TX_byte(SLA);					     //Send slave address, write
         if(slave_ack()==0)
         {
             stop_bit();
             continue;
             }						     //Send command
             TX_byte(cmdR);
         if(slave_ack()==0)
         {
             stop_bit();
             continue;
         }
                                     //Send Repeated start bit
         start_bit(); 					     //Send slave address, read
         TX_byte(SLA+1);
         if(slave_ack()==0)
         {
             stop_bit();
             continue;
         }
    
         DataL=RX_byte(0);					 //
                                             //Read two bytes data
         DataH=RX_byte(0);					 //
    
         PEC=RX_byte(ack_nack);				 //Read PEC from MLX90614
         if(ack_nack==1)					 //Master sends ack or nack //This depends on the pec calculation, if the PEC is not correct, send nack and goto begin
         {
             stop_bit();
             continue;
         }							 //Send stop bit      
         break;
    }
    stop_bit();
    arr[5]=(SLA);
    arr[4]=cmdR;
    arr[3]=(SLA+1);               
    arr[2]=DataL;
    arr[1]=DataH;
    arr[0]=0;                    											 
    Pecreg=PEC_cal(arr,6);  				 //Calculate CRC 
    
    if(PEC==Pecreg)
        ack_nack=0;
    else
        ack_nack=1;
    Data=(DataH*256)+DataL;   
    return Data;
}

/**
  * @brief  向MLX90614写入数据 
  * @details 
  * @param  slave_addR 从地址
  * @param   cmdR   命令
  * @param  DataL  低字节
  * @param  DataH  高字节
  * @return  NULL
 **/
void EEPROM_WRITE(uint8 slave_addW,uint8 cmdW,uint8 DataL,uint8 DataH)
{

    uint8 Pecreg;					//Calculated PEC byte storage
    uint8 SLA;
    uint8 arr[6];					//Buffer for the transmitted bytes
    uint8 i = 5;
    SLA=(slave_addW<<1);
    
    arr[5]=0;
    arr[4]=SLA;
    arr[3]=cmdW;
    arr[2]=DataL;
    arr[1]=DataH;
    arr[0]=0;
    Pecreg=PEC_cal(arr,6);
    while(i--)
    {
        start_bit();						//Send start bit
        TX_byte(SLA);						//Send slave address, write
        if(slave_ack()==0)								  
        {
            stop_bit();
            continue;
        }
        TX_byte(cmdW);						//Send command
        if(slave_ack()==0)
        {
            stop_bit();
            continue;
        }
        TX_byte(DataL);						//Send Low Data byte
        if(slave_ack()==0)
        {
            stop_bit();
            continue;
        }
        TX_byte(DataH);						//Send High Data byte
        if(slave_ack()==0)
        {
            stop_bit();
            continue;
        }
        TX_byte(Pecreg);						//Send PEC 
        if(slave_ack()==0)
        {
            stop_bit();
            continue;
        }   
        break;
    }
    stop_bit();	            				//Send stop bit
    Delay(200);						//Wait 5ms
    
}
													
	 
/**
  * @brief  SMBUS初始化 
  * @details 
  * @param  NULL
  * @return  NULL
 **/

void SMBus_Init(void)
{
    _SCL_IO;
    _SDA_OUTPUT;
    
    SCL=0;			//
    Delay(1200);		//SMBus request，Switch PWM mode to SMBus mode(at least 2ms)
    SCL=1;			//
}


/**
  * @brief  启动位函数
  * @details 
  * @param  NULL
  * @return  NULL
 **/
void start_bit()
{
   
    _SDA_OUTPUT;				      //Set SDA as output 
    SDA=1;				      //Set SDA line
    _nop_();_nop_();				  
    SCL=1;				      //Set SCL line
    Delay(5);				      //Generate bus free time between stop and start condition (Tbuf=4.7us min)
    SDA=0;				      //Clear SDA line
    Delay(5);				      //Hold time after (Repeated) start condition, after this time, generate the first clock
                          //Thd:sta=4us min
    SCL=0;				      //Clear SCL line
    _nop_();_nop_();
}

/**
  * @brief  停止位函数
  * @details 
  * @param  NULL
  * @return  NULL
 **/
void stop_bit()
{
  
    _SDA_OUTPUT;					   //Set SDA as output
    SCL=0;			     		   //Clear SCL line
    Delay(5);
    SDA=0;					   //Clear SDA line
    Delay(5);
    SCL=1;					   //Set SCL line
    Delay(5);					   //Stop condition setup time(Tsu:sto=4.0us min)
    SDA=1;					   //Set SDA line
    
}


/**
  * @brief  发送位函数
  * @details 
  * @param  NULL
  * @return  NULL
 **/
void send_bit(uint8 bit_out)
{
    
    _SDA_OUTPUT;					  //Set SDA as output to transmit data on SMBus
    if(bit_out==0)				  //Check bit
        SDA=0;
    else							  
        SDA=1;				  //Clear SDA if bit_out=0
    _nop_();						  //
    _nop_();						  //Tsu:dat=250ns minimum
    _nop_();						  //
    SCL=1;						  //Set SCL line
    Delay(4);						  //High Level of Clock Pulse (10.6us)
    SCL=0;						  //Clear SCL line
    Delay(4);
   
}


/**
  * @brief  接收位函数
  * @details 
  * @param  NULL
  * @return  NULL
 **/
uint8 receive_bit()
{
  uint8 bit_in;
 
    _SDA_INPUT;				       //Set SDA as input
    SCL=1;				       //Set SCL line
    Delay(2);
    
    if(SDA==1)				       //Read bit, save it in bit_in
       bit_in=1;
    else
       bit_in=0;
    
    Delay(2);
    SCL=0;				       //Clear SCL line
    Delay(4);
    return bit_in;

}

/**
  * @brief  从设备应答函数
  * @details 
  * @param  NULL
  * @return  NULL
 **/
uint8 slave_ack()
{
    uint8 ack;
    ack=0;
    _SDA_INPUT;				        //Set SDA as input
    SCL=1;					//Set SCL line
    Delay(2);
    if(SDA==1)					//Read bit, save it in ack
        ack=0;
    else
        ack=1;  
    Delay(2);   
    SCL=0;					//Clear SCL line
    Delay(4);   
    return ack;
}


/**
  * @brief  发送字节函数
  * @details 
  * @param   TX_buffer 发送缓冲区
  * @return  NULL
 **/
void TX_byte(uint8 TX_buffer)
{
   uint8 Bit_counter;
   uint8 bit_out;
   
   for(Bit_counter=8;Bit_counter;Bit_counter--)
   {
        if(TX_buffer&0x80)
            bit_out=1;		          //If the current bit of TX_buffer is 1, set bit_out
        else
            bit_out=0;	         	  //Otherwise clear bit_out
        send_bit(bit_out);			  //Send the current bit on SMBus
        TX_buffer<<=1;			  //Get next bit to check	  
	}		            	                      
}


/**
  * @brief  接收字节函数
  * @details 
  * @param   ack_nack 接收应答位
  *          0 - 有应答
  *          1 - 无应答
  * @return  NULL
 **/
uint8 RX_byte(uint8 ack_nack)
{ 
    uint8 RX_buffer;
    uint8 Bit_counter;
    
    for(Bit_counter=8;Bit_counter;Bit_counter--)
    {           	
        if(receive_bit()==1)	   //Read a bit from the SDA line   
        {
            RX_buffer<<=1;	   //If the bit is HIGH save 1 in RX_buffer 
            RX_buffer|=0x01;
        }
        else			   //If the bit is LOW save 0 in RX_buffer
        {
            RX_buffer<<=1;
            RX_buffer&=0xfe;
        }	
	 } 
	 send_bit(ack_nack);				   //Sends acknowledgment bit		
	 return RX_buffer;

}


/** 
  * @brief  MLX90614初始化
  * @details 
  * @param  NULL 				
  * @return NULL
 **/
void MLX90614_Init(void)
{
    SMBus_Init();
}

/** 
  * @brief  读取 MLX90614 测温温度值
  * @details 
  * @param  NULL 				
  * @return mahm: 存放温度值数组的首地址
 **/
uint16 MLX90614_TemperRead(void)
{
    uint8 slaveaddress;	
    uint32 buff;

    slaveaddress=MEM_READ(0x00,0x2E);                       
    //Get the slave address which stored in EEPROM "0Eh"
    buff=MEM_READ(slaveaddress,0x07);			   
		           
    return buff;
}
/** 
  * @brief  获取温度函数
  * @details 
  * @param  NULL 				
  * @return sensor_data: 存放温度值数组的首地址
 **/
uint8 *XfGetIRTempValue(void)
{
    signed char error = 0;
    uint8 *ds18b20_data;
    float result = 0.0;
    static uint8 sensor_data[2] = {0};
    result = ((float)MLX90614_TemperRead()*0.02) - 273.15;
    sensor_data[1] = (uint8)result;
    sensor_data[0] = (uint8)(result*10 - sensor_data[0]*10);
    
#ifdef UART0_DEBUG
    uart0_printf("IR temper: %d.%d\r\t", sensor_data[1], sensor_data[0]);
#endif
    ds18b20_data = XfGetTemperture();     
    error = (char)(sensor_data[1] - *++ds18b20_data);
    
#ifdef UART0_DEBUG
    uart0_printf("error: %d\r\n",error);
#endif   
    if(error <= 3 && error >= -3 )
    {
        sensor_data[0] = 0;
        sensor_data[1] = 0;
    }
    return sensor_data;
}