#include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <math.h>
 #include "Async_Delay.h"
 #include "Sync.h"
#include "UART1.h"
#include "UART2.h"
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
Player self = {
  22, // X position
  12, // Y position
  -1, // X direction
  0, // Y direction
  0, // Plane direction
  .66,
  50, // Health
  1, // is vulnerable
  0, // is shooting
  0, // number of kills
  0 // is on target
};

Player other = {
  22,
  12,
  -1,
  0,
  0,
  .66,
  50,
  1,
  0,
  0,
  0
};

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
  start_delay(1000, &self.isVulnerable);

  getRandomMapPos(&respawnX, &respawnY);

  self.posX = respawnX + .5;
  self.posY = respawnY + .5;

  // Reset some graphics
  self.isShooting = 0;
  hitTimer = 0;
}

void DeathScreen(){
    Inventory_currentItem(&inventory)->enabled = 0;
    ST7735_DrawBitmapFromSDC(0, 127, (!IS_DOMINANT_CONTROLLER) ? "GEEB.bin" : "TWEO.bin", 160, 128);
    
    uint8_t i = 0;
    const char * const *texts = deathScreenTexts[language];

    while (texts[i]) {
        if (i == 0) {
            ST7735_DrawTextBoxS(0, 0, 160, texts[i], ST7735_WHITE, ST7735_WHITE, 2, 1, 0);
        } 
        else if (texts[i+1] == 0) {
            ST7735_DrawTextBoxS(0, 112, 160, texts[i], ST7735_WHITE, ST7735_WHITE, 1, 1, 0);
        } 
        else {
            ST7735_FillRect(0, 74, 160, 8, ST7735_BLACK);
            ST7735_DrawTextBoxS(0, 74, 160, texts[i], ST7735_WHITE, ST7735_BLACK, 1, 1, 0);
            if (texts[i+1] != 0 && texts[i+2] != 0) Clock_Delay1ms(1000);
        }
        i++;
    }

    if (other.playerHealth <= 0) other.playerHealth = 50; // If they died at the same time, we know they'll respawn

    while (!(GPIOA->DIN31_0 & ((1<<28) | (1<<27)))) {}
}


uint8_t isPlaying;
void GameLoop(){
  self.playerHealth = other.playerHealth = 50;
  self.kills = other.kills = 0;
  SoundEffects_enable();
  Inventory_add(&inventory, &pistol);
  RandomizeSprites();
  // Starting weapon
  respawnPlayer();
  isPlaying = 1;
  while(isPlaying) {
   if (other.playerHealth == 0){
    Sprites[0].scale = 0; 
    healthCode = DEADCODE;
   }else {
    Sprites[0].scale = 5;
   }
   RenderScene();
  
   // In case of death
   if (self.playerHealth <= 0){
    DeathScreen();
    if (GPIOA->DIN31_0 & (1<<27)) {
      isPlaying = 0;
      healthCode = RESTARTCODE;
    }
    else{
      self.playerHealth = 50;
      healthCode = RESPAWNCODE;
      other.kills++;
      respawnPlayer();
    }
   }
  }
}

void Menus_Run(void);

int main() {
  SystemInit();

  while (1){
    Menus_Run();
    GameLoop();
  }
}