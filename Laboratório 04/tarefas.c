#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

//osThreadId_t thread1_id, thread2_id;

osThreadId_t BlinkLED_id; 

typedef struct{
  uint8_t select;
  uint32_t time;
}set;

set ledconfig[4];

ledconfig[1].select = 1;
ledconfig[1].time = 200;
ledconfig[2].select = 2;
ledconfig[2].time = 300;
ledconfig[3].select = 3;
ledconfig[3].time = 500;
ledconfig[4].select = 4;
ledconfig[4].time = 700;



/*
void thread1(void *arg){
  uint8_t state = 0;
  
  while(1){
    state ^= LED1;
    LEDWrite(LED1, state);
    osDelay(100);
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    
    state ^= LED2;
    LEDWrite(LED2, state);
    
    osDelayUntil(tick + 100);
  } // while
} // thread2

*/

void BlinkLED(set *ledconfig){
  uint8_t state = 0;
  uint32_t tick;
  uint8_t i;
  
  while(1){
    tick = osKernelGetTickCount();
    
    for(i = 1; i<5; i++){
      switch(*ledconfig[i].select){
      case 1:
        state ^= LED1;
        LEDWrite(LED1, state);
        break;
      case 2:
        state ^= LED2;
        LEDWrite(LED2, state);
        break;
      case 3:
        state ^= LED3;
        LEDWrite(LED3, state);
        break;
      case 4:
        state ^= LED4;
        LEDWrite(LED4, state);
        break;
      } // switch case
      
      osDelayUntil(tick + *ledconfig[i].time);
      
    } // for
  } //while
} //BlinkLED

void main(void){
  LEDInit(LED1 | LED2);
  LEDInit(LED3 | LED4);

  osKernelInitialize();

  //thread1_id = osThreadNew(thread1, NULL, NULL);
  //thread2_id = osThreadNew(thread2, NULL, NULL);
  
  BlinkLED_id = osThreadNew(BlinkLED, &ledconfig, NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
