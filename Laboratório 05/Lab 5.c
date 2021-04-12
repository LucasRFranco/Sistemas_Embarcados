#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "driverbuttons.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

#define BUFFER_SIZE 8

//RTOS
osThreadId_t consumidor_id;
osSemaphoreId_t vazio_id, cheio_id;

//Globals
uint8_t buffer[BUFFER_SIZE];
uint8_t index_i = 0, count = 0;

//Handlers
void GPIOJ_Handler(void){
  
  ButtonIntClear(USW1);
  ButtonIntDisable(USW1);
  
  osSemaphoreAcquire(vazio_id, 0); // há espaço disponível?
  buffer[index_i] = count; // coloca no buffer
  osSemaphoreRelease(cheio_id); // sinaliza um espaço a menos
  
  index_i++; // incrementa índice de colocação no buffer
  if(index_i >= BUFFER_SIZE){
    index_i = 0;
  }
   
  count++;
  count &= 0x0F; // produz nova informação  
}// produtor

//Threads
void consumidor(void *arg){
  uint8_t index_o = 0, state;
  
  while(1){
    osSemaphoreAcquire(cheio_id, osWaitForever); // há dado disponível?
    state = buffer[index_o]; // retira do buffer
    osSemaphoreRelease(vazio_id); // sinaliza um espaço a mais
    
    index_o++; // incrementa índice de retirada do buffer
    if(index_o >= BUFFER_SIZE)
      index_o = 0;
    
    LEDWrite(LED4 | LED3 | LED2 | LED1, state); // apresenta informação consumida
    osDelay(500);
    
    ButtonIntClear(USW1);
    ButtonIntEnable(USW1);
    
  } // while
} // consumidor

//Main
void main(void){
  SystemInit();
  LEDInit(LED4 | LED3 | LED2 | LED1);
  ButtonInit(USW1);
  
  ButtonIntClear(USW1);
  ButtonIntEnable(USW1);

  osKernelInitialize();

  consumidor_id = osThreadNew(consumidor, NULL, NULL);

  vazio_id = osSemaphoreNew(BUFFER_SIZE, BUFFER_SIZE, NULL); // espaços disponíveis = BUFFER_SIZE
  cheio_id = osSemaphoreNew(BUFFER_SIZE, 0, NULL); // espaços ocupados = 0
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
