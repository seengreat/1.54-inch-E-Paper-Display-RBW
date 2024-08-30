#include <stdio.h>
#include "sg_lgpio.h"
#include "1in54_epd.h"
#include "Ap_154demo.h"	
#include "epd_gui.h"
#include "fonts.h"

/***define pin conection in BCM number
1.54inch_EPD    Raspberry Pi
      VCC   --->   3.3V
      GND   --->   GND
      RST   --->   D17
      BUSY  --->   D24
      D/C   --->   D25
      MOSI  --->   MOSI
      CLK   --->   CLK
      CS    --->   (CE0)D8
*****************************/


//Tips//
/*
1.When the e-paper is refreshed in full screen, the picture flicker is a normal phenomenon, and the main function is to clear the display afterimage in the previous picture.
2.When the partial refresh is performed, the screen does not flash.
3.After the e-paper is refreshed, you need to put it into sleep mode, please do not delete the sleep command.
4.Please do not take out the electronic paper when power is on.
5.Wake up from sleep, need to re-initialize the e-paper.
6.When you need to transplant the driver, you only need to change the corresponding IO. The BUSY pin is the input mode and the others are the output mode.
*/
int	main(void)
{
	unsigned char Fen_L,Fen_H,Miao_L,Miao_H;
	printf("1.54inch epaper demo\r\n");
	EPD_GPIO_Init();       //EPD GPIO  initialization
	while(1)
	{
		/************ picture display*******************/
		EPD_HW_Init(); //EPD init 
		EPD_WhiteScreen_ALL(gImage_0,gImage_1);
	    //TEST_PIN = 0;
		EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
		delay(2000); //2s	
					
		//Clear
		EPD_HW_Init(); //EPD init
		EPD_WhiteScreen_ALL(gImage_2,gImage_3);//EPD_picture1	
		EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
		delay(2000); //2s	

		
////////////////////////////////////////////////////////////////////////	
	//Clear screen
	Clear:
		EPD_HW_Init(); //Electronic paper initialization
		EPD_WhiteScreen_White(); //Show all white
		EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
		delay(3000);				
				
		///////////////////////////GUI///////////////////////////////////////////////////////////////////////////////////
		//Data initialization settings
    	printf("NewImage:BlackImage and RedImage\r\n");
		Image_Init(BWimage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE); //Set screen size and display orientation
    	Image_Init(RWimage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
//		Gui_SelectImage(image);//Set the virtual canvas data storage location

		/**************Drawing**********************/
		EPD_HW_Init_GUI();  //EPD init GUI
		Gui_Clear(WHITE);
		//Point   
		Gui_Draw_Point(5, 10, BLACK, PIXEL_1X1, DOT_STYLE_DFT);
		Gui_Draw_Point(5, 25, BLACK, PIXEL_2X2, DOT_STYLE_DFT);
		Gui_Draw_Point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT);
		Gui_Draw_Point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT);
		//Line
		Gui_Draw_Line(15, 10, 55, 50, BLACK, PIXEL_1X1, SOLID);
		Gui_Draw_Line(15, 50, 55, 10, BLACK, PIXEL_1X1, SOLID);
		//Rectangle
		Gui_Draw_Rectangle(15, 10, 55, 50, BLACK, EMPTY, PIXEL_1X1);
		Gui_Draw_Rectangle(70, 10, 110, 50, RED, FULL, PIXEL_1X1);
		//Circle
		Gui_Draw_Circle(135, 30, 18, BLACK, EMPTY, PIXEL_1X1);
		Gui_Draw_Circle(176, 30, 18, BLACK, FULL, PIXEL_1X1);
		Gui_Draw_Str(10, 75, "abcdefg", &Font12, WHITE, BLACK); //7*12
		Gui_Draw_Str(10, 100, "ABCabc012345", &Font16, WHITE, BLACK); //11*16
		Gui_Draw_Str(10, 130, "1.54\" E-Paper", &Font20, WHITE, RED); //14*20
		Gui_Draw_Str(10, 160, "SEENGREAT", &Font24, WHITE, RED); //17*24
		//TEST_PIN = 1;
		EPD_Display(BWimage,RWimage);
		//TEST_PIN = 0;
		EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
		delay(2000); //2s					

		//Clear screen
		EPD_HW_Init(); //Electronic paper initialization
		EPD_WhiteScreen_White(); //Show all white
		EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
		delay(2000);	
		printf("end\n");
		Gpio_unexport();
        return 0;
	}
}	


