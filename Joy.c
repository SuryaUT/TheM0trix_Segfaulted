#include <ti/devices/msp/msp.h>
#include "Joy.h"
#include "../inc/Clock.h"
#include "../inc/ADC.h"
#include "../inc/LaunchPad.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"

extern int Joy_x;
extern int Joy_y;

void Joy_Init(){
    // Initialize x and y analog inputs
    ADC_InitDual(ADC1, 2, 1, ADCVREF_VDDA);

    // Initialize joystick button
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00050081; // input, pull down
}

uint8_t Joy_InButton(){
    return GPIOB->DIN31_0 & (1<<16);
}

void Joy_In(uint32_t* x, uint32_t* y){
    // sample ADC
    ADC_InDual(ADC1, x, y);
}

void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    
    uint32_t x, y;
    Joy_In(&x, &y);

    Joy_x = -((int32_t)((x>>9) +1)/2 - 2);
    Joy_y = (int32_t)((y>>9)+1)/2 - 2;
    
  }
}