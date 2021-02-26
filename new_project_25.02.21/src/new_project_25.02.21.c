#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

uint8_t LED_D4 = 0;

void SysTick_Handler(void){
  LED_D4 ^= GPIO_PIN_0; // Troca estado do LED D4
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, LED_D4); // Acende ou apaga LED D4
} // SysTick_Handler

void main(void){

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Habilita GPIO F (LED D3 = PF4, LED D4 = PF0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); // Aguarda final da habilitação
    
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0); // LED D4 como saída
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0); // LED D4 apagado
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);  
  
  uint32_t clock = 24000000; // Clock = 24Mhz
  //uint32_t clock = 120000000; // Clock = 120Mhz
  SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, clock); // Define o clock do sistema de acordo com a variável clock
  
  while(1){
    SysCtlDelay(SysCtlClockGet() * 3);
    SysTick_Handler();
  } // while
} // main

// O tempo real de execução é metade do esperado: Na configuração para comutar o estado do LED a cada 1s (config atual), o estado do LED comuta a cada 0.5s
// Não consegui encontrar o erro