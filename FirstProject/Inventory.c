
#include "Main.h"

#include "Inventory.h"
#include "InventoryItems.h"
#include "MonsterStatsScreen.h"
#include "MonsterData.h"


//// SELECTEDSLOT MENU VARIABLES ////

BOOL gHasSelectedMonster = FALSE;
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

uint16_t gUseableSlotIndex[/*_countof(gMI_InventoryUseable_Items)*/17] = { 0 };   //for PPI_InventoryScreen to know what item is in the selected menu window
int32_t gUseableSlotOffset = 0;                                           //shuffle items in menu windows when cursor is at top or bottom of menu boxes
uint16_t gUseableHasItemSort[NUM_USABLE_ITEMS] = { 0 };                    // simple sorting algorithm that sorts the gUseableItems.Index's that have a non zero count first but keeps the order of Index, if an Index is zero it returns 0xFFFF for that value (example; 0, 2, 3, 5, 8, 13, 21, 23, 45 (last non-zero index), 65535, 65535, 65535, 65535, ...)
uint16_t gUseableItemCount = 0;                                           //total number of non-zero index's in the usable pocket (number of unique resore items owned by the player)
uint8_t gUseableItemEffect = ITEM_USE_EFFECT_NULL;                       //for knowing what effect to perform after selecting a monster

//// USABLESLOT MENU VARIABLES ////

MENUITEM gMI_Inventory_Useable_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM gMI_Inventory_Useable_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_ItemSelected_Action };

MENUITEM* gMI_InventoryUseable_Items[] = { &gMI_Inventory_Useable_Slot0, &gMI_Inventory_Useable_Slot1, &gMI_Inventory_Useable_Slot2, &gMI_Inventory_Useable_Slot3, &gMI_Inventory_Useable_Slot4, &gMI_Inventory_Useable_Slot5, &gMI_Inventory_Useable_Slot6, &gMI_Inventory_Useable_Slot7, &gMI_Inventory_Useable_Slot8, &gMI_Inventory_Useable_Slot9, &gMI_Inventory_Useable_Slot10, &gMI_Inventory_Useable_Slot11, &gMI_Inventory_Useable_Slot12, &gMI_Inventory_Useable_Slot13, &gMI_Inventory_Useable_Slot14, &gMI_Inventory_Useable_Slot15, &gMI_Inventory_Useable_Slot16 };

