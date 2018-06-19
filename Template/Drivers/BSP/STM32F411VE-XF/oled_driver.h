#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H	


#include <includes.h>	    
	    						  
/*-----------------OLED端口定义----------------*/ 
#if 0
#define   OLED_RST_PIN                        GPIO_PIN_1/* res复位*/
#define   OLED_RST_PORT                       GPIOE

#define   OLED_RS_PIN                         GPIO_PIN_0/*dc 数据、命令*/
#define   OLED_RS_PORT                        GPIOE

#endif

#define   OLED_RST_PIN                        GPIO_PIN_7/* res复位*/
#define   OLED_RST_PORT                       GPIOD

#define   OLED_RS_PIN                         GPIO_PIN_6/*dc 数据、命令*/
#define   OLED_RS_PORT                        GPIOD




#define   OLED_SCLK_PIN                       GPIO_PIN_8/* res复位*/
#define   OLED_SCLK_PORT                      GPIOB

#define   OLED_SDIN_PIN                       GPIO_PIN_9/*dc 数据、命令*/
#define   OLED_SDIN_PORT                      GPIOB

#define   DEFAULT_MODE                       GPIO_MODE_OUTPUT_PP/*推挽输出*/
#define   DEFAULT_PULL                       GPIO_PULLUP/*上拉*/


#define OLED_CS_Clr()  {}
#define OLED_CS_Set()  {}
#if 0
#define OLED_RST_Clr()  PEout(1)=0 //SCL  /*复位*/
#define OLED_RST_Set()  PEout(1)=1 

#define OLED_RS_Clr()   PEout(0)=0    /*数据、命令*/
#define OLED_RS_Set()   PEout(0)=1
#endif

#define OLED_RST_Clr()  PDout(7)=0 //SCL  /*复位*/
#define OLED_RST_Set()  PDout(7)=1 

#define OLED_RS_Clr()   PDout(6)=0    /*数据、命令*/
#define OLED_RS_Set()   PDout(6)=1



#define OLED_SCLK_Clr() PBout(8)=0   /*时钟*/
#define OLED_SCLK_Set() PBout(8)=1

#define OLED_SDIN_Clr() PBout(9)=0  /*输入*/
#define OLED_SDIN_Set() PBout(9)=1 

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//这两个参数定义动画每帧的长宽。
#define MOVIE_XSIZE	128 
#define MOVIE_YSIZE 64  
#define FRAME_SIZE MOVIE_XSIZE*MOVIE_YSIZE/8 


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 mode); 
void OLED_ShowString12x12(u8 x,u8 y,const u8 *p,u8 mode);
void Movie_Show_Img(u16 x,u16 y,u32 picx);
void OLED_ShowFont_16x16(u8 x,u8 y,u8 fno,u8 mode);
void OLED_ShowFont_12x12(u8 x,u8 y,u8 fno,u8 mode);
void OLED_ShowString12x12(u8 x,u8 y,const u8 *p,u8 mode);
void Display_print(u8 x, u8 y, const char *pstring, ...);
void LCD_Print(const char *pstring, ...);
void OLED_logo(void); 
void OLED_Rectangle(u8 x0,u8 y0,u8 x1,u8 y1,u8 Color);
void OLED_HoriLine(u8 x,u8 y,u8 Length,u8 Color);
void OLED_VertLine(u8 x,u8 y,u8 Length,u8 Color);
void Movie_Show_draw_bmp_32x32(u16 x,u16 y,const u8 *gImage,u8 mode);
void Movie_Show_draw_bmp_32x64(u16 x,u16 y,const u8 *gImage,u8 mode);
#endif  
	 



