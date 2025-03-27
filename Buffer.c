#include "Graphics.h"
#include "Buffer.h"
#include "../inc/ST7735.h"

uint16_t renderBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

uint16_t floorGradient[SCREEN_HEIGHT / 2];

void PrecalculateFloorGradient() {
    for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
        double factor = (double)y / (SCREEN_HEIGHT / 2);
        uint16_t baseGreen = MATRIX_DARKER_GREEN & 0x07E0; // Extract green component
        uint16_t scaledGreen = (uint16_t)(baseGreen * (1 - factor)) & 0x07E0;
        floorGradient[y] = scaledGreen;
    }
}

void clearRenderBuffer(uint16_t color) {
    // Render pre-calculated floor gradient in the bottom half of the buffer
    for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
        uint16_t floorColor = floorGradient[y];
        int startIndex = y * BUFFER_WIDTH;
        for (int x = 0; x < BUFFER_WIDTH; x++) {
            renderBuffer[startIndex + x] = floorColor;
        }
    }

    // Clear the top half (sky) to background color
    for (int i = (BUFFER_WIDTH * BUFFER_HEIGHT) / 2; i < (BUFFER_WIDTH * BUFFER_HEIGHT); i++) {
        renderBuffer[i] = BACKGROUND_COLOR;
    }
}

void setPixelBuffer(int x, int y, uint16_t color) {
  if (x >= BUFFER_WIDTH){
    x -= BUFFER_WIDTH;
  }
  if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
   int index = y * BUFFER_WIDTH + x;
   renderBuffer[index] = color;
  }
 }

void RenderBuffer(int side){
  if(side == 0) ST7735_DrawBitmap(0, BUFFER_HEIGHT, renderBuffer, BUFFER_WIDTH, BUFFER_HEIGHT);
  else ST7735_DrawBitmap(SCREEN_WIDTH/2, BUFFER_HEIGHT, renderBuffer, BUFFER_WIDTH, BUFFER_HEIGHT);
}