MENU gMenu_InventoryUseable = { "Useable Items", 0, _countof(gMI_InventoryUseable_Items), gMI_InventoryUseable_Items };



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
        ReSortAdventureitems();
        ReSortValuableitems();
        ReSortUsableitems();
        ReSortEquipableitems();
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    if ((gCurrentPockets == POCKETSTATE_MONSTER) || (gCurrentPockets == POCKETSTATE_MONSTER_SELECT))
    {
        DrawWindow(192, 1, 184, 192, &COLOR_NES_WHITE, &COLOR_FORREST_GREEN, &COLOR_DARK_GRAY, WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    }
    else
    {
        DrawWindow(192, 1, 184, 192, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    }
    
    //sprite box
    DrawWindow(24, 24, 32, 32, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    if ((gCurrentPockets != POCKETSTATE_MONSTER) && (gCurrentPockets != POCKETSTATE_MONSTER_SELECT))
    {
        //description box
        DrawWindow(8, 152, 176, 64, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    }

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
            DrawUseablePocket();
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
        case POCKETSTATE_MONSTER_SELECT:
        {
            DrawMonsterParty();
            if (gHasSelectedMonster == TRUE)
            {
                
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
            else            ////gHasSelectedInvSlot == TRUE
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
        case POCKETSTATE_USABLE:        //TODO: BUG when selecting(but not using) the last itme in USABLE and then using an item in battle, the cursor will be beyond the bottom-most item if the menu shrinks
        {
            if (gHasSelectedInvSlot == FALSE)
            {
                if (gMenu_InventoryUseable.SelectedItem >= gUseableItemCount - 1 && gUseableItemCount > _countof(gMI_InventoryUseable_Items))
                {
                    gMenu_InventoryUseable.SelectedItem = 0;
                }

                if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
                {
                    gPreviousGameState = GAMESTATE_INVENTORYSCREEN;
                    gCurrentGameState = GAMESTATE_OVERWORLD;
                }

                if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
                {
                    if (gMenu_InventoryUseable.SelectedItem == 0 && gUseableItemCount > _countof(gMI_InventoryUseable_Items))
                    {
                        gUseableSlotOffset--;      //change names on boxes when at bottom box and pushing "down" key
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventoryUseable.SelectedItem > 0)
                    {
                        gMenu_InventoryUseable.SelectedItem--;    //changes selected box
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                }

                if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
                {
                    if (gMenu_InventoryUseable.SelectedItem == 16 /*&& gUseableItemCount > _countof(gMI_InventoryUseable_Items)*/)              //stops navigating when at bottom of menu?? good idea???
                    {
                        gUseableSlotOffset++;      //change names on boxes when at top box and pushing "up" key
                        PlayGameSound(&gSoundMenuNavigate);
                    }
                    else if (gMenu_InventoryUseable.SelectedItem < gUseableItemCount - 1)
                    {
                        gMenu_InventoryUseable.SelectedItem++;    //changes selected box
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

                if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (gMenu_InventoryUseable.SelectedItem <= 16 && gMenu_InventoryUseable.SelectedItem >= 0) && gEquipItemCount != 0)
                {
                    if (gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem] != 0xFFFF)
                    {
                        gMI_InventoryUseable_Items[gMenu_InventoryUseable.SelectedItem]->Action();
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
                    gMenu_InventorySelectedItem.SelectedItem = 0;
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
                        PlayGameSound(&gSoundMenuChoose);
                    }
                    else if (gSwitchingMonster != gMenu_InventoryMonster.SelectedItem)
                    {
                        struct Monster MonsterSlotA = gPlayerParty[gSwitchingMonster];
                        struct Monster MonsterSlotB = gPlayerParty[gMenu_InventoryMonster.SelectedItem];
                        
                        gPlayerParty[gSwitchingMonster] = MonsterSlotB;
                        gPlayerParty[gMenu_InventoryMonster.SelectedItem] = MonsterSlotA;
                        gSwitchingMonster = 255;
                        PlayGameSound(&gSoundMenuChoose);
                    }
                    else if (gSwitchingMonster == gMenu_InventoryMonster.SelectedItem)
                    {
                        gSwitchingMonster = 255;
                        PlayGameSound(&gSoundMenuNavigate);
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
        case POCKETSTATE_MONSTER_SELECT:
        {
            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                gCurrentPockets = gPreviousPockets;
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

void DrawUseablePocket(void)
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


    for (uint8_t UseableBox = 0; UseableBox < _countof(gMI_InventoryUseable_Items); UseableBox++)
    {
        uint16_t SlotLoop = 0x800C;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:



        SlotOrder = UseableBox + gUseableSlotOffset + SlotLoop;

        if (gUseableItemCount != 0)
        {
            SlotOrder %= gUseableItemCount;
        }

        SlotIndex = gUseableHasItemSort[SlotOrder];

        if (gUseableHasItemSort[UseableBox] == 0xFFFF)
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

        gUseableSlotIndex[UseableBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_InventoryUseable_Items[UseableBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            uint16_t ItemCountSize = snprintf(NULL, 0, "x%d", gUseableItems[SlotIndex].Count);
            char* ItemCountString = malloc(ItemCountSize + 1);
            snprintf(ItemCountString, ItemCountSize + 1, "x%d", gUseableItems[SlotIndex].Count);

            BlitStringToBuffer(gUseableItems[SlotIndex].Name, &g6x7Font, &COLOR_BLACK, gMI_InventoryUseable_Items[UseableBox]->x, gMI_InventoryUseable_Items[UseableBox]->y);

            BlitStringToBuffer(ItemCountString, &g6x7Font, &COLOR_BLACK, gMI_InventoryUseable_Items[UseableBox]->x + 149, gMI_InventoryUseable_Items[UseableBox]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_InventoryUseable_Items[gMenu_InventoryUseable.SelectedItem]->x - 6, gMI_InventoryUseable_Items[gMenu_InventoryUseable.SelectedItem]->y);

    BlitItemDescription(gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Description);
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

void DrawEquipUseMonsterText(void)
{

}

void DrawSelectedMonsterOptions(void)
{

}

void MenuItem_Inventory_ItemSelected_Action(void)
{
    gHasSelectedInvSlot = TRUE;
    
}

void MenuItem_Inventory_MonsterSelected_Action(void)
{
    if (gCurrentPockets == POCKETSTATE_MONSTER)
    {
        gSelectedMonster = gMenu_InventoryMonster.SelectedItem;

        gHasSelectedInvSlot = TRUE;
    }
    else if (gCurrentPockets == POCKETSTATE_MONSTER_SELECT)
    {
        if (gPreviousPockets == POCKETSTATE_USABLE)
        {
            gSelectedMonster = gMenu_InventoryMonster.SelectedItem;

            gHasSelectedMonster = TRUE;

            switch (gUseableItemEffect)
            {
                case ITEM_USE_EFFECT_HEAL_MONSTER: 
                {
                    if (gPlayerParty[gSelectedMonster].Health == gPlayerParty[gSelectedMonster].MaxHealth | gPlayerParty[gSelectedMonster].Health == 0)      ////selected monster has full hp or is knocked out
                    {
                        gCurrentPockets = POCKETSTATE_USABLE;
                        gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                    }
                    else
                    {
                        uint16_t healthBeforeHeal = gPlayerParty[gSelectedMonster].Health;
                        uint16_t healAmount = 0;
                        switch (gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem])
                        {
                            case INV_USABLE_ITEM_0: 
                            {
                                healAmount = 20;
                                break;
                            }
                            case INV_USABLE_ITEM_1:
                            {
                                healAmount = 50;
                                break;
                            }
                            case INV_USABLE_ITEM_2:
                            {
                                healAmount = 100;
                                break;
                            }
                            case INV_USABLE_ITEM_3:
                            {
                                healAmount = 250;
                                break;
                            }
                            case INV_USABLE_ITEM_4:
                            {
                                healAmount = gPlayerParty[gSelectedMonster].MaxHealth;
                                break;
                            }
                            default:
                            {
                                ASSERT(FALSE, "unknown Usable Item with effect ITEM_USE_EFFECT_HEAL_MONSTER");
                            }
                        }

                        for (uint16_t healedHealth = gPlayerParty[gSelectedMonster].Health; healedHealth <= healthBeforeHeal + healAmount; healedHealth++)
                        {
                            gPlayerParty[gSelectedMonster].Health = healedHealth;
                            if (gPlayerParty[gSelectedMonster].Health == gPlayerParty[gSelectedMonster].MaxHealth)
                            {
                                break;
                            }
                        }

                        gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count--;

                        gCurrentPockets = POCKETSTATE_USABLE;
                        gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                        gHasSelectedInvSlot = FALSE;

                        ReSortUsableitems();
                    }

                    break;
                }
                case ITEM_USE_EFFECT_EXP_MONSTER:
                {
                    if (gPlayerParty[gSelectedMonster].Level <= 99)
                    {
                        uint32_t ExpBeforeBoost = gPlayerParty[gSelectedMonster].DriveMonster.Experience;
                        uint32_t ExpAmount = 0;
                        switch (gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem])
                        {
                            case INV_USABLE_ITEM_8:
                            {
                                ExpAmount = 100;
                                break;
                            }
                            case INV_USABLE_ITEM_9:
                            {
                                ExpAmount = 250;
                                break;
                            }
                            case INV_USABLE_ITEM_10:
                            {
                                ExpAmount = 500;
                                break;
                            }
                            case INV_USABLE_ITEM_11:
                            {
                                ExpAmount = 1000;
                                break;
                            }
                            case INV_USABLE_ITEM_12:
                            {
                                ExpAmount = 3000;
                                break;
                            }
                            case INV_USABLE_ITEM_13:
                            {
                                ExpAmount = 10000;
                                break;
                            }
                            case INV_USABLE_ITEM_14:
                            {
                                ExpAmount = gExperienceTables[gBaseStats[gPlayerParty[gSelectedMonster].DriveMonster.Index].growthRate][gPlayerParty[gSelectedMonster].Level + 1] - gPlayerParty[gSelectedMonster].DriveMonster.Experience;
                                break;
                            }
                            default:
                            {
                                ASSERT(FALSE, "unknown Usable Item with effect ITEM_USE_EFFECT_EXP_MONSTER");
                            }
                        }

                        for (uint32_t newExpVal = ExpBeforeBoost; newExpVal <= ExpBeforeBoost + ExpAmount; newExpVal++)
                        {
                            gPlayerParty[gSelectedMonster].DriveMonster.Experience = newExpVal;
                            if (gExperienceTables[gBaseStats[gPlayerParty[gSelectedMonster].DriveMonster.Index].growthRate][100] == newExpVal)  //hit level 100 before full amount was given
                            {
                                break;
                            }
                        }

                        BOOL DidMonsterLevelUp = FALSE;

                    TryLevelUp:

                        DidMonsterLevelUp = TryIncrementMonsterLevel(&gPlayerParty[gSelectedMonster]);

                        if (DidMonsterLevelUp == TRUE)
                        {
                            CalculateMonsterStats(&gPlayerParty[gSelectedMonster]);

                            MonsterTryLearningNewMove(&gPlayerParty[gSelectedMonster], TRUE);

                            goto TryLevelUp;
                        }

                        gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count--;

                        gCurrentPockets = POCKETSTATE_USABLE;
                        gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                        gHasSelectedInvSlot = FALSE;

                        ReSortUsableitems();
                    }
                    
                    break;
                }
                case ITEM_USE_EFFECT_CAPTURE:
                {
                    gCurrentPockets = POCKETSTATE_USABLE;
                    gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                    break;
                }
                case ITEM_USE_EFFECT_UPGRADE:
                {
                    if (gPlayerParty[gSelectedMonster].DriveMonster.Index == MONSTER_WOLF)
                    {
                        switch (gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem])
                        {
                            case INV_USABLE_ITEM_15:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_EARTHWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_16:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_AIRWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_17:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_FIREWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_18:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_WATERWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_19:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_ELECTRICWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_20:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_METALWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_21:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_SOULWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_22:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_LIFEWOLF;
                                break;
                            }
                            case INV_USABLE_ITEM_23:
                            {
                                gPlayerParty[gSelectedMonster].DriveMonster.Index = MONSTER_DEATHWOLF;
                                break;
                            }
                            default:
                            {
                                ASSERT(FALSE, "unknown Usable Item with effect ITEM_USE_EFFECT_UPGRADE");
                            }
                        }

                        gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count--;

                        gCurrentPockets = POCKETSTATE_USABLE;
                        gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                        gHasSelectedInvSlot = FALSE;

                        ReSortUsableitems();

                        CalculateMonsterStats(&gPlayerParty[gSelectedMonster]);
                    }
                    else
                    {
                        gCurrentPockets = POCKETSTATE_USABLE;
                        gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                    }
                    break;
                }
                case ITEM_USE_EFFECT_REVIVE_MONSTER:
                {
                    if (gPlayerParty[gSelectedMonster].Health == 0)
                    {
                        switch (gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem])
                        {
                            case INV_USABLE_ITEM_24:
                            {
                                gPlayerParty[gSelectedMonster].Health = gPlayerParty[gSelectedMonster].MaxHealth / 8;
                                break;
                            }
                            case INV_USABLE_ITEM_25:
                            {
                                gPlayerParty[gSelectedMonster].Health = gPlayerParty[gSelectedMonster].MaxHealth / 2;
                                break;
                            }
                            case INV_USABLE_ITEM_26:
                            {
                                gPlayerParty[gSelectedMonster].Health = gPlayerParty[gSelectedMonster].MaxHealth;
                                break;
                            }
                            default:
                            {
                                ASSERT(FALSE, "unknown Usable Item with effect ITEM_USE_EFFECT_REVIVE_MONSTER");
                            }
                        }

                        gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count--;

                        gCurrentPockets = POCKETSTATE_USABLE;
                        gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                        gHasSelectedInvSlot = FALSE;

                        ReSortUsableitems();
                    }
                    else
                    {
                        gCurrentPockets = POCKETSTATE_USABLE;
                        gPreviousPockets = POCKETSTATE_MONSTER_SELECT;
                    }
                    break;
                }
                case ITEM_USE_EFFECT_NULL:
                default:
                {
                    ASSERT(FALSE, "unknown item effect when using an item on a selected monster!");
                }
            }
        }
        else if (gPreviousPockets == POCKETSTATE_EQUIPABLE)
        {

        }
    }
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
    switch (gCurrentPockets)
    {
        case POCKETSTATE_EQUIPABLE:
        {
            break;
        }
        case POCKETSTATE_USABLE:
        {
            if (gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Effect == ITEM_USE_EFFECT_HEAL_MONSTER && gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count > 0)
            {
                gUseableItemEffect = ITEM_USE_EFFECT_HEAL_MONSTER;
                gPreviousPockets = POCKETSTATE_USABLE;
                gCurrentPockets = POCKETSTATE_MONSTER_SELECT;
                gSwitchingMonster = 255;
            }
            else if (gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Effect == ITEM_USE_EFFECT_EXP_MONSTER && gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count > 0)
            {
                gUseableItemEffect = ITEM_USE_EFFECT_EXP_MONSTER;
                gPreviousPockets = POCKETSTATE_USABLE;
                gCurrentPockets = POCKETSTATE_MONSTER_SELECT;
                gSwitchingMonster = 255;
            }
            else if (gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Effect == ITEM_USE_EFFECT_UPGRADE && gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count > 0)
            {
                gUseableItemEffect = ITEM_USE_EFFECT_UPGRADE;
                gPreviousPockets = POCKETSTATE_USABLE;
                gCurrentPockets = POCKETSTATE_MONSTER_SELECT;
                gSwitchingMonster = 255;
            }
            else if (gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Effect == ITEM_USE_EFFECT_REVIVE_MONSTER && gUseableItems[gUseableSlotIndex[gMenu_InventoryUseable.SelectedItem]].Count > 0)
            {
                gUseableItemEffect = ITEM_USE_EFFECT_REVIVE_MONSTER;
                gPreviousPockets = POCKETSTATE_USABLE;
                gCurrentPockets = POCKETSTATE_MONSTER_SELECT;
                gSwitchingMonster = 255;
            }
            break;
        }
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