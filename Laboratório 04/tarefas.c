#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS


osThreadId_t BlinkLED_id;

typedef struct{
  uint8_t select;
  uint32_t time;
}set;

set ledconfig[4];


void BlinkLED(void *ledconfig){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    
    state ^= ((set*)ledconfig)->select; // Exclusive OR -> To change LED status
    LEDWrite(((set*)ledconfig)->select, state);
    
    osDelayUntil(tick + ((set*)ledconfig)->time);
  } // while
} //BlinkLED

void main(void){
  LEDInit(LED1 | LED2);
  LEDInit(LED3 | LED4);
  
  ledconfig[0].select = LED1;
  ledconfig[1].select = LED2;
  ledconfig[2].select = LED3;
  ledconfig[3].select = LED4;
  
  ledconfig[0].time = 200;
  ledconfig[1].time = 300;
  ledconfig[2].time = 500;
  ledconfig[3].time = 700;
  
  
  osKernelInitialize();
  
  BlinkLED_id = osThreadNew(BlinkLED, (void*)&ledconfig[0], NULL);
  BlinkLED_id = osThreadNew(BlinkLED, (void*)&ledconfig[1], NULL);
  BlinkLED_id = osThreadNew(BlinkLED, (void*)&ledconfig[2], NULL);
  BlinkLED_id = osThreadNew(BlinkLED, (void*)&ledconfig[3], NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
