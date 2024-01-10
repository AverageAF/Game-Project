
#include "Main.h"

#include "Inventory.h"
#include "InventoryItems.h"

//// MONSTERSLOT MENU VARIABLES ////



//// MONSTERSLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Monster_Slot0 = { "Slot1", 204, 36 + (10 * 2), TRUE, MenuItem_Inventory_Monster_Slot_Action };

MENUITEM gMI_Inventory_Monster_Slot1 = { "Slot2", 260, 36 + (10 * 4), TRUE, MenuItem_Inventory_Monster_Slot_Action };

MENUITEM gMI_Inventory_Monster_Slot2 = { "Slot3", 204, 36 + (10 * 6), TRUE, MenuItem_Inventory_Monster_Slot_Action };

MENUITEM gMI_Inventory_Monster_Slot3 = { "Slot4", 260, 36 + (10 * 8), TRUE, MenuItem_Inventory_Monster_Slot_Action };

MENUITEM gMI_Inventory_Monster_Slot4 = { "Slot5", 204, 36 + (10 * 10), TRUE, MenuItem_Inventory_Monster_Slot_Action };

MENUITEM gMI_Inventory_Monster_Slot5 = { "Slot6", 260, 36 + (10 * 12), TRUE, MenuItem_Inventory_Monster_Slot_Action };

MENUITEM* gMI_InventoryMonster_Items[] = { &gMI_Inventory_Monster_Slot0, &gMI_Inventory_Monster_Slot1, &gMI_Inventory_Monster_Slot2, &gMI_Inventory_Monster_Slot3, &gMI_Inventory_Monster_Slot4, &gMI_Inventory_Monster_Slot5 };

MENU gMenu_InventoryMonster = { "Monster Items", 0, _countof(gMI_InventoryMonster_Items), gMI_InventoryMonster_Items };

//// ADVENTURESLOT MENU VARIABLES ////

