#ifndef ITEMS_H
#define ITEMS_H
#include "Inventory.h"
#include "Sounds.h"
#include "images.h"
#include "Graphics.h"
#include "sprites.h"

#define MAX_SPRITES 9
int numsprites = 4;

Sprite Sprites[MAX_SPRITES] = {
    {12, 12, agent, 0xEF5D, 32, 32, 5},
    {18, 12, Shotgunsprite, 0, 16, 16, 2, SHOTGUN},
    {18, 18, Medkitsprite, 0, 16, 16, 3, MEDKIT},
    {20, 20, Ammosprite, 0, 16, 16, 2, AMMO_SMALL}
};

Sprite gunFlash = {SCREEN_HEIGHT/2, SCREEN_WIDTH/2, ExplosionImage, 0xFFFF, 32, 32, 1};

Item pistol = {
    "Pistol",
    PISTOL,
    {8, SCREEN_HEIGHT, pistolsprite, 0xFFFF, 16, 16, 1, PISTOL},
    {SCREEN_WIDTH/2 + 13, SCREEN_HEIGHT, PistolImage, 0xFFFF, 32, 32, 3},
    SoundEffects + PISTOL_SOUND,
    8,
    8,
    32,
    6,
    1,
    SCREEN_WIDTH/2 + 8,
    SCREEN_HEIGHT/2 + 24,
    PISTOLCODE
  };

Item shotgun = {
    "Shotgun",
    SHOTGUN,
    {8, SCREEN_HEIGHT, Shotgunsprite, 0, 16, 16, 1, SHOTGUN},
    {SCREEN_WIDTH/2 + 24, SCREEN_HEIGHT, ShotgunImage, 0, 32, 32, 3},
    SoundEffects + SHOTGUN_SOUND,
    1,
    1,
    5,
    12,
    1,
    SCREEN_WIDTH/2 + 8,
    SCREEN_HEIGHT/2 + 32,
    SHOTGUNCODE
  };

Item medkit = {
    "Medkit",
    MEDKIT,
    {8, SCREEN_HEIGHT, Medkitsprite, 0, 16, 16, 1, MEDKIT},
    {SCREEN_WIDTH-32, SCREEN_HEIGHT+16, Medkitsprite, 0, 16, 16, 3},
    SoundEffects + RELOAD_SOUND,
    1,
    1,
    1,
    0,
    1,
    0,
    0,
    MEDKITCODE
};

Item ammo_small = {
  "",
  AMMO_SMALL,
};

Item* items[4] = {&pistol, &shotgun, &medkit, &ammo_small};

#endif