#include "1in54_epd.h"

/****************************
* PB12-----SPI2_NSS
* PB13-----SPI2_SCK
* PB15-----SPI2_MOSI
* PA8------DC
* PA11-----RST
* PA12-----BUSY
***************************/

void EPD_GPIO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
										
		
		 //CS-->PB12   SCK-->PB13  SDO--->PB15 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;		//Port configuration
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		
		GPIO_Init(GPIOB, &GPIO_InitStructure);	  	
			
		 // D/C--->PA8	   RES-->PA11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;		//Port configuration
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		
		GPIO_Init(GPIOA, &GPIO_InitStructure);	  				     		
		
		// BUSY--->PA12
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//Pull up input
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);				//Initialize GPIO
		
		 //TEST_PIN 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//Port configuration
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		TEST_PIN = 0;
}



void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;

    for(i=0; i<8; i++)   
    {
        EPD_W21_CLK = 0;
        if(value & 0x80)
        	EPD_W21_MOSI = 1;
        else
        	EPD_W21_MOSI = 0;		
        value = (value << 1); 
        EPD_W21_CLK = 1; 
    }
}

void EPD_W21_WriteCMD(unsigned char command)
{
  EPD_W21_CS = 0;                   
	EPD_W21_DC = 0;		// command write
	SPI_Write(command);
	EPD_W21_CS = 1;
}
void EPD_W21_WriteDATA(unsigned char data)
{
  EPD_W21_CS = 0;                   
	EPD_W21_DC = 1;		// data write
	SPI_Write(data);
	EPD_W21_CS = 1;
}

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
           Epaper_Write_Data(~RWImage[i + j * width]);
        }
    }
    EPD_Update();    
}

void Epaper_Spi_WriteByte(unsigned char TxData)
{				   			 
		unsigned char TempData;
		unsigned char scnt;
		TempData=TxData;

		EPD_W21_CLK = 0;  
		for(scnt=0;scnt<8;scnt++)
		{ 
				if(TempData&0x80)
					EPD_W21_MOSI = 1 ;
				else
					EPD_W21_MOSI = 0 ;
				EPD_W21_CLK = 1;  
				EPD_W21_CLK = 0;  
				TempData=TempData<<1;
		}
}

void Epaper_READBUSY(void)
{ 
	
		while(1)
		{	
       delay_xms(10);
			 //=1 BUSY
			 if(isEPD_W21_BUSY==0) break;
		}  
}

void Epaper_Write_Command(unsigned char cmd)
{
		EPD_W21_CS = 1;
		EPD_W21_CS = 0;
		EPD_W21_DC = 0;  // D/C#   0:command  1:data

		Epaper_Spi_WriteByte(cmd);
		EPD_W21_CS = 1;
}

void Epaper_Write_Data(unsigned char data)
{
		EPD_W21_CS = 1;
		EPD_W21_CS = 0;
		EPD_W21_DC = 1;  // D/C#   0:command  1:data

		Epaper_Spi_WriteByte(data);
		EPD_W21_CS = 1;
}


	
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

//SSD1681
void EPD_HW_Init(void)
{
		EPD_W21_RST = 0;  // Module reset   
		delay_xms(10);//At least 10ms delay 
		EPD_W21_RST = 1;
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
		EPD_W21_RST = 0;  // Module reset   
		delay_xms(10);//At least 10ms delay 
		EPD_W21_RST = 1;
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
void EPD_WhiteScreen_ALL(const unsigned char * BWdatas,const unsigned char * RWdatas)
{
   unsigned int i;	
   Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<5000;i++)
   {               
       Epaper_Write_Data(*BWdatas);
			   BWdatas++;
   }
	 Epaper_Write_Command(0x26);   //write RAM for red(0)/white (1)
   for(i=0;i<5000;i++)
   {               
       Epaper_Write_Data(~*RWdatas);
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

