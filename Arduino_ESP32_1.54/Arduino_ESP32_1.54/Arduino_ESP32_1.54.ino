#include "epd_demo.h"
#include "epd_1inch54.h"
#include "epd_gui.h"
#include "fonts.h"
#include <SPI.h>
/***************************************************************************************
 * Project  :
 * Experimental Platform :Arduino ESP32 + 1.54inch E-Paper Display
 * Hardware Connection :Arduino ESP32-> 1.54inch E-Paper
 *      IO27 --> CS
 *      IO18 --> CLK
 *      IO23 --> MOSI
 *      IO14  --> DC
 *      IO33  --> RST
 *      IO13  --> BUSY
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
 * */
void setup() {
//    Serial.begin(115200);
//    printf_begin();
//   printf("Init gpio for arduino\r\n");

   pinMode(BUSY_Pin, INPUT_PULLUP); 
   pinMode(RES_Pin, OUTPUT);
   EPD_W21_RST_1;  
   pinMode(DC_Pin, OUTPUT);    
   pinMode(CS_Pin, OUTPUT);    
   pinMode(SCK_Pin, OUTPUT);    
   pinMode(SDI_Pin, OUTPUT);
   pinMode(2, OUTPUT); //LED 
   digitalWrite(2,HIGH);
   
}


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
    ///////////////////////////GUI///////////////////////////////////////////////////////////////////////////////////
   //Data initialization settings
		Image_Init(BWimage, EPD_WIDTH, EPD_HEIGHT, 90, WHITE); //Set screen size and display orientation
    Image_Init(RWimage, EPD_WIDTH, EPD_HEIGHT, 90, WHITE);
    /**************Drawing**********************/
    EPD_HW_Init_GUI(); //EPD init GUI
    Gui_Clear(WHITE);
    //Point   
    Gui_Draw_Point(5, 10, BLACK, PIXEL_1X1, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 25, BLACK, PIXEL_2X2, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT);
    //Line
    Gui_Draw_Line(15, 10, 55, 50, BLACK, PIXEL_1X1, SOLID);
    Gui_Draw_Line(55, 10, 15, 50, BLACK, PIXEL_1X1, SOLID);
    //Rectangle
    Gui_Draw_Rectangle(15, 10, 55, 50, BLACK, EMPTY, PIXEL_1X1);
    Gui_Draw_Rectangle(70, 10, 110, 50, BLACK, FULL, PIXEL_1X1);
    //Circle
    Gui_Draw_Circle(135, 30, 18, BLACK, EMPTY, PIXEL_1X1);
    Gui_Draw_Circle(176, 30, 18, RED, FULL, PIXEL_1X1);

    Gui_Draw_Str(10, 75, "abcdefg", &Font12, WHITE, BLACK); //7*12
    Gui_Draw_Str(10, 100, "ABCabc012345", &Font16, WHITE, BLACK); //11*16
    Gui_Draw_Str(10, 130, "1.54\" E-Paper", &Font20, WHITE, RED); //14*20
    Gui_Draw_Str(10, 160, "SEENGREAT", &Font24, WHITE, RED); //17*24
    TEST_1;
    EPD_Display(BWimage,RWimage);//display image
    TEST_0;  
    EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);         
  
    //Clear screen
    EPD_HW_Init(); //Electronic paper initialization
    EPD_WhiteScreen_White(); //Show all white
    EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);  

    while(1); 
}
