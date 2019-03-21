
#include "main.h"

unsigned char treshold  = 60;
unsigned char state     = 0;
unsigned char flag      = 0;
unsigned char time_position[] ={11,14,6,9,14};

MenuLevel menu_state = MAIN_LEVEL;
//LcdString string;

struct Time dTime;
struct Date dDate;


#pragma vector = TIM4_OVR_UIF_vector

__interrupt void TIM4_UPD_OVF_IRQHandler(void){
    
    TIM4_SR_bit.UIF = 0;
	
    state = BUT_Debrief();
    
    if(state > 0 && treshold == 0){
      
      treshold=60;
    }
	treshold--;
  }

#pragma vector = TIM2_OVR_UIF_vector

__interrupt void TIM2_UPD_OVF_IRQHandler(void){
   
    TIM2_SR1_bit.UIF = 0; //приблизительно 15с
    
     led_delay++;
    if( led_delay == 30){
       
      PC_ODR_bit.ODR3 = 0;
      led_delay=0;
    }
  }

  static void delay(unsigned int t){
    
    while(t--);
  }

int main( void ){
  
  __disable_interrupt();
  
  dTime.hour  = 15;
  dTime.min   = 23;
  dTime.sec   = 0; 
  dDate.day   = 13;
  dDate.month = 3;
  dDate.year  = 19; 
  
  InitSystemClock();
  Port_Init();
  BUT_Init();
  InitTimer2();
  InitTimer4();
  
  __enable_interrupt(); 
  
  LcdInit();
  
    while(1)
    {
        
	switch (menu_state){
	
	 case MAIN_LEVEL:main_menu();break;
	         
	 case TIME_LEVEL:time_date_menu();break;
	 
	 case TIMER_LEVEL:timers_menu();break;
	                          
	 case SET_LEVEL:tester_menu();break;

	 default:break;}
    } 
}

void main_menu(void){
  
  unsigned char j=0;
   
  LcdClear();
  
  LcdGotoXY(0,STRING7);
  LcdPutcWide((((dTime.hour)/10)+0x30));
  LcdPutcWide(rBCD(dTime.hour));
//  LcdGotoXY(5,STRING7);
//  LcdPutc('-',0);
//  LcdGotoXY(3,STRING7);
//  LcdPutc((((dTime.min)/10)+0x30),0);
//  LcdPutc(rBCD(dTime.min),0);
  
  LcdGotoXY(11,STRING8);
  LcdPrintF("MENU");
  
  while(flag != 1){
  
   if(BUT_GetKey() == KEY_MENU)
        { 
          menu_state = TIME_LEVEL;
          flag = 1;
        }
		
   if(BUT_GetKey() == KEY_SET)
        { 		
                 
        }
		
  }
flag = 0;
}

