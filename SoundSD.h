// SoundSD.h
#ifndef __SOUNDSD_H__
#define __SOUNDSD_H__

#include "integer.h"
#include <stdint.h>
#include "ff.h"

// call once at startup (before any playback)
FRESULT SoundSD_Mount(void);

// configure DAC+SysTick — similar to Sound_Init
void SoundSD_Init(uint32_t period, uint32_t priority);

// begin streaming from a file on the SD card
// filename: 8.3 name on the card (e.g. "music.bin")
// returns FR_OK on success, other codes for errors
FRESULT SoundSD_Play(const char *filename);


void SoundSD_Stop();

void friendlyDelay(uint32_t ms);

// must be called periodically in your main loop to refill buffers
void SoundSD_Service(void);

#endif
