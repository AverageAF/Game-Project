#pragma once

typedef enum INV_POCKETS
{
	POCKETSTATE_EQUIPABLE,
	POCKETSTATE_RESTORE,
	POCKETSTATE_VALUABLE,
	POCKETSTATE_ADVENTURE,
	POCKETSTATE_MONSTER,

} INV_POCKETS;

INV_POCKETS gCurrentPockets;

void DrawInventoryScreen(void);

void PPI_InventoryScreen(void);

void DrawEquipablePocket(void);

void DrawRestorePocket(void);

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

void MenuItem_Inventory_SelectedItem_Equip(void);

void MenuItem_Inventory_SelectedItem_Inspect(void);

void MenuItem_Inventory_SelectedItem_Give(void);