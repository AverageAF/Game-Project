
#include "Main.h"
#include "StoreScreen.h"
#include "InventoryItems.h"


//usable items
uint16_t gStoreType1[MAX_STORE_ITEMS] = {
INV_USABLE_ITEM_0, INV_USABLE_ITEM_1, INV_USABLE_ITEM_5,
};
//usable items
uint16_t gStoreType2[MAX_STORE_ITEMS] = {
INV_USABLE_ITEM_0, INV_USABLE_ITEM_1, INV_USABLE_ITEM_2, INV_USABLE_ITEM_5, INV_USABLE_ITEM_6
};
//usable items
uint16_t gStoreType3[MAX_STORE_ITEMS] = {
INV_USABLE_ITEM_0, INV_USABLE_ITEM_1, INV_USABLE_ITEM_2, INV_USABLE_ITEM_3, INV_USABLE_ITEM_5, INV_USABLE_ITEM_6
};
//usable items
uint16_t gStoreType4[MAX_STORE_ITEMS] = {
INV_USABLE_ITEM_0, INV_USABLE_ITEM_1, INV_USABLE_ITEM_2, INV_USABLE_ITEM_3, INV_USABLE_ITEM_5, INV_USABLE_ITEM_6, INV_USABLE_ITEM_7
};
//usable items
uint16_t gStoreType5[MAX_STORE_ITEMS] = {
INV_USABLE_ITEM_0, INV_USABLE_ITEM_1, INV_USABLE_ITEM_2, INV_USABLE_ITEM_3, INV_USABLE_ITEM_4, INV_USABLE_ITEM_5, INV_USABLE_ITEM_6, INV_USABLE_ITEM_7
};
//equipable items
uint16_t gStoreType6[MAX_STORE_ITEMS] = {
INV_EQUIP_ITEM_1, INV_EQUIP_ITEM_2, INV_EQUIP_ITEM_3, INV_EQUIP_ITEM_4, INV_EQUIP_ITEM_5, INV_EQUIP_ITEM_6, INV_EQUIP_ITEM_7, INV_EQUIP_ITEM_8, INV_EQUIP_ITEM_9, INV_EQUIP_ITEM_10
};
//equipable items
uint16_t gStoreType7[MAX_STORE_ITEMS] = {
INV_EQUIP_ITEM_11, INV_EQUIP_ITEM_12, INV_EQUIP_ITEM_13, INV_EQUIP_ITEM_14, INV_EQUIP_ITEM_15, INV_EQUIP_ITEM_16, INV_EQUIP_ITEM_17, INV_EQUIP_ITEM_18, INV_EQUIP_ITEM_19, INV_EQUIP_ITEM_20
};
//equipable items
uint16_t gStoreType8[MAX_STORE_ITEMS] = {
1, 2, 3, 4, 5
};
//usable items
uint16_t gStoreType9[MAX_STORE_ITEMS] = {
1, 2, 3, 4, 5
};

