#include "buttons.h"

volatile unsigned char key = 0;

//_______________________________________
void BUT_Init(void)
{
//  BUTTON_DDR &= ~(MASK_BUTTONS); 
//  BUTTON_CR1 |= MASK_BUTTONS;
//  BUTTON_CR2 &= ~(MASK_BUTTONS);
//  SystemTimerInit_TIM4 (); 
}

//_______________________________________
unsigned char BUT_Debrief(void)
{
  //последовательный опрос выводов мк
  if (PD_IDR_bit.IDR4==0)     
  {key = KEY_MENU;PC_ODR_bit.ODR3 = 1;}
else if (PD_IDR_bit.IDR5==0)  
  {key = KEY_NEXT;PC_ODR_bit.ODR3 = 1;}
else if (PD_IDR_bit.IDR6==0)        
  {key = KEY_SET;PC_ODR_bit.ODR3 = 1;}    
else {
    key = KEY_NULL;
  }
  return key;
}

//__________________________
unsigned char BUT_GetKey(void)
{
  unsigned char pressedKey = key;
  key = KEY_NULL;
  return pressedKey;
}


//____________________________
void BUT_SetKey(unsigned char pressedKey)
{
    key = pressedKey;
}

