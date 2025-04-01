#include <stdint.h>
#include "Joy.h"
#include "../inc/Timer.h"
#include "ti/devices/msp/msp.h"
#include "../inc/LaunchPad.h"
#include "Sounds.h"
#include "Input.h"
#include "Items.h"

extern Inventory inventory;

void Input_Init(){
  TimerG12_IntArm(2666666, 2); // Initialize sampling for joystick, 30Hz
  Joy_Init();
 
  // Initialize buttons
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040081; // regular GPIO input, shoot key
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00040081; // regular GPIO input, right key
  IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00040081; // regular GPIO input, down key
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; // regular GPIO input, left key
  IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00040081; // regular GPIO input, up key
 }

uint8_t ReadKeys(){
  return (GPIOA->DIN31_0 >> 24) & 0x1F;
 }

double fastSin(double x) {
  return x - (x * x * x) / 6.0;
 }
 
 double fastCos(double x) {
  return 1.0f - (x * x) / 2.0;
 }

uint8_t lastInput = 0;
void MovePlayer(uint8_t input, double moveSpeed_FB, double moveSpeed_LR, double rotSpeed) {
  // Shoot sound
  if (input & 1 && !(lastInput & 1)){
    if (ammo){
      Sound_Start(*inventory.items[inventory.index]->sound);
      ammo--;
    }
    else{
      Sound_Start(SoundEffects[OUTOFAMMO_SOUND]);
    }
  }
  if (input & (1<<4) && !(lastInput & (1<<4))){
    Sound_Start(SoundEffects[RELOAD_SOUND]);
    ammo = max_ammo;
  }
  if (input & (1<<2) && !(lastInput & (1<<2))){
    Sound_Start(SoundEffects[WEAPLOAD_SOUND]);
    Inventory_next(&inventory);
  }
 
  // Rotate left or right
  if (input & (1 << 1)){
   double cosRot = fastCos(rotSpeed);
   double sinRot = -fastSin(rotSpeed);
   // Both camera direction and camera plane must be rotated
   double oldDirX = dirX;
   dirX = dirX * cosRot - dirY * sinRot;
   dirY = oldDirX * sinRot + dirY * cosRot;
   // Camera plane must be perpendicular to camera direction
   planeX = dirY;
   planeY = -dirX;
  }
 
  if (input & (1<<3)){
   double cosRot = fastCos(rotSpeed);
   double sinRot = fastSin(rotSpeed);
   // Both camera direction and camera plane must be rotated
   double oldDirX = dirX;
   dirX = dirX * cosRot - dirY * sinRot;
   dirY = oldDirX * sinRot + dirY * cosRot;
   // Camera plane must be perpendicular to camera direction
   planeX = dirY;
   planeY = -dirX;
  }
 
  // Joystick input:
  // Move forward or backward, checking for walls
  uint8_t isCollision_X = worldMap[(int)(posX + 2* dirX * moveSpeed_FB)][(int)posY] != 0;
  uint8_t isCollision_Y = worldMap[(int)posX][(int)(posY + 2*dirY * moveSpeed_FB)] != 0;
  if(!isCollision_X) posX += dirX * moveSpeed_FB;
  if(!isCollision_Y) posY += dirY * moveSpeed_FB;
  // Move left or right, checking for walls
  isCollision_X = worldMap[(int)(posX + 2* planeX * moveSpeed_LR)][(int)posY] != 0;
  isCollision_Y = worldMap[(int)posX][(int)(posY + 2*planeY * moveSpeed_LR)] != 0;
  if(!isCollision_X) posX += planeX * moveSpeed_LR;
  if(!isCollision_Y) posY += planeY * moveSpeed_LR;

  lastInput = input;
 }

void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    
    uint32_t x, y;
    Joy_In(&x, &y);

    Joy_x = -((int32_t)((x>>9) +1)/2 - 2);
    Joy_y = (int32_t)((y>>9)+1)/2 - 2;

    // Speed modifiers
   double moveSpeed = .033* 2.5; // squares/sec
   double rotSpeed = .033 * 3.0;  // rads/sec
   double moveSpeed_FB = moveSpeed * Joy_y;
   double moveSpeed_LR = moveSpeed * Joy_x;
 
   MovePlayer(ReadKeys(), moveSpeed_FB, moveSpeed_LR, rotSpeed);
  }
}