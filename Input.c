#include <stdint.h>
#include "Joy.h"
#include "../inc/Timer.h"
#include "ti/devices/msp/msp.h"
#include "../inc/LaunchPad.h"
#include "Sounds.h"
#include "Input.h"
#include "Inventory.h"
#include "Async_Delay.h"
#include "Sync.h"

#define RELOAD_TIME 1500
#define RIFLERATEMS 200

extern Inventory inventory;
extern uint8_t outX;
extern uint8_t outY;
extern uint8_t itemsStatus;
extern uint8_t canSwapItems;
uint8_t isSwapping;

void Input_Init(){
  TimerG12_IntArm(2666666, 1); // Initialize sampling for joystick, 30Hz
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
uint8_t isShooting;
uint8_t reloaded; // Reload flag
extern uint8_t healthCode;
uint8_t rifleCanShoot = 1;

void MovePlayer(uint8_t input, double moveSpeed_FB, double moveSpeed_LR, double rotSpeed) {
  Item* current = Inventory_currentItem(&inventory);
  // Sprint using joystick button
  if (Joy_InButton() && current->enabled){ // Can't sprint while reloading
    moveSpeed_FB *= 2;
    moveSpeed_LR *= 2;
  }

  // Shoot weapon
  if (input & 1 && (!(lastInput & 1) || (current->type == RIFLE && rifleCanShoot)) && current->enabled){
    if (!Item_isSpent(current)){
      if(current->type == RIFLE) start_delay(RIFLERATEMS, &rifleCanShoot);
      if (Item_isWeapon(current)){ 
        isShooting = 1;
      }
      Sound_Start(*current->sound);
      current->ammo--;
      if (self.isOnTarget){
        healthCode = current->healthcode;
        switch (healthCode){
          case PISTOLCODE: other.playerHealth -= 2; break;
          case SHOTGUNCODE: other.playerHealth -= 15; break;
          case RIFLECODE: other.playerHealth -= 3; break;
          case DEADCODE: other.playerHealth = 0; break; //for railgun
        }
      }
      else if(current->type == MEDKIT) {
        healthCode = current->healthcode;
        self.playerHealth += 20;
        if (self.playerHealth > 50) self.playerHealth = 50;
      }

      if (other.playerHealth < 0) other.playerHealth = 0;

      if (Item_isSpent(current) && (current->type == MEDKIT || current->type == RAILGUN)){
        Inventory_removeCurrent(&inventory);
      }
    }
    else{
      if(current->type == RIFLE) start_delay(RIFLERATEMS, &(current->enabled));
      Sound_Start(SoundEffects[OUTOFAMMO_SOUND]);
    }
  }

  // Reload or swap items
  if (input & (1<<4) && !(lastInput & (1<<4))){
    if (canSwapItems && inventory.index != 0){
      Sound_Start(SoundEffects[WEAPLOAD_SOUND]);
      isSwapping = 1;
    }
    if (Item_isWeapon(current) && current->enabled && (current->ammo < current->mag_ammo)){
      if(current->tot_ammo > 0){
        Sound_Start(SoundEffects[RELOAD_SOUND]);
        current->enabled = 0;
        start_delay(RELOAD_TIME, &reloaded);
      } else {
        Sound_Start(SoundEffects[OUTOFAMMO_SOUND]);
      }
    }
  }
  if (reloaded){
    current->enabled = 1; // Enable weapon use and switching
    if (current->tot_ammo >= (current->mag_ammo - current->ammo)){
      current->tot_ammo -= (current->mag_ammo - current->ammo);
      current->ammo  = current->mag_ammo;
    }
    else{
      current->ammo += current->tot_ammo;
      current->tot_ammo = 0;
    }
    reloaded = 0;
  }

  // Switch Weapons
  if (input & (1<<2) && !(lastInput & (1<<2)) && current->enabled && inventory.size > 1){
    Sound_Start(SoundEffects[WEAPLOAD_SOUND]);
    Inventory_next(&inventory);
  }
 
  // Rotate left or right
  if (input & (1 << 1)){
   double cosRot = fastCos(rotSpeed);
   double sinRot = -fastSin(rotSpeed);
   // Both camera direction and camera plane must be rotated
   double oldDirX = self.dirX;
   self.dirX = self.dirX * cosRot - self.dirY * sinRot;
   self.dirY = oldDirX * sinRot + self.dirY * cosRot;
   // Camera plane must be perpendicular to camera direction
   self.planeX = self.dirY;
   self.planeY = -self.dirX;
  }
 
  if (input & (1<<3)){
   double cosRot = fastCos(rotSpeed);
   double sinRot = fastSin(rotSpeed);
   // Both camera direction and camera plane must be rotated
   double oldDirX = self.dirX;
   self.dirX = self.dirX * cosRot - self.dirY * sinRot;
   self.dirY = oldDirX * sinRot + self.dirY * cosRot;
   // Camera plane must be perpendicular to camera direction
   self.planeX = self.dirY;
   self.planeY = -self.dirX;
  }
 
  // Joystick input:
  // Move forward or backward, checking for walls
  uint8_t isCollision_X = worldMap[(int)(self.posX + 2* self.dirX * moveSpeed_FB)][(int)self.posY] != 0;
  uint8_t isCollision_Y = worldMap[(int)self.posX][(int)(self.posY + 2*self.dirY * moveSpeed_FB)] != 0;
  if(!isCollision_X) self.posX += self.dirX * moveSpeed_FB;
  if(!isCollision_Y) self.posY += self.dirY * moveSpeed_FB;
  // Move left or right, checking for walls
  isCollision_X = worldMap[(int)(self.posX + 2* self.planeX * moveSpeed_LR)][(int)self.posY] != 0;
  isCollision_Y = worldMap[(int)self.posX][(int)(self.posY + 2*self.planeY * moveSpeed_LR)] != 0;
  if(!isCollision_X) self.posX += self.planeX * moveSpeed_LR;
  if(!isCollision_Y) self.posY += self.planeY * moveSpeed_LR;

  lastInput = input;
 }

void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    
    uint32_t x, y;
    Joy_In(&x, &y);

    Joy_x = -((int32_t)((x>>9) +1)/2 - 2);
    Joy_y = (int32_t)((y>>9)+1)/2 - 2;

    // Speed modifiers
   double moveSpeed = .033 * 1; // squares/sec
   double rotSpeed = .033 * 2.0;  // rads/sec
   double moveSpeed_FB = moveSpeed * Joy_y;
   double moveSpeed_LR = moveSpeed * Joy_x;

   MovePlayer(ReadKeys(), moveSpeed_FB, moveSpeed_LR, rotSpeed);
  }
}