#ifndef _1INCH54_EPD_H_
#define _1INCH54_EPD_H_

#define EPD_WIDTH   200
#define EPD_HEIGHT  200

//define pin conection in BCM number
#define PIN_CS      8   
#define PIN_RST     17  
#define PIN_DC      25   
#define PIN_BUSY    24 


//void SPI_Write(unsigned char value);
//void EPD_W21_WriteDATA(unsigned char data);
//void EPD_W21_WriteCMD(unsigned char command);
void EPD_GPIO_Init(void);

//EPD
void Epaper_READBUSY(void);
//void Epaper_Spi_WriteByte(unsigned char TxData);
void Epaper_Write_Command(unsigned char cmd);
void Epaper_Write_Data(unsigned char data);


void Epaper_HW_SW_RESET(void);
void EPD_HW_Init(void); //Electronic paper initialization
void EPD_Update(void);

void EPD_Part_Init(void);//Local refresh initialization
void EPD_Part_Update(void); 

void EPD_WhiteScreen_White(void);
void EPD_DeepSleep(void);
//Display 
void EPD_WhiteScreen_ALL(const unsigned char *BWdatas,const unsigned char *RWdatas);

//Display canvas function
void EPD_HW_Init_GUI(void); //EPD init GUI
void EPD_Display(unsigned char *BWImage,unsigned char *RWImage); 
void EPD_HW_Init_P(void);
void EPD_Standby(void);

#endif
/***********************************************************
						end file
***********************************************************/


