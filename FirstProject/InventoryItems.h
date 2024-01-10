#pragma once

#include "Main.h"


typedef struct InventoryItem
{
	uint32_t ValueCurrency;				//TOUSE: ValueTokens , idea is more than one currency where an item could sell for money or tokens or whatever
	BOOL HasItem;
	uint16_t Count;
	char* Name;
	uint16_t Effect;
	char* Description;
	GAMEBITMAP Sprite;
	BOOL Favorite;

} InventoryItem;

InventoryItem gEquipableItems[NUM_EQUIP_ITEMS];

InventoryItem gUsableItems[NUM_USABLE_ITEMS];

InventoryItem gValuableItems[NUM_VALUABLE_ITEMS];

InventoryItem gAdventureItems[NUM_ADVENTURE_ITEMS];
