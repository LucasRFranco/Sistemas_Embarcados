#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

#define MSGQUEUE_OBJECTS 16

osThreadId_t produtor_id, consumidor_id;
osMessageQueueId_t prodcons_id;

void produtor(void *arg){
  uint8_t count = 0;
  
  while(1){
    osMessageQueuePut(prodcons_id, &count, 0, 0);
  
    if(count >= 16){
      count = 0;
    }
    else{
      count++;
    }

    count &= 0x0F; // produz nova informação
    osDelay(500);
  } // while
} // produtor

void consumidor(void *arg){
  uint8_t state;
  
  while(1){
    osMessageQueueGet(prodcons_id, &state, 0, 0);
    
    LEDWrite(LED4 | LED3 | LED2 | LED1, state); // apresenta informação consumida
    osDelay(500);
  } // while
} // consumidor

void main(void){
  SystemInit();
  LEDInit(LED4 | LED3 | LED2 | LED1);

  osKernelInitialize();

  produtor_id = osThreadNew(produtor, NULL, NULL);
  consumidor_id = osThreadNew(consumidor, NULL, NULL);
  
  prodcons_id = osMessageQueueNew(MSGQUEUE_OBJECTS, 1, NULL); // cria fila de mensagens de 1 byte
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main