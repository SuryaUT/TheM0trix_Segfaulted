  #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <math.h>
#include "images.h"
 #include "ti/devices/msp/msp.h"
 #include "../inc/ST7735.h"
 #include "../inc/SPI.h"
 #include "../inc/Clock.h"
 #include "../inc/Timer.h"
 #include "../inc/LaunchPad.h"
 #include "Graphics.h"
 #include "Sounds.h"
 #include "sprites.h"
 #include "Buffer.h"
 #include "Input.h"
 #include "Items.h"
 
 // Player state
 double posX = 22, posY = 12;    // x and y start position
 double dirX = -1, dirY = 0;    // initial direction vector
 double planeX = 0, planeY = 0.66;  // the 2d raycaster version of camera plane
 int playerHealth = 50;
 uint8_t isOnTarget = 0;
 uint8_t accuracyRad = 6;
 uint8_t ammo = 8, max_ammo = 8;

void SystemInit() {
  __disable_irq();
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Sound_Init(80000000/11000, 0);
  Input_Init();
  Graphics_Init();
  __enable_irq();
 }

Inventory inventory = {0, 3, {}, 0};

#define MAX_SPRITES 10
int numsprites = 3;
Sprite Sprites[MAX_SPRITES] = {
    {12, 12, target, 1, 32, 32, 3},
    {18, 12, Shotgunsprite, 0, 16, 16, 1, SHOTGUN},
};

Item pistol = {
    PISTOL,
    {8, SCREEN_HEIGHT, pistolsprite, 0xFFFF, 16, 16, 1, PISTOL},
    {SCREEN_WIDTH/2, SCREEN_HEIGHT, PistolImage, 0, 32, 32, 3},
    SoundEffects + PISTOL_SOUND
  };

Item pistol2 = {
    SHOTGUN,
    {8, SCREEN_HEIGHT, Shotgunsprite, 0, 16, 16, 1, SHOTGUN},
    {SCREEN_WIDTH/2 + 24, SCREEN_HEIGHT, ShotgunImage, 0, 32, 32, 3},
    SoundEffects + SHOTGUN_SOUND
  };

Item* items[2] = {&pistol, &pistol2};

int main() {
  SystemInit();

  Inventory_add(&inventory, &pistol);

  while(1) {
   RenderScene();

   if (playerHealth > 50) playerHealth = 50; else if (playerHealth < 0) playerHealth = 0;
 
   // End in case of death or exit button
   if (GPIOA->DIN31_0 & (1<<18) || playerHealth <= 0){
    ST7735_FillScreen(0);
    printf("Game Over!\n");
    printf("(You died lol)\n");
    printf("get better lil bro\n");
    __asm volatile("bkpt; \n"); // breakpoint here
   };
  }
 }