MENUITEM gMI_Store_Buy = { "Buy", 80, 138, TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Back = { "Back", 30, 138, TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM* gMI_SelectedStore_Items[] = { &gMI_Store_Back, &gMI_Store_Buy };

MENU gMenu_SelectedStore = { "Selected Store Items", 0, _countof(gMI_SelectedStore_Items), gMI_SelectedStore_Items };

////

MENUITEM gMI_Store_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM gMI_Store_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Store_ItemSelected_Action };

MENUITEM* gMI_Store_Items[] = { &gMI_Store_Slot0, &gMI_Store_Slot1, &gMI_Store_Slot2, &gMI_Store_Slot3, &gMI_Store_Slot4, &gMI_Store_Slot5, &gMI_Store_Slot6, &gMI_Store_Slot7, &gMI_Store_Slot8, &gMI_Store_Slot9, &gMI_Store_Slot10, &gMI_Store_Slot11, &gMI_Store_Slot12, &gMI_Store_Slot13, &gMI_Store_Slot14, &gMI_Store_Slot15, &gMI_Store_Slot16 };

MENU gMenu_Store = { "Store Items", 0, _countof(gMI_Store_Items), gMI_Store_Items };

void DrawStoreScreen(void)
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
        gHasSelectedStoreItem = FALSE;
        ReSortAdventureitems();
        ReSortValuableitems();
        ReSortUsableitems();
        ReSortEquipableitems();
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawStoreMenuSlots();

    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void DrawStoreMenuSlots(void) 
{

    /// Background window
    DrawWindow(192, 1, 184, 192, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    ////Merchant name
    DrawWindow(194, 26, 70, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);

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


    //// Currency box
    DrawWindow(310, 10, 56, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    uint16_t CurrencySize = snprintf(NULL, 0, "$%d", gPlayer.Currency);
    char* CurrencyString = malloc(CurrencySize + 1);
    snprintf(CurrencyString, CurrencySize + 1, "$%d", gPlayer.Currency);

    BlitStringToBuffer(CurrencyString, &g6x7Font, &COLOR_BLACK, 312, 12);


    //sprite box
    DrawWindow(24, 24, 32, 32, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    //description box
    DrawWindow(8, 152, 176, 64, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    BlitStringToBuffer(gCharacterSprite[gStoreSpriteIndex].Name, &g6x7Font, &COLOR_BLACK, 198, 32);

    ////////////////
    
        switch (gStoreType[gStoreSpriteIndex])
        {
            case 0:
            {
                LogMessageA(LL_ERROR, "[%s] ERROR, Store NPC gCharacterSprite[%d] had a gStoreType of 0, value of 1 used instead.", __FUNCTION__, gStoreSpriteIndex);
            }
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            {
                //usable items
                gSelectedStoreItemIsUsable = TRUE;
                BlitUsableStoreItems(gStoreType[gStoreSpriteIndex]);
                break;
            }
            case 6:
            case 7:
            {
                //equipable items
                gSelectedStoreItemIsUsable = FALSE;
                BlitEquipableStoreItems(gStoreType[gStoreSpriteIndex]);
                break;
            }
            default:
            {
                ASSERT(FALSE, "[%s] Error. Unknown gStoreType[gStoreSpriteIndex] in DrawStoreMenuSlots.", __FUNCTION__);
            }
        }

        if (gHasSelectedStoreItem)
        {
            DrawSelectedStoreOptions();
        }

}

void PPI_StoreScreen(void)
{
    if (gHasSelectedStoreItem)
    {
        if (gMenu_SelectedStore.SelectedItem == 0 && gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
        {
            gMenu_SelectedStore.SelectedItem = 1;
            PlayGameSound(&gSoundMenuNavigate);
        }

        if (gMenu_SelectedStore.SelectedItem == 1 && gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
        {
            gMenu_SelectedStore.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }

        if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed && gMenu_SelectedStore.SelectedItem == 1)
        {
            gMenu_SelectedStore.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed && gMenu_SelectedStore.SelectedItem == 0)
        {
            gHasSelectedStoreItem = FALSE;
            PlayGameSound(&gSoundMenuNavigate);
        }

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            PlayGameSound(&gSoundMenuChoose);
            gMI_Store_Items[gMenu_Store.SelectedItem]->Action();
        }
    }
    else
    {
        if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
        {
            gCurrentGameState = gPreviousGameState;
            gPreviousGameState = GAMESTATE_INVENTORYSCREEN;     ////probably dont want to accidently hit Tab and go into a store menu at some later point while playing
        }

        if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed && gMenu_Store.SelectedItem > 0)
        {
            PlayGameSound(&gSoundMenuNavigate);
            gMenu_Store.SelectedItem--;
        }

        if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed && gMenu_Store.SelectedItem < gLastStoreItem)
        {
            PlayGameSound(&gSoundMenuNavigate);
            gMenu_Store.SelectedItem++;
        }

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && gMenu_Store.SelectedItem <= gLastStoreItem)
        {
            PlayGameSound(&gSoundMenuChoose);
            gMI_Store_Items[gMenu_Store.SelectedItem]->Action();
        }
    }
}

void MenuItem_Store_ItemSelected_Action(void)
{
    //set bool for StoreItemSelected or something, bring up menu for buy/cancel
    if (gHasSelectedStoreItem == TRUE)
    {
        if (gMenu_SelectedStore.SelectedItem == 0)  //back
        {
            gHasSelectedStoreItem = FALSE;
        }
        else    // SelectedItem == 1    //buy
        {
            if (gSelectedStoreItemIsUsable)
            {
                if (TRUE == CanPlayerAffordCurrencyCost(gUseableItems[gSelectedStoreItem].ValueCurrency * 2))
                {
                    gUseableItems[gSelectedStoreItem].Count++;
                    gPlayer.Currency -= gUseableItems[gSelectedStoreItem].ValueCurrency * 2;
                    ReSortUsableitems();
                }
                else
                {
                    gMenu_SelectedStore.SelectedItem = 0;
                }
            }
            else    ////selected item is equipable
            {
                if (TRUE == CanPlayerAffordCurrencyCost(gEquipableItems[gSelectedStoreItem].ValueCurrency * 2))
                {
                    gEquipableItems[gSelectedStoreItem].Count++;
                    gPlayer.Currency -= gEquipableItems[gSelectedStoreItem].ValueCurrency * 2;
                    ReSortEquipableitems();
                }
                else
                {
                    gMenu_SelectedStore.SelectedItem = 0;
                }
            }
        }
    }
    else
    {
        gHasSelectedStoreItem = TRUE;
        switch (gStoreType[gStoreSpriteIndex])
        {
            case 0:
            case 1:
                gSelectedStoreItem = gStoreType1[gMenu_Store.SelectedItem];
                break;
            case 2:
                gSelectedStoreItem = gStoreType2[gMenu_Store.SelectedItem];
                break;
            case 3:
                gSelectedStoreItem = gStoreType3[gMenu_Store.SelectedItem];
                break;
            case 4:
                gSelectedStoreItem = gStoreType4[gMenu_Store.SelectedItem];
                break;
            case 5:
                gSelectedStoreItem = gStoreType5[gMenu_Store.SelectedItem];
                break;
            case 6:
                gSelectedStoreItem = gStoreType6[gMenu_Store.SelectedItem];
                break;
            case 7:
                gSelectedStoreItem = gStoreType7[gMenu_Store.SelectedItem];
                break;
            default:
            {
                ASSERT(FALSE, "[%s] Error. Unknown gStoreType[gStoreSpriteIndex] after selecting an item in a store.", __FUNCTION__);
            }

        }
    }
}

void BlitUsableStoreItems(uint8_t storetype)
{
    uint16_t ItemArray[MAX_STORE_ITEMS] = { 0 };
    switch (storetype)
    {
        case 0:
        case 1:
        {
            for (uint8_t Item = 0; Item < MAX_STORE_ITEMS; Item++)
            {
                if (gStoreType1[Item] == 0 && Item != 0)
                {
                    gLastStoreItem = Item - 1;
                    break;
                }
                else
                {
                    ItemArray[Item] = gStoreType1[Item];
                }
            }
            break;
        }
        case 2:
        {
            for (uint8_t Item = 0; Item < MAX_STORE_ITEMS; Item++)
            {
                if (gStoreType2[Item] == 0 && Item != 0)
                {
                    gLastStoreItem = Item - 1;
                    break;
                }
                else
                {
                    ItemArray[Item] = gStoreType2[Item];
                }
            }
            break;
        }
        case 3:
        {
            for (uint8_t Item = 0; Item < MAX_STORE_ITEMS; Item++)
            {
                if (gStoreType3[Item] == 0 && Item != 0)
                {
                    gLastStoreItem = Item - 1;
                    break;
                }
                else
                {
                    ItemArray[Item] = gStoreType3[Item];
                }
            }
            break;
        }
        case 4:
        {
            for (uint8_t Item = 0; Item < MAX_STORE_ITEMS; Item++)
            {
                if (gStoreType4[Item] == 0 && Item != 0)
                {
                    gLastStoreItem = Item - 1;
                    break;
                }
                else
                {
                    ItemArray[Item] = gStoreType4[Item];
                }
            }
            break;
        }
        case 5:
        {
            for (uint8_t Item = 0; Item < MAX_STORE_ITEMS; Item++)
            {
                if (gStoreType5[Item] == 0 && Item != 0)
                {
                    gLastStoreItem = Item - 1;
                    break;
                }
                else
                {
                    ItemArray[Item] = gStoreType5[Item];
                }
            }
            break;
        }
        default:
        {
            ASSERT(FALSE, "[%s] Error. Unknown gStoreType[gStoreSpriteIndex] in BlitUsableStoreItems.", __FUNCTION__);
        }
    }

    for (uint8_t StoreSlot = 0; StoreSlot < _countof(gMI_Store_Items); StoreSlot++)
    {
        if (!(ItemArray[StoreSlot] == 0 && StoreSlot != 0))
        {
            uint16_t ItemCostSize = snprintf(NULL, 0, "$%d", (gUseableItems[ItemArray[StoreSlot]].ValueCurrency * 2) );
            char* ItemCostString = malloc(ItemCostSize + 1);
            snprintf(ItemCostString, ItemCostSize + 1, "$%d", (gUseableItems[ItemArray[StoreSlot]].ValueCurrency * 2) );

            BlitStringToBuffer(gUseableItems[ItemArray[StoreSlot]].Name, &g6x7Font, &COLOR_BLACK, gMI_Store_Items[StoreSlot]->x, gMI_Store_Items[StoreSlot]->y);

            BlitStringToBuffer(ItemCostString, &g6x7Font, &COLOR_BLACK, gMI_Store_Items[StoreSlot]->x + 140, gMI_Store_Items[StoreSlot]->y);
        }
    }

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_Store_Items[gMenu_Store.SelectedItem]->x - 6, gMI_Store_Items[gMenu_Store.SelectedItem]->y);

    BlitItemDescription(gUseableItems[ItemArray[gMenu_Store.SelectedItem]].Description);
}

void BlitEquipableStoreItems(uint8_t storetype)
{
    uint16_t ItemArray[MAX_STORE_ITEMS] = { 0 };
    switch (storetype)
    {
        case 6:
        {
            for (uint8_t Item = 0; Item < MAX_STORE_ITEMS; Item++)
            {
                if (gStoreType6[Item] == 0 && Item != 0)
                {
                    gLastStoreItem = Item - 1;
                    break;
                }
                else
                {
                    ItemArray[Item] = gStoreType6[Item];
                }
            }
            break;
        }
        case 7:
        {
            for (uint8_t Item = 0; Item < MAX_STORE_ITEMS; Item++)
            {
                if (gStoreType7[Item] == 0 && Item != 0)
                {
                    gLastStoreItem = Item - 1;
                    break;
                }
                else
                {
                    ItemArray[Item] = gStoreType7[Item];
                }
            }
            break;
        }
        default:
        {
            ASSERT(FALSE, "[%s] Error. Unknown gStoreType[gStoreSpriteIndex] in BlitEquipableStoreItems.", __FUNCTION__);
        }
    }

    for (uint8_t StoreSlot = 0; StoreSlot < _countof(gMI_Store_Items); StoreSlot++)
    {
        if (!(ItemArray[StoreSlot] == 0 && StoreSlot != 0))
        {
            uint16_t ItemCostSize = snprintf(NULL, 0, "$%d", (gEquipableItems[ItemArray[StoreSlot]].ValueCurrency * 2));
            char* ItemCostString = malloc(ItemCostSize + 1);
            snprintf(ItemCostString, ItemCostSize + 1, "$%d", (gEquipableItems[ItemArray[StoreSlot]].ValueCurrency * 2));

            BlitStringToBuffer(gEquipableItems[ItemArray[StoreSlot]].Name, &g6x7Font, &COLOR_BLACK, gMI_Store_Items[StoreSlot]->x, gMI_Store_Items[StoreSlot]->y);

            BlitStringToBuffer(ItemCostString, &g6x7Font, &COLOR_BLACK, gMI_Store_Items[StoreSlot]->x + 140, gMI_Store_Items[StoreSlot]->y);
        }
    }

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_Store_Items[gMenu_Store.SelectedItem]->x - 6, gMI_Store_Items[gMenu_Store.SelectedItem]->y);

    BlitItemDescription(gEquipableItems[ItemArray[gMenu_Store.SelectedItem]].Description);
}

void DrawSelectedStoreOptions(void)
{
    DrawWindow(20 + (48 * 0) + 0, 152 - 18, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    
    DrawWindow(20 + (48 * 1) + 2, 152 - 18, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    BlitStringToBuffer(gMI_Store_Back.Name, &g6x7Font, &COLOR_BLACK, gMI_Store_Back.x, gMI_Store_Back.y);

    BlitStringToBuffer(gMI_Store_Buy.Name, &g6x7Font, &COLOR_BLACK, gMI_Store_Buy.x, gMI_Store_Buy.y);

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_SelectedStore_Items[gMenu_SelectedStore.SelectedItem]->x - 6, gMI_SelectedStore_Items[gMenu_SelectedStore.SelectedItem]->y);
}
