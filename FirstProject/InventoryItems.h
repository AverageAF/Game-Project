#pragma once

#include "Main.h"

#define ITEM_USE_EFFECT_NULL 0

#define ITEM_USE_EFFECT_HEAL_MONSTER 1

#define ITEM_USE_EFFECT_CAPTURE 2

#define ITEM_USE_EFFECT_EXP_MONSTER 3

#define ITEM_LASTUSE_HEAL INV_USABLE_ITEM_4

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

InventoryItem gUseableItems[NUM_USABLE_ITEMS];

InventoryItem gValuableItems[NUM_VALUABLE_ITEMS];

InventoryItem gAdventureItems[NUM_ADVENTURE_ITEMS];
