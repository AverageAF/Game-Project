
#include "Main.h"

#include "Inventory.h"
#include "InventoryItems.h"
#include "MonsterStatsScreen.h"


//// SELECTEDSLOT MENU VARIABLES ////

BOOL gHasSelectedInvSlot = FALSE;

//// SELECTEDSLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_SelectedItem_Back = { "Back", 28 + (48 * 0) + 0, 152 - 12, TRUE, MenuItem_Inventory_SelectedItem_Back };

MENUITEM gMI_Inventory_SelectedItem_Trash = { "Trash", 28 + (48 * 1) + 2, 152 - 12, TRUE, MenuItem_Inventory_SelectedItem_Trash };

MENUITEM gMI_Inventory_SelectedItem_Use = { "Use", 28 + (48 * 2) + 4, 152 - 12, TRUE, MenuItem_Inventory_SelectedItem_Use };

MENUITEM gMI_Inventory_SelectedItem_Equip = { "Equip", 28 + (48 * 2) + 4, 152 - 12, TRUE, MenuItem_Inventory_SelectedItem_Equip };

MENUITEM gMI_Inventory_SelectedItem_Inspect = { "Stats", 28 + (48 * 1) + 2, 152 - 12, TRUE, MenuItem_Inventory_SelectedItem_Inspect };     //replaces the trash pos when used on monsters

MENUITEM gMI_Inventory_SelectedItem_Give = { "Give", 28 + (48 * 2) + 4, 152 - 12, TRUE, MenuItem_Inventory_SelectedItem_Give };        //replaces the use/equip pos when used on monsters

MENUITEM* gMI_InventorySelectedItem_Items[] = { &gMI_Inventory_SelectedItem_Back, &gMI_Inventory_SelectedItem_Trash, &gMI_Inventory_SelectedItem_Use, &gMI_Inventory_SelectedItem_Equip, &gMI_Inventory_SelectedItem_Inspect, &gMI_Inventory_SelectedItem_Give };

MENU gMenu_InventorySelectedItem = { "Selected Item Options", 0, _countof(gMI_InventorySelectedItem_Items), gMI_InventorySelectedItem_Items };

//// MONSTERSLOT MENU VARIABLES ////

uint8_t gSelectedMonster = 255;       //tells game what monster was selected from inventory
uint8_t gSwitchingMonster = 0;      //used for swaping party slots in inventory

//// MONSTERSLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Monster_Slot0 = { "Slot1", 204, 36 + (10 * 2), TRUE, MenuItem_Inventory_MonsterSelected_Action };

MENUITEM gMI_Inventory_Monster_Slot1 = { "Slot2", 260, 36 + (10 * 5), TRUE, MenuItem_Inventory_MonsterSelected_Action };

MENUITEM gMI_Inventory_Monster_Slot2 = { "Slot3", 204, 36 + (10 * 8), TRUE, MenuItem_Inventory_MonsterSelected_Action };

MENUITEM gMI_Inventory_Monster_Slot3 = { "Slot4", 260, 36 + (10 * 11), TRUE, MenuItem_Inventory_MonsterSelected_Action };

MENUITEM gMI_Inventory_Monster_Slot4 = { "Slot5", 204, 36 + (10 * 14), TRUE, MenuItem_Inventory_MonsterSelected_Action };

MENUITEM gMI_Inventory_Monster_Slot5 = { "Slot6", 260, 36 + (10 * 17), TRUE, MenuItem_Inventory_MonsterSelected_Action };

MENUITEM* gMI_InventoryMonster_Items[] = { &gMI_Inventory_Monster_Slot0, &gMI_Inventory_Monster_Slot1, &gMI_Inventory_Monster_Slot2, &gMI_Inventory_Monster_Slot3, &gMI_Inventory_Monster_Slot4, &gMI_Inventory_Monster_Slot5 };

MENU gMenu_InventoryMonster = { "Monster Items", 0, _countof(gMI_InventoryMonster_Items), gMI_InventoryMonster_Items };

//// ADVENTURESLOT MENU VARIABLES ////

