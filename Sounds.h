#ifndef __SOUNDS_H__
#define __SOUNDS_H__

extern const uint8_t 	shoot[4080];

typedef struct{
  const uint8_t *SoundPt;
  uint32_t Length;
} sound_t;


extern sound_t Sounds[];

void Sound_Init(uint32_t period, uint32_t priority);
void Sound_Start(sound_t sound);


#endif