#include "Graphics.h"
#include "Inventory.h"

uint8_t inInventory(Inventory* invent, Item* item){
    for(int i = 0; i < invent->size; i++){
        if(invent->items[i] == item){
            return 1;
        }
    }
    return 0;
}

uint8_t Inventory_add(Inventory* invent, Item* item){
    if (item->type == AMMO_SMALL) return AMMO_SMALL; // codes for ammo
    if (item->type == AMMO_BIG) return AMMO_BIG;
    if (invent->size == invent->cap) return 0;
    if(!inInventory(invent, item)){ //only adds item if not in inventory
        item->invent_sprite.scale = 1;
        invent->items[invent->size] = item;
        invent->items[invent->size]->ammo =  invent->items[invent->size]->mag_ammo;
        invent->items[invent->size]->tot_ammo = invent->items[invent->size]->start_ammo;
        invent->items[invent->size]->enabled = 1;
        invent->items[invent->size]->invent_sprite.x = 8 + (invent->size*16);
        invent->items[invent->size]->invent_sprite.y = SCREEN_HEIGHT-1;
        invent->size++;
        return 1;
    }
    return 0;
}

void Inventory_replace(Inventory* invent, Item* item){
    // if it's in your inventory already, we replace that item
    for (int i = 0; i < invent->size; i++){
        if(invent->items[i] == item){
            invent->index = i;
        }
    }
    item->invent_sprite.scale = 1;
    invent->items[invent->index] = item;
    invent->items[invent->index]->invent_sprite.x = 8 + (invent->index*16);
    invent->items[invent->index]->invent_sprite.y = SCREEN_HEIGHT-1;
    // refill ammo
    invent->items[invent->index]->ammo =  invent->items[invent->index]->mag_ammo;
    invent->items[invent->index]->tot_ammo = invent->items[invent->index]->start_ammo;
    invent->items[invent->index]->enabled = 1;
}

void Inventory_removeCurrent(Inventory* invent){
    if(invent->index < (invent->size-1)){ //shift only if not removing last
        for (int i = invent->index; i < invent->size - 1; i++){
        invent->items[i+1]->invent_sprite.x = invent->items[i]->invent_sprite.x; //shift next item left in inventory
        invent->items[i] = invent->items[i+1]; //replace removed elem with next elem
        }
    }
    if(invent->index > 0) invent->index--; //dont dec if index is 0 to avoid hardfault
    invent->size--;
}

void Inventory_next(Inventory* invent){
    invent->index++;
    if (invent->index >= invent->size) invent->index = 0;
}

Item* Inventory_currentItem(Inventory* invent){
    return invent->items[invent->index];
}

uint8_t Item_isWeapon(Item* item){
    return item->type < NUMWEAPONS;
}

uint8_t Item_isSpent(Item* item){
    return item->ammo == 0;
}