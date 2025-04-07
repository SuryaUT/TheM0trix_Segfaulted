#include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <math.h>
#include "UART1.h"
#include "UART2.h"
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
 #include "Inventory.h"
 #include "Items.h"
 
 // Player state
 double posX = 22, posY = 12;    // x and y start position
 double dirX = -1, dirY = 0;    // initial direction vector
 double planeX = 0, planeY = 0.66;  // the 2d raycaster version of camera plane
 int playerHealth = 50;
 uint8_t healthCode;

 double otherPosX, otherPosY;
 int otherHealth = 50;
 uint8_t isOnTarget = 0;

void SystemInit() {
  __disable_irq();
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Sound_Init(80000000/11000, 0);
  Input_Init();
  Graphics_Init();
  UART1_Init();
  UART2_Init();
  TimerG8_IntArm(2000000/128, 128, 1);
  __enable_irq();
}

Inventory inventory = {0, 3, {}, 0};

void getUARTPacket(){
  uint8_t inX = UART2_InChar();
  uint8_t inY = UART2_InChar();
  uint8_t inHealthCode = UART2_InChar();
  Sprites[0].x = inX/10.0;
  Sprites[0].y = inY/10.0;
  switch (inHealthCode){
    case PISTOL: playerHealth-=2; break;
    case SHOTGUN: playerHealth -= 12; break;
    case MEDKIT: otherHealth += 20; break;
  }
}

void sendUARTPacket(){
  uint8_t sendX = (uint8_t) ((posX+0.05)*10);//convert into fixed point
  uint8_t sendY = (uint8_t) ((posY+0.05)*10);//convert into fixed point

  UART1_OutChar('<');
  UART1_OutChar(sendX);
  UART1_OutChar(sendY);
  UART1_OutChar(healthCode);
  healthCode = 0;
}

int main() {
  SystemInit();

  Inventory_add(&inventory, &pistol);

  while(1) {
   while (UART2_InChar() != '<'){}
   getUARTPacket();
   RenderScene();
   
   // End in case of death or exit button
   if (GPIOA->DIN31_0 & (1<<18) || playerHealth == 255){
    ST7735_FillScreen(0);
    printf("Game Over!\n");
    printf("(You died lol)\n");
    printf("get better lil bro\n");
    printf("Shoot to respawn");
    while ((GPIOA->DIN31_0 & 1) == 0) {}
    playerHealth = 50;
   };
  }
 }

 void TIMG8_IRQHandler(void){
  if((TIMG8->CPU_INT.IIDX) == 1){ // this will acknowledge
    sendUARTPacket();
  }
}