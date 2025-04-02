#include "sprites.h"
#include "Graphics.h"
#include "Buffer.h"
#include "Inventory.h"
#include <stdlib.h>
#include <math.h>

extern double posX, posY;
extern double dirX, dirY;
extern double planeX, planeY;
extern double ZBuffer[SCREEN_WIDTH];
extern uint8_t accuracyRad;
extern uint8_t isOnTarget;

extern int numsprites;
extern Sprite Sprites[];
extern const uint16_t target[];
extern Inventory inventory;
extern Item* items[];

// Helper structure for sorting sprites
typedef struct {
    int index;
    double distance;
} SpriteDistancePair;

// Comparison function for sorting sprites
int compareSprites(const void *a, const void *b) {
    const SpriteDistancePair *spriteA = (SpriteDistancePair *)a;
    const SpriteDistancePair *spriteB = (SpriteDistancePair *)b;
    if (spriteA->distance < spriteB->distance) return 1;
    if (spriteA->distance > spriteB->distance) return -1;
    return 0;
}

void RenderSprite(Sprite sprite, int side, int sprite_index) {
    if (sprite.image == target) isOnTarget = 0;
    // Sprite position relative to the player
    double spriteX = sprite.x - posX;
    double spriteY = sprite.y - posY;

    // Inverse camera transformation
    double invDet = 1.0 / (planeX * dirY - dirX * planeY);
    double transformX = invDet * (dirY * spriteX - dirX * spriteY);
    double transformY = invDet * (-planeY * spriteX + planeX * spriteY);

    // Pick up sprite if it's an item
    if (sprite.image != target && transformY <= .5 && transformY > 0 && Inventory_add(&inventory, items[sprite.type])){ // Inventory_add() will return 0 if inventory is full
        Sprites[sprite_index].scale = 0;
    }

    // Ignore if behind player
    if (transformY <= 0.1) return;

    // Project sprite to screen
    int spriteScreenX = (int)((SCREEN_WIDTH / 2) * (1 + transformX / transformY));

    // Calculate sprite height and width
    int originalSpriteHeight = abs((int)(SCREEN_HEIGHT / transformY));
    int originalSpriteWidth = abs((int)(SCREEN_HEIGHT / transformY * sprite.width / sprite.height));

    // Scale sprite based on size
    int spriteHeight = originalSpriteHeight *sprite.scale/8.0;
    int spriteWidth = originalSpriteWidth * sprite.scale/8.0;

    double pushdown = (originalSpriteHeight - spriteHeight) / 2.0;

    // Calculate vertical drawing boundaries
    int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2 - pushdown;
    int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2 - pushdown;

    // Calculate horizontal drawing boundaries
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    int drawEndX = (spriteWidth + 1) / 2 + spriteScreenX;

    int bufferBoundary = SCREEN_WIDTH / 2;

    // Draw sprite to buffer
    for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
        int bufferX = -1;

        if (side == 0 && stripe >= 0 && stripe < bufferBoundary) {
            bufferX = stripe;
        } else if (side == 1 && stripe >= bufferBoundary && stripe < SCREEN_WIDTH) {
            bufferX = stripe - bufferBoundary;
        }

        if (bufferX != -1 && transformY < ZBuffer[stripe]) {
            if ((stripe >= SCREEN_WIDTH/2 - accuracyRad && stripe <= SCREEN_WIDTH/2 + accuracyRad) && sprite.image == target) isOnTarget = 1;
            // Calculate texture x coordinate
            int texX = (stripe - drawStartX) * sprite.width / spriteWidth;
            if (texX >= 0 && texX < sprite.width) {
                for (int y = drawStartY; y < drawEndY; y++) {
                    if (y >= 0 && y < SCREEN_HEIGHT) {
                        // Calculate texture y coordinate
                        int texY = (int)((drawEndY - y) * sprite.height / spriteHeight);
                        if (texY >= 0 && texY < sprite.height) {
                            int index = texY * sprite.width + texX;
                            int pixelColor = sprite.image[index];

                            if (pixelColor != sprite.transparent) {
                                // If the current pixel is not transparent
                                setPixelBuffer(bufferX, y, pixelColor);
                            }
                        }
                    }
                }
            }
        }
    }
}


void RenderSprites(int side) {
    SpriteDistancePair spriteOrder[numsprites];
    for (int i = 0; i < numsprites; i++) {
        spriteOrder[i].index = i;
        spriteOrder[i].distance = pow(posX - Sprites[i].x, 2) + pow(posY - Sprites[i].y, 2);
    }

    qsort(spriteOrder, numsprites, sizeof(SpriteDistancePair), compareSprites);

    for (int i = 0; i < numsprites; i++) {
        if (Sprites[spriteOrder[i].index].scale > 0) RenderSprite(Sprites[spriteOrder[i].index], side, spriteOrder[i].index);
    }
}

void RenderInventory(int side){
    for (int i = 0; i < inventory.size; i++){
        drawForegroundSpriteToBuffer(side, inventory.items[i]->invent_sprite);
    }

    if (side == 0){
         for (int i = 0; i < 16; i++){
             setPixelBuffer(i+(inventory.index*16), 1, MATRIX_NEON_GREEN);
         }
    }
}

void RenderForegroundSprites(int side){
    drawForegroundSpriteToBuffer(side, Inventory_currentItem(&inventory)->holding_sprite);
    RenderInventory(side);
}