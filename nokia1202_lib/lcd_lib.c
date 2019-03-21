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
// ������� ������
void LcdClear(void)
{
	LcdWrite(CmdLcdMode,0x40); // Y = 0
	LcdWrite(CmdLcdMode,0xB0);
	LcdWrite(CmdLcdMode,0x10); // X = 0
	LcdWrite(CmdLcdMode,0x00);
	
	lcd_xcurr=0; lcd_ycurr=0;		  // ������������� � 0 ������� ���������� � �����������
	
	//LcdWrite(CmdLcdMode,0xAE); // disable display;
	
	for(unsigned int i=0;i<864;i++) LcdWrite(DataLcdMode,0x00);
	
	//LcdWrite(CmdLcdMode,0x07);
	//LcdWrite(CmdLcdMode,0xAF); // enable display;
}

//******************************************************************************
// ������������� �����������
void LcdInit(void)
{	
	// �������������� ���� �� ����� ��� ������ � LCD-������������
	LcdDdr |= (1<<LcdSclk)|(1<<LcdMosi)|(1<<LcdSs)|(1<<LcdLed);
    
	//DigitalWrite(LcdLed,HIGH);
	
	DigitalWrite(LcdSs,LOW);

	_delay_ms(10);

	LcdWrite(CmdLcdMode,0xE2); // *** SOFTWARE RESET 

	LcdWrite(CmdLcdMode,0x3A); // *** Use internal oscillator
	LcdWrite(CmdLcdMode,0xEF); // *** FRAME FREQUENCY:
	LcdWrite(CmdLcdMode,0x04); // *** 80Hz
	LcdWrite(CmdLcdMode,0xD0); // *** 1:65 divider
	
	LcdWrite(CmdLcdMode,0x20); // ������ � ������� Vop 
	LcdWrite(CmdLcdMode,0x90);

	LcdWrite(CmdLcdMode,0xA4); // all on/normal display

	LcdWrite(CmdLcdMode,0x2F); // Power control set(charge pump on/off)
									  // ���������� �������������

	LcdWrite(CmdLcdMode,0x40); // set start row address = 0
	LcdWrite(CmdLcdMode,0xB0); // ���������� Y-����� = 0
	LcdWrite(CmdLcdMode,0x10); // ���������� X-�����, ������� 3 ����
	LcdWrite(CmdLcdMode,0x0);  // ���������� X-�����, ������� 4 ����

	//LcdWrite(CmdLcdMode,0xC8); // mirror Y axis (about X axis)
	//LcdWrite(CmdLcdMode,0xA1); // ������������� ����� �� �����������

	LcdWrite(CmdLcdMode,0xAC); // set initial row (R0) of the display
	LcdWrite(CmdLcdMode,0x07);

	//LcdWrite(CmdLcdMode,0xF9); //

	LcdWrite(CmdLcdMode,0xAF); // ����� ���/����
	
	LcdClear(); // clear LCD
}

//******************************************************************************
// ������������� ������ � ����������� ���������. ������ ���������� � ������� 
// ����� ����. �� ����������� 16 ���������, �� ��������� - 8
//  x: 0..15
//  y: 0..7    
void LcdGotoXY(char x,char y)
{
	x=x*6;	// ��������� �� ���������� � ����������� � ����������� � ��������

	lcd_xcurr=x;
	lcd_ycurr=y;

	LcdWrite(CmdLcdMode,(0xB0|(y&0x0F)));      // ��������� ������ �� Y: 0100 yyyy         
    LcdWrite(CmdLcdMode,(0x00|(x&0x0F)));      // ��������� ������ �� X: 0000 xxxx - ���� (x3 x2 x1 x0)
    LcdWrite(CmdLcdMode,(0x10|((x>>4)&0x07))); // ��������� ������ �� X: 0010 0xxx - ���� (x6 x5 x4)

}

//******************************************************************************
// ������������� ������ � ��������. ������ ���������� � ������� 
// ����� ����. �� ����������� 96 ��������, �� ��������� - 67
//  x: 0..95
//  y: 0..67
void LcdGotoXYpix(char x,char y)
{
	lcd_xcurr=x;
	lcd_ycurr=y/8;

	LcdWrite(CmdLcdMode,(0xB0|(lcd_ycurr&0x0F)));      // ��������� ������ �� Y: 0100 yyyy         
    LcdWrite(CmdLcdMode,(0x00|(x&0x0F)));      // ��������� ������ �� X: 0000 xxxx - ���� (x3 x2 x1 x0)
    LcdWrite(CmdLcdMode,(0x10|((x>>4)&0x07))); // ��������� ������ �� X: 0010 0xxx - ���� (x6 x5 x4)
}

//******************************************************************************
// ����� ������� �� LCD-����� NOKIA 1202 � ������� �����
//  c: ��� �������
void LcdPutc(unsigned char c)
{
	for ( unsigned char i = 0; i < 5; i++ )
		 LcdWrite(DataLcdMode,pgm_read_byte(&(lcd_Font[c-32][i])));
    
	LcdWrite(DataLcdMode,0x00); // ����� ����� ��������� �� ����������� � 1 �������
}

//******************************************************************************
// ����� �������� ������� �� LCD-����� NOKIA 1202 � ������� �����
//  c: ��� �������
void LcdPutcWide(unsigned char c)
{
	for ( unsigned char i = 0; i < 5; i++ )
	{
		unsigned char glyph = pgm_read_byte(&(lcd_Font[c-32][i]));
	    LcdWrite(DataLcdMode,glyph);
	   	LcdWrite(DataLcdMode,glyph);
    }
    
	LcdWrite(DataLcdMode,0x00); // ����� ����� ��������� �� ����������� � 1 �������
	LcdWrite(DataLcdMode,0x00); // ����� ������� ��� �����
}

//******************************************************************************
// ������������� ����� �������� ����� ������. ������ �� ������ �� ����������, ������ �������������
//  mode: //******************************************************************************
// ������������� ����� �������� ����� ������. ������ �� ������ �� ����������, ������ �������������
//  mode: INV_MODE_ON ��� INV_MODE_OFF
void LcdInverse(unsigned char mode)
{
	if (mode) LcdWrite(CmdLcdMode,0xA6);
	 else LcdWrite(CmdLcdMode,0xA7);
}

//******************************************************************************
// ����� ������ �������� �� LCD-����� NOKIA 1100 � ������� �����. ���� ������ �������
// �� ����� � ������� ������, �� ������� ����������� �� ��������� ������.
//  message: ��������� �� ������ ��������. 0x00 - ������� ����� ������.
void LcdPrint(char * message)
{
	while (*message) LcdPutc(*message++); // ����� ������ ��������� �����
}

//******************************************************************************
// ����� ������ �������� �� LCD-����� NOKIA 1202 � ������� ����� �� ����������� ������.
// ���� ������ ������� �� ����� � ������� ������, �� ������� ����������� �� ��������� ������.
//  message: ��������� �� ������ �������� � ����������� ������. 0x00 - ������� ����� ������.
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
// ����� ������ �������� ������� ������ �� LCD-����� NOKIA 1202 � ������� �����
// �� ����������� ������. ���� ������ ������� �� ����� � ������� ������, �� �������
// ����������� �� ��������� ������.
//  message: ��������� �� ������ �������� � ����������� ������. 0x00 - ������� ����� ������.
void LcdPrintWide(unsigned char * message)
{
	while (*message) LcdPutcWide(*message++);  // ����� ������ ��������� �����
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