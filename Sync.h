#ifndef SYNC_H_
#define SYNC_H_

#include <stdint.h>

#define PISTOLCODE 2
#define SHOTGUNCODE 3
#define RIFLECODE 4
#define MEDKITCODE 5
#define RESPAWNCODE 6
#define DEADCODE 7
#define RESTARTCODE 8

#define PICKUPCODE 1
#define SPAWNCODE 2

#define IS_DOMINANT_CONTROLLER 1 // Adjust depending on which controller you're using; one should be dominant to tell the other controller where the sprites are

extern uint8_t needSync;

void handshake();

typedef struct {
  double posX;
  double posY;
  double dirX;
  double dirY;
  double planeX;
  double planeY;
  int playerHealth;
  uint8_t isVulnerable;
  uint8_t isShooting;
  int kills;
  uint8_t isOnTarget;
} Player;

extern Player self, other;

// hit‐indicator state
typedef enum { HIT_FRONT, HIT_RIGHT, HIT_BACK, HIT_LEFT } HitDir_t;
extern HitDir_t  hitDir;
extern uint16_t  hitTimer;            // counts down frames to keep the indicator on
#define HIT_INDICATOR_DURATION  10   // ~20 frames

void getRandomMapPos(uint8_t* x, uint8_t* y);
void RandomizeSprites();
void Sync_Init();

#endif