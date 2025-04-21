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

Inventory inventory = {0, 3, {}, 0};

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
}

int main() {
  SystemInit();
  RandomizeSprites();
  // Starting weapon
  Inventory_add(&inventory, &pistol);
  respawnPlayer();
  while(1) {
   if (otherHealth == 0){
    Sprites[0].scale = 0; 
    healthCode = DEADCODE;
   }else {
    Sprites[0].scale = 5;
   }
   RenderScene();
   
   // if (GPIOA->DIN31_0 & (1<<18) for use of side switch
   
   // End in case of death
   if (GPIOA->DIN31_0 & (1<<18) || playerHealth <= 0){
    isShooting = 0;
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("You died! \nGet better lil bro\nReload to respawn in\n3");
    uint8_t canRespawn;
    Clock_Delay1ms(1000);
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("You died! \nGet better lil bro\nReload to respawn in\n2");
    Clock_Delay1ms(1000);
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("You died! \nGet better lil bro\nReload to respawn in\n1");
    Clock_Delay1ms(1000);
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("You died! \nGet better lil bro\nReload to respawn");
    while ((GPIOA->DIN31_0 & (1<<28)) == 0) {}
    playerHealth = 50;
    healthCode = RESPAWNCODE;
    otherKills++;
    respawnPlayer();
    
    ST7735_FillScreen(0);
   };
  }
 }