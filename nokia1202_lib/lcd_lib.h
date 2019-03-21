/*
 * lcd_lib.h
 *
 * Created: 10.12.2012 0:18:23
 *  Author: Xplain
 */ 


#ifndef LCD_LIB_H_
#define LCD_LIB_H_

#ifdef _MEGA_LCD_CONNECT_
#define LcdPort    PORTA
#define LcdPin     PINA
#define LcdDdr     DDRA

#define LcdMosi     0
#define LcdSclk     1
#define LcdSs       2
#define LcdLed      3
#endif

#ifdef _XPLAIN_LCD_CONNECT_
#define LcdPort    PORTB.OUT
#define LcdPin     PORTB.IN
#define LcdDdr     PORTB.DIR

#define LcdMosi     4
#define LcdSclk     7
#define LcdSs       5
#define LcdLed      6
#endif

#define CmdLcdMode  0
#define DataLcdMode 1

#define HIGH        1
#define LOW         0

#define INV_MODE_ON  0
#define INV_MODE_OFF 1

#define ClearBit(reg,bit)     reg &= (~(1<<(bit)))
#define SetBit(reg,bit)       reg |= (1<<(bit))
#define InvBit(reg,bit)       reg ^= 1<<bit

// ���������� ������� � ��������
#define NLCD_X_RES	96		// ���������� �� �����������
#define NLCD_Y_RES	68		// ���������� �� ���������

// ����������. �������� ����� ������, ��� ��� �� ����������� NOKIA1100 ������ ������ ������, � ���
// ������������ ������ ��� ����� ����� ���������� �����������. (9 ������ �� 96 ����)  
static unsigned char lcd_memory[NLCD_X_RES-1][(NLCD_Y_RES/8)+1];
											
// ������� ���������� (���������) � �����������
// nlcd_xcurr - � ��������, nlcd_ycurr- � ������ (�������)
static unsigned char lcd_xcurr, lcd_ycurr;

void DigitalWrite (char pin_sel, char pin_level);
void LcdSendBit (char Bit);
void LcdWrite(char cmd,char data);
void LcdClear(void);
void LcdInit(void);
void LcdGotoXY(char x,char y);
void LcdGotoXYpix(char x,char y);
void LcdPutc(unsigned char c);
void LcdPutcWide(unsigned char c);
void LcdInverse(unsigned char mode);
void LcdPrint(char * message);
void LcdPrintF(unsigned char * message);
void LcdPrintWide(unsigned char * message);
void LcdTest (void);

#endif /* LCD_LIB_H_ */