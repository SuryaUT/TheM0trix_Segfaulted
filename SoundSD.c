// SoundSD.c
#include "SoundSD.h"
#include "Sounds.h"
#include "../inc/DAC.h"
#include "ti/devices/msp/msp.h"

extern uint32_t Length;
extern const uint8_t* SoundPt;

// from fatfs
static FATFS fs;
static FIL    fil;
static UINT   bytesRead;

// buffering
#define SD_BUFSIZE 256       // 256 samples per buffer
static uint16_t buf1[SD_BUFSIZE];
static uint16_t buf2[SD_BUFSIZE];
static uint16_t *front = buf1, *back = buf2;
static volatile int needRefill;

// simple block counter to rewind at EOF (optional)
static uint32_t blockCount;
#define NUM_BLOCKS 128       // adjust per file length if you want looping

// mount the SD (call once)
FRESULT SoundSD_Mount(void){
  return f_mount(&fs,"",0);
}

// init DAC + SysTick interrupt
void SoundSD_Init(uint32_t period, uint32_t priority){
  DAC_Init();
  SysTick->CTRL = 0;            // disable
  SysTick->LOAD = period - 1;   // tick rate
  SysTick->VAL  = 0;            // clear
  // set IRQ priority
  SCB->SHP[1] = (SCB->SHP[1] & ~0xC0000000) | (priority<<30);
  SysTick->CTRL = 0x0007;       // enable with core clock+int
}

// start streaming a file
FRESULT SoundSD_Play(const char *filename){
  SoundMode = 1;
  FRESULT res;
  // open file
  res = f_open(&fil, filename, FA_READ);
  if(res != FR_OK) return res;
  // preload first buffer
  front = buf1; back = buf2;
  needRefill = 1;
  blockCount = 0;
  return FR_OK;
}

// call this in your main loop to keep data flowing
void SoundSD_Service(void){
  if(needRefill){
    needRefill = 0;
    // read next chunk
    if(f_read(&fil, back, SD_BUFSIZE*2, &bytesRead) != FR_OK || bytesRead != SD_BUFSIZE*2){
      // simple loop: rewind to start
      f_lseek(&fil, 0);
      blockCount = 0;
      f_read(&fil, back, SD_BUFSIZE*2, &bytesRead);
    }
    blockCount++;
    // swap buffers
    uint16_t *tmp = front;
    front = back;
    back  = tmp;
  }
}

void SysTick_Handler(void){
  static uint32_t idx = 0;

  if(SoundMode == 1){ // SD streaming
    DAC_Out(front[idx++]);
    if(idx >= SD_BUFSIZE){
      idx = 0;
      needRefill = 1;
    }
  }
  else if (Length){  // Flash-based audio
    DAC_Out(*SoundPt);
    SoundPt++;
    Length--;
  }
  else {
    SysTick->LOAD = 0; // Stop playing
  }
}