/*
 * lcd_lib.h
 *
 * Created: 10.12.2012 0:18:23
 *  Author: Xplain
 */ 


#ifndef LCD_LIB_H_
#define LCD_LIB_H_


#define LcdMosi     6
#define LcdSclk     5
#define LcdSs       4
#define LcdLed      3
   
#define LcdPort     PC_ODR

#define CmdLcdMode  0
#define DataLcdMode 1

#define HIGH        1
#define LOW         0

#define INV_MODE_ON  0
#define INV_MODE_OFF 1
   
#define ClearBit(reg,bit)     reg &= (~(1<<(bit)))
#define SetBit(reg,bit)       reg |= (1<<(bit))
#define InvBit(reg,bit)       reg ^= 1<<bit

// Разрешение дисплея в пикселях
#define NLCD_X_RES	96		// разрешение по горизонтали
#define NLCD_Y_RES	68		// разрешение по вертикали

typedef enum{
    STRING1,
    STRING2,
    STRING3,
    STRING4,
    STRING5,
    STRING6,
    STRING7,
    STRING8
}LcdString;

// Видеобуфер. Работаем через буффер, так как из контроллера NOKIA1100 читать данные нельзя, а для
// графического режима нам нужно знать содержимое видеопамяти. (9 банков по 96 байт)  
static unsigned char lcd_memory[NLCD_X_RES-1][(NLCD_Y_RES/8)+1];
											
// Тукущие координаты (указатели) в видеобуфере
// nlcd_xcurr - в пикселах, nlcd_ycurr- в банках (строках)
static unsigned char lcd_xcurr, lcd_ycurr;

void DigitalWrite (char pin_sel, char pin_level);
void LcdSendBit (char Bit);
void LcdWrite(char cmd,char data);
void LcdClear(void);
void LcdInit(void);
void LcdGotoXY(char x,char y);
void LcdGotoXYpix(char x,char y);
void LcdPutc(unsigned char c, unsigned char inv);
void LcdPutcWide(unsigned char c);
void LcdInverse(unsigned char mode);
void LcdPrint(char * message);
void LcdPrintF(unsigned char * message);
void LcdPrintWide(unsigned char * message);
void LcdTest (void);

#endif /* LCD_LIB_H_ */