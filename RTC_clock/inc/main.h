

#ifndef MAIN_H_
#define MAIN_H_

#include <iostm8s103f3.h> 
#include "intrinsics.h"
#include "lcd_lib.h"

volatile unsigned int led_delay  = 0;

#include "buttons.h"
#include "pca2129_drv.h"

typedef enum{
   MAIN_LEVEL =1,
   TIME_LEVEL,
   TIMER_LEVEL,
   SET_LEVEL    
}MenuLevel;

unsigned char rBCD (unsigned char y);

void InitSystemClock(void);
void Port_Init(void); 
void InitTimer2(void);
void InitTimer4(void);
  
void main_menu(void);
void time_date_menu(void);
void timers_menu(void);
void tester_menu(void);

#endif /* MAIN_H_ */
