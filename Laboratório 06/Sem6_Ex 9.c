#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

osThreadId_t tid_phaseA;                /* Thread id of thread: phase_a      */
osThreadId_t tid_phaseB;                /* Thread id of thread: phase_b      */
osThreadId_t tid_phaseC;                /* Thread id of thread: phase_c      */
osThreadId_t tid_phaseD;                /* Thread id of thread: phase_d      */

osMutexId_t phases_mut_id;

const osMutexAttr_t Phases_Mutex_attr = {
  "PhasesMutex",                            // human readable mutex name
  osMutexRecursive | osMutexPrioInherit,    // attr_bits
  NULL,                                     // memory for control block   
  0U                                        // size for control block
  };

/*----------------------------------------------------------------------------
 *      Thread 1 'phaseA': Phase A output
 *---------------------------------------------------------------------------*/
void phaseA (void *argument) {
  uint8_t state = 0;
  uint8_t count = 0;
  
  for (;;) {
    osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);    /* wait for an event flag 0x0001 */
    
    state ^= LED1;
    osMutexAcquire(phases_mut_id, osWaitForever); // try to acquire mutex
    LEDWrite(LED1, state);
    osMutexRelease(phases_mut_id);
    
    count++;
    
    if(count == 2){
      count = 0;
      osThreadFlagsSet(tid_phaseB, 0x0001);            /* set signal to thread 'thread' */
    }
    else{
      osThreadFlagsSet(tid_phaseA, 0x0001);            /* set signal to thread 'thread' */
    }
    
    osDelay(500);                                    /* delay 500ms                   */
    
  }
}

/*----------------------------------------------------------------------------
 *      Thread 2 'phaseB': Phase B output
 *---------------------------------------------------------------------------*/
void phaseB (void *argument) {
  uint8_t state = 0;
  uint8_t count = 0;
  
  for (;;) {
    osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);    /* wait for an event flag 0x0001 */
    
    state ^= LED2;
    osMutexAcquire(phases_mut_id, osWaitForever); // try to acquire mutex
    LEDWrite(LED2, state);
    osMutexRelease(phases_mut_id);
      
    count++;
    
    if(count == 2){
      count = 0;
      osThreadFlagsSet(tid_phaseC, 0x0001);            /* set signal to thread 'thread' */
    }
    else{
      osThreadFlagsSet(tid_phaseA, 0x0001);            /* set signal to thread 'thread' */
    }
  }
}

/*----------------------------------------------------------------------------
 *      Thread 3 'phaseC': Phase C output
 *---------------------------------------------------------------------------*/
void phaseC (void *argument) {
  uint8_t state = 0;
  uint8_t count = 0;
  
  for (;;) {
    osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);    /* wait for an event flag 0x0001 */
    
    state ^= LED3;
    osMutexAcquire(phases_mut_id, osWaitForever); // try to acquire mutex
    LEDWrite(LED3, state);
    osMutexRelease(phases_mut_id);
      
    count++;
    
    if(count == 2){
      count = 0;
      osThreadFlagsSet(tid_phaseD, 0x0001);            /* set signal to thread 'thread' */
    }
    else{
      osThreadFlagsSet(tid_phaseA, 0x0001);            /* set signal to thread 'thread' */
    }
  }
}

/*----------------------------------------------------------------------------
 *      Thread 4 'phaseD': Phase D output
 *---------------------------------------------------------------------------*/
void phaseD (void *argument) {
  uint8_t state = 0;
  
  for (;;) {
   osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);    /* wait for an event flag 0x0001 */
    
    state ^= LED4;
    osMutexAcquire(phases_mut_id, osWaitForever); // try to acquire mutex
    LEDWrite(LED4, state);
    osMutexRelease(phases_mut_id);
    
    osThreadFlagsSet(tid_phaseA, 0x0001);            /* set signal to thread 'thread' */
  }
}

/*----------------------------------------------------------------------------
 *      Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
  tid_phaseA = osThreadNew(phaseA, NULL, NULL);
  tid_phaseB = osThreadNew(phaseB, NULL, NULL);
  tid_phaseC = osThreadNew(phaseC, NULL, NULL);
  tid_phaseD = osThreadNew(phaseD, NULL, NULL);

  phases_mut_id = osMutexNew(&Phases_Mutex_attr);
  
  osThreadFlagsSet(tid_phaseA, 0x0001);          /* set signal to phaseA thread   */

  osDelay(osWaitForever);
  while(1);
}

int main (void) {
  // System Initialization
  LEDInit(LED4 | LED3 | LED2 | LED1);

  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  if (osKernelGetState() == osKernelReady) {
    osKernelStart();                    // Start thread execution
  }

  while(1);
}

/*

O que deve ser alterado na implementação do sistema para se obter uma contagem binária decrescente nos quatro LEDs do kit?
Basta iniciar com todos os LEDs acesos e mudar o valor inicial das variáveis state de 0 para 0xFF.

Seria possível executar uma contagem binária utilizando quatro instâncias de uma mesma tarefa nesse sistema?
Sim, com pequenas alterações.

Bastaria passar como parâmetro uma struct que contenha o LED a ser controlado, a próxima tarefa a ser liberada (A->B, C->D ou D->A), e o delay da respectiva tarefa (500ms para A e 0 para as demais).
*/