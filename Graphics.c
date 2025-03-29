/* main.c
 * Elijah Silguero
 * date
 */
 
 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <math.h>
 #include "ti/devices/msp/msp.h"
 #include "../inc/ST7735.h"
 #include "../inc/SPI.h"
 #include "../inc/Clock.h"
 #include "../inc/Timer.h"
 #include "../inc/LaunchPad.h"
 #include "Graphics.h"
 #include "Joy.h"
 #include "maps.h"
 #include "Sounds.h"
 #include "sprites.h"
 #include "Buffer.h"
 #include "Textures.h"
 
 uint8_t worldMap[MAP_WIDTH][MAP_HEIGHT];
 uint16_t miniMap[MAP_WIDTH * MAP_HEIGHT] = {0};
 
 void FillMap(const uint8_t map[MAP_WIDTH][MAP_HEIGHT]) {
  for (int i = MAP_WIDTH - 1; i >= 0; i--) {
   for (int j = 0; j < MAP_HEIGHT; j++) {
    worldMap[i][j] = map[i][j];
    //int index = (MAP_WIDTH - 1 - i) * MAP_HEIGHT + j;
    miniMap[i*MAP_HEIGHT+j] = (map[i][j] != 0) ? ST7735_WHITE : 0;
   }
  }
 }
 
 // Player state
 double posX = 22, posY = 12;    // x and y start position
 double dirX = -1, dirY = 0;    // initial direction vector
 double planeX = 0, planeY = 0.66;  // the 2d raycaster version of camera plane
 int playerHealth = 50;
 uint8_t isOnTarget = 0;
 uint8_t accuracyRad = 6;
 
 double ZBuffer[SCREEN_WIDTH];
 
 void CastRays(int side) {
   //Determine loop bounds.
  int startX = (side == 0) ? 0 : SCREEN_WIDTH / 2;
  int endX = (side == 0) ? SCREEN_WIDTH / 2 : SCREEN_WIDTH;
 
  for (int x = startX; x < endX; x++) {
   // Calculate ray position and direction
   double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
   double rayDirX = dirX + planeX * cameraX;
   double rayDirY = dirY + planeY * cameraX;
 
   // Which box of the map we're in
   int mapX = (int)posX;
   int mapY = (int)posY;
 
   // Length of ray from current position to next x or y-side
   double sideDistX;
   double sideDistY;
 
   // Length of ray from one x or y-side to next x or y-side
   double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
   double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
   double perpWallDist;
 
   // What direction to step in x or y direction
   int stepX;
   int stepY;
 
   int hit = 0;   // Was there a wall hit?
   int sideHit;  // Was a NS or EW wall hit?
 
   // Calculate step and initial sideDist
   if (rayDirX < 0) {
    stepX = -1;
    sideDistX = (posX - mapX) * deltaDistX;
   } else {
    stepX = 1;
    sideDistX = (mapX + 1.0 - posX) * deltaDistX;
   }
   if (rayDirY < 0) {
    stepY = -1;
    sideDistY = (posY - mapY) * deltaDistY;
   } else {
    stepY = 1;
    sideDistY = (mapY + 1.0 - posY) * deltaDistY;
   }
 
   // Perform DDA
   while (hit == 0) {
    // Jump to next map square in x or y direction
    if (sideDistX < sideDistY) {
     sideDistX += deltaDistX;
     mapX += stepX;
     sideHit = 0;
    } else {
     sideDistY += deltaDistY;
     mapY += stepY;
     sideHit = 1;
    }
 
    // Check if ray hit a wall
    if (worldMap[mapX][mapY] > 0) hit = 1;
   }
 
   // Calculate distance from wall to camera plane
   if (sideHit == 0)
    perpWallDist = (sideDistX - deltaDistX);
   else
    perpWallDist = (sideDistY - deltaDistY);
   ZBuffer[x] = perpWallDist;
 
   // Calculate height of line to draw on screen
   int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
 
   // Calculate lowest and highest pixel to fill in current stripe
   int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
   if (drawStart < 0) drawStart = 0;
   int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
   if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
 
   int texNum = (worldMap[mapX][mapY] % 6) - 1; // Texture index based on map value (0-7)
    double wallX; // Where exactly the wall was hit
    if (sideHit == 0) wallX = posY + perpWallDist * rayDirY;
    else           wallX = posX + perpWallDist * rayDirX;
    wallX -= floor(wallX);

    int texX = (int)(wallX * (double)TEX_WIDTH);
    if (sideHit == 0 && rayDirX > 0) texX = TEX_WIDTH - texX - 1;
    if (sideHit == 1 && rayDirY < 0) texX = TEX_WIDTH - texX - 1;

    // Calculate how much to increase the texture coordinate per screen pixel
    double step = 1.0 * TEX_HEIGHT / lineHeight;
    // Starting texture coordinate
    double texPos = (drawStart - SCREEN_HEIGHT / 2.0 + lineHeight / 2.0) * step; //SCREEN_HEIGHT might be h

    for (int y = drawStart; y < drawEnd; y++) {
        // Integer texture coordinate
        int texY = (TEX_HEIGHT - 1) - ((int)texPos & (TEX_HEIGHT - 1)); // Flip texture vertically
        texPos += step;
        uint16_t color = textures[texNum][texY * TEX_WIDTH + texX];

        // Make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if (sideHit == 1) {color = (color >> 1) & 0x7BEF;}
        
        setPixelBuffer(x, y, color);
    }
  }
 }

 void RenderSky(){
    for (int i = 0; i < MAX_FALLING_BARS; i++) {
        uint8_t barX = rand() % BUFFER_WIDTH;
        uint8_t yStartOffset = rand() % (SCREEN_HEIGHT / 2); // Random start within the sky
        uint8_t length = 24 + i/2; // Random-ish length
        uint16_t color = MATRIX_GLOW_GREEN + ((rand()%100 - 50) << 5);

        uint8_t startY = SKY_HEIGHT_START + yStartOffset;
        uint8_t endY = startY + length;

        for (int y = startY; y < endY && y < SCREEN_HEIGHT; y++) {
            for (int xOffset = 0; xOffset < BAR_THICKNESS; xOffset++) {
                int currentX = barX + xOffset;
                if (currentX < BUFFER_WIDTH) {
                    setPixelBuffer(currentX, y, color);
                }
            }
        }
    }
 }

