#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "../inc/DAC.h"
#include <stdint.h>
#include "Sounds.h"
#include "Soundeffects.h"

uint32_t Length;
const uint8_t* SoundPt;

sound_t SoundEffects[] = {
  {pistol, 10932},
  {shotgun, 14183},
  {weapload, 2394},
  {outofammo, 2383},
  {reload, 18688}
};

void Sound_Init(uint32_t period, uint32_t priority){
  DAC_Init();
  SysTick->CTRL = 0; // Disable SysTick
  SysTick->LOAD = period-1; // Reload Value
  SysTick->VAL = 0; // Clear
  SCB->SHP[1] = (SCB->SHP[1] & ~0xC0000000) | priority << 30; // Set priority
  SysTick->CTRL = 0x0007; // Enable SysTick with core clock and interrupts
}

void Sound_Start(sound_t sound){
    Length = sound.Length;
    SoundPt = sound.SoundPt;
    SysTick->LOAD = 80000000/11000-1;
    SysTick->VAL = 0;
}

void SysTick_Handler(void){
  if (Length){
  DAC_Out(*SoundPt);
  SoundPt++;
  Length--;
  }
  else {
    SysTick->LOAD = 0;
  }
}