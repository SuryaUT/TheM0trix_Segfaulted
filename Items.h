#ifndef ITEMS_H
#define ITEMS_H
#include "Inventory.h"
#include "Sounds.h"
#include "images.h"
#include "Graphics.h"
#include "sprites.h"

#define MAX_SPRITES 15
int numsprites = 11;

Sprite Sprites[MAX_SPRITES] = {
    {12, 12, (IS_DOMINANT_CONTROLLER) ? DrM0Front:AgentYFront, 0xA254, 64, 64, 5},
    // Item sprites will start out nonexistent, and spawn in over time, so their width is set to 0
    {18, 12, Shotgunsprite, 0xA254, 0, 16, 2, SHOTGUN},
    {18, 12, Shotgunsprite, 0xA254, 0, 16, 2, SHOTGUN},
    {18, 14, Riflesprite, 0xA254, 0, 16, 2, RIFLE},
    {18, 14, Riflesprite, 0xA254, 0, 16, 2, RIFLE},
    {18, 18, Medkitsprite, 0, 0, 16, 3, MEDKIT},
    {18, 18, Medkitsprite, 0, 0, 16, 3, MEDKIT},
    {18, 18, Medkitsprite, 0, 0, 16, 3, MEDKIT},
    {20, 20, Ammosprite, 0, 0, 16, 2, AMMO_SMALL},
    {20, 20, Ammosprite, 0, 0, 16, 2, AMMO_SMALL},
    {20, 22, Ammosprite, 0, 0, 16, 3, AMMO_BIG}
};

Sprite gunFlash = {SCREEN_HEIGHT/2, SCREEN_WIDTH/2, ExplosionImage, 0xFFFF, 32, 32, 1};

const char* const pistolName[] = {"Pistol", "Pistola", "Grakk"};
const char* const shotgunName[] = {"Shotgun", "Escopeta", "Blamz"};
const char* const rifleName[] = {"Rifle", "Rifle", "Skrr"};
const char* const medkitName[] = {"Medkit", "Botiquin", "Zibzab"};


Item pistol = {
    pistolName,
    PISTOL,
    {8, SCREEN_HEIGHT, Pistolsprite, 0xA254, 16, 16, 1, PISTOL},
    {SCREEN_WIDTH/2 + 30, SCREEN_HEIGHT, PistolImage, 0xA254, 32, 32, 3},
    SoundEffects + PISTOL_SOUND,
    8,
    8,
    32,
    32,
    6,
    1,
    SCREEN_WIDTH/2 + 9,
    SCREEN_HEIGHT/2 + 29,
    PISTOLCODE
  };

Item shotgun = {
    shotgunName,
    SHOTGUN,
    {8, SCREEN_HEIGHT, Shotgunsprite, 0xA254, 16, 16, 1, SHOTGUN},
    {SCREEN_WIDTH/2 + 24, SCREEN_HEIGHT, ShotgunImage, 0xA254, 32, 32, 3},
    SoundEffects + SHOTGUN_SOUND,
    1,
    1,
    5,
    5,
    12,
    1,
    SCREEN_WIDTH/2 + 11,
    SCREEN_HEIGHT/2 + 39,
    SHOTGUNCODE
  };

Item rifle = {
    rifleName,
    RIFLE,
    {8, SCREEN_HEIGHT, Riflesprite, 0xA254, 16, 16, 1, RIFLE},
    {SCREEN_WIDTH/2 + 34, SCREEN_HEIGHT, RifleImage, 0xA254, 32, 32, 4},
    SoundEffects + PISTOL_SOUND,
    15,
    15,
    45,
    45,
    4,
    1,
    SCREEN_WIDTH/2 + 9,
    SCREEN_HEIGHT/2 + 21,
    RIFLECODE
  };

Item medkit = {
    medkitName,
    MEDKIT,
    {8, SCREEN_HEIGHT, Medkitsprite, 0, 16, 16, 1, MEDKIT},
    {SCREEN_WIDTH-32, SCREEN_HEIGHT+16, Medkitsprite, 0, 16, 16, 3},
    SoundEffects + RELOAD_SOUND,
    1,
    1,
    1,
    1,
    0,
    1,
    0,
    0,
    MEDKITCODE
};

Item* items[6] = {&pistol, &shotgun, &rifle, &medkit};

#endif