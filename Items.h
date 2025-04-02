#ifndef ITEMS_H
#define ITEMS_H
#include "Inventory.h"
#include "Sounds.h"
#include "images.h"
#include "sprites.h"

#define PISTOL 0
#define SHOTGUN 1
#define MEDKIT 2

#define MAX_SPRITES 10
int numsprites = 3;
Sprite Sprites[MAX_SPRITES] = {
    {12, 12, target, 1, 32, 32, 6},
    {18, 12, Shotgunsprite, 0, 16, 16, 2, SHOTGUN},
    {18, 18, Medkitsprite, 0, 16, 16, 3, MEDKIT}
};

Item pistol = {
    "Pistol",
    PISTOL,
    {8, SCREEN_HEIGHT, pistolsprite, 0xFFFF, 16, 16, 1, PISTOL},
    {SCREEN_WIDTH/2, SCREEN_HEIGHT, PistolImage, 0, 32, 32, 3},
    SoundEffects + PISTOL_SOUND,
    8,
    8
  };

Item shotgun = {
    "Shotgun",
    SHOTGUN,
    {8, SCREEN_HEIGHT, Shotgunsprite, 0, 16, 16, 1, SHOTGUN},
    {SCREEN_WIDTH/2 + 24, SCREEN_HEIGHT, ShotgunImage, 0, 32, 32, 3},
    SoundEffects + SHOTGUN_SOUND,
    5,
    5
  };

Item medkit = {
    "Medkit",
    MEDKIT,
    {8, SCREEN_HEIGHT, Medkitsprite, 0, 16, 16, 1, MEDKIT},
    {SCREEN_WIDTH-32, SCREEN_HEIGHT+16, Medkitsprite, 0, 16, 16, 3},
    SoundEffects + RELOAD_SOUND,
    1,
    1
};

Item* items[3] = {&pistol, &shotgun, &medkit};

#endif