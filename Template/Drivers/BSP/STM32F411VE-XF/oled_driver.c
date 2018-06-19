#include "oled_driver.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "spi.h"

//OLED的显存
			   
static u8 OLED_GRAM[128][8];	 

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
    u8 i,n;		    
    for(i=0;i<8;i++)  
    {  
	OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
	OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
	OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
	for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
    }   
}

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
} 
  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}


//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 mode)
{
    #define MAX_CHAR_POSX 122
    #define MAX_CHAR_POSY 58   
   OLED_Clear();
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,mode);	 
        x+=8;
        p++;
    } 
      OLED_Refresh_Gram();
}
void Display_print2( const char *pstring, ...)
{
   #define buffer_size 100
  
    char temp_buffer[buffer_size];
    
    va_list args;
    va_start(args, pstring);
    vsnprintf(temp_buffer, buffer_size, pstring, args);
    va_end(args);
    
    OLED_ShowString( 0, 0,(u8 *)temp_buffer,1);
}

void Display_print(u8 x, u8 y, const char *pstring, ...)
{
   #define buffer_size 100
  
    char temp_buffer[buffer_size];
    
    va_list args;
    va_start(args, pstring);
    vsnprintf(temp_buffer, buffer_size, pstring, args);
    va_end(args);
    
    OLED_ShowString( x, y,(u8 *)temp_buffer,1);
}

void LCD_Print(const char *pstring, ...)
{
     #define buffer_size 100
  
    char temp_buffer[buffer_size];
    
    va_list args;
    va_start(args, pstring);
    vsnprintf(temp_buffer, buffer_size, pstring, args);
    va_end(args);
    
    OLED_ShowString(0, 0,(u8 *)temp_buffer,1);
}

void OLED_ShowString12x12(u8 x,u8 y,const u8 *p,u8 mode)
{
	#define MAX_CHAR_POSX 122
	#define MAX_CHAR_POSY 58          
	while(*p!='\0')
		{       
		if(x>MAX_CHAR_POSX){x=0;y+=12;}
		if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
		OLED_ShowChar(x,y,*p,12,mode);	 
		x+=6;
		p++;
		}  
}


//在指定位置显示一个汉字
//x:0~127
//y:0~63
//fno:汉字编号
//mode:0,反白显示;1,正常显示				 
//22*22大小的字符

void OLED_ShowFont_16x16(u8 x,u8 y,u8 fno,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;				   
    for(t=0;t<32;t++)
    {   
		temp=user_hz16x16[fno][t];    //调用16x16汉字	                           
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


void OLED_ShowFont_12x12(u8 x,u8 y,u8 fno,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;				   
    for(t=0;t<24;t++)
    {   
		temp=user_hz12x12[fno][t];    //调用12x12汉字	                           
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==12)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}



//一列64个点向下画点，一列一列画,共128列
void Movie_Show_Img(u16 x,u16 y,u32 picx)
{      			    
	u16 temp,t,t1;
	u16 y0=y;	
	picx*=FRAME_SIZE;				    				   
    for(t=0;t<FRAME_SIZE;t++)
    {   
		temp=movie_data[picx+t];  //得到图片的一个字节数据	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,1); 
			else OLED_DrawPoint(x,y,0);		   
			temp<<=1;
			y++;
			if((y-y0)==MOVIE_YSIZE)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

/***************************************
向LCD指定位置画一条长度为Length的指定颜色的水平线
****************************************/
void OLED_HoriLine(u8 x,u8 y,u8 Length,u8 Color)
{
	u8 i;
	if(Length==0)
		return;
	for(i=0;i<Length;i++)
	{
		OLED_DrawPoint(x+i,y,Color);
	}
}

/***************************************
向LCD指定位置画一条长度为Length的指定颜色的垂直线
****************************************/
void OLED_VertLine(u8 x,u8 y,u8 Length,u8 Color)
{
	u8 i;
	if(Length==0)
		return;
	for(i=0;i<Length;i++)
	{
		OLED_DrawPoint(x,y+i,Color);
	}
}


/*******************************************
向LCD指定左上角坐标和右下角坐标画一个指定颜色的矩形
********************************************/
void OLED_Rectangle(u8 x0,u8 y0,u8 x1,u8 y1,u8 Color)
{
    u8 Temp;
    if(x0>x1)
    {
        Temp=x0;
        x0=x1;
        x1=Temp;
    }	
    if(y0>y1)
    {
        Temp=y0;
        y0=y1;
        y1=Temp;
    }
    OLED_VertLine(x0,y0,y1-y0+1,Color);
    OLED_VertLine(x1,y0,y1-y0+1,Color);
    OLED_HoriLine(x0,y0,x1-x0+1,Color);
    OLED_HoriLine(x0,y1,x1-x0+1,Color);	
    OLED_Refresh_Gram();//更新显示

}


//画位图 32*32
void Movie_Show_draw_bmp_32x32(u16 x,u16 y,const u8 *gImage,u8 mode)
{      			    
    u16 temp,t,t1;
    u16 y0=y;	
    u16 MAX_YSIZE = 32;
				    				   
    for(t=0;t<128;t++)
    {   
	temp=gImage[t];  //得到图片的一个字节数据	                          
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,mode); 
            else OLED_DrawPoint(x,y,!mode);		   
            temp<<=1;
            y++;
            if((y-y0)==MAX_YSIZE)
            {
		y=y0;
		x++;
		break;
            }
	}  	 
    }          
}


//画位图 32*64
void Movie_Show_draw_bmp_32x64(u16 x,u16 y,const u8 *gImage,u8 mode)
{      			    
    u16 temp,t,t1;
    u16 y0=y;	
    u16 MAX_YSIZE = 32;
				    				   
    for(t=0;t<256;t++)
    {   
	temp=gImage[t];  //得到图片的一个字节数据	                          
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,mode); 
            else OLED_DrawPoint(x,y,!mode);		   
            temp<<=1;
            y++;
            if((y-y0)==MAX_YSIZE)
            {
		y=y0;
		x++;
		break;
            }
	}  	 
    }          
}
void  OLED_logo(void)
{
	Movie_Show_Img(0,0,0);//图片
	OLED_Refresh_Gram();//更新显示
//	delay_ms(5000);	
//	OLED_Clear();
}

//初始化SSD1306					    
void OLED_Init(void)
{ 	
 char *  i = "huanemng";
        BSP_GPIO_Init(OLED_RST_PORT, OLED_RST_PIN|OLED_RS_PIN, DEFAULT_MODE, DEFAULT_PULL);
  	BSP_GPIO_Init(OLED_SCLK_PORT, OLED_SCLK_PIN|OLED_SDIN_PIN, DEFAULT_MODE, DEFAULT_PULL);						  
	
        HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN|OLED_RS_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(OLED_SCLK_PORT, OLED_SCLK_PIN|OLED_SDIN_PIN, GPIO_PIN_SET);  
           
        OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();
        
        Display_print2("12=%s\r\n%s",i,i);
      
        
}  





























