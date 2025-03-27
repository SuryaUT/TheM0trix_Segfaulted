#include <stdint.h>

#define SPRITE_WIDTH 24
#define SPRITE_HEIGHT 24

#define WHITE  0xFFFF // RGB565 White
#define BLACK  0x0000 // Transparent
#define REDS    0xF000 // RGB565 Red

typedef struct{
    double x;
    double y;
} Sprite;

extern double ZBuffer[];
extern Sprite targetSprite;
extern int target[SPRITE_HEIGHT][SPRITE_WIDTH];

void InitSprites();
void RenderSprite();