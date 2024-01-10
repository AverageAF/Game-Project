#pragma once

#include "Main.h"

#define ITEM_USE_EFFECT_NULL 0

#define ITEM_USE_EFFECT_HEAL_MONSTER 1

#define ITEM_USE_EFFECT_CAPTURE 2

#define ITEM_USE_EFFECT_EXP_MONSTER 3

#define ITEM_USE_EFFECT_HAPPINESS 4

#define ITEM_USE_EFFECT_TRAINING_INC 5

#define ITEM_USE_EFFECT_TRAINING_DEC 6

#define ITEM_USE_EFFECT_UPGRADE 7

#define ITEM_USE_EFFECT_TEACHMOVE_ONCE 8

#define ITEM_USE_EFFECT_TEACHMOVE_INFINITE 9

#define ITEM_USE_EFFECT_CURE_STATUS 10

#define ITEM_USE_EFFECT_PREVENT_ENCOUNTER 11

#define ITEM_USE_EFFECT_REVIVE_MONSTER 12

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
