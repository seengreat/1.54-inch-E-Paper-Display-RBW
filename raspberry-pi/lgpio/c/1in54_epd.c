#include <string.h>
#include <stdlib.h>	
#include <stdio.h>
#include <stdio.h>	
#include "sg_lgpio.h"
#include "1in54_epd.h"

//define pin conection in wiringpi number
/****************************
* PIN_CS      8
* PIN_RST     17 
* PIN_DC      25
* PIN_BUSY    24
***************************/
unsigned char tmp[2];

void EPD_GPIO_Init(void)
{
	Lgpio_export();
	printf("Init gpio for lgpio\r\n");
    // Lgpio_set_pinmode(PIN_CS, LGPIO_OUTPUT);
    Lgpio_set_pinmode(PIN_RST, LGPIO_OUTPUT);
    Lgpio_set_pinmode(PIN_DC, LGPIO_OUTPUT);
    Lgpio_set_pinmode(PIN_BUSY, LGPIO_INPUT);
    // pullUpDnControl(PIN_BUSY, PUD_DOWN);
    LG_SPI_Init(0,0,8000000);  
	if(SPI_handle < 0)//use lgpio Pin number
    { 
        printf("lgpio lib init failed! \r\n");
    } 
}



//void SPI_Write(unsigned char value)                                    
//{                                                           
    //unsigned char i;

    //for(i=0; i<8; i++)   
    //{
        //EPD_W21_CLK = 0;
        //if(value & 0x80)
        	//EPD_W21_MOSI = 1;
        //else
        	//EPD_W21_MOSI = 0;		
        //value = (value << 1); 
        //EPD_W21_CLK = 1; 
    //}
//}

//void EPD_W21_WriteCMD(unsigned char command)
//{
    //digitalWrite(PIN_CS, 0);                    
	//digitalWrite(PIN_DC, 0); 		// command write
	////SPI_Write(command);
	//wiringPiSPIDataRW(0,&command,1); 
	//digitalWrite(PIN_CS, 1); 
//}
//void EPD_W21_WriteDATA(unsigned char data)
//{
    //digitalWrite(PIN_CS, 0);                    
	//digitalWrite(PIN_DC, 1); 		// data write
	////SPI_Write(data);
	//wiringPiSPIDataRW(0,&data,1); 
	//digitalWrite(PIN_CS, 1); 
//}

void delay_xms(unsigned int xms)
{
	unsigned int i;
	while(xms--)
	{
		i=12000;
		while(i--);
	}
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
           Epaper_Write_Data(~ RWImage[i + j * width]);
        }
    }
    EPD_Update();		 
}


void Epaper_READBUSY(void)
{ 
	while(1)
	{	 //=1 BUSY
		 if(Lgpio_read_pin(PIN_BUSY)==LGPIO_LOW) 
		 break;
	}  
}

void Epaper_Write_Command(unsigned char cmd)
{
	tmp[0] = cmd;
	// digitalWrite(PIN_CS, 1); 
	// digitalWrite(PIN_CS, 0); 
	Lgpio_write_pin(PIN_DC, LGPIO_LOW);   // D/C#   0:command  1:data
    lgSpiWrite(SPI_handle,tmp, 1);
//	Epaper_Spi_WriteByte(cmd);
	// digitalWrite(PIN_CS, 1); 
}

void Epaper_Write_Data(unsigned char data)
{
	tmp[0] = data;
	// digitalWrite(PIN_CS, 1); 
	// digitalWrite(PIN_CS, 0); 
	Lgpio_write_pin(PIN_DC, LGPIO_HIGH);   // D/C#   0:command  1:data
	// wiringPiSPIDataRW(0,&data,1); 
    lgSpiWrite(SPI_handle,tmp, 1);
//	Epaper_Spi_WriteByte(data);
	// digitalWrite(PIN_CS, 1); 
}


	
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

//SSD1681
void EPD_HW_Init(void)
{

    Lgpio_write_pin(PIN_RST, LGPIO_LOW); // Module reset   
	delay_xms(10);//At least 10ms delay 
    Lgpio_write_pin(PIN_RST, LGPIO_HIGH);
	delay_xms(10); //At least 10ms delay 
		
	Epaper_READBUSY();   
	Epaper_Write_Command(0x12);  //SWRESET
	Epaper_READBUSY();   
			
	Epaper_Write_Command(0x01); //Driver output control     
	delay_xms(10); //At least 10ms delay 	
	Epaper_Write_Data(0xC7);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x00);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x00);

	Epaper_Write_Command(0x11); //data entry mode       
	Epaper_Write_Data(0x01);

	Epaper_Write_Command(0x44); //set Ram-X address start/end position   
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x18);    //0x0C-->(18+1)*8=200

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

void EPD_HW_Init_GUI(void)
{
	Lgpio_write_pin(PIN_RST, LGPIO_LOW); // Module reset   
	delay_xms(10);//At least 10ms delay 
    Lgpio_write_pin(PIN_RST, LGPIO_HIGH);
	delay_xms(10); //At least 10ms delay 
	
	Epaper_READBUSY();   
	Epaper_Write_Command(0x12);  //SWRESET
	Epaper_READBUSY();   
		
	Epaper_Write_Command(0x01); //Driver output control 
	delay_xms(10); //At least 10ms delay 	
	Epaper_Write_Data(0xC7);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x00);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x01);//0x00:Show normal 0x01:Show mirror

	Epaper_Write_Command(0x11); //data entry mode       
	Epaper_Write_Data(0x01);

	Epaper_Write_Command(0x44); //set Ram-X address start/end position   
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x18);    //0x0C-->(18+1)*8=200

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
/////////////////////////////////////////////////////////////////////////////////////////
/*When the electronic paper screen is updated, do not unplug the electronic paper to avoid damage to the screen*/

void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); //Display Update Control
  Epaper_Write_Data(0xF7);   
  Epaper_Write_Command(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   

}

//////////////////////////////All screen update////////////////////////////////////////////
void EPD_WhiteScreen_ALL(const unsigned char *BWdatas,const unsigned char *RWdatas)
{
   unsigned int i;	
   Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<5000;i++)
   {               
       Epaper_Write_Data(*BWdatas);
	   BWdatas++;
   }
      Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
   for(i=0;i<5000;i++)
   {               
       Epaper_Write_Data(~ *RWdatas);
	   RWdatas++;
   }
   EPD_Update();	 
}


void EPD_DeepSleep(void)
{  	
    Epaper_Write_Command(0x10); //enter deep sleep
    Epaper_Write_Data(0x01); 
    delay_xms(100);
}


/////////////////////////////////Single display////////////////////////////////////////////////

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


/***********************************************************
						end file
***********************************************************/

