#pragma once
#include <stdint.h>
#include "i2c.h"




#define OLED_adress 0x78
#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3 
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D
#define OLED_SWITCHCAPVCC 0x2
#define OLED_NOP 0xE3

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_BUFFERSIZE (OLED_WIDTH*OLED_HEIGHT)/8
#define OLED_DEFAULT_SPACE 5

#define COMAND 0x00
#define DATA 0x40



enum enPikiNames  // требуется чёткое соответстиве порядку инициализации массива структур Пики
{
	PikiTahoma2 = 0,
	PikiTahoma4 = 11,
	PikiTahoma6 = 22,
	PikiBatteryKorp=33,
	PikiBatteryDelenie
	
	
	
	
	
	
	
	
		
};
enum LCD_PrintType
{
	LCDpt_NOINV = 0,
	LCDpt_INV   = 1,
	LCDpt_DEL   = 3
		         
};


char HexToBitIn32_Char[32];
void HexToChar(unsigned int H);
void LCD_PrintReg(unsigned int RegHex, char* RegName);
void LCD_init(void);

void sendCommand(unsigned char command);
void LCD_Clear(void);
void LCD_Char2(unsigned int c);
void LCD_GoTo(unsigned char x, unsigned char y);
void LCD_DrawImage(unsigned char num_image);
void LCD_String(char *string);
void LCD_Num_To_Str(unsigned int value, unsigned char nDigit);
void LCD_PrintPik(uint8_t Page, uint8_t Segment, enum enPikiNames Pik, enum LCD_PrintType PT);






//#define Piki_0 0;
//#define Piki_1 1;
//#define Piki_2 2;
//#define Piki_3 3;
//#define Piki_4 4;
//#define Piki_5 5;
//#define Piki_6 6;
//#define Piki_7 7;
//#define Piki_8 8;
//#define Piki_9 9;
//#define Piki_10 10;

struct Pik
{
	uint8_t Pages;
	uint8_t Segments;
	uint16_t FirstByte;
	//const uint8_t   Pipik[];
};