uint16_t gAdventureSlotIndex[/*_countof(gMI_InventoryAdventure_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window (gAdventureSlotIndex[gMenu_InventoryAdventure.SelectedItem] is the item.index of the selected item
int32_t gAdventureSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gAdventureHasItemSort[NUM_ADVENTURE_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gAdventureItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gAdventureItemCount = 0;                                           //total number of non-zero index's in the adventure pocket (number of unique resore items owned by the player)

//// ADVENTURESLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Adventure_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Adventure_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM* gMI_InventoryAdventure_Items[] = { &gMI_Inventory_Adventure_Slot0, &gMI_Inventory_Adventure_Slot1, &gMI_Inventory_Adventure_Slot2, &gMI_Inventory_Adventure_Slot3, &gMI_Inventory_Adventure_Slot4, &gMI_Inventory_Adventure_Slot5, &gMI_Inventory_Adventure_Slot6, &gMI_Inventory_Adventure_Slot7, &gMI_Inventory_Adventure_Slot8, &gMI_Inventory_Adventure_Slot9, &gMI_Inventory_Adventure_Slot10, &gMI_Inventory_Adventure_Slot11, &gMI_Inventory_Adventure_Slot12, &gMI_Inventory_Adventure_Slot13, &gMI_Inventory_Adventure_Slot14, &gMI_Inventory_Adventure_Slot15, &gMI_Inventory_Adventure_Slot16 };

MENU gMenu_InventoryAdventure = { "Adventure Items", 0, _countof(gMI_InventoryAdventure_Items), gMI_InventoryAdventure_Items };





//// VALUABLESLOT MENU VARIABLES ////

uint16_t gValuableSlotIndex[/*_countof(gMI_InventoryValuable_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gValuableSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gValuableHasItemSort[NUM_VALUABLE_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gValuableItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gValuableItemCount = 0;                                           //total number of non-zero index's in the valuable pocket (number of unique resore items owned by the player)

//// VALUABLESLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Valuable_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Valuable_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM* gMI_InventoryValuable_Items[] = { &gMI_Inventory_Valuable_Slot0, &gMI_Inventory_Valuable_Slot1, &gMI_Inventory_Valuable_Slot2, &gMI_Inventory_Valuable_Slot3, &gMI_Inventory_Valuable_Slot4, &gMI_Inventory_Valuable_Slot5, &gMI_Inventory_Valuable_Slot6, &gMI_Inventory_Valuable_Slot7, &gMI_Inventory_Valuable_Slot8, &gMI_Inventory_Valuable_Slot9, &gMI_Inventory_Valuable_Slot10, &gMI_Inventory_Valuable_Slot11, &gMI_Inventory_Valuable_Slot12, &gMI_Inventory_Valuable_Slot13, &gMI_Inventory_Valuable_Slot14, &gMI_Inventory_Valuable_Slot15, &gMI_Inventory_Valuable_Slot16 };

MENU gMenu_InventoryValuable = { "Valuable Items", 0, _countof(gMI_InventoryValuable_Items), gMI_InventoryValuable_Items };




//// USABLESLOT MENU VARIABLES ////

uint16_t gUsableSlotIndex[/*_countof(gMI_InventoryUsable_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gUsableSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gUsableHasItemSort[NUM_USABLE_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gUsableItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gUsableItemCount = 0;                                           //total number of non-zero index's in the usable pocket (number of unique resore items owned by the player)

//// USABLESLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Usable_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Usable_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM* gMI_InventoryUsable_Items[] = { &gMI_Inventory_Usable_Slot0, &gMI_Inventory_Usable_Slot1, &gMI_Inventory_Usable_Slot2, &gMI_Inventory_Usable_Slot3, &gMI_Inventory_Usable_Slot4, &gMI_Inventory_Usable_Slot5, &gMI_Inventory_Usable_Slot6, &gMI_Inventory_Usable_Slot7, &gMI_Inventory_Usable_Slot8, &gMI_Inventory_Usable_Slot9, &gMI_Inventory_Usable_Slot10, &gMI_Inventory_Usable_Slot11, &gMI_Inventory_Usable_Slot12, &gMI_Inventory_Usable_Slot13, &gMI_Inventory_Usable_Slot14, &gMI_Inventory_Usable_Slot15, &gMI_Inventory_Usable_Slot16 };

MENU gMenu_InventoryUsable = { "Usable Items", 0, _countof(gMI_InventoryUsable_Items), gMI_InventoryUsable_Items };



//// EQUIPSLOT MENU VARIABLES ////

uint16_t gEquipSlotIndex[/*_countof(gMI_InventoryEquipable_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gEquipSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gEquipHasItemSort[NUM_EQUIP_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gEquipableItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gEquipItemCount = 0;                                           //total number of non-zero index's in the equipables pocket (number of unique equipable items owned by the player)

//// EQUIPSLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Equipable_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Equipable_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_ItemSelected_Action };

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
        for (uint16_t i = 0; i < NUM_USABLE_ITEMS; i++)
        {
            if (gUsableItems[i].Count > 0)
            {
                gUsableItems[i].HasItem = TRUE;
            }
            else
            {
                gUsableItems[i].HasItem = FALSE;
            }
            if (gUsableItems[i].HasItem == TRUE)
            {
                gUsableHasItemSort[count] = i;
                count++;
            }
            if (i == NUM_USABLE_ITEMS - 1)
            {
                gUsableItemCount = count;
                for (uint8_t j = 0; j < NUM_USABLE_ITEMS - count; j++)
                {
                    gUsableHasItemSort[count + j] = 0xFFFF;
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
            if (gHasSelectedInvSlot == TRUE)
            {
                DrawSelectedItemOptions();
            }
            break;
        }
        case POCKETSTATE_USABLE:
        {
            DrawUsablePocket();
            if (gHasSelectedInvSlot == TRUE)
            {
                DrawSelectedItemOptions();
            }
            break;
        }
        case POCKETSTATE_VALUABLE:
        {
            DrawValuablePocket();
            if (gHasSelectedInvSlot == TRUE)
            {
                DrawSelectedItemOptions();
            }
            break;
        }
        case POCKETSTATE_ADVENTURE:
        {
            DrawAdvemturePocket();
            if (gHasSelectedInvSlot == TRUE)
            {
                DrawSelectedItemOptions();
            }
            break;
        }
        case POCKETSTATE_MONSTER:
        {
            DrawMonsterParty();
            if (gHasSelectedInvSlot == TRUE)
            {
                DrawSelectedItemOptions();
            }
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
            if (gHasSelectedInvSlot == FALSE)
            {
                if (gMenu_InventoryEquipable.SelectedItem >= gEquipItemCount - 1 && gEquipItemCount > _countof(gMI_InventoryEquipable_Items))
                {
                    gMenu_InventoryEquipable.SelectedItem = 0;
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gPreviousGameState = GAMESTATE_INVENTORYSCREEN;
                    gCurrentGameState = GAMESTATE_OVERWORLD;
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
                    gSwitchingMonster = 255;
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    gCurrentPockets = POCKETSTATE_USABLE;
                    PlayGameSound(&gSoundMenuNavigate);
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryEquipable.SelectedItem <= 16 && gMenu_InventoryEquipable.SelectedItem >= 0) && gEquipItemCount != 0)
                {
                    if (gEquipSlotIndex[gMenu_InventoryEquipable.SelectedItem] != 0xFFFF)
                    {
                        gMI_InventoryEquipable_Items[gMenu_InventoryEquipable.SelectedItem]->Action();
                        PlayGameSound(&gSoundMenuChoose);
                    }
                }
            }
            else
            {
                if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 1)
                    {
                        gMenu_InventorySelectedItem.SelectedItem--;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventorySelectedItem.SelectedItem == 3)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 1;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 0)
                    {
                        gMenu_InventorySelectedItem.SelectedItem++;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventorySelectedItem.SelectedItem == 1)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 3;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
                {
                    gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gHasSelectedInvSlot = FALSE;
                    gMenu_InventorySelectedItem.SelectedItem = 0;
                }
            }
            break;
        }
        case POCKETSTATE_USABLE:
        {
            if (gHasSelectedInvSlot == FALSE)
            {
                if (gMenu_InventoryUsable.SelectedItem >= gUsableItemCount - 1 && gUsableItemCount > _countof(gMI_InventoryUsable_Items))
                {
                    gMenu_InventoryUsable.SelectedItem = 0;
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gPreviousGameState = GAMESTATE_INVENTORYSCREEN;
                    gCurrentGameState = GAMESTATE_OVERWORLD;
                }

                if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
                {
                    if (gMenu_InventoryUsable.SelectedItem == 0 && gUsableItemCount > _countof(gMI_InventoryUsable_Items))
                    {
                        gUsableSlotOffset--;      //change names on boxes when at bottom box and pushing "down" key
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventoryUsable.SelectedItem > 0)
                    {
                        gMenu_InventoryUsable.SelectedItem--;    //changes selected box
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }

                if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
                {
                    if (gMenu_InventoryUsable.SelectedItem == 16)
                    {
                        gUsableSlotOffset++;      //change names on boxes when at top box and pushing "up" key
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventoryUsable.SelectedItem < gUsableItemCount - 1)
                    {
                        gMenu_InventoryUsable.SelectedItem++;    //changes selected box
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

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryUsable.SelectedItem <= 16 && gMenu_InventoryUsable.SelectedItem >= 0) && gEquipItemCount != 0)
                {
                    if (gUsableSlotIndex[gMenu_InventoryUsable.SelectedItem] != 0xFFFF)
                    {
                        gMI_InventoryUsable_Items[gMenu_InventoryUsable.SelectedItem]->Action();
                        PlayGameSound(&gSoundMenuChoose);
                    }
                }
            }
            else
            {
                if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem > 0)
                    {
                        gMenu_InventorySelectedItem.SelectedItem--;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem < 3)
                    {
                        gMenu_InventorySelectedItem.SelectedItem++;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
                {
                    gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gHasSelectedInvSlot = FALSE;
                    gMenu_InventorySelectedItem.SelectedItem = 0;
                }
            }

            break;
        }
        case POCKETSTATE_VALUABLE:
        {
            if (gHasSelectedInvSlot == FALSE)
            {
                if (gMenu_InventoryValuable.SelectedItem >= gEquipItemCount - 1 && gEquipItemCount > _countof(gMI_InventoryValuable_Items))
                {
                    gMenu_InventoryValuable.SelectedItem = 0;
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gPreviousGameState = GAMESTATE_INVENTORYSCREEN;
                    gCurrentGameState = GAMESTATE_OVERWORLD;
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
                    gCurrentPockets = POCKETSTATE_USABLE;
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    gCurrentPockets = POCKETSTATE_ADVENTURE;
                    PlayGameSound(&gSoundMenuNavigate);
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryValuable.SelectedItem <= 16 && gMenu_InventoryValuable.SelectedItem >= 0) && gEquipItemCount != 0)
                {
                    if (gEquipSlotIndex[gMenu_InventoryValuable.SelectedItem] != 0xFFFF)
                    {
                        gMI_InventoryValuable_Items[gMenu_InventoryValuable.SelectedItem]->Action();
                        PlayGameSound(&gSoundMenuChoose);
                    }
                }
            }
            else
            {
                if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 1)
                    {
                        gMenu_InventorySelectedItem.SelectedItem--;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 0)
                    {
                        gMenu_InventorySelectedItem.SelectedItem++;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
                {
                    gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gHasSelectedInvSlot = FALSE;
                    gMenu_InventorySelectedItem.SelectedItem = 0;
                }
            }

            break;
        }
        case POCKETSTATE_ADVENTURE:
        {
            if (gHasSelectedInvSlot == FALSE)
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
                    gSwitchingMonster = 255;
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryAdventure.SelectedItem <= 16 && gMenu_InventoryAdventure.SelectedItem >= 0) && gEquipItemCount != 0)
                {
                    if (gEquipSlotIndex[gMenu_InventoryAdventure.SelectedItem] != 0xFFFF)
                    {
                        gMI_InventoryAdventure_Items[gMenu_InventoryAdventure.SelectedItem]->Action();
                        PlayGameSound(&gSoundMenuChoose);
                    }
                }
            }
            else
            {
                if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 2)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 0;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 0)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 2;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
                {
                    gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gHasSelectedInvSlot = FALSE;
                    gMenu_InventorySelectedItem.SelectedItem = 0;
                }
            }
            break;
        }
        case POCKETSTATE_MONSTER:
        {
            if (gHasSelectedInvSlot == FALSE)
            {
                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gPreviousGameState = GAMESTATE_INVENTORYSCREEN;
                    gCurrentGameState = GAMESTATE_OVERWORLD;
                }

                if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
                {
                    gCurrentPockets = POCKETSTATE_ADVENTURE;
                    PlayGameSound(&gSoundMenuNavigate);
                    gSwitchingMonster = 255;
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    gCurrentPockets = POCKETSTATE_EQUIPABLE;
                    PlayGameSound(&gSoundMenuNavigate);
                    gSwitchingMonster = 255;
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

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryAdventure.SelectedItem <= 5 && gMenu_InventoryAdventure.SelectedItem >= 0) && gEquipItemCount != 0)
                {
                    gMI_InventoryMonster_Items[gMenu_InventoryMonster.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                    gSwitchingMonster = 255;
                }

                if (gGameInput.QKeyPressed && !gGameInput.QKeyAlreadyPressed)
                {
                    if (gSwitchingMonster == 255)
                    {
                        gSwitchingMonster = gMenu_InventoryMonster.SelectedItem;
                    }
                    else if (gSwitchingMonster != gMenu_InventoryMonster.SelectedItem)
                    {
                        struct Monster MonsterSlotA = gPlayerParty[gSwitchingMonster];
                        struct Monster MonsterSlotB = gPlayerParty[gMenu_InventoryMonster.SelectedItem];
                        
                        gPlayerParty[gSwitchingMonster] = MonsterSlotB;
                        gPlayerParty[gMenu_InventoryMonster.SelectedItem] = MonsterSlotA;
                        gSwitchingMonster = 255;
                    }
                    else if (gSwitchingMonster == gMenu_InventoryMonster.SelectedItem)
                    {
                        gSwitchingMonster = 255;
                    }
                }
            }
            else
            {
                if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 5)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 4;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventorySelectedItem.SelectedItem == 4)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 0;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }
                else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
                {
                    if (gMenu_InventorySelectedItem.SelectedItem == 0)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 4;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventorySelectedItem.SelectedItem == 4)
                    {
                        gMenu_InventorySelectedItem.SelectedItem = 5;
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
                {
                    gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gHasSelectedInvSlot = FALSE;
                    gMenu_InventorySelectedItem.SelectedItem = 0;
                }
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

void DrawUsablePocket(void)
{

    //equip pockets
    DrawWindow(194, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
    //restore pockets
    DrawWindow(194 + (36 * 1), 26, 35, 18, &COLOR_BLACK, &COLOR_NES_PINK, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);
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


    for (uint8_t UsableBox = 0; UsableBox < _countof(gMI_InventoryUsable_Items); UsableBox++)
    {
        uint16_t SlotLoop = 0x800C;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:



        SlotOrder = UsableBox + gUsableSlotOffset + SlotLoop;

        if (gUsableItemCount != 0)
        {
            SlotOrder %= gUsableItemCount;
        }

        SlotIndex = gUsableHasItemSort[SlotOrder];

        if (gUsableHasItemSort[UsableBox] == 0xFFFF)
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

        gUsableSlotIndex[UsableBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_InventoryUsable_Items[UsableBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            uint16_t ItemCountSize = snprintf(NULL, 0, "x%d", gUsableItems[SlotIndex].Count);
            char* ItemCountString = malloc(ItemCountSize + 1);
            snprintf(ItemCountString, ItemCountSize + 1, "x%d", gUsableItems[SlotIndex].Count);

            BlitStringToBuffer(gUsableItems[SlotIndex].Name, &g6x7Font, &COLOR_BLACK, gMI_InventoryUsable_Items[UsableBox]->x, gMI_InventoryUsable_Items[UsableBox]->y);

            BlitStringToBuffer(ItemCountString, &g6x7Font, &COLOR_BLACK, gMI_InventoryUsable_Items[UsableBox]->x + 149, gMI_InventoryUsable_Items[UsableBox]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryUsable_Items[gMenu_InventoryUsable.SelectedItem]->x - 6, gMI_InventoryUsable_Items[gMenu_InventoryUsable.SelectedItem]->y);
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
    DrawWindow(194 + (36 * 4) + 1, 26, 35, 18, &COLOR_BLACK, &COLOR_NES_PURPLE, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);

    if (gSwitchingMonster != 255)
    {
        DrawWindow(gMI_InventoryMonster_Items[gSwitchingMonster]->x - 5, gMI_InventoryMonster_Items[gSwitchingMonster]->y - 12, 100 + 20, 19, &COLOR_NES_PURPLE, NULL, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_THICK);
    }

    BlitStringToBuffer("Q: SWITCH", &g6x7Font, &COLOR_BLACK, gMI_InventoryMonster_Items[1]->x + 60, gMI_InventoryMonster_Items[0]->y - 6);

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryMonster_Items[gMenu_InventoryMonster.SelectedItem]->x - 9, gMI_InventoryMonster_Items[gMenu_InventoryMonster.SelectedItem]->y - 5);

    for (uint8_t playerParty = 0; playerParty < gPlayerPartyCount; playerParty++)
    {
        uint16_t HpPercent = 100 - ((gPlayerParty[playerParty].Health * 100) / (gPlayerParty[playerParty].MaxHealth));

        uint16_t ExpPercent = 100 - (((gPlayerParty[playerParty].DriveMonster.Experience - gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level]) * 100) / (gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level + 1] - gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level]));

        DrawMonsterHpBar(gMI_InventoryMonster_Items[playerParty]->x, gMI_InventoryMonster_Items[playerParty]->y, HpPercent, ExpPercent, gPlayerParty[playerParty].Level, gPlayerParty[playerParty].DriveMonster.nickname, TRUE);
    }

}

void DrawSelectedItemOptions(void)
{
    DrawWindow(20 + (48 * 0) + 0, 152 - 18, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    if (gCurrentPockets != POCKETSTATE_ADVENTURE)
    {
        DrawWindow(20 + (48 * 1) + 2, 152 - 18, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    }

    if (gCurrentPockets != POCKETSTATE_VALUABLE)
    {
        DrawWindow(20 + (48 * 2) + 4, 152 - 18, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    }

    switch (gCurrentPockets)
    {
        case POCKETSTATE_EQUIPABLE:
        {
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Back.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Back.x, gMI_Inventory_SelectedItem_Back.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Trash.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Trash.x, gMI_Inventory_SelectedItem_Trash.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Equip.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Equip.x, gMI_Inventory_SelectedItem_Equip.y);

            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->x - 6, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->y);

            break;
        }
        case POCKETSTATE_USABLE:
        {
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Back.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Back.x, gMI_Inventory_SelectedItem_Back.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Trash.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Trash.x, gMI_Inventory_SelectedItem_Trash.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Use.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Use.x, gMI_Inventory_SelectedItem_Use.y);

            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->x - 6, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->y);

            break;
        }
        case POCKETSTATE_VALUABLE:
        {
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Back.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Back.x, gMI_Inventory_SelectedItem_Back.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Trash.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Trash.x, gMI_Inventory_SelectedItem_Trash.y);

            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->x - 6, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->y);

            break;
        }
        case POCKETSTATE_ADVENTURE:
        {
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Back.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Back.x, gMI_Inventory_SelectedItem_Back.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Use.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Use.x, gMI_Inventory_SelectedItem_Use.y);

            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->x - 6, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->y);

            break;
        }
        case POCKETSTATE_MONSTER:
        {
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Back.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Back.x, gMI_Inventory_SelectedItem_Back.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Inspect.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Trash.x, gMI_Inventory_SelectedItem_Trash.y);
            BlitStringToBuffer(gMI_Inventory_SelectedItem_Give.Name, &g6x7Font, &COLOR_BLACK, gMI_Inventory_SelectedItem_Give.x, gMI_Inventory_SelectedItem_Give.y);

            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->x - 6, gMI_InventorySelectedItem_Items[gMenu_InventorySelectedItem.SelectedItem]->y);

            break;
        }
    }

}

void DrawSelectedMonsterOptions(void)
{

}

void MenuItem_Inventory_ItemSelected_Action(void)
{
    gHasSelectedInvSlot = TRUE;

    //TODO: make items usable with effects
    //gEquipableItems[gEquipSlotIndex[gMenu_InventoryEquipable.SelectedItem]]       is the currently selected item after pressing the "choose" key
    
}

void MenuItem_Inventory_MonsterSelected_Action(void)
{
    gSelectedMonster = gMenu_InventoryMonster.SelectedItem;

    gHasSelectedInvSlot = TRUE;
}



void MenuItem_Inventory_SelectedItem_Back(void)
{
    gHasSelectedInvSlot = FALSE;
    gMenu_InventorySelectedItem.SelectedItem = 0;
}

void MenuItem_Inventory_SelectedItem_Trash(void)        //TODO: all that is left with inventory is item effect functions and applying them to monsters
{

}

void MenuItem_Inventory_SelectedItem_Use(void)
{
    if (gUsableSlotIndex[gMenu_InventoryUsable.SelectedItem] == INV_USABLE_ITEM_0 && gCurrentPockets == POCKETSTATE_USABLE)
    {
        //TODO: draw monster party and select monster to use item on
        //increase selected monster health by 20 and remove one from item.count
    }
}

void MenuItem_Inventory_SelectedItem_Equip(void)
{

}

void MenuItem_Inventory_SelectedItem_Inspect(void)
{
    gMonsterToViewStats = gMenu_InventoryMonster.SelectedItem;
    gPreviousGameState = GAMESTATE_INVENTORYSCREEN;
    gCurrentGameState = GAMESTATE_MONSTERSTATS;
}

void MenuItem_Inventory_SelectedItem_Give(void)
{

}