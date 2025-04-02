#ifndef ITEMS_H
#define ITEMS_H
#include "Inventory.h"
#include "sprites.h"

#define MAX_SPRITES 10
int numsprites = 2;
Sprite Sprites[MAX_SPRITES] = {
    {12, 12, target, 1, 32, 32, 6},
    {18, 12, Shotgunsprite, 0, 16, 16, 2, SHOTGUN}
};

Item pistol = {
    PISTOL,
    {8, SCREEN_HEIGHT, pistolsprite, 0xFFFF, 16, 16, 1, PISTOL},
    {SCREEN_WIDTH/2, SCREEN_HEIGHT, PistolImage, 0, 32, 32, 3},
    SoundEffects + PISTOL_SOUND
  };

Item shotgun = {
    SHOTGUN,
    {8, SCREEN_HEIGHT, Shotgunsprite, 0, 16, 16, 1, SHOTGUN},
    {SCREEN_WIDTH/2 + 24, SCREEN_HEIGHT, ShotgunImage, 0, 32, 32, 3},
    SoundEffects + SHOTGUN_SOUND
  };

Item* items[2] = {&pistol, &shotgun};

#endif