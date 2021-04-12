#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

osThreadId_t thread1_id, thread2_id;
osMutexId_t mutex1_id, mutex2_id;;

const char *thread1_name, *thread2_name, *mutex1_name, *mutex2_name;

const osThreadAttr_t thread1_attr = {
  "Blink LED1"
};

const osThreadAttr_t thread2_attr = {
  "Blink LED2"
};

const osMutexAttr_t mutex1_attr = {
  "Mutex 1",
  osMutexRecursive | osMutexPrioInherit,
  NULL, // memory for control block
  0U    // size of provided memory
};

const osMutexAttr_t mutex2_attr = {
  "Mutex 2",
  osMutexRecursive | osMutexPrioInherit,
  NULL, // memory for control block
  0U    // size of provided memory
};

void thread1(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    osMutexAcquire(mutex2_id, osWaitForever);
    tick = osKernelGetTickCount();
    state ^= LED1;
    
    osMutexAcquire(mutex1_id, osWaitForever);
    LEDWrite(LED1, state);
    osMutexRelease(mutex1_id);
    
    osDelayUntil(tick + 100);
    osMutexRelease(mutex2_id);
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    osMutexAcquire(mutex2_id, osWaitForever);
    tick = osKernelGetTickCount();
    state ^= LED2;
    
    osMutexAcquire(mutex1_id, osWaitForever);
    LEDWrite(LED2, state);
    osMutexRelease(mutex1_id);
    
    osDelayUntil(tick + 100);
    osMutexRelease(mutex2_id);
  } // while
} // thread2

void main(void){
  LEDInit(LED2 | LED1);
  LEDWrite(LED1, 0);
  LEDWrite(LED2, 0);
  
  osKernelInitialize();

  // Threads
  thread1_id = osThreadNew(thread1, NULL, &thread1_attr);
  thread1_name = osThreadGetName(thread1_id);
  
  thread2_id = osThreadNew(thread2, NULL, &thread2_attr);
  thread2_name = osThreadGetName(thread2_id);
  
  //// Mutexes
  mutex1_id = osMutexNew(&mutex1_attr);
  mutex1_name = osMutexGetName(mutex1_id);
  
  mutex2_id = osMutexNew(&mutex2_attr);
  mutex2_name = osMutexGetName(mutex2_id);

  // Kernel Start
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main