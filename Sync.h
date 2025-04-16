#ifndef SYNC_H_
#define SYNC_H_

#include <stdint.h>

#define PISTOLCODE 2
#define SHOTGUNCODE 3
#define RIFLECODE 4
#define MEDKITCODE 5
#define RESPAWNCODE 6
#define DEADCODE 7

#define PICKUPCODE 1
#define SPAWNCODE 2

#define IS_DOMINANT_CONTROLLER 0 // Adjust depending on which controller you're using; one should be dominant to tell the other controller where the sprites are

void getRandomMapPos(uint8_t* x, uint8_t* y);
void RandomizeSprites();
void Sync_Init();

#endif