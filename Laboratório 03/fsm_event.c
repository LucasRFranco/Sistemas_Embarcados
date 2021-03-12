#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "driverlib/systick.h"
#include "driverleds.h" // Projects/drivers

// MEF com apenas 2 estados e 1 evento temporal que alterna entre eles
// Seleção por evento

typedef enum {DEC_0, DEC_1, DEC_2, DEC_3, DEC_4, DEC_5, DEC_6, DEC_7} state_t;

volatile uint8_t Evento = 0;

void SysTick_Handler(void){
  Evento = 1;
} // SysTick_Handler

void main(void){
  static state_t Estado = DEC_0; // estado inicial da MEF
  
  LEDInit(LED1);
  LEDInit(LED2);
  LEDInit(LED3);
  SysTickPeriodSet(12000000); // f = 1Hz para clock = 24MHz
  SysTickIntEnable();
  SysTickEnable();

  while(1){
    if(Evento){
      Evento = 0;
      switch(Estado){
        case DEC_0:
          LEDOff(LED1);
          LEDOff(LED2);
          LEDOff(LED3);
          Estado = DEC_1;
          break;
        case DEC_1:
          LEDOff(LED1);
          LEDOff(LED2);
          LEDOn (LED3);
          Estado = DEC_2;
          break;
        case DEC_2:
          LEDOff(LED1);
          LEDOn (LED2);
          LEDOn (LED3);
          Estado = DEC_3;
          break;
        case DEC_3:
          LEDOff(LED1);
          LEDOn (LED2);
          LEDOff(LED3);
          Estado = DEC_4;
          break;
        case DEC_4:
          LEDOn (LED1);
          LEDOn (LED2);
          LEDOff(LED3);
          Estado = DEC_5;
          break;
        case DEC_5:
          LEDOn (LED1);
          LEDOn (LED2);
          LEDOn (LED3);
          Estado = DEC_6;
          break;
        case DEC_6:
          LEDOn (LED1);
          LEDOff(LED2);
          LEDOn (LED3);
          Estado = DEC_7;
          break;
         case DEC_7:
          LEDOn (LED1);
          LEDOff(LED2);
          LEDOff(LED3);
          Estado = DEC_0;
          break;
      } // switch
    } // if
  } // while
} // main
