#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS


osThreadId_t BlinkLED_id;

typedef struct{
  uint16_t select;
  uint32_t time;
}set;

set ledconfig[2];

ledconfig[0].select = 1;
ledconfig[0].time = 100;
ledconfig[1].select = 2;
ledconfig[1].time = 100;


void BlinkLED(void *ledconfig){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    
    switch(*(uint16_t*)ledconfig.select)
    {
      case 1:
        state ^= LED1;
        LEDWrite(LED1, state);
        break;
        
      case 2:
        state ^= LED2;
        LEDWrite(LED2, state);
        break;      
    } // switch case    
    
    osDelayUntil(tick + *(uint32_t*)ledconfig.time);
  } // while
} //BlinkLED

void main(void){
  LEDInit(LED1 | LED2);

  osKernelInitialize();
  
  BlinkLED_id = osThreadNew(BlinkLED, (void*)&ledconfig[0], NULL);
  BlinkLED_id = osThreadNew(BlinkLED, (void*)&ledconfig[1], NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
