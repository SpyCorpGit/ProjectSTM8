/*
 * nokia1202_test.c
 *
 * Created: 10.12.2012 0:17:18
 *  Author: Xplain
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "lcd_lib.h"
#include "lcd_font.h"

//******************************************************************************
//
void DigitalWrite (char pin_sel, char pin_level)
{
  if (pin_level != 0)
  {
	  SetBit(LcdPort,pin_sel);
  }  
  else
  {
	 ClearBit(LcdPort,pin_sel);
  }
  	
}

//******************************************************************************
//
void LcdSendBit (char Bit)
{
	DigitalWrite(LcdMosi,Bit);
	_delay_us(2);
	DigitalWrite(LcdSclk,HIGH);
	_delay_us(2);
	DigitalWrite(LcdSclk,LOW);
} 

//******************************************************************************
// 
void LcdWrite(char cmd,char data)
{
	DigitalWrite(LcdSs,LOW);
	LcdSendBit(cmd);
	for (int i=0;i<8;i++)
	{
		LcdSendBit(data & 0x80);
		data <<=1;
		_delay_us(50);
	}
	DigitalWrite(LcdSs,HIGH);
}

//******************************************************************************
// Очистка экрана
void LcdClear(void)
{
	LcdWrite(CmdLcdMode,0x40); // Y = 0
	LcdWrite(CmdLcdMode,0xB0);
	LcdWrite(CmdLcdMode,0x10); // X = 0
	LcdWrite(CmdLcdMode,0x00);
	
	lcd_xcurr=0; lcd_ycurr=0;		  // Устанавливаем в 0 текущие координаты в видеобуфере
	
	//LcdWrite(CmdLcdMode,0xAE); // disable display;
	
	for(unsigned int i=0;i<864;i++) LcdWrite(DataLcdMode,0x00);
	
	//LcdWrite(CmdLcdMode,0x07);
	//LcdWrite(CmdLcdMode,0xAF); // enable display;
}

//******************************************************************************
// Инициализация контроллера
void LcdInit(void)
{	
	// Инициализируем порт на вывод для работы с LCD-контроллером
	LcdDdr |= (1<<LcdSclk)|(1<<LcdMosi)|(1<<LcdSs)|(1<<LcdLed);
    
	//DigitalWrite(LcdLed,HIGH);
	
	DigitalWrite(LcdSs,LOW);

	_delay_ms(10);

	LcdWrite(CmdLcdMode,0xE2); // *** SOFTWARE RESET 

	LcdWrite(CmdLcdMode,0x3A); // *** Use internal oscillator
	LcdWrite(CmdLcdMode,0xEF); // *** FRAME FREQUENCY:
	LcdWrite(CmdLcdMode,0x04); // *** 80Hz
	LcdWrite(CmdLcdMode,0xD0); // *** 1:65 divider
	
	LcdWrite(CmdLcdMode,0x20); // Запись в регистр Vop 
	LcdWrite(CmdLcdMode,0x90);

	LcdWrite(CmdLcdMode,0xA4); // all on/normal display

	LcdWrite(CmdLcdMode,0x2F); // Power control set(charge pump on/off)
									  // Определяет контрастность

	LcdWrite(CmdLcdMode,0x40); // set start row address = 0
	LcdWrite(CmdLcdMode,0xB0); // установить Y-адрес = 0
	LcdWrite(CmdLcdMode,0x10); // установить X-адрес, старшие 3 бита
	LcdWrite(CmdLcdMode,0x0);  // установить X-адрес, младшие 4 бита

	//LcdWrite(CmdLcdMode,0xC8); // mirror Y axis (about X axis)
	//LcdWrite(CmdLcdMode,0xA1); // Инвертировать экран по горизонтали

	LcdWrite(CmdLcdMode,0xAC); // set initial row (R0) of the display
	LcdWrite(CmdLcdMode,0x07);

	//LcdWrite(CmdLcdMode,0xF9); //

	LcdWrite(CmdLcdMode,0xAF); // экран вкл/выкл
	
	LcdClear(); // clear LCD
}

//******************************************************************************
// Устанавливает курсор в необходимое положение. Отсчет начинается в верхнем 
// левом углу. По горизонтали 16 знакомест, по вертикали - 8
//  x: 0..15
//  y: 0..7    
void LcdGotoXY(char x,char y)
{
	x=x*6;	// Переходим от координаты в знакоместах к координатам в пикселях

	lcd_xcurr=x;
	lcd_ycurr=y;

	LcdWrite(CmdLcdMode,(0xB0|(y&0x0F)));      // установка адреса по Y: 0100 yyyy         
    LcdWrite(CmdLcdMode,(0x00|(x&0x0F)));      // установка адреса по X: 0000 xxxx - биты (x3 x2 x1 x0)
    LcdWrite(CmdLcdMode,(0x10|((x>>4)&0x07))); // установка адреса по X: 0010 0xxx - биты (x6 x5 x4)

}

//******************************************************************************
// Устанавливает курсор в пикселях. Отсчет начинается в верхнем 
// левом углу. По горизонтали 96 пикселей, по вертикали - 67
//  x: 0..95
//  y: 0..67
void LcdGotoXYpix(char x,char y)
{
	lcd_xcurr=x;
	lcd_ycurr=y/8;

	LcdWrite(CmdLcdMode,(0xB0|(lcd_ycurr&0x0F)));      // установка адреса по Y: 0100 yyyy         
    LcdWrite(CmdLcdMode,(0x00|(x&0x0F)));      // установка адреса по X: 0000 xxxx - биты (x3 x2 x1 x0)
    LcdWrite(CmdLcdMode,(0x10|((x>>4)&0x07))); // установка адреса по X: 0010 0xxx - биты (x6 x5 x4)
}

//******************************************************************************
// Вывод символа на LCD-экран NOKIA 1202 в текущее место
//  c: код символа
void LcdPutc(unsigned char c)
{
	for ( unsigned char i = 0; i < 5; i++ )
		 LcdWrite(DataLcdMode,pgm_read_byte(&(lcd_Font[c-32][i])));
    
	LcdWrite(DataLcdMode,0x00); // Зазор между символами по горизонтали в 1 пиксель
}

//******************************************************************************
// Вывод широкого символа на LCD-экран NOKIA 1202 в текущее место
//  c: код символа
void LcdPutcWide(unsigned char c)
{
	for ( unsigned char i = 0; i < 5; i++ )
	{
		unsigned char glyph = pgm_read_byte(&(lcd_Font[c-32][i]));
	    LcdWrite(DataLcdMode,glyph);
	   	LcdWrite(DataLcdMode,glyph);
    }
    
	LcdWrite(DataLcdMode,0x00); // Зазор между символами по горизонтали в 1 пиксель
	LcdWrite(DataLcdMode,0x00); // Можно сделать две линии
}

//******************************************************************************
// Устанавливаер режим инверсии всего экрана. Данные на экране не изменяются, только инвертируются
//  mode: //******************************************************************************
// Устанавливаер режим инверсии всего экрана. Данные на экране не изменяются, только инвертируются
//  mode: INV_MODE_ON или INV_MODE_OFF
void LcdInverse(unsigned char mode)
{
	if (mode) LcdWrite(CmdLcdMode,0xA6);
	 else LcdWrite(CmdLcdMode,0xA7);
}

//******************************************************************************
// Вывод строки символов на LCD-экран NOKIA 1100 в текущее место. Если строка выходит
// за экран в текущей строке, то остаток переносится на следующую строку.
//  message: указатель на строку символов. 0x00 - признак конца строки.
void LcdPrint(char * message)
{
	while (*message) LcdPutc(*message++); // Конец строки обозначен нулем
}

//******************************************************************************
// Вывод строки символов на LCD-экран NOKIA 1202 в текущее место из программной памяти.
// Если строка выходит за экран в текущей строке, то остаток переносится на следующую строку.
//  message: указатель на строку символов в программной памяти. 0x00 - признак конца строки.
void LcdPrintF(unsigned char * message)
{
	unsigned char data;
	while (data=pgm_read_byte(message), data)
	{ 
    	LcdPutc(data);
		message++;
    }
}

//******************************************************************************
// Вывод строки символов двойной ширины на LCD-экран NOKIA 1202 в текущее место
// из оперативной памяти. Если строка выходит за экран в текущей строке, то остаток
// переносится на следующую строку.
//  message: указатель на строку символов в программной памяти. 0x00 - признак конца строки.
void LcdPrintWide(unsigned char * message)
{
	while (*message) LcdPutcWide(*message++);  // Конец строки обозначен нулем
}

void LcdTest (void)
{
	LcdInit();

	DigitalWrite(LcdLed,HIGH);

	LcdPutc('A');
	LcdPutcWide('A');
	LcdPutc('M');
	LcdGotoXY(0,2);
	LcdPutcWide('S');
	LcdGotoXY(0,3);
	LcdPrintF(hello);
	LcdGotoXY(0,4);
	LcdPrintWide("Hello");
}