void DrawMinimap() {
    for (int y = SCREEN_HEIGHT - 24; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < 24; x++) {
            int screenYOffset = y - (SCREEN_HEIGHT - 24); // Normalize y to 0-23 range for map indexing
            if (screenYOffset < MAP_HEIGHT && x < MAP_WIDTH) {
                int worldMapRow = MAP_WIDTH - 1 - screenYOffset;
                int worldMapCol = x;
                int index = worldMapRow * MAP_HEIGHT + worldMapCol;

                uint16_t mapColor = (miniMap[index] != 0) ? ST7735_WHITE : BACKGROUND_COLOR;
                setPixelBuffer(x, y, mapColor);
            }
        }
    }

    // Draw player position on the minimap
    int playerMinimapY = (SCREEN_HEIGHT - 24) + ((MAP_HEIGHT - 1) - (int)posX);
    int playerMinimapX = (int)posY;

    if (playerMinimapY >= SCREEN_HEIGHT - 24 && playerMinimapY < SCREEN_HEIGHT && playerMinimapX >= 0 && playerMinimapX < 24) {
        setPixelBuffer(playerMinimapX, playerMinimapY, MATRIX_GREEN);
    }
}

int lastHealth = 0;
void DrawHealthBar(){
    int16_t barWidth = 50, barHeight = 10;
    int startX = SCREEN_WIDTH - barWidth - 6; // Top right, with 6 pixels margin
    int startY = SCREEN_HEIGHT - barHeight - 6;

    uint16_t healthColor;
    if (playerHealth >= 30){
        healthColor = MATRIX_DARK_GREEN;
    }
    else if (playerHealth >= 15){
        healthColor = ST7735_YELLOW;
    }
    else{
        healthColor = ST7735_RED;
    }

    // Draw the health bar
    for (int y = startY; y < startY + barHeight; y++) {
        for (int x = startX; x < startX + barWidth; x++) {
            if (x >= 0 && x < startX + playerHealth && y >= 0 && y < SCREEN_HEIGHT) {
                if (x < startX + playerHealth) {
                    setPixelBuffer(x, y, healthColor);
                }
            }
        }
    }
    lastHealth = playerHealth;
}

