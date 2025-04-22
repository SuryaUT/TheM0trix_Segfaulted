#include "../inc/Timer.h"
#include "ti/devices/msp/msp.h"
#include "Async_Delay.h"

uint32_t ms_passed;
uint32_t ms_done;
uint8_t* flag_done;
void start_delay(uint32_t milliseconds, uint8_t* flag){
    *flag = 0;
    ms_passed = 0;
    ms_done = milliseconds;
    flag_done = flag;
    TimerG0_IntArm(1000, 40, 0); // Enable timer G0 interrupt
}

void TIMG0_IRQHandler(void){
  if((TIMG0->CPU_INT.IIDX) == 1){ // this will acknowledge
    ms_passed++;
  }

  if (ms_passed == ms_done){
    *flag_done = 1;
    NVIC->ICER[0] = (1<<16); // Disable timer G0 interrupt
  }
}