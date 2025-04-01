#include "Graphics.h"
#include "Items.h"

uint8_t Inventory_add(Inventory* invent, Item* item){
    if (invent->size == invent->cap) return 0;
    item->invent_sprite.scale = 1;
    invent->items[invent->size] = item;
    invent->items[invent->size]->invent_sprite.x = 8 + (invent->size*16);
    invent->items[invent->size]->invent_sprite.y = SCREEN_HEIGHT-1;
    invent->size++;
    return 1;
}

void Inventory_replace(Inventory* invent, Item* item){
    invent->items[invent->index] = item;
}

void Inventory_next(Inventory* invent){
    invent->index++;
    if (invent->index >= invent->size) invent->index = 0;
}