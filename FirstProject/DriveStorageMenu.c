
#include "DriveStorageMenu.h"

#define RESET_MARKER 127
uint8_t gCurrentDrive = 0; 
int8_t gSelectedDriveSlot64 = 1;
int8_t gMarkedDriveSlot64 = RESET_MARKER;
//struct DriveMonster* gTempMonster1 = NULL;
//struct DriveMonster* gTempMonster2 = NULL;

void DrawStorageMenuScreen(void)
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

    ////
     
     
    //menu
    DrawWindow(7, 6, 256, 227, &COLOR_DARK_GRAY, &COLOR_DARK_WHITE, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED /*| WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_VERT_CENTERED*/);

    char drivename = 'A';
    for (uint8_t row = 0; row < ROWS_IN_DRIVE; row++)
    {
        for (uint8_t column = 0; column < COLUMNS_IN_DRIVE; column++)
        {
            //drive discs
            if (gCurrentDrive == column)    
            {
                //selected drive
                DrawWindow(10, 9 + (column * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_NES_YELLOW, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
            }
            else        
            {
                //grayed out drives
                DrawWindow(10, 9 + (column * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
            }

            //name for each drive A:, B:, C:... etc
            char* drivename = malloc(4);
            sprintf_s(drivename, sizeof(drivename), "%c:", 65 + column);
            BlitStringToBuffer(drivename, &g6x7Font, &COLOR_NES_RED, 13, 15 + (column * 29));

            //sprite boxes
            if (gSelectedDriveSlot64 == (row) + (column * 8) && gMarkedDriveSlot64 == (row) + (column * 8))
            {
                //selected AND marked slot
                DrawWindow(39 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_MAGENTA, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else if (gSelectedDriveSlot64 == (row) + (column * 8))
            {
                //selected slot
                DrawWindow(39 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_LIGHT_BLUE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else if (gMarkedDriveSlot64 == (row) + (column * 8))
            {
                //marked for interaction
                DrawWindow(39 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_PURPLE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else
            {
                //grayed out slots
                DrawWindow(39 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }

            if (GetDriveMonsterDataAt(gCurrentDrive, (row) + (column * 8), MONSTER_DATA_INDEX) != MONSTER_NULL)
            {
                BlitStringToBuffer("X", &g6x7Font, &COLOR_FORREST_GREEN, 42 + (row * 29), 12 + (column * 29));
            }
        }
    }
     
    ////

    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_StorageMenu(void)
{
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        gCurrentGameState = GAMESTATE_OVERWORLD;
    }

    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gCurrentDrive++;
        if (gCurrentDrive >= COLUMNS_IN_DRIVE)
        {
            gCurrentDrive = 0;
        }
    }

    if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        gSelectedDriveSlot64++;
        if (gSelectedDriveSlot64 == 64)
        {
            gSelectedDriveSlot64 = 0;
        }
    }
    else if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        gSelectedDriveSlot64--;
        if (gSelectedDriveSlot64 == -1)
        {
            gSelectedDriveSlot64 = 63;
        }
    }
    else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        gSelectedDriveSlot64 += 8;
        if (gSelectedDriveSlot64 > 63)
        {
            gSelectedDriveSlot64 -= 64;
        }
    }
    else if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        gSelectedDriveSlot64 -= 8;
        if (gSelectedDriveSlot64 < 0)
        {
            gSelectedDriveSlot64 += 64;
        }
    }

    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuChoose);

        ////TODO: make a menu here and call below from a function of that menu, example a "move" or "switch" menu button
        if (gMarkedDriveSlot64 == RESET_MARKER)
        {
            //mark a slot or reset marker
            if (gMarkedDriveSlot64 == gSelectedDriveSlot64)
            {
                gMarkedDriveSlot64 = RESET_MARKER;
            }
            else
            {
                gMarkedDriveSlot64 = gSelectedDriveSlot64;
            }
        }
        else    //switches marked with selected
        {
            ////TODO: working but im sure it has problems
            struct DriveMonster TempMonster;

            CopyDriveMonsterAt(gCurrentDrive, gSelectedDriveSlot64, &TempMonster);  //copy selected into temp
            SetDriveMonsterAt(gCurrentDrive, gSelectedDriveSlot64, GetDriveMonPtr(gCurrentDrive, gMarkedDriveSlot64));  //set marked into selected
            ZeroDriveMonsterAt(gCurrentDrive, gMarkedDriveSlot64);  //clear marked
            SetDriveMonsterAt(gCurrentDrive, gMarkedDriveSlot64, &TempMonster);     //set temp into marked
            gMarkedDriveSlot64 = RESET_MARKER;

            
        }
    }
}