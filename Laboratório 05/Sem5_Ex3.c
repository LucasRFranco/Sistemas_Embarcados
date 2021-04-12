#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

osThreadId_t thread1_id, thread2_id;
osMutexId_t mutex_id;

const char *thread1_name, *thread2_name, *mutex_name;

const osThreadAttr_t thread1_attr = {
  "Blink LED1"
};

const osThreadAttr_t thread2_attr = {
  "Blink LED2"
};

const osMutexAttr_t mutex_attr = {
  "Mutex",
  osMutexRecursive | osMutexPrioInherit,
  NULL, // memory for control block
  0U    // size of provided memory
};

void thread1(void *arg){
  uint8_t state = 0;
  
  while(1){
    state ^= LED1;
    osMutexAcquire(mutex_id, osWaitForever);
    LEDWrite(LED1, state);
    osMutexRelease(mutex_id);
    osDelay(100);
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    
    state ^= LED2;
    osMutexAcquire(mutex_id, osWaitForever);
    LEDWrite(LED2, state);
    osMutexRelease(mutex_id);
    
    osDelayUntil(tick + 100);
  } // while
} // thread2

void main(void){
  LEDInit(LED2 | LED1);
  
  osKernelInitialize();

  thread1_id = osThreadNew(thread1, NULL, &thread1_attr);
  thread1_name = osThreadGetName(thread1_id);
  
  thread2_id = osThreadNew(thread2, NULL, &thread2_attr);
  thread2_name = osThreadGetName(thread2_id);
  
  mutex_id = osMutexNew(&mutex_attr);
  mutex_name = osMutexGetName(mutex_id);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main