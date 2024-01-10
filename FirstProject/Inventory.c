
#include "Main.h"

#include "Inventory.h"

MENUITEM gMI_Inventory_Equipable_Slot0 = { "Slot1", 200, 26 + (10 * 2), TRUE, MenuItem_Inventory_Equipable_Slot0 };

MENUITEM gMI_Inventory_Equipable_Slot1 = { "Slot2", 200, 26 + (10 * 3), TRUE, MenuItem_Inventory_Equipable_Slot1 };

MENUITEM gMI_Inventory_Equipable_Slot2 = { "Slot3", 200, 26 + (10 * 4), TRUE, MenuItem_Inventory_Equipable_Slot2 };

MENUITEM gMI_Inventory_Equipable_Slot3 = { "Slot4", 200, 26 + (10 * 5), TRUE, MenuItem_Inventory_Equipable_Slot3 };

MENUITEM gMI_Inventory_Equipable_Slot4 = { "Slot5", 200, 26 + (10 * 6), TRUE, MenuItem_Inventory_Equipable_Slot4 };

MENUITEM gMI_Inventory_Equipable_Slot5 = { "SLot6" , 200, 26 + (10 * 7), TRUE, MenuItem_Inventory_Equipable_Slot5};

MENUITEM gMI_Inventory_Equipable_Slot6 = { "SLot7" , 200, 26 + (10 * 8), TRUE, MenuItem_Inventory_Equipable_Slot6 };

MENUITEM gMI_Inventory_Equipable_Slot7 = { "SLot8" , 200, 26 + (10 * 9), TRUE, MenuItem_Inventory_Equipable_Slot7 };

MENUITEM gMI_Inventory_Equipable_Slot8 = { "SLot9" , 200, 26 + (10 * 10), TRUE, MenuItem_Inventory_Equipable_Slot8 };

MENUITEM gMI_Inventory_Equipable_Slot9 = { "SLot10" , 200, 26 + (10 * 11), TRUE, MenuItem_Inventory_Equipable_Slot9 };

MENUITEM gMI_Inventory_Equipable_Slot10 = { "SLot11" , 200, 26 + (10 * 12), TRUE, MenuItem_Inventory_Equipable_Slot10 };

MENUITEM gMI_Inventory_Equipable_Slot11 = { "SLot12" , 200, 26 + (10 * 13), TRUE, MenuItem_Inventory_Equipable_Slot11 };

MENUITEM gMI_Inventory_Equipable_Slot12 = { "SLot13" , 200, 26 + (10 * 14), TRUE, MenuItem_Inventory_Equipable_Slot12 };

MENUITEM gMI_Inventory_Equipable_Slot13 = { "SLot14" , 200, 26 + (10 * 15), TRUE, MenuItem_Inventory_Equipable_Slot13 };

MENUITEM gMI_Inventory_Equipable_Slot14 = { "SLot15" , 200, 26 + (10 * 16), TRUE, MenuItem_Inventory_Equipable_Slot14 };

MENUITEM gMI_Inventory_Equipable_Slot15 = { "SLot16" , 200, 26 + (10 * 17), TRUE, MenuItem_Inventory_Equipable_Slot15 };

MENUITEM gMI_Inventory_Equipable_Slot16 = { "SLot17" , 200, 26 + (10 * 18), TRUE, MenuItem_Inventory_Equipable_Slot16 };

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
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawWindow(192, 1, 184, 192, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

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
            break;
        }
        case POCKETSTATE_VALUABLE:
        {
            break;
        }
        case POCKETSTATE_ADVENTURE:
        {
            break;
        }
        case POCKETSTATE_FAVORITE:
        {
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
            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                gDesiredGameState = gPreviousGameState;
                gPreviousGameState = gCurrentGameState;
                gCurrentGameState = gDesiredGameState;
            }
            break;
        }
        case POCKETSTATE_RESTORE:
        {
            break;
        }
        case POCKETSTATE_VALUABLE:
        {
            break;
        }
        case POCKETSTATE_ADVENTURE:
        {
            break;
        }
        case POCKETSTATE_FAVORITE:
        {
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
    DrawWindow(194, 26, 35, 18, &COLOR_BLACK, &COLOR_LIGHT_BLUE, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);

    DrawWindow(194 + (36 * 1), 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);

    DrawWindow(194 + (36 * 2), 26, 36, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);  //middle box is 1 pixel wider

    DrawWindow(194 + (36 * 3) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);//offset next two boxes by one pixel

    DrawWindow(194 + (36 * 4) + 1, 26, 35, 18, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK);


                    
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

    DrawWindow(194, 25 + (10 * 18), 180, 9, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);


    for (uint8_t Counter = 0; Counter < _countof(gMI_InventoryEquipable_Items); Counter++)
    {
        if (gMI_InventoryEquipable_Items[Counter]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMI_InventoryEquipable_Items[Counter]->Name, &g6x7Font, &COLOR_BLACK, gMI_InventoryEquipable_Items[Counter]->x, gMI_InventoryEquipable_Items[Counter]->y);
        }
    }

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_InventoryEquipable_Items[gMenu_InventoryEquipable.SelectedItem]->x - 6, gMI_InventoryEquipable_Items[gMenu_InventoryEquipable.SelectedItem]->y);
}

void DrawRestorePocket(void)
{

}

void DrawValuablePocket(void)
{

}

void MenuItem_Inventory_Equipable_Slot0(void)
{

}

void MenuItem_Inventory_Equipable_Slot1(void)
{

}

void MenuItem_Inventory_Equipable_Slot2(void)
{

}

void MenuItem_Inventory_Equipable_Slot3(void)
{

}

void MenuItem_Inventory_Equipable_Slot4(void)
{

}

void MenuItem_Inventory_Equipable_Slot5(void)
{

}

void MenuItem_Inventory_Equipable_Slot6(void)
{

}

void MenuItem_Inventory_Equipable_Slot7(void)
{

}

void MenuItem_Inventory_Equipable_Slot8(void)
{

}

void MenuItem_Inventory_Equipable_Slot9(void)
{

}

void MenuItem_Inventory_Equipable_Slot10(void)
{

}

void MenuItem_Inventory_Equipable_Slot11(void)
{

}

void MenuItem_Inventory_Equipable_Slot12(void)
{

}

void MenuItem_Inventory_Equipable_Slot13(void)
{

}

void MenuItem_Inventory_Equipable_Slot14(void)
{

}

void MenuItem_Inventory_Equipable_Slot15(void)
{

}

void MenuItem_Inventory_Equipable_Slot16(void)
{

}