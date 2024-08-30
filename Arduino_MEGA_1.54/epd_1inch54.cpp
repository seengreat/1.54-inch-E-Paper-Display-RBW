#include <SPI.h>
#include <string.h>
#include <stdlib.h>		

#include "epd_1inch54.h"
#include "epd_gui.h"

//Configure the serial port to use the standard printf function
//start
int serial_putc( char c, struct __file * )
{
    Serial.write( c );
    return c;
}
void printf_begin(void)
{
    fdevopen( &serial_putc, 0 );
}
//end
   
///////////////////EXTERNAL FUNCTION////////////////////////////////////////////////////////////////////////
/////////////////////delay//////////////////////////////////////
void driver_delay_us(unsigned int xus)  //1us
{
    for(;xus>1;xus--);
}
void driver_delay_xms(unsigned long xms) //1ms
{  
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
    {
        for(i=0; i<256; i++);
    }
}
void DELAY_S(unsigned int delaytime)     
{
    int i,j,k;
    for(i=0;i<delaytime;i++)
    {
        for(j=0;j<4000;j++)           
        {
            for(k=0;k<222;k++);               
        }
    }
}
//////////////////////SPI///////////////////////////////////
void SPI_Delay(unsigned char xrate)
{
  unsigned char i;
  while(xrate)
  {
      for(i=0;i<2;i++);
      xrate--;
  }
}
void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;  
    SPI_Delay(1);
    for(i=0; i<8; i++)   
    {
        EPD_W21_CLK_0;
        SPI_Delay(1);
        if(value & 0x80)
           EPD_W21_MOSI_1;
        else
           EPD_W21_MOSI_0;   
        value = (value << 1); 
        SPI_Delay(1);
        driver_delay_us(1);
        EPD_W21_CLK_1; 
        SPI_Delay(1);
    }
}

void Epaper_Write_Command(unsigned char command)
{
   SPI_Delay(1);
   EPD_W21_CS_0;                   
   EPD_W21_DC_0;   // command write
   SPI_Write(command);
   EPD_W21_CS_1;
}
void Epaper_Write_Data(unsigned char data)
{
   SPI_Delay(1);
   EPD_W21_CS_0;                   
   EPD_W21_DC_1;   // command write
   SPI_Write(data);
   EPD_W21_CS_1;
}

/////////////////EPD settings Functions/////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void EPD_HW_Init(void)
{
  EPD_W21_RST_0;     
  delay(1); 
  EPD_W21_RST_1; //hard reset  
  delay(1); 

  Epaper_READBUSY();   
  Epaper_Write_Command(0x12);  //SWRESET
  Epaper_READBUSY();   
    
  Epaper_Write_Command(0x01); //Driver output control      
  Epaper_Write_Data(0xC7);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);

  Epaper_Write_Command(0x11); //data entry mode       
  Epaper_Write_Data(0x01);

  Epaper_Write_Command(0x44); //set Ram-X address start/end position   
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x18);    //0x18-->(24+1)*8=200

  Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
  Epaper_Write_Data(0xC7);   //0xC7-->(199+1)=200
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00); 

  Epaper_Write_Command(0x3C); //BorderWavefrom
  Epaper_Write_Data(0x05);  
      
  Epaper_Write_Command(0x18); 
  Epaper_Write_Data(0x80);  

  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(0x00);
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;    
  Epaper_Write_Data(0xC7);
  Epaper_Write_Data(0x00);
  Epaper_READBUSY();
  
}
//////////////////////////////All screen update////////////////////////////////////////////
void EPD_WhiteScreen_ALL(const unsigned char * BWdatas,const unsigned char * RWdatas)
{
   unsigned int i;
   Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     Epaper_Write_Data(pgm_read_byte(&BWdatas[i]));
   }
   Epaper_Write_Command(0x26);   //write RAM for red(0)/white (1)
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     Epaper_Write_Data(~pgm_read_byte(&RWdatas[i]));
   }
   EPD_Update();   
}

/////////////////////////////////////////////////////////////////////////////////////////
void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); 
  Epaper_Write_Data(0xF7);   
  Epaper_Write_Command(0x20); 
  Epaper_READBUSY();  

}

void EPD_DeepSleep(void)
{  
  Epaper_Write_Command(0x10); //enter deep sleep
  Epaper_Write_Data(0x01); 
  delay(100);
}
void Epaper_READBUSY(void)
{ 
  while(1)
  {   //=1 BUSY
     if(isEPD_W21_BUSY==0) break;
  }  
}

void EPD_HW_Init_GUI(void)
{
  EPD_W21_RST_0;  // Module reset   
  delay(10);//At least 10ms delay 
  EPD_W21_RST_1;
  delay(10); //At least 10ms delay 
  
  Epaper_READBUSY();   
  Epaper_Write_Command(0x12);  //SWRESET
  Epaper_READBUSY();   
    
  Epaper_Write_Command(0x01); //Driver output control 
  delay(10); //At least 10ms delay  
  Epaper_Write_Data(0xC7);
  delay(10); //At least 10ms delay 
  Epaper_Write_Data(0x00);
  delay(10); //At least 10ms delay 
  Epaper_Write_Data(0x01);//0x00:Show normal 0x01:Show mirror

  Epaper_Write_Command(0x11); //data entry mode       
  Epaper_Write_Data(0x01);

  Epaper_Write_Command(0x44); //set Ram-X address start/end position   
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x18);    //0x18-->(24+1)*8=200

  Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
  Epaper_Write_Data(0xC7);    //0xC7-->(199+1)=200
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00); 

  Epaper_Write_Command(0x3C); //BorderWavefrom
  Epaper_Write_Data(0x05);  
      
  Epaper_Write_Command(0x18); //Read built-in temperature sensor
  Epaper_Write_Data(0x80);  

  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(0x00);
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;    
  Epaper_Write_Data(0xC7);
  Epaper_Write_Data(0x00);
  Epaper_READBUSY();
  
}



/////////////////////////////////Single display////////////////////////////////////////////////
// void EPD_WhiteScreen_Black(void)

// {
//     unsigned int i,k;
//     Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
//     for(k=0;k<200;k++)
//     {
//         for(i=0;i<25;i++)
//         {
//             Epaper_Write_Data(0x00);
//         }
//     }
//     EPD_Update();
// }

void EPD_WhiteScreen_White(void)

{
    unsigned int i,k;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
    for(k=0;k<200;k++)
    {
        for(i=0;i<25;i++)
        {
           Epaper_Write_Data(0xff);
        }
    }
    Epaper_Write_Command(0x26);  
    for(k=0;k<200;k++)
    {
        for(i=0;i<25;i++)
        {
           Epaper_Write_Data(0x00);
        }
    }
    EPD_Update();
}

void EPD_Display(unsigned char *BWImage,unsigned char *RWImage)
{
    unsigned int width, Height,i,j;
    width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    Epaper_Write_Command(0x24);
    for ( j = 0; j < Height; j++) 
    {
        for ( i = 0; i < width; i++) 
        {
           Epaper_Write_Data(BWImage[i + j * width]);
        }
    }
    Epaper_Write_Command(0x26);
    for ( j = 0; j < Height; j++) 
    {
        for ( i = 0; i < width; i++) 
        {
           Epaper_Write_Data(~RWImage[i + j * width]);
        }
    }
    EPD_Update();    
}


//////////////////////////////////END//////////////////////////////////////////////////