void time_date_menu(void){
  
  unsigned char i=0;
  
  LcdClear();
  LcdGotoXY(0,STRING2);
  LcdPrintF("Time Set    :");
  
  LcdGotoXY(10,STRING2);
  LcdPutc((((dTime.hour)/10)+0x30),0);
  LcdPutc(rBCD(dTime.hour),0);
			  
  LcdGotoXY(13,STRING2);
  LcdPutc((((dTime.min)/10)+0x30),0);
  LcdPutc(rBCD(dTime.min),0);
		  
  LcdGotoXY(0,STRING4);
  LcdPrintF("Date   .  .20");
  LcdGotoXY(5,STRING4);
  LcdPutc((((dDate.day)/10)+0x30),0);
  LcdPutc(rBCD(dDate.day),0);
   
  LcdGotoXY(8,STRING4);
  LcdPutc((((dDate.month)/10)+0x30),0);
  LcdPutc(rBCD(dDate.month),0);

  LcdGotoXY(13,STRING4);
  LcdPutc((((dDate.year)/10)+0x30),0);
  LcdPutc(rBCD(dDate.year),0);
  
  LcdGotoXY(0,STRING8);
  LcdPrintF("SET   NEXT   OK");
  
	while(flag != 1){
  
		if(BUT_GetKey() == KEY_MENU)
        { 
          menu_state = TIMER_LEVEL;
          flag = 1;
        }
		
		if(BUT_GetKey() == KEY_SET)
        { 
            if (i==0){
				
				LcdGotoXY(10,STRING2);
		        LcdPutc((((dTime.hour)/10)+0x30),0);
				LcdPutc(rBCD(dTime.hour),0);
				dTime.hour++;
				
				if (dTime.hour==23){ dTime.hour = 0;}
			}
			
			if (i==1){
				
				
				LcdGotoXY(13,STRING2);
		        LcdPutc((((dTime.min)/10)+0x30),0);
				LcdPutc(rBCD(dTime.min),0);
				dTime.min++;
				
				if (dTime.min==59){ dTime.min = 0;}
			}
			
			if (i==2){
				
				dDate.day++;
				LcdGotoXY(5,STRING4);
		        LcdPutc((((dDate.day)/10)+0x30),0);
				LcdPutc(rBCD(dDate.day),0);
				
				if (dDate.day==31){ dDate.day = 0;}
				
				else if (dDate.day==28&&dDate.month==2){dDate.day = 0;}
			}
			
			if (i==3){
				
				dDate.month++;
				LcdGotoXY(8,STRING4);
		        LcdPutc((((dDate.month)/10)+0x30),0);
				LcdPutc(rBCD(dDate.month),0);	
				
				if (dDate.month==12){ dDate.month = 0;}
			}
			
			if (i==4){
				
				dDate.year++;
				LcdGotoXY(13,STRING4);
		        LcdPutc((((dDate.year)/10)+0x30),0);
				LcdPutc(rBCD(dDate.year),0);
				
				if (dDate.year==99){ dDate.year = 0;}
			}
			     
        }
		
		if(BUT_GetKey() == KEY_NEXT)
        { 
	     
		 if(i<=1){
		
			LcdGotoXY(time_position[i],STRING2); 
			LcdPutc('0',1);
			i++;
		    }
         
		   else if (i<5){
			
			LcdGotoXY(time_position[i],STRING4);
			LcdPutc('0',1);
			i++;
		    }
		   else if (i==5){
			   i=0;
			}
			}	 
		    		 
        		
  }	
flag = 0;
}

void timers_menu(void){
  
  LcdClear();
  LcdGotoXY(0,STRING2);
  LcdPrintF("Timer1_ON");
  LcdGotoXY(11,STRING2);
  LcdPutc('0',0);
  LcdGotoXY(12,STRING2);
  LcdPutc('0',0);
  LcdGotoXY(13,STRING2);
  LcdPutc(':',0);
  LcdGotoXY(14,STRING2);
  LcdPutc('0',0);
  LcdGotoXY(15,STRING2);
  LcdPutc('0',0);
  
  LcdGotoXY(0,STRING3);
  LcdPrintF("Timer1_OFF");
  LcdGotoXY(11,STRING3);
  LcdPutc('0',0);
  LcdGotoXY(12,STRING3);
  LcdPutc('0',0);
  LcdGotoXY(13,STRING3);
  LcdPutc(':',0);
  LcdGotoXY(14,STRING3);
  LcdPutc('0',0);
  LcdGotoXY(15,STRING3);
  LcdPutc('0',0);
  
  LcdGotoXY(0,STRING5);
  LcdPrintF("Timer2_ON");
  LcdGotoXY(11,STRING5);
  LcdPutc('0',0);
  LcdGotoXY(12,STRING5);
  LcdPutc('0',0);
  LcdGotoXY(13,STRING5);
  LcdPutc(':',0);
  LcdGotoXY(14,STRING5);
  LcdPutc('0',0);
  LcdGotoXY(15,STRING5);
  LcdPutc('0',0);
  
  LcdGotoXY(0,STRING6);
  LcdPrintF("Timer2_OFF");
  LcdGotoXY(11,STRING6);
  LcdPutc('0',0);
  LcdGotoXY(12,STRING6);
  LcdPutc('0',0);
  LcdGotoXY(13,STRING6);
  LcdPutc(':',0);
  LcdGotoXY(14,STRING6);
  LcdPutc('0',0);
  LcdGotoXY(15,STRING6);
  LcdPutc('0',0);
  
  LcdGotoXY(0,STRING8);
  LcdPrintF(" SET  NEXT   OK ");
  
  while(flag != 1){
  
   if(BUT_GetKey() == KEY_MENU)
        {
          menu_state = MAIN_LEVEL;
          flag = 1;
        }
  }
flag = 0;

}

