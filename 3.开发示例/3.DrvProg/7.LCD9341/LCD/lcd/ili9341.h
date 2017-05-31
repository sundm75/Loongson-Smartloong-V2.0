#ifndef __ILI9341_H
#define __ILI9341_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define u32 unsigned int 
#define u16 unsigned short 
#define u8  unsigned char 

#define msleep(x) usleep(x*1000)
#define udelay(x) usleep(x)
#define mdelay(x) msleep(x)

#define		SPI_MISO	80
#define		SPI_MOSO    91
#define		SPI_CS	    84
#define		SPI_CLK	    95
#define		PEN		    87

#define		LCD_d0		    58
#define		LCD_d6		    64

#define LCD_BL  74			//
#define LCD_CS  77			//
#define LCD_WR  88			//
#define LCD_RD  89			//
#define LCD_RS  90			//

/** 引脚控制宏定义 */
#define	LCD_CS_SET gpio_set(LCD_CS)
#define	LCD_WR_SET gpio_set(LCD_WR)	
#define	LCD_RD_SET gpio_set(LCD_RD)	
#define	LCD_RS_SET gpio_set(LCD_RS) 
#define	LCD_CS_CLR  gpio_clr(LCD_CS) 
#define	LCD_WR_CLR	gpio_clr(LCD_WR)
#define	LCD_RD_CLR	gpio_clr(LCD_RD) 
#define	LCD_RS_CLR	gpio_clr(LCD_RS) 
 

#define lcd_set_bl()  gpio_set(LCD_BL)
#define lcd_clr_bl()  gpio_clr(LCD_BL)


//之所以使用这样预定义方式 是为了尽量的缩短屏幕读写时间 当然也可以写成函数
#define write_data(data1) {\
	data_write(LCD_d0, 6 ,data1);\
	data_write(LCD_d6, 10 ,data1>>6);\
}

#define LCD_DATA_gpio_out_mod(){\
	gpio_en_data(58,6,0);\
	gpio_en_data(64,10,0);\
}

#define LCD_DATA_gpio_in_mod(){\
	gpio_en_data(58,6,1);\
	gpio_en_data(64,10,1);\
}

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

//////////////////////////////////////////////////////////////////////
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
#define LIGHTGREEN     	 0X841F //浅绿色 
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
#define HYALINE        0x0001		 // 透明
void LCD_Init(void);
u8 Init_ili9341(void);
void LCD_Clear(u16 color);
void gpio_init(void);
void LCD_BK_LED(void);
u16 read_data(void);
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(u16 Color);	 												//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//设置光标
void LCD_DrawPoint(u16 x,u16 y);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//快速画点
u16  LCD_ReadPoint(u16 x,u16 y); 											//读点 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);					    			//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);		  
void LCD_Scan_Dir(u8 dir);
void LCD_Display_Dir(u8 dir);								//设置屏幕显示方向
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//设置窗口	

void DrawPicture_Center(u16 *PictureAddr)  ;
void lcd_draw_hanzi(u16 x,u16 y,u8 *hanziaddr,u16 charColor,u16 bkColor);

//LCD分辨率设置
#define SSD_HOR_RESOLUTION		480		//LCD水平分辨率
#define SSD_VER_RESOLUTION		320		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif

