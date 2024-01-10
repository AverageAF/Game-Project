#pragma once

typedef enum INV_POCKETS
{
	POCKETSTATE_EQUIPABLE,
	POCKETSTATE_USABLE,
	POCKETSTATE_VALUABLE,
	POCKETSTATE_ADVENTURE,
	POCKETSTATE_MONSTER,
	POCKETSTATE_MONSTER_SELECT,

} INV_POCKETS;

INV_POCKETS gCurrentPockets;
INV_POCKETS gPreviousPockets;


//// USABLESLOT MENU GLOBALS ////

uint16_t gUseableSlotIndex[/*_countof(gMI_InventoryUseable_Items)*/17];   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gUseableSlotOffset;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gUseableHasItemSort[NUM_USABLE_ITEMS];						// simple sorting algorithm that sorts the gUseableItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gUseableItemCount;                                           //total number of non-zero index's in the usable pocket (number of unique resore items owned by the player)

uint16_t gEquipHasItemSort[NUM_EQUIP_ITEMS]; 
uint16_t gEquipItemCount;
uint16_t gValuableHasItemSort[NUM_VALUABLE_ITEMS];
uint16_t gValuableItemCount;
uint16_t gAdventureHasItemSort[NUM_ADVENTURE_ITEMS];
uint16_t gAdventureItemCount;

void DrawInventoryScreen(void);

void PPI_InventoryScreen(void);

void DrawEquipablePocket(void);

void DrawUseablePocket(void);

void DrawValuablePocket(void);

void DrawAdvemturePocket(void);

void DrawMonsterParty(void);

void DrawSelectedItemOptions(void);

void DrawSelectedMonsterOptions(void);


void MenuItem_Inventory_ItemSelected_Action(void);

void MenuItem_Inventory_MonsterSelected_Action(void);

void MenuItem_Inventory_SelectedItem_Back(void);

void MenuItem_Inventory_SelectedItem_Trash(void);

void MenuItem_Inventory_SelectedItem_Use(void);

void MenuItem_Inventory_SelectedItem_Inspect(void);

void MenuItem_Inventory_SelectedItem_Rename(void);

void MenuItem_Inventory_SelectedItem_Equip(void);

void MenuItem_Inventory_SelectedItem_Unequip(void);

void MenuItem_Inventory_SelectedItem_Sell(void);