uint16_t gAdventureSlotIndex[/*_countof(gMI_InventoryAdventure_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gAdventureSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gAdventureHasItemSort[NUM_ADVENTURE_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gAdventureItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gAdventureItemCount = 0;                                           //total number of non-zero index's in the adventure pocket (number of unique resore items owned by the player)

//// ADVENTURESLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Adventure_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM gMI_Inventory_Adventure_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_Adventure_Slot_Action };

MENUITEM* gMI_InventoryAdventure_Items[] = { &gMI_Inventory_Adventure_Slot0, &gMI_Inventory_Adventure_Slot1, &gMI_Inventory_Adventure_Slot2, &gMI_Inventory_Adventure_Slot3, &gMI_Inventory_Adventure_Slot4, &gMI_Inventory_Adventure_Slot5, &gMI_Inventory_Adventure_Slot6, &gMI_Inventory_Adventure_Slot7, &gMI_Inventory_Adventure_Slot8, &gMI_Inventory_Adventure_Slot9, &gMI_Inventory_Adventure_Slot10, &gMI_Inventory_Adventure_Slot11, &gMI_Inventory_Adventure_Slot12, &gMI_Inventory_Adventure_Slot13, &gMI_Inventory_Adventure_Slot14, &gMI_Inventory_Adventure_Slot15, &gMI_Inventory_Adventure_Slot16 };

MENU gMenu_InventoryAdventure = { "Adventure Items", 0, _countof(gMI_InventoryAdventure_Items), gMI_InventoryAdventure_Items };





//// VALUABLESLOT MENU VARIABLES ////

uint16_t gValuableSlotIndex[/*_countof(gMI_InventoryValuable_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gValuableSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gValuableHasItemSort[NUM_VALUABLE_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gValuableItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gValuableItemCount = 0;                                           //total number of non-zero index's in the valuable pocket (number of unique resore items owned by the player)

//// VALUABLESLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Valuable_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM gMI_Inventory_Valuable_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_Valuable_Slot_Action };

MENUITEM* gMI_InventoryValuable_Items[] = { &gMI_Inventory_Valuable_Slot0, &gMI_Inventory_Valuable_Slot1, &gMI_Inventory_Valuable_Slot2, &gMI_Inventory_Valuable_Slot3, &gMI_Inventory_Valuable_Slot4, &gMI_Inventory_Valuable_Slot5, &gMI_Inventory_Valuable_Slot6, &gMI_Inventory_Valuable_Slot7, &gMI_Inventory_Valuable_Slot8, &gMI_Inventory_Valuable_Slot9, &gMI_Inventory_Valuable_Slot10, &gMI_Inventory_Valuable_Slot11, &gMI_Inventory_Valuable_Slot12, &gMI_Inventory_Valuable_Slot13, &gMI_Inventory_Valuable_Slot14, &gMI_Inventory_Valuable_Slot15, &gMI_Inventory_Valuable_Slot16 };

MENU gMenu_InventoryValuable = { "Valuable Items", 0, _countof(gMI_InventoryValuable_Items), gMI_InventoryValuable_Items };




//// RESTORESLOT MENU VARIABLES ////

uint16_t gRestoreSlotIndex[/*_countof(gMI_InventoryRestore_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gRestoreSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gRestoreHasItemSort[NUM_RESTORE_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gRestoreItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gRestoreItemCount = 0;                                           //total number of non-zero index's in the restore pocket (number of unique resore items owned by the player)

//// RESTORESLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Restore_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM gMI_Inventory_Restore_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_Restore_Slot_Action };

MENUITEM* gMI_InventoryRestore_Items[] = { &gMI_Inventory_Restore_Slot0, &gMI_Inventory_Restore_Slot1, &gMI_Inventory_Restore_Slot2, &gMI_Inventory_Restore_Slot3, &gMI_Inventory_Restore_Slot4, &gMI_Inventory_Restore_Slot5, &gMI_Inventory_Restore_Slot6, &gMI_Inventory_Restore_Slot7, &gMI_Inventory_Restore_Slot8, &gMI_Inventory_Restore_Slot9, &gMI_Inventory_Restore_Slot10, &gMI_Inventory_Restore_Slot11, &gMI_Inventory_Restore_Slot12, &gMI_Inventory_Restore_Slot13, &gMI_Inventory_Restore_Slot14, &gMI_Inventory_Restore_Slot15, &gMI_Inventory_Restore_Slot16 };

MENU gMenu_InventoryRestore = { "Restore Items", 0, _countof(gMI_InventoryRestore_Items), gMI_InventoryRestore_Items };



//// EQUIPSLOT MENU VARIABLES ////

uint16_t gEquipSlotIndex[/*_countof(gMI_InventoryEquipable_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gEquipSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gEquipHasItemSort[NUM_EQUIP_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gEquipableItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gEquipItemCount = 0;                                           //total number of non-zero index's in the equipables pocket (number of unique equipable items owned by the player)

//// EQUIPSLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Equipable_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM gMI_Inventory_Equipable_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_Equipable_Slot_Action };

MENUITEM* gMI_InventoryEquipable_Items[] = { &gMI_Inventory_Equipable_Slot0, &gMI_Inventory_Equipable_Slot1, &gMI_Inventory_Equipable_Slot2, &gMI_Inventory_Equipable_Slot3, &gMI_Inventory_Equipable_Slot4, &gMI_Inventory_Equipable_Slot5, &gMI_Inventory_Equipable_Slot6, &gMI_Inventory_Equipable_Slot7, &gMI_Inventory_Equipable_Slot8, &gMI_Inventory_Equipable_Slot9, &gMI_Inventory_Equipable_Slot10, &gMI_Inventory_Equipable_Slot11, &gMI_Inventory_Equipable_Slot12, &gMI_Inventory_Equipable_Slot13, &gMI_Inventory_Equipable_Slot14, &gMI_Inventory_Equipable_Slot15, &gMI_Inventory_Equipable_Slot16 };

MENU gMenu_InventoryEquipable = { "Equipable Items", 0, _countof(gMI_InventoryEquipable_Items), gMI_InventoryEquipable_Items };


void DrawInventoryScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    static uint16_t InventoryTextLine1CharactersToShow = 0;

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
        uint16_t count = 0;
        for (uint16_t i = 0; i < NUM_EQUIP_ITEMS; i++)
        {
            if (gEquipableItems[i].Count > 0)
            {
                gEquipableItems[i].HasItem = TRUE;
            }
            else
            {
                gEquipableItems[i].HasItem = FALSE;
            }
            if (gEquipableItems[i].HasItem == TRUE)
            {
                gEquipHasItemSort[count] = i;
                count++;
            }
            if (i == NUM_EQUIP_ITEMS - 1)
            {
                gEquipItemCount = count;
                for (uint8_t j = 0; j < NUM_EQUIP_ITEMS - count; j++)
                {
                    gEquipHasItemSort[count + j] = 0xFFFF;
                }

            }
        }

        count = 0;
        for (uint16_t i = 0; i < NUM_RESTORE_ITEMS; i++)
        {
            if (gRestoreItems[i].Count > 0)
            {
                gRestoreItems[i].HasItem = TRUE;
            }
            else
            {
                gRestoreItems[i].HasItem = FALSE;
            }
            if (gRestoreItems[i].HasItem == TRUE)
            {
                gRestoreHasItemSort[count] = i;
                count++;
            }
            if (i == NUM_RESTORE_ITEMS - 1)
            {
                gRestoreItemCount = count;
                for (uint8_t j = 0; j < NUM_RESTORE_ITEMS - count; j++)
                {
                    gRestoreHasItemSort[count + j] = 0xFFFF;
                }

            }
        }

        count = 0;
        for (uint16_t i = 0; i < NUM_VALUABLE_ITEMS; i++)
        {
            if (gValuableItems[i].Count > 0)
            {
                gValuableItems[i].HasItem = TRUE;
            }
            else
            {
                gValuableItems[i].HasItem = FALSE;
            }
            if (gValuableItems[i].HasItem == TRUE)
            {
                gValuableHasItemSort[count] = i;
                count++;
            }
            if (i == NUM_VALUABLE_ITEMS - 1)
            {
                gValuableItemCount = count;
                for (uint8_t j = 0; j < NUM_VALUABLE_ITEMS - count; j++)
                {
                    gValuableHasItemSort[count + j] = 0xFFFF;
                }

            }
        }

        count = 0;
        for (uint16_t i = 0; i < NUM_ADVENTURE_ITEMS; i++)
        {
            if (gAdventureItems[i].Count > 0)
            {
                gAdventureItems[i].HasItem = TRUE;
            }
            else
            {
                gAdventureItems[i].HasItem = FALSE;
            }
            if (gAdventureItems[i].HasItem == TRUE)
            {
                gAdventureHasItemSort[count] = i;
                count++;
            }
            if (i == NUM_ADVENTURE_ITEMS - 1)
            {
                gAdventureItemCount = count;
                for (uint8_t j = 0; j < NUM_ADVENTURE_ITEMS - count; j++)
                {
                    gAdventureHasItemSort[count + j] = 0xFFFF;
                }

            }
        }
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    if (gCurrentPockets == POCKETSTATE_MONSTER)
    {
        DrawWindow(192, 1, 184, 192, &COLOR_NES_WHITE, &COLOR_FORREST_GREEN, &COLOR_DARK_GRAY, WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    }
    else
    {
        DrawWindow(192, 1, 184, 192, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    }

    DrawWindow(24, 24, 32, 32, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    DrawWindow(8, 152, 176, 64, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    switch (gCurrentPockets)
    {
        case POCKETSTATE_EQUIPABLE:
        {
            DrawEquipablePocket();
            break;
        }
        case POCKETSTATE_RESTORE:
        {
            DrawRestorePocket();
            break;
        }
        case POCKETSTATE_VALUABLE:
        {
            DrawValuablePocket();
            break;
        }
        case POCKETSTATE_ADVENTURE:
        {
            DrawAdvemturePocket();
            break;
        }
        case POCKETSTATE_MONSTER:
        {
            DrawMonsterParty();
            break;
        }
        default: 
        {
            ASSERT(FALSE, "Unknown pocketstate in DrawInventoryScreen!");
        }
    }


    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_InventoryScreen(void)
{
    switch (gCurrentPockets)
    {
        case POCKETSTATE_EQUIPABLE:
        {
            if (gMenu_InventoryEquipable.SelectedItem >= gEquipItemCount - 1 && gEquipItemCount > _countof(gMI_InventoryEquipable_Items))
            {
                gMenu_InventoryEquipable.SelectedItem = 0;
            }

            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                gDesiredGameState = gPreviousGameState;
                gPreviousGameState = gCurrentGameState;
                gCurrentGameState = gDesiredGameState;
            }

            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                if (gMenu_InventoryEquipable.SelectedItem == 0 && gEquipItemCount > _countof(gMI_InventoryEquipable_Items))
                {
                    gEquipSlotOffset--;      //change names on boxes when at bottom box and pushing "down" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryEquipable.SelectedItem > 0)
                {
                    gMenu_InventoryEquipable.SelectedItem--;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                if (gMenu_InventoryEquipable.SelectedItem == 16)
                {
                    gEquipSlotOffset++;      //change names on boxes when at top box and pushing "up" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryEquipable.SelectedItem < gEquipItemCount - 1)
                {
                    gMenu_InventoryEquipable.SelectedItem++;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed && gPlayerPartyCount > 0)
            {
                gCurrentPockets = POCKETSTATE_MONSTER;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_RESTORE;
                PlayGameSound(&gSoundMenuNavigate);
            }

            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryEquipable.SelectedItem <= 16 && gMenu_InventoryEquipable.SelectedItem >= 0))
            {
                if (gEquipSlotIndex[gMenu_InventoryEquipable.SelectedItem] != 0xFFFF)
                {
                    gMI_InventoryEquipable_Items[gMenu_InventoryEquipable.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
            }
            break;
        }
        case POCKETSTATE_RESTORE:
        {
            if (gMenu_InventoryRestore.SelectedItem >= gRestoreItemCount - 1 && gRestoreItemCount > _countof(gMI_InventoryRestore_Items))
            {
                gMenu_InventoryRestore.SelectedItem = 0;
            }

            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                gDesiredGameState = gPreviousGameState;
                gPreviousGameState = gCurrentGameState;
                gCurrentGameState = gDesiredGameState;
            }

            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                if (gMenu_InventoryRestore.SelectedItem == 0 && gRestoreItemCount > _countof(gMI_InventoryRestore_Items))
                {
                    gRestoreSlotOffset--;      //change names on boxes when at bottom box and pushing "down" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryRestore.SelectedItem > 0)
                {
                    gMenu_InventoryRestore.SelectedItem--;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                if (gMenu_InventoryRestore.SelectedItem == 16)
                {
                    gRestoreSlotOffset++;      //change names on boxes when at top box and pushing "up" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryRestore.SelectedItem < gRestoreItemCount - 1)
                {
                    gMenu_InventoryRestore.SelectedItem++;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_EQUIPABLE;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_VALUABLE;
                PlayGameSound(&gSoundMenuNavigate);
            }

            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryRestore.SelectedItem <= 16 && gMenu_InventoryRestore.SelectedItem >= 0))
            {
                if (gRestoreSlotIndex[gMenu_InventoryRestore.SelectedItem] != 0xFFFF)
                {
                    gMI_InventoryRestore_Items[gMenu_InventoryRestore.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
            }
            break;
        }
        case POCKETSTATE_VALUABLE:
        {
            if (gMenu_InventoryValuable.SelectedItem >= gEquipItemCount - 1 && gEquipItemCount > _countof(gMI_InventoryValuable_Items))
            {
                gMenu_InventoryValuable.SelectedItem = 0;
            }

            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                gDesiredGameState = gPreviousGameState;
                gPreviousGameState = gCurrentGameState;
                gCurrentGameState = gDesiredGameState;
            }

            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                if (gMenu_InventoryValuable.SelectedItem == 0 && gValuableItemCount > _countof(gMI_InventoryValuable_Items))
                {
                    gValuableSlotOffset--;      //change names on boxes when at bottom box and pushing "down" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryValuable.SelectedItem > 0)
                {
                    gMenu_InventoryValuable.SelectedItem--;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                if (gMenu_InventoryValuable.SelectedItem == 16)
                {
                    gValuableSlotOffset++;      //change names on boxes when at top box and pushing "up" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryValuable.SelectedItem < gValuableItemCount - 1)
                {
                    gMenu_InventoryValuable.SelectedItem++;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_RESTORE;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_ADVENTURE;
                PlayGameSound(&gSoundMenuNavigate);
            }

            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryValuable.SelectedItem <= 16 && gMenu_InventoryValuable.SelectedItem >= 0))
            {
                if (gEquipSlotIndex[gMenu_InventoryValuable.SelectedItem] != 0xFFFF)
                {
                    gMI_InventoryValuable_Items[gMenu_InventoryValuable.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
            }
            break;
        }
        case POCKETSTATE_ADVENTURE:
        {

            if (gMenu_InventoryAdventure.SelectedItem >= gEquipItemCount - 1 && gEquipItemCount > _countof(gMI_InventoryAdventure_Items))
            {
                gMenu_InventoryAdventure.SelectedItem = 0;
            }

            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                gDesiredGameState = gPreviousGameState;
                gPreviousGameState = gCurrentGameState;
                gCurrentGameState = gDesiredGameState;
            }

            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                if (gMenu_InventoryAdventure.SelectedItem == 0 && gAdventureItemCount > _countof(gMI_InventoryAdventure_Items))
                {
                    gAdventureSlotOffset--;      //change names on boxes when at bottom box and pushing "down" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryAdventure.SelectedItem > 0)
                {
                    gMenu_InventoryAdventure.SelectedItem--;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                if (gMenu_InventoryAdventure.SelectedItem == 16)
                {
                    gAdventureSlotOffset++;      //change names on boxes when at top box and pushing "up" key
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_InventoryAdventure.SelectedItem < gAdventureItemCount - 1)
                {
                    gMenu_InventoryAdventure.SelectedItem++;    //changes selected box
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_VALUABLE;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed && gPlayerPartyCount > 0)
            {
                gCurrentPockets = POCKETSTATE_MONSTER;
                PlayGameSound(&gSoundMenuNavigate);
            }

            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryAdventure.SelectedItem <= 16 && gMenu_InventoryAdventure.SelectedItem >= 0))
            {
                if (gEquipSlotIndex[gMenu_InventoryAdventure.SelectedItem] != 0xFFFF)
                {
                    gMI_InventoryAdventure_Items[gMenu_InventoryAdventure.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
            }
            break;
        }
        case POCKETSTATE_MONSTER:
        {
            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                gDesiredGameState = gPreviousGameState;
                gPreviousGameState = gCurrentGameState;
                gCurrentGameState = gDesiredGameState;
            }

            if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_ADVENTURE;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
            {
                gCurrentPockets = POCKETSTATE_EQUIPABLE;
                PlayGameSound(&gSoundMenuNavigate);
            }

            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed && gMenu_InventoryMonster.SelectedItem > 0)
            {
                gMenu_InventoryMonster.SelectedItem--;
                PlayGameSound(&gSoundMenuNavigate);
            }

            if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed && gMenu_InventoryMonster.SelectedItem < gPlayerPartyCount - 1)
            {
                gMenu_InventoryMonster.SelectedItem++;
                PlayGameSound(&gSoundMenuNavigate);
            }

            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryAdventure.SelectedItem <= 5 && gMenu_InventoryAdventure.SelectedItem >= 0))
            {
                gMI_InventoryMonster_Items[gMenu_InventoryMonster.SelectedItem]->Action();
                PlayGameSound(&gSoundMenuChoose);
            }

            break;
        }
        default:
        {
            ASSERT(FALSE, "Unknown pocketstate in PPI inventory screen!");
        }
    }
}

void DrawEquipablePocket(void)
{

    //equip pockets
    DrawWindow(194, 26, 35, 18, &COLOR_BLACK, &COLOR_LIGHT_BLUE, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //restore pockets
    DrawWindow(194 + (36 * 1), 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //valuable pockets
    DrawWindow(194 + (36 * 2), 26, 36, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);  //middle box is 1 pixel wider
    //adventure pockets
    DrawWindow(194 + (36 * 3) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);//offset next two boxes by one pixel
    //monsters
    DrawWindow(194 + (36 * 4) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);



    ////  --- / \ --- 
    DrawWindow(194, 25 + (10 * 2), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                    
    DrawWindow(194, 25 + (10 * 3), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                       
    DrawWindow(194, 25 + (10 * 4), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                       
    DrawWindow(194, 25 + (10 * 5), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                        
    DrawWindow(194, 25 + (10 * 6), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                       
    DrawWindow(194, 25 + (10 * 7), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                      
    DrawWindow(194, 25 + (10 * 8), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                       
    DrawWindow(194, 25 + (10 * 9), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                   
    DrawWindow(194, 25 + (10 * 10), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
                   
    DrawWindow(194, 25 + (10 * 11), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 12), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 13), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 14), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 15), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 16), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 17), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
    ////  --- \ / --- 
    DrawWindow(194, 25 + (10 * 18), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);


    for (uint8_t EquipBox = 0; EquipBox < _countof(gMI_InventoryEquipable_Items); EquipBox++)
    {
        uint16_t SlotLoop = 0x800A;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:



        SlotOrder = EquipBox + gEquipSlotOffset + SlotLoop;

        if (gEquipItemCount != 0)
        {
            SlotOrder %= gEquipItemCount;
        }

        SlotIndex = gEquipHasItemSort[SlotOrder];

        if (gEquipHasItemSort[EquipBox] == 0xFFFF)
        {
            SlotIndex = 0xFFFF;
            goto SlotIndexed;
        }
        if (SlotIndex == 0xFFFF)
        {
            SlotLoop++;
            goto SlotIndexing;
        }

    SlotIndexed:

        gEquipSlotIndex[EquipBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_InventoryEquipable_Items[EquipBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            uint16_t ItemCountSize = snprintf(NULL, 0, "x%d", gEquipableItems[SlotIndex].Count);
            char* ItemCountString = malloc(ItemCountSize + 1);
            snprintf(ItemCountString, ItemCountSize + 1, "x%d", gEquipableItems[SlotIndex].Count);

            BlitStringToBuffer(gEquipableItems[SlotIndex].Name, &g6x7Font, &COLOR_BLACK, gMI_InventoryEquipable_Items[EquipBox]->x, gMI_InventoryEquipable_Items[EquipBox]->y);

            BlitStringToBuffer(ItemCountString, &g6x7Font, &COLOR_BLACK, gMI_InventoryEquipable_Items[EquipBox]->x + 149, gMI_InventoryEquipable_Items[EquipBox]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryEquipable_Items[gMenu_InventoryEquipable.SelectedItem]->x - 6, gMI_InventoryEquipable_Items[gMenu_InventoryEquipable.SelectedItem]->y);
}

void DrawRestorePocket(void)
{

    //equip pockets
    DrawWindow(194, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //restore pockets
    DrawWindow(194 + (36 * 1), 26, 35, 18, &COLOR_BLACK, &COLOR_PINK, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //valuable pockets
    DrawWindow(194 + (36 * 2), 26, 36, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);  //middle box is 1 pixel wider
    //adventure pockets
    DrawWindow(194 + (36 * 3) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);//offset next two boxes by one pixel
    //monsters
    DrawWindow(194 + (36 * 4) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);



    ////  --- / \ --- 
    DrawWindow(194, 25 + (10 * 2), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 3), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 4), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 5), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 6), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 7), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 8), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 9), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 10), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 11), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 12), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 13), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 14), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 15), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 16), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 17), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
    ////  --- \ / --- 
    DrawWindow(194, 25 + (10 * 18), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);


    for (uint8_t RestoreBox = 0; RestoreBox < _countof(gMI_InventoryRestore_Items); RestoreBox++)
    {
        uint16_t SlotLoop = 0x800C;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:



        SlotOrder = RestoreBox + gRestoreSlotOffset + SlotLoop;

        if (gRestoreItemCount != 0)
        {
            SlotOrder %= gRestoreItemCount;
        }

        SlotIndex = gRestoreHasItemSort[SlotOrder];

        if (gRestoreHasItemSort[RestoreBox] == 0xFFFF)
        {
            SlotIndex = 0xFFFF;
            goto SlotIndexed;
        }
        if (SlotIndex == 0xFFFF)
        {
            SlotLoop++;
            goto SlotIndexing;
        }

    SlotIndexed:

        gRestoreSlotIndex[RestoreBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_InventoryRestore_Items[RestoreBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            uint16_t ItemCountSize = snprintf(NULL, 0, "x%d", gRestoreItems[SlotIndex].Count);
            char* ItemCountString = malloc(ItemCountSize + 1);
            snprintf(ItemCountString, ItemCountSize + 1, "x%d", gRestoreItems[SlotIndex].Count);

            BlitStringToBuffer(gRestoreItems[SlotIndex].Name, &g6x7Font, &COLOR_BLACK, gMI_InventoryRestore_Items[RestoreBox]->x, gMI_InventoryRestore_Items[RestoreBox]->y);

            BlitStringToBuffer(ItemCountString, &g6x7Font, &COLOR_BLACK, gMI_InventoryRestore_Items[RestoreBox]->x + 149, gMI_InventoryRestore_Items[RestoreBox]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryRestore_Items[gMenu_InventoryRestore.SelectedItem]->x - 6, gMI_InventoryRestore_Items[gMenu_InventoryRestore.SelectedItem]->y);
}

void DrawValuablePocket(void)
{

    //equip pockets
    DrawWindow(194, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //restore pockets
    DrawWindow(194 + (36 * 1), 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //valuable pockets
    DrawWindow(194 + (36 * 2), 26, 36, 18, &COLOR_BLACK, &COLOR_GOLD, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);  //middle box is 1 pixel wider
    //adventure pockets
    DrawWindow(194 + (36 * 3) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);//offset next two boxes by one pixel
    //monsters
    DrawWindow(194 + (36 * 4) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);



    ////  --- / \ --- 
    DrawWindow(194, 25 + (10 * 2), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 3), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 4), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 5), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 6), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 7), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 8), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 9), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 10), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 11), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 12), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 13), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 14), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 15), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 16), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 17), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
    ////  --- \ / --- 
    DrawWindow(194, 25 + (10 * 18), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);


    for (uint8_t ValuableBox = 0; ValuableBox < _countof(gMI_InventoryValuable_Items); ValuableBox++)
    {
        uint16_t SlotLoop = 0x8010;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:



        SlotOrder = ValuableBox + gValuableSlotOffset + SlotLoop;

        if (gValuableItemCount != 0)
        {
            SlotOrder %= gValuableItemCount;
        }

        SlotIndex = gValuableHasItemSort[SlotOrder];

        if (gValuableHasItemSort[ValuableBox] == 0xFFFF)
        {
            SlotIndex = 0xFFFF;
            goto SlotIndexed;
        }
        if (SlotIndex == 0xFFFF)
        {
            SlotLoop++;
            goto SlotIndexing;
        }

    SlotIndexed:

        gValuableSlotIndex[ValuableBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_InventoryValuable_Items[ValuableBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            uint16_t ItemCountSize = snprintf(NULL, 0, "x%d", gValuableItems[SlotIndex].Count);
            char* ItemCountString = malloc(ItemCountSize + 1);
            snprintf(ItemCountString, ItemCountSize + 1, "x%d", gValuableItems[SlotIndex].Count);

            BlitStringToBuffer(gValuableItems[SlotIndex].Name, &g6x7Font, &COLOR_BLACK, gMI_InventoryValuable_Items[ValuableBox]->x, gMI_InventoryValuable_Items[ValuableBox]->y);

            BlitStringToBuffer(ItemCountString, &g6x7Font, &COLOR_BLACK, gMI_InventoryValuable_Items[ValuableBox]->x + 149, gMI_InventoryValuable_Items[ValuableBox]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryValuable_Items[gMenu_InventoryValuable.SelectedItem]->x - 6, gMI_InventoryValuable_Items[gMenu_InventoryValuable.SelectedItem]->y);
}

void DrawAdvemturePocket(void)
{

    //equip pockets
    DrawWindow(194, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //restore pockets
    DrawWindow(194 + (36 * 1), 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //valuable pockets
    DrawWindow(194 + (36 * 2), 26, 36, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);  //middle box is 1 pixel wider
    //adventure pockets
    DrawWindow(194 + (36 * 3) + 1, 26, 35, 18, &COLOR_BLACK, &COLOR_FORREST_GREEN, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);//offset next two boxes by one pixel
    //monsters
    DrawWindow(194 + (36 * 4) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);



    ////  --- / \ --- 
    DrawWindow(194, 25 + (10 * 2), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 3), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 4), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 5), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 6), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 7), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 8), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 9), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 10), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 11), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 12), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 13), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 14), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 15), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 16), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(194, 25 + (10 * 17), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);
    ////  --- \ / --- 
    DrawWindow(194, 25 + (10 * 18), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);


    for (uint8_t AdventureBox = 0; AdventureBox < _countof(gMI_InventoryAdventure_Items); AdventureBox++)
    {
        uint16_t SlotLoop = 0x800D;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:



        SlotOrder = AdventureBox + gAdventureSlotOffset + SlotLoop;

        if (gAdventureItemCount != 0)
        {
            SlotOrder %= gAdventureItemCount;
        }

        SlotIndex = gAdventureHasItemSort[SlotOrder];

        if (gAdventureHasItemSort[AdventureBox] == 0xFFFF)
        {
            SlotIndex = 0xFFFF;
            goto SlotIndexed;
        }
        if (SlotIndex == 0xFFFF)
        {
            SlotLoop++;
            goto SlotIndexing;
        }

    SlotIndexed:

        gAdventureSlotIndex[AdventureBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_InventoryAdventure_Items[AdventureBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            uint16_t ItemCountSize = snprintf(NULL, 0, "x%d", gAdventureItems[SlotIndex].Count);
            char* ItemCountString = malloc(ItemCountSize + 1);
            snprintf(ItemCountString, ItemCountSize + 1, "x%d", gAdventureItems[SlotIndex].Count);

            BlitStringToBuffer(gAdventureItems[SlotIndex].Name, &g6x7Font, &COLOR_BLACK, gMI_InventoryAdventure_Items[AdventureBox]->x, gMI_InventoryAdventure_Items[AdventureBox]->y);

            BlitStringToBuffer(ItemCountString, &g6x7Font, &COLOR_BLACK, gMI_InventoryAdventure_Items[AdventureBox]->x + 149, gMI_InventoryAdventure_Items[AdventureBox]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryAdventure_Items[gMenu_InventoryAdventure.SelectedItem]->x - 6, gMI_InventoryAdventure_Items[gMenu_InventoryAdventure.SelectedItem]->y);
}

void DrawMonsterParty(void)
{

    //equip pockets
    DrawWindow(194, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //restore pockets
    DrawWindow(194 + (36 * 1), 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //valuable pockets
    DrawWindow(194 + (36 * 2), 26, 36, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);  //middle box is 1 pixel wider
    //adventure pockets
    DrawWindow(194 + (36 * 3) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);//offset next two boxes by one pixel
    //monsters
    DrawWindow(194 + (36 * 4) + 1, 26, 35, 18, &COLOR_BLACK, &COLOR_PURPLE, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);


    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryMonster_Items[gMenu_InventoryMonster.SelectedItem]->x - 9, gMI_InventoryMonster_Items[gMenu_InventoryMonster.SelectedItem]->y - 5);

    for (uint8_t playerParty = 0; playerParty < gPlayerPartyCount; playerParty++)
    {
        uint16_t HpPercent = 100 - ((gPlayerParty[playerParty].Health * 100) / (gPlayerParty[playerParty].MaxHealth));

        uint16_t ExpPercent = 100 - (((gPlayerParty[playerParty].DriveMonster.Experience - gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level]) * 100) / (gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level + 1] - gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level]));

        DrawMonsterHpBar(gMI_InventoryMonster_Items[playerParty]->x, gMI_InventoryMonster_Items[playerParty]->y, HpPercent, ExpPercent, gPlayerParty[playerParty].Level, gPlayerParty[playerParty].DriveMonster.nickname, TRUE);
    }
}

void MenuItem_Inventory_Equipable_Slot_Action(void)
{
    //TODO: make items usable with effects
    //gEquipableItems[gEquipSlotIndex[gMenu_InventoryEquipable.SelectedItem]]       is the currently selected item after pressing the "choose" key
    
}

void MenuItem_Inventory_Restore_Slot_Action(void)
{
    //TODO: make items usable with effects
    //gRestoreItems[gEquipSlotIndex[gMenu_InventoryRestore.SelectedItem]]       is the currently selected item after pressing the "choose" key

}

void MenuItem_Inventory_Valuable_Slot_Action(void)
{
    //TODO: make items usable with effects
    //gValuableItems[gEquipSlotIndex[gMenu_InventoryValuable.SelectedItem]]       is the currently selected item after pressing the "choose" key

}

void MenuItem_Inventory_Adventure_Slot_Action(void)
{
    //TODO: make items usable with effects
    //gValuableItems[gEquipSlotIndex[gMenu_InventoryValuable.SelectedItem]]       is the currently selected item after pressing the "choose" key

}

void MenuItem_Inventory_Monster_Slot_Action(void)
{
    
}