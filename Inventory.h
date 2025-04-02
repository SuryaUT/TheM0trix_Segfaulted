#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdint.h>
#include "Sounds.h"
#include "sprites.h"

typedef struct{
    char* name;
    int8_t type;
    Sprite invent_sprite;
    Sprite holding_sprite;
    sound_t* sound;
    uint8_t ammo;
    uint8_t max_ammo;
} Item;

typedef struct {
    uint8_t size;
    uint8_t cap;
    Item* items[3];
    uint8_t index;
} Inventory;

uint8_t Inventory_add(Inventory* invent, Item* item);

void Inventory_replace(Inventory* invent, Item* item);

void Inventory_next(Inventory* invent);

Item* Inventory_currentItem(Inventory* invent);

#define NUMWEAPONS 2

uint8_t Item_isWeapon(Item* item);

uint8_t Item_isSpent(Item* item);

#endif