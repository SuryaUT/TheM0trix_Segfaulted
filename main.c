#include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <math.h>
 #include "Async_Delay.h"
 #include "Sync.h"
 #include "ti/devices/msp/msp.h"
 #include "ST7735_SDC.h"
 #include "../inc/SPI.h"
 #include "../inc/Clock.h"
 #include "../inc/LaunchPad.h"
 #include "../inc/ADC.h"
 #include "Graphics.h"
 #include "Sounds.h"
 #include "sprites.h"
 #include "Buffer.h"
 #include "Input.h"
 #include "Inventory.h"
 #include "Items.h"
 #include "SoundSD.h"
 
// Player state
double posX = 22, posY = 12;    // x and y start position
double dirX = -1, dirY = 0;    // initial direction vector
double planeX = 0, planeY = 0.66;  // the 2d raycaster version of camera plane
int playerHealth = 50;
uint8_t isVulnerable = 1;
extern uint8_t isShooting;
int kills = 0, otherKills = 0;

double otherPosX, otherPosY;
double otherDirX, otherDirY;
int otherHealth = 50;
uint8_t isOnTarget = 0;
uint8_t healthCode = 1;

extern uint8_t language;
extern const char* const* deathScreenTexts[];


Inventory inventory = {0, 3, {}, 0};

void initRandom(){
  uint32_t randomSeed;
  ADC0_Init(5, ADCVREF_VDDA);
  randomSeed = ADC0_In() * ADC0_In();
  srand(randomSeed);
}

void SystemInit() {
  __disable_irq();
  initRandom();
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Sound_Init(80000000/11000, 0);
  Input_Init();
  __enable_irq();
  Graphics_Init();
  Sync_Init();
}

void respawnPlayer(){
  // Reset inventory
  inventory.index = inventory.size - 1;
  while (inventory.index > 0){
    Inventory_removeCurrent(&inventory);
  }
  Item* current = Inventory_currentItem(&inventory);
  current->ammo = current->mag_ammo;
  current->tot_ammo = current->start_ammo;
  current->enabled = 1;
  
  // Random location
  uint8_t respawnX;
  uint8_t respawnY;
  // Temporary invulnerability
  start_delay(1000, &isVulnerable);

  getRandomMapPos(&respawnX, &respawnY);

  posX = respawnX + .5;
  posY = respawnY + .5;

  isShooting = 0;
}

void DeathScreen(){
    Inventory_currentItem(&inventory)->enabled = 0;
    ST7735_DrawBitmapFromSDC(0, 127, "MENU.bin", 160, 128);
    uint8_t deathScreenIndex = 0;
    while (deathScreenTexts[language][deathScreenIndex] != 0){
      if (deathScreenIndex == 0) {
        ST7735_DrawTextBoxS(0, 0, 160, deathScreenTexts[language][deathScreenIndex], ST7735_WHITE, ST7735_WHITE, 2, 1, 0);
      }
      else if (deathScreenTexts[language][deathScreenIndex+1] == 0){
        ST7735_DrawTextBoxS(0, 112, 160, deathScreenTexts[language][deathScreenIndex], ST7735_WHITE, ST7735_WHITE, 1, 1, 0);
      }
      else{
        ST7735_DrawTextBoxS(0, 24, 160, deathScreenTexts[language][deathScreenIndex], ST7735_WHITE, ST7735_BLACK, 1, 1, 0);
        Clock_Delay1ms(1000);
      }
      deathScreenIndex++;
    }
    while (!((GPIOA->DIN31_0 & (1<<28)) || GPIOA->DIN31_0 & (1<<27))) {}
}

uint8_t isPlaying;
void GameLoop(){
  playerHealth = otherHealth = 50;
  kills = otherKills = 0;
  SoundEffects_enable();
  Inventory_add(&inventory, &pistol);
  RandomizeSprites();
  // Starting weapon
  respawnPlayer();
  isPlaying = 1;
  while(isPlaying) {
   if (otherHealth == 0){
    Sprites[0].scale = 0; 
    healthCode = DEADCODE;
   }else {
    Sprites[0].scale = 5;
   }
   RenderScene();
   
   // if (GPIOA->DIN31_0 & (1<<18) for use of side switch
   
   // In case of death
   if (GPIOA->DIN31_0 & (1<<18) || playerHealth <= 0){
    DeathScreen();
    if (GPIOA->DIN31_0 & (1<<27)) {
      isPlaying = 0;
      healthCode = RESTARTCODE;
    }
    else{
      playerHealth = 50;
      healthCode = RESPAWNCODE;
      otherKills++;
      respawnPlayer();
    }
   }
  }
}

void Menus_Run(void);

int main() {
  SystemInit();
  while (1){
    SoundSD_Mount();
    SoundSD_Init(80000000/11025, 1); 
    SoundSD_Play("CLUB.bin"); // Menu music
    Menus_Run();
    GameLoop();
  }
}