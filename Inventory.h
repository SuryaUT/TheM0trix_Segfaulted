#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdint.h>
#include "Sounds.h"
#include "sprites.h"

#define PISTOL 0
#define SHOTGUN 1
#define RIFLE 2
#define RAILGUN 3
#define MEDKIT 4
#define AMMO_SMALL 5
#define AMMO_BIG 6

typedef struct{
    const char* const* name;
    int8_t type;
    Sprite invent_sprite;
    Sprite holding_sprite;
    sound_t* sound;
    uint8_t ammo;
    uint8_t mag_ammo;
    uint8_t tot_ammo;
    uint8_t start_ammo;
    uint8_t crosshair_size;
    uint8_t enabled;
    uint8_t muzzleX;
    uint8_t muzzleY;
    uint8_t healthcode;
    uint8_t range;
} Item;

typedef struct {
    uint8_t size;
    uint8_t cap;
    Item* items[3];
    uint8_t index;
} Inventory;

uint8_t Inventory_add(Inventory* invent, Item* item);

void Inventory_replace(Inventory* invent, Item* item);

void Inventory_removeCurrent(Inventory* invent);

void Inventory_next(Inventory* invent);

Item* Inventory_currentItem(Inventory* invent);

#define NUMWEAPONS 4

uint8_t Item_isWeapon(Item* item);

uint8_t Item_isSpent(Item* item);

#endif