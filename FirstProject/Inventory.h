#pragma once

typedef enum INV_POCKETS
{
	POCKETSTATE_EQUIPABLE,
	POCKETSTATE_RESTORE,
	POCKETSTATE_VALUABLE,
	POCKETSTATE_ADVENTURE,
	POCKETSTATE_FAVORITE

} INV_POCKETS;

INV_POCKETS gCurrentPockets;

void DrawInventoryScreen(void);

void PPI_InventoryScreen(void);

void DrawEquipablePocket(void);

void DrawRestorePocket(void);

void DrawValuablePocket(void);

void MenuItem_Inventory_Equipable_Slot0(void);

void MenuItem_Inventory_Equipable_Slot1(void);

void MenuItem_Inventory_Equipable_Slot2(void);

void MenuItem_Inventory_Equipable_Slot3(void);

void MenuItem_Inventory_Equipable_Slot4(void);

void MenuItem_Inventory_Equipable_Slot5(void);

void MenuItem_Inventory_Equipable_Slot6(void);

void MenuItem_Inventory_Equipable_Slot7(void);

void MenuItem_Inventory_Equipable_Slot8(void);

void MenuItem_Inventory_Equipable_Slot9(void);

void MenuItem_Inventory_Equipable_Slot10(void);

void MenuItem_Inventory_Equipable_Slot11(void);

void MenuItem_Inventory_Equipable_Slot12(void);

void MenuItem_Inventory_Equipable_Slot13(void);

void MenuItem_Inventory_Equipable_Slot14(void);

void MenuItem_Inventory_Equipable_Slot15(void);

void MenuItem_Inventory_Equipable_Slot16(void);