void tester_menu(void){
 
}

 void Port_Init (){
    
    //Port A init
    
    //Port B init
    PB_DDR_DDR4 = 1;      //    I2C SCL
    PB_CR1_bit.C14 =1;
    PB_CR2_bit.C24 =1;
    
    PB_DDR_DDR5 = 1;      //    I2C SDA
    PB_CR1_bit.C15 =1;
    PB_CR2_bit.C25 =1;
    
    //Port C init
    PC_DDR_DDR3 = 1;      //    LCD_LED
    PC_CR1_bit.C13 =1;
    PC_CR2_bit.C23 =1;
    
    PC_DDR_DDR4 = 1;      //    LCD_SS
    PC_CR1_bit.C14 =1;
    PC_CR2_bit.C24 =1;
      
    PC_DDR_DDR5 = 1;      //    LCD_SCK    
    PC_CR1_bit.C15 =1;
    PC_CR2_bit.C25 =1;
    
    PC_DDR_DDR6 = 1;      //    LCD_MOSI   
    PC_CR1_bit.C16 =1;
    PC_CR2_bit.C26 =1;
    
    //Port D init
    PD_DDR_DDR4 = 0;      //    SW_MENU_OK
    PD_CR1_bit.C14 =1;
    PD_CR2_bit.C24 =0;
    
    PD_DDR_DDR5 = 0;      //    SW_NEXT
    PD_CR1_bit.C15 =1;
    PD_CR2_bit.C25 =0;
      
    PD_DDR_DDR6 = 0;      //    SW_SET      
    PD_CR1_bit.C16 =1;
    PD_CR2_bit.C26 =0;
    
  }

void InitSystemClock(){
  
    CLK_ICKR = 0; // 
    CLK_ICKR_HSIEN = 1; // 
    CLK_ECKR = 0; // 
    while (CLK_ICKR_HSIRDY == 0); // 
    CLK_CKDIVR = 0; // 
    CLK_PCKENR1 = 0xf7; // 
    CLK_PCKENR2 = 0xff; // 
    CLK_CCOR = 0; // 
    CLK_HSITRIMR = 0; // Turn off any HSIU trimming.
    CLK_SWIMCCR = 0; // Set SWIM to run at clock / 2.
    CLK_SWR = 0xe1; // 
    CLK_SWCR = 0; // 
    CLK_SWCR_SWEN = 1; //  HSI
    while (CLK_SWCR_SWBSY != 0); // 
}

//  Setup Timer 2 ready to process the pulse data.

void InitTimer2(){

    TIM2_SR1_bit.UIF = 0;
    TIM2_PSCR = 0x07;
    TIM2_ARRH = 0xff;
    TIM2_ARRL = 0x00;
   // TIM2_EGR_UG = 1;
    TIM2_IER_bit.UIE = 1;       //  Enable the update interrupts.
    TIM2_CR1_bit.CEN = 1;
}

//  Setup Timer 4 ready to process the pulse data.  //1ms

void InitTimer4(){     

    TIM4_SR_bit.UIF = 0;
    TIM4_PSCR = 0x06;
    TIM4_ARR = 0xFA;
    TIM4_IER_bit.UIE = 1;       //  Enable the update interrupts.
    TIM4_CR1_bit.CEN = 1;
}

unsigned char rBCD (unsigned char y){
	
	unsigned char x = 0;
	
	x = y/10;
	y = (y-(x*10))+0x30;
	return y;
  
}