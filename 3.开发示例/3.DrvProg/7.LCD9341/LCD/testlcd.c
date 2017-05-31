#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/ioctl.h>   
#include <stdint.h>  
#include <string.h>  
#include <sys/mman.h>  
#include <sys/types.h>  
#include  <sys/stat.h>   
#include<linux/types.h>  
#include<linux/stat.h>  
#include <ili9341.h>
#include <gpio_fun.h>
#include "picture.h"
#include "hanzi.h"

extern _lcd_dev lcddev;
int fd; 
int main(int argc,char * argv[])   
{  
	 u8 x=0;
	 u8 lcd_id[64];			//存放LCD ID字符串	
	gpio_open();
	Init_ili9341();
	POINT_COLOR=RED; 
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。
	
	while(1)
	{		 
		switch(x)
		{
			case 0:LCD_Clear(WHITE);
			LCD_Display_Dir(1);		 	//横屏
			POINT_COLOR = BLACK;
			BACK_COLOR = WHITE;
		LCD_ShowString(30,40,400,24,24,"OPEN LOONGSON Smart Loong V2.0^_^");	
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST ");
		LCD_ShowString(30,90,200,16,16,"sundm75");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2017/5/24");	      					 
			
			break;
			case 1://LCD_Clear(BLACK);
			POINT_COLOR = WHITE;
			BACK_COLOR = BLACK;
		LCD_ShowString(30,40,400,24,24,"OPEN LOONGSON Smart Loong V2.0^_^");	
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST ");
		LCD_ShowString(30,90,200,16,16,"sundm75");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2017/5/24");	      					 
			
			break;
			case 2://LCD_Clear(BLUE);
			POINT_COLOR = RED;
			BACK_COLOR = BLUE;
		LCD_ShowString(30,40,400,24,24,"OPEN LOONGSON Smart Loong V2.0^_^");	
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST ");
		LCD_ShowString(30,90,200,16,16,"sundm75");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2017/5/24");	      					 
			
			break;
			case 3://LCD_Clear(RED);
			POINT_COLOR = BLUE;
			BACK_COLOR = RED;
		LCD_ShowString(30,40,400,24,24,"OPEN LOONGSON Smart Loong V2.0^_^");	
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST ");
		LCD_ShowString(30,90,200,16,16,"sundm75");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2017/5/24");	      					 
			
			break;
			case 4:
				LCD_Display_Dir(0);		 	//竖屏
				LCD_Clear(BLUE);
				lcd_draw_hanzi(5,420,(u8*)Hanzi, WHITE, HYALINE); 
				LCD_Display_Dir(1);		 	//横屏
				DrawPicture_Center(gImage_picture);
				msleep(500);
			break;
			case 5:
				LCD_Display_Dir(1);		 	//横屏
				POINT_COLOR = RED;
				BACK_COLOR = BLUE;			
				LCD_DrawLine(20,20, 20,300);	
				POINT_COLOR = GREEN;
				LCD_DrawLine(405,30, 405,290);	
				LCD_DrawRectangle(3,15,450,310);
				LCD_Display_Dir(0);		 	//竖屏
				LCD_ShowString(30,45,400,29,24,"Smart Loong V2.0");				
				LCD_ShowString(100,20,400,4,24,"Loongson");	
				POINT_COLOR = RED;
				LCD_Draw_Circle(30,15,10);	
				LCD_Draw_Circle(50,15,10);					
				LCD_Draw_Circle(70,15,10);					
				LCD_Draw_Circle(90,15,10);					
				LCD_Draw_Circle(110,15,10);					
				LCD_Draw_Circle(130,15,10);					
				LCD_Draw_Circle(150,15,10);					
				LCD_Draw_Circle(180,15,10);					
				LCD_Draw_Circle(200,15,10);					
				LCD_Draw_Circle(220,15,10);					
				LCD_Draw_Circle(240,15,10);					
				LCD_Draw_Circle(260,15,10);					
				LCD_Draw_Circle(280,15,10);					
				LCD_Draw_Circle(300,15,10);					
				msleep(800);
			break;
			
		}
	    x++;
		if(x==6)x=0;
		msleep(300);	
	}	
		gpio_close();
	
	
	
	return 0;
}



