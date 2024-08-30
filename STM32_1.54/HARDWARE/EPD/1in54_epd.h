#ifndef _1INCH54_EPD_H_
#define _1INCH54_EPD_H_

#include "sys.h"

#define EPD_WIDTH   200
#define EPD_HEIGHT  200

//#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOB, GPIO_Pin_15)
//#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOB, GPIO_Pin_15)

//#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOB, GPIO_Pin_13)
//#define EPD_W21_CLK_1	GPIO_SetBits(GPIOB, GPIO_Pin_13)

//#define EPD_W21_CS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_12)
//#define EPD_W21_CS_1	GPIO_SetBits(GPIOB, GPIO_Pin_12)

//#define EPD_W21_DC_0	GPIO_ResetBits(GPIOA, GPIO_Pin_8)
//#define EPD_W21_DC_1	GPIO_SetBits(GPIOA, GPIO_Pin_8)

//#define EPD_W21_RST_0	GPIO_ResetBits(GPIOA, GPIO_Pin_11)
//#define EPD_W21_RST_1	GPIO_SetBits(GPIOA, GPIO_Pin_11)

//#define isEPD_W21_BUSY GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) // for solomen solutions

#define	EPD_W21_CS      PBout(12)
#define	EPD_W21_DC      PAout(8)
#define	EPD_W21_RST     PAout(11)
#define	isEPD_W21_BUSY  PAin(12)

#define EPD_W21_MOSI	  PBout(15)

#define EPD_W21_CLK	    PBout(13)

#define	TEST_PIN  PBout(8) 


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char data);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_GPIO_Init(void);

//EPD
void Epaper_READBUSY(void);
void Epaper_Spi_WriteByte(unsigned char TxData);
void Epaper_Write_Command(unsigned char cmd);
void Epaper_Write_Data(unsigned char data);


void Epaper_HW_SW_RESET(void);
void EPD_HW_Init(void); //Electronic paper initialization
void EPD_Update(void);
 

void EPD_WhiteScreen_White(void);
void EPD_DeepSleep(void);
//Display 
void EPD_WhiteScreen_ALL(const unsigned char * BWdatas,const unsigned char * RWdatas);
//Display canvas function
void EPD_HW_Init_GUI(void); //EPD init GUI
void EPD_Display(unsigned char *BWImage,unsigned char *RWImage); 
void EPD_HW_Init_P(void);
void EPD_Standby(void);

#endif
/***********************************************************
						end file
***********************************************************/


