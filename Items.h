#ifndef __ITEMS_H
#define __ITEMS_H

#include <stdint.h>
#include "Sounds.h"
#include "sprites.h"

#define PISTOL 0
#define PISTOL2 1

typedef struct{
    int8_t type;
    Sprite invent_sprite;
    Sprite holding_sprite;
    sound_t* sound;
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

#endif