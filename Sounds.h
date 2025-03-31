#ifndef __SOUNDS_H__
#define __SOUNDS_H__

extern const uint8_t 	pistol[10932];
extern const uint8_t 	shotgun[14163];
extern const uint8_t 	weapload[2394];
extern const uint8_t 	outofammo[2383];
extern const uint8_t 	reload[18688];

#define PISTOL_SOUND 0
#define SHOTGUN_SOUND 1
#define WEAPLOAD_SOUND 2
#define OUTOFAMMO_SOUND 3
#define RELOAD_SOUND 4

typedef struct{
  const uint8_t *SoundPt;
  uint32_t Length;
} sound_t;


extern sound_t SoundEffects[];

void Sound_Init(uint32_t period, uint32_t priority);
void Sound_Start(sound_t sound);


#endif