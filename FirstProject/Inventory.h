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


void MenuItem_Inventory_Equipable_Slot_Action(void);

void MenuItem_Inventory_Restore_Slot_Action(void);

void MenuItem_Inventory_Valuable_Slot_Action(void);

void MenuItem_Inventory_Adventure_Slot_Action(void);

void MenuItem_Inventory_Monster_Slot_Action(void);