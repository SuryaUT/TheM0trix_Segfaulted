#include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <math.h>
#include "Sync.h"
 #include "ti/devices/msp/msp.h"
 #include "../inc/ST7735.h"
 #include "../inc/SPI.h"
 #include "../inc/Clock.h"
 #include "../inc/LaunchPad.h"
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

Sprite* otherPlayer = Sprites;
double otherPosX, otherPosY;
int otherHealth = 50;
uint8_t isOnTarget = 0;
uint8_t healthCode = 1;

void SystemInit() {
  __disable_irq();
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Sound_Init(80000000/11000, 0);
  Input_Init();
  Graphics_Init();
  Sync_Init();
  __enable_irq();
}

Inventory inventory = {0, 3, {}, 0};

int main() {
  SystemInit();

  Inventory_add(&inventory, &pistol);

  while(1) {
   if (otherHealth == 0) Sprites[0].scale = 0; else Sprites[0].scale = 5;
   RenderScene();
   
   // End in case of death or exit button
   if (GPIOA->DIN31_0 & (1<<18) || playerHealth <= 0){
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("Game Over!\n");
    printf("(You died lol)\n");
    printf("get better lil bro\n");
    printf("Shoot to respawn");
    while ((GPIOA->DIN31_0 & (1<<24)) == 0) {}
    playerHealth = 50;
    healthCode = RESPAWNCODE;
   };
  }
 }