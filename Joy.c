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