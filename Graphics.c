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
 #include "maps.h"
 #include "Sounds.h"
 #include "sprites.h"
 #include "Buffer.h"
 #include "Textures.h"
 #include "Input.h"
 
 uint8_t worldMap[MAP_WIDTH][MAP_HEIGHT];
 uint16_t miniMap[MAP_WIDTH * MAP_HEIGHT];
 
 void FillMap(const uint8_t map[MAP_WIDTH][MAP_HEIGHT]) {
  for (int i = MAP_WIDTH - 1; i >= 0; i--) {
   for (int j = 0; j < MAP_HEIGHT; j++) {
    worldMap[i][j] = map[i][j];
    int index = (MAP_WIDTH - 1 - i) * MAP_HEIGHT + j;
    miniMap[index] = (map[i][j] != 0) ? ST7735_WHITE : 0;
   }
  }
 }

extern double posX, posY;
extern double dirX, dirY;
extern double planeX, planeY;
extern uint8_t accuracyRad;
extern uint8_t isOnTarget;
extern uint8_t ammo, max_ammo;
extern int playerHealth;
 
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
    int minimapDestX = 0;
    int minimapDestY = SCREEN_HEIGHT - 24;

    blitBufferToRenderBuffer(miniMap, MAP_WIDTH, MAP_HEIGHT, minimapDestX, minimapDestY);

    // Draw player position on the minimap (remains the same)
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

void printAmmo(int side) {
    char buffer[10];

    // Format the ammo string using sprintf
    sprintf(buffer, "Ammo: %d/%d", ammo, max_ammo);

    // Call your existing printToBuffer function to display the ammo
    printToBuffer(buffer, 0, SCREEN_HEIGHT-24, ST7735_WHITE, side);
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
    RenderForegroundSprites(side);
    printAmmo(side);
}

 
 void RenderScene(){
  static int side = 0;
  //Clear buffer
  clearRenderBuffer();
  // Render random falling bars in the sky
  RenderSky();
  //Draw background elements like walls to renderBuffer
  CastRays(side);
  RenderSprites(side);
  RenderHUD(side);
  RenderBuffer(side);
  side = !side;
 }
 
 void Graphics_Init(){
  SPI_Init();
  ST7735_InitR(INITR_REDTAB);
  ST7735_SetRotation(1);
  FillMap(OGMap);  // Pick map here
  PrecalculateFloorGradient();
 }