void DrawCrosshair(int side, int spacing, uint16_t color) {
    int size = 6;
    int thickness = 2;

    int centerX_fullScreen = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    if (side == 0) { // Left side
        // Upper vertical
        for (int y = centerY - size; y < centerY - spacing; y++) {
            for (int i = 0; i < thickness - 1; i++) {
                setPixelBuffer(centerX_fullScreen - thickness + i + 1, y, color);
            }
        }
        // Lower vertical
        for (int y = centerY + 1 + spacing; y < centerY + 1 + size; y++) {
            for (int i = 0; i < thickness - 1; i++) {
                setPixelBuffer(centerX_fullScreen - thickness + i + 1, y, color);
            }
        }
        // Left horizontal
        for (int i = -thickness / 2; i <= thickness / 2; i++) {
            for (int x = centerX_fullScreen - size; x < centerX_fullScreen - spacing; x++) {
                setPixelBuffer(x, centerY + i, color);
            }
        }
    } else if (side == 1) { // Right side
        // Upper vertical
        for (int y = centerY - size; y < centerY - spacing; y++) {
            for (int i = 0; i < thickness; i++) {
                setPixelBuffer(i, y, color);
            }
        }
        // Lower vertical
        for (int y = centerY + 1 + spacing; y < centerY + 1 + size; y++) {
            for (int i = 0; i < thickness; i++) {
                setPixelBuffer(i, y, color);
            }
        }
        // Right horizontal
        for (int i = -thickness / 2; i <= thickness / 2; i++) {
            for (int x = 1 + spacing; x <= size; x++) {
                setPixelBuffer(x, centerY + i, color);
            }
        }
    }
}

 void RenderHUD(int side){
    uint16_t crosshair_color = (isOnTarget) ? ST7735_RED : ST7735_WHITE;
    DrawCrosshair(side, 1, crosshair_color);
    if (side == 0){
        DrawMinimap();
    }
    else{
        DrawHealthBar();
    }
}

 
 void RenderScene(int side){
  //Clear buffer
  clearRenderBuffer(BACKGROUND_COLOR);
  // Render random falling bars in the sky
  RenderSky();
  //Draw background elements like walls to renderBuffer
  CastRays(side);
  RenderSprites(side);
  RenderHUD(side);
  RenderBuffer(side);
 }
 
 
 double fastSin(double x) {
  return x - (x * x * x) / 6.0;
 }
 
 double fastCos(double x) {
  return 1.0f - (x * x) / 2.0;
 }
 
 uint8_t lastTriggerIn = 0;
 void MovePlayer(uint8_t input, double moveSpeed_FB, double moveSpeed_LR, double rotSpeed) {
  // Shoot sound
  if (input & 1 && !lastTriggerIn){
   Sound_Start(Sounds[0]);
   lastTriggerIn = 1;
  }
  else if (!(input & 1)){
   lastTriggerIn = 0;
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
 }
 
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
 
 void Graphics_Init(){
  SPI_Init();
  ST7735_InitPrintf();
  ST7735_SetRotation(1);
  FillMap(OGMap);  // Pick map here
  PrecalculateFloorGradient();
 }
 
 void SystemInit() {
  __disable_irq();
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Sound_Init(80000000/11000, 0);
  Input_Init();
  Graphics_Init();
  __enable_irq();
 }
 
 uint8_t ReadKeys(){
  return (GPIOA->DIN31_0 >> 24) & 0x1F;
 }
 
 int Joy_x, Joy_y;
  int main() {
  SystemInit();
  int side = 0;
  while(1) {
   RenderScene(side);
   //Renderscene called for both sides
   side = 1 - side;

   if (playerHealth > 50) playerHealth = 50; else if (playerHealth < 0) playerHealth = 0;
 
   // Speed modifiers
   double moveSpeed = .05 * 2.5; // squares/sec
   double rotSpeed = .05 * 3.0;  // rads/sec
   double moveSpeed_FB = moveSpeed * Joy_y;
   double moveSpeed_LR = moveSpeed * Joy_x;
 
   MovePlayer(ReadKeys(), moveSpeed_FB, moveSpeed_LR, rotSpeed);
 
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
