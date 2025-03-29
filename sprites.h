#include <stdint.h>

typedef struct{
    double x;
    double y;
    const uint16_t* image;
    uint16_t transparent;
    int width;
    int height;
    int scale;
} Sprite;

void RenderSprites(int side);