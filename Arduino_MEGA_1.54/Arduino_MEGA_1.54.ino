#include "epd_demo.h"
#include "epd_1inch54.h"
#include "epd_gui.h"
#include "fonts.h"
#include <SPI.h>
/***************************************************************************************
 * Project  :
 * Experimental Platform :Arduino Mega 2560 + 1.54inch E-Paper Display
 * Hardware Connection :Arduino Mega 2560-> 1.54inch E-Paper
 *      D53 --> CS
 *      D52 --> CLK
 *      D51 --> MOSI
 *      D8  --> DC
 *      D9  --> RST
 *      D10  --> BUSY
 * Author      : Andy Li
 * Web Site    :www.seengreat.com
***************************************************************************************/
/******************************
 * image :
 * 0------------> x (1~200) 
 * |
 * |
 * |
 * |
 * y (1~200)
 * 
 * Pin wiring definion
 * VCC---->5V
 * GND---->GND
 * RST---->D9
 * BUSY--->D10
 * D/C---->D8
 * MOSI--->D51
 * CLK---->D52
 * CS----->D53
 * */
#define SPI_USE_HARDWARE 0 //1:using hardware SPI; 0: Using GPIO to emulate SPI
void setup() {
    Serial.begin(115200);
    printf_begin();
   printf("Init gpio for arduino\r\n");
   pinMode(BUSY_Pin, INPUT); 
   pinMode(RES_Pin, OUTPUT);  
   pinMode(DC_Pin, OUTPUT);    
   pinMode(CS_Pin, OUTPUT);    
   pinMode(SCK_Pin, OUTPUT);    
   pinMode(SDI_Pin, OUTPUT);
   pinMode(TEST_Pin, OUTPUT);    
   
}

// unsigned char BWimage[5000];//Define canvas space  
// unsigned char RWimage[5000];

//Tips//
/*
1.When the e-paper is refreshed in full screen, the picture flicker is a normal phenomenon, and the main function is to clear the display afterimage in the previous picture.
2.When the partial refresh is performed, the screen does not flash.
3.After the e-paper is refreshed, you need to put it into sleep mode, please do not delete the sleep command.
4.Please do not take out the electronic paper when power is on.
5.Wake up from sleep, need to re-initialize the e-paper.
6.When you need to transplant the driver, you only need to change the corresponding IO. The BUSY pin is the input mode and the others are the output mode.
*/
void loop() {
    unsigned char i,Miao_L,Miao_H,Fen_L,Fen_H; 

    EPD_HW_Init(); //Electronic paper initialization
    EPD_WhiteScreen_ALL(gImage_0,gImage_1); //Refresh the picture in full screen
    EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);

    //Clear
    EPD_HW_Init(); //EPD init
    EPD_WhiteScreen_ALL(gImage_2,gImage_3);//EPD_picture1  
    EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);     
////////////////////////////////////////////////////////////////////////  
  //Clear screen
    EPD_HW_Init(); //Electronic paper initialization
    EPD_WhiteScreen_White(); //Show all white
    EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);    
  //   ///////////////////////////GUI///////////////////////////////////////////////////////////////////////////////////
  //  //Data initialization settings
	// 	Image_Init(BWimage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE); //Set screen size and display orientation
  //   Image_Init(RWimage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
  //   /**************Drawing**********************/
  //  EPD_HW_Init_GUI(); //EPD init GUI
  //  #if 1
  //   //Draw black and white pictures
	// 	Gui_SelectImage(BWimage);
	// 	Gui_Clear(WHITE);
  //   //Point   
  //   Gui_Draw_Point(3, 1, BLACK, PIXEL_1X1, DOT_STYLE_DFT);
  //   Gui_Draw_Point(3, 15, BLACK, PIXEL_2X2, DOT_STYLE_DFT);
  //   Gui_Draw_Point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT);
  //   Gui_Draw_Point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT);
  //   //Line
  //   Gui_Draw_Line(5, 5, 30, 110, BLACK, PIXEL_1X1, SOLID);
  //   Gui_Draw_Line(5, 30, 45, 30, BLACK, PIXEL_1X1, SOLID);
  //   //Rectangle
  //   Gui_Draw_Rectangle(15, 5, 55, 45, BLACK, EMPTY, PIXEL_1X1);
  //   Gui_Draw_Rectangle(75, 5, 110, 45, BLACK, FULL, PIXEL_1X1);
  //   //Circle
  //   Gui_Draw_Circle(150, 25, 18, BLACK, EMPTY, PIXEL_1X1);
  //   Gui_Draw_Circle(216, 25, 18, BLACK, FULL, PIXEL_1X1);

  //   TEST_1;
  //   EPD_Display(BW,BWimage);
  //  #endif

  //  #if 0
  //  //Draw red and white pictures
	// 	Gui_SelectImage(RWimage);
	// 	Gui_Clear(WHITE);

  //   Gui_Draw_Str(20, 50, "abcdefg", &Font12, WHITE, RED); //7*12
  //   Gui_Draw_Str(100, 50, "ABCabc012345", &Font16, WHITE, RED); //11*16
  //   Gui_Draw_Str(40, 70, "2.13\" E-Paper", &Font20, WHITE, RED); //14*20
  //   Gui_Draw_Str(60, 95, "SEENGREAT", &Font24, WHITE, RED); //17*24

  //   TEST_1;
  //   EPD_Display(RW,RWimage);//display image
  //  #endif
  //   TEST_0;  
  //   EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
  //   delay(2000);         
  
  //   //Clear screen
  //   EPD_HW_Init(); //Electronic paper initialization
  //   EPD_WhiteScreen_White(); //Show all white
  //   EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
  //   delay(2000);  

    while(1); 
}
