#pragma once

#define MAX_STORE_ITEMS 32

uint8_t gLastStoreItem;

uint8_t gStoreType[NUM_CHAR_SPRITES];

BOOL gHasSelectedStoreItem;

uint16_t gSelectedStoreItem;

BOOL gSelectedStoreItemIsUsable;


//////////////////////////////

void DrawStoreScreen(void);

void DrawStoreMenuSlots(void);

void PPI_StoreScreen(void);

void MenuItem_Store_ItemSelected_Action(void);

void BlitUsableStoreItems(uint8_t storetype);

void BlitEquipableStoreItems(uint8_t storetype);

void DrawSelectedStoreOptions(void);