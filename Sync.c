// Manages synchronization between player states through UART

#include "Sync.h"
#include "FIFO.h"
#include "UART1.h"
#include "UART2.h"
#include "sprites.h"
#include "../inc/Timer.h"
#include "ti/devices/msp/msp.h"

extern double otherPosX, otherPosY;
extern double otherDirX, otherDirY;
extern int otherHealth;
extern Sprite Sprites[];
Sprite* otherPlayer = Sprites;
extern double posX, posY;
extern double dirX, dirY;
extern int playerHealth;
extern uint8_t healthCode;
uint8_t itemsStatus = 1;
extern const uint16_t AgentPixelFront[];
extern const uint16_t AgentPixelBack[];

void Sync_Init(){
  UART1_Init();
  UART2_Init();
  TimerG8_IntArm(1000000/128, 128, 1);
  TimerG7_IntArm(400000/128, 128, 2);
  TimerG6_IntArm(40000, 200, 3);
}

uint8_t getPositionPacket(){
  if (RxFifo_Size() < 4) return 0;
  while (UART2_InChar() != '<'){}
  uint8_t inX = UART2_InChar();
  uint8_t inY = UART2_InChar();
  uint8_t endSentinel = UART2_InChar();
  if (endSentinel == '>'){
    otherPlayer->x = inX/10.0;
    otherPlayer->y = inY/10.0;
    otherPosX = otherPlayer->x;
    otherPosY = otherPlayer->y;
  }
  return 1;
}

uint8_t getDirectionPacket(){
  if (RxFifo_Size() < 4) return 0;
  while (UART2_InChar() != '[') {}

  int8_t rawDirX = UART2_InChar();
  int8_t rawDirY = UART2_InChar();
  uint8_t endSentinel = UART2_InChar();

  if (endSentinel == ']') {
    otherDirX = rawDirX / 100.0;
    otherDirY = rawDirY / 100.0;

    double vecToMeX = posX - otherPosX;
    double vecToMeY = posY - otherPosY;

    // Dot product without normalization
    double dot = (otherDirX * vecToMeX) + (otherDirY * vecToMeY);

    // Approximate threshold — tweak as needed
    if (dot > 0.5) {
      otherPlayer->image = AgentPixelFront;
    } else {
      otherPlayer->image = AgentPixelBack;
    }
  }
  return 1;
}


uint8_t getInfoPacket(){
  if (RxFifo_Size() < 4) return 0;
  while (UART2_InChar() != '('){}
  uint8_t inHealthCode = UART2_InChar();
  uint8_t inItemsStatus = UART2_InChar();
  uint8_t endSentinel = UART2_InChar();
  if (endSentinel == ')'){
    switch (inHealthCode){
      case PISTOLCODE: playerHealth-=2; break;
      case SHOTGUNCODE: playerHealth -= 12; break;
      case RIFLECODE: playerHealth -= 3; break;
      case MEDKITCODE: otherHealth += 20; break;
      case RESPAWNCODE: otherHealth = 50; break;
    }
    uint8_t itemCode = (inItemsStatus >> 6) & 3; // Get top two bits for event code
    uint8_t sprite_index = inItemsStatus & 0xF; // Get bottom four bits for index of sprite to alter
    if (itemCode == PICKUPCODE){
      Sprites[sprite_index].width = 0;
    }
    else if (itemCode == SPAWNCODE){
      Sprites[sprite_index].width = Sprites[sprite_index].height;
    }
  }
  return 1;
}

void sendPositionPacket(){
  uint8_t sendX = (uint8_t) ((posX+0.05)*10);//convert into fixed point
  uint8_t sendY = (uint8_t) ((posY+0.05)*10);//convert into fixed point

  UART1_OutChar('<');
  UART1_OutChar(sendX);
  UART1_OutChar(sendY);
  UART1_OutChar('>');
}

void sendDirectionPacket(){
  int8_t sendX = (int8_t) ((dirX+.005)*100);//convert into fixed point
  int8_t sendY = (int8_t) ((dirY+.005)*100);//convert into fixed point

  UART1_OutChar('[');
  UART1_OutChar(sendX);
  UART1_OutChar(sendY);
  UART1_OutChar(']');
}

void sendInfoPacket(){
  UART1_OutChar('(');
  UART1_OutChar(healthCode);
  UART1_OutChar(itemsStatus);
  UART1_OutChar(')');
  healthCode = 1; // 1 means do nothing
  itemsStatus = 1; // 1 means do nothing
}

 void TIMG8_IRQHandler(void){
  if((TIMG8->CPU_INT.IIDX) == 1){ // this will acknowledge
    static uint8_t packetIndex = 0;
    if (packetIndex == 0) sendPositionPacket();
    else if (packetIndex == 1) sendDirectionPacket();
    else if (packetIndex == 2) sendInfoPacket();
    else{
      packetIndex = -1; // Will be set to 0 in the next line
    }
    packetIndex++;
  }
}

 void TIMG7_IRQHandler(void){
  if((TIMG7->CPU_INT.IIDX) == 1){ // this will acknowledge
    static uint8_t packetIndex = 0;
    if (packetIndex == 0) packetIndex += getPositionPacket();
    else if (packetIndex == 1) packetIndex += getDirectionPacket();
    else if (packetIndex == 2) packetIndex += getInfoPacket();
    else packetIndex = 0;
  }
}

void TIMG6_IRQHandler(void){ // Spawn new item every 30 seconds
  static uint8_t timeCt = 1;
  if((TIMG6->CPU_INT.IIDX) == 1){ // this will acknowledge
    if(timeCt == 0) generateSprite();
    timeCt = (timeCt + 1) % 450;
  }
}