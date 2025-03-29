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

 void blitBufferToRenderBuffer(uint16_t* srcBuffer, int srcWidth, int srcHeight, int destX, int destY) {
    for (int y = 0; y < srcHeight; y++) {
        int renderY = destY + y;
        if (renderY >= 0 && renderY < BUFFER_HEIGHT) {
            for (int x = 0; x < srcWidth; x++) {
                int renderX = destX + x;
                if (renderX >= 0 && renderX < BUFFER_WIDTH) {
                    renderBuffer[renderY * BUFFER_WIDTH + renderX] = srcBuffer[y * srcWidth + x];
                }
            }
        }
    }
}

void drawForegroundSpriteToBuffer(int side, Sprite sprite) {
    // Let's say scale of 1 makes it a certain fraction of screen height
    double sizeFactor = sprite.scale / 8.0;

    int scaledSpriteHeight = (int)(SCREEN_HEIGHT * sizeFactor);
    int scaledSpriteWidth = (int)(scaledSpriteHeight * (double)sprite.width / sprite.height);

    int spriteBottomCenterY = (int)sprite.y;
    int spriteBottomCenterX = (int)sprite.x;

    int spriteTopY = spriteBottomCenterY - scaledSpriteHeight;
    int spriteLeftX = spriteBottomCenterX - scaledSpriteWidth/2;

    // Calculate screen-space drawing boundaries of the sprite
    int drawStartX = spriteLeftX;
    int drawEndX = spriteLeftX + scaledSpriteWidth;
    int drawStartY = spriteTopY;
    int drawEndY = spriteTopY + scaledSpriteHeight;

    int bufferBoundary = SCREEN_WIDTH / 2;

    for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
        int bufferX = -1;

        if (side == 0 && stripe >= 0 && stripe < bufferBoundary) {
            bufferX = stripe;
        } else if (side == 1 && stripe >= bufferBoundary && stripe < SCREEN_WIDTH) {
            bufferX = stripe - bufferBoundary;
        }

        if (bufferX != -1) {
            // Calculate texture x coordinate
            int texX = (stripe - drawStartX) * sprite.width / scaledSpriteWidth;
            if (texX >= 0 && texX < sprite.width) {
                for (int y = drawStartY; y < drawEndY; y++) {
                    if (y >= 0 && y < SCREEN_HEIGHT) {
                        // Calculate texture y coordinate (assuming sprite image is top-down)
                        int texY = (y - drawStartY) * sprite.height / scaledSpriteHeight;
                        if (texY >= 0 && texY < sprite.height) {
                            int index = texY * sprite.width + texX;
                            uint16_t pixelColor = sprite.image[index];

                            if (pixelColor != sprite.transparent) {
                                setPixelBuffer(bufferX, BUFFER_HEIGHT - 1 - y, pixelColor); // Use setPixelBuffer and invert y
                            }
                        }
                    }
                }
            }
        }
    }
}

void RenderBuffer(int side){
  if(side == 0) ST7735_DrawBitmap(0, BUFFER_HEIGHT, renderBuffer, BUFFER_WIDTH, BUFFER_HEIGHT);
  else ST7735_DrawBitmap(SCREEN_WIDTH/2, BUFFER_HEIGHT, renderBuffer, BUFFER_WIDTH, BUFFER_HEIGHT);
}