
#include "DriveStorageMenu.h"

#define RESET_MARKER 127
uint8_t gCurrentDrive = 0;
uint8_t gMarkedDrive = RESET_MARKER;
int8_t gSelectedDriveSlot64 = 0;
int8_t gMarkedDriveSlot64 = RESET_MARKER;
int8_t gSelectedPartySlot = 0;
int8_t gMarkedPartySlot = RESET_MARKER;
BOOL gSelectingParty = FALSE;

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
     
     
    //storage window
    DrawWindow(4, 6, 256, 227, &COLOR_DARK_GRAY, &COLOR_DARK_WHITE, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    char drivename = 'A';
    for (uint8_t row = 0; row < ROWS_IN_DRIVE; row++)
    {
        for (uint8_t column = 0; column < COLUMNS_IN_DRIVE; column++)
        {
            //drive discs
            if (gCurrentDrive == column)    
            {
                //selected drive
                DrawWindow(6, 9 + (column * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_NES_YELLOW, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
            }
            else        
            {
                //grayed out drives
                DrawWindow(6, 9 + (column * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
            }

            //name for each drive A:, B:, C:... etc
            char* drivename = malloc(16);
            sprintf_s(drivename, 16, "%c:", 65 + column);
            BlitStringToBuffer(drivename, &g6x7Font, &COLOR_NES_RED, 10, 15 + (column * 29));

            //sprite boxes
            if (gSelectedDriveSlot64 == (row) + (column * 8) && gMarkedDriveSlot64 == (row) + (column * 8) && gSelectingParty == FALSE && gMarkedDrive == gCurrentDrive)
            {
                //selected AND marked slot
                DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_MAGENTA, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else if (gSelectedDriveSlot64 == (row) + (column * 8) && gSelectingParty == FALSE)
            {
                //selected slot
                DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_LIGHT_BLUE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else if (gMarkedDriveSlot64 == (row) + (column * 8) && gMarkedDrive == gCurrentDrive)
            {
                //marked for interaction
                DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_PURPLE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else
            {
                //grayed out slots
                DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }

            //display monster level on each slot
            if (GetDriveMonsterDataAt(gCurrentDrive, (row) + (column * 8), MONSTER_DATA_INDEX) != MONSTER_NULL)
            {
                char* slotlvl = malloc(16);
                uint8_t level = GetLevelFromDriveMonsterExp(GetDriveMonPtr(gCurrentDrive, (row)+(column * 8)));
                sprintf_s(slotlvl, 16, "%d", level);
                BlitStringToBuffer(slotlvl, &g6x7Font, &COLOR_FORREST_GREEN, 40 + (row * 29), 12 + (column * 29));
            }
        }
    }

    //player party window
    DrawWindow(262, 6, 120, 227, &COLOR_DARK_GRAY, &COLOR_FORREST_GREEN, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    
    for (uint8_t partymember = 0; partymember < MAX_PARTY_SIZE; partymember++)
    {
        if (gPlayerParty[partymember].DriveMonster.hasIndex == TRUE)
        {
            uint16_t HpPercent = 100 - ((gPlayerParty[partymember].Health * 100) / (gPlayerParty[partymember].MaxHealth));

            uint16_t ExpPercent = 100 - (((gPlayerParty[partymember].DriveMonster.Experience - gExperienceTables[gBaseStats[gPlayerParty[partymember].DriveMonster.Index].growthRate][gPlayerParty[partymember].Level]) * 100) / (gExperienceTables[gBaseStats[gPlayerParty[partymember].DriveMonster.Index].growthRate][gPlayerParty[partymember].Level + 1] - gExperienceTables[gBaseStats[gPlayerParty[partymember].DriveMonster.Index].growthRate][gPlayerParty[partymember].Level]));

            DrawMonsterHpBar(267, 24 + (partymember * 36), HpPercent, ExpPercent, gPlayerParty[partymember].Level, gPlayerParty[partymember].DriveMonster.nickname, TRUE);
        }

        if (gSelectingParty == TRUE && gSelectedPartySlot == partymember && gSelectedPartySlot != gMarkedPartySlot)
        {
            //selected
            DrawWindow(267 - 3, 24 - 10 + (partymember * 36), 116, 15, &COLOR_LIGHT_BLUE, NULL, NULL, WINDOW_FLAG_ROUNDED | WINDOW_FLAG_BORDERED);
        }
        else if (gSelectingParty == TRUE && gSelectedPartySlot == partymember && gSelectedPartySlot == gMarkedPartySlot)
        {
            //selected and marked
            DrawWindow(267 - 3, 24 - 10 + (partymember * 36), 116, 15, &COLOR_NES_MAGENTA, NULL, NULL, WINDOW_FLAG_ROUNDED | WINDOW_FLAG_BORDERED);
        }
        else if (gMarkedPartySlot == partymember)
        {
            //marked
            DrawWindow(267 - 3, 24 - 10 + (partymember * 36), 116, 15, &COLOR_NES_PURPLE, NULL, NULL, WINDOW_FLAG_ROUNDED | WINDOW_FLAG_BORDERED);
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
        if (gSelectingParty == FALSE && (gSelectedDriveSlot64 + 1) % 8 == 0)
        {
            gSelectingParty = TRUE;
            gSelectedDriveSlot64 = RESET_MARKER;

        }
        else if (gSelectingParty == FALSE)
        {
            gSelectedDriveSlot64++;
            if (gSelectedDriveSlot64 == 64)
            {
                gSelectedDriveSlot64 = 0;
            }
        }
        else
        {
            switch (gSelectedPartySlot)     //only 6 slots but 8 rows
            {
                case 0:
                {
                    gSelectedDriveSlot64 = 0;
                    break;
                }
                case 1:
                {
                    //skip 8
                    gSelectedDriveSlot64 = 16;
                    break;
                }
                case 2:
                {
                    gSelectedDriveSlot64 = 24;
                    break;
                }
                case 3:
                {
                    gSelectedDriveSlot64 = 32;
                    break;
                }
                case 4:
                {
                    gSelectedDriveSlot64 = 40;
                    break;
                }
                case 5:
                {
                    //skip 48
                    gSelectedDriveSlot64 = 56;
                    break;
                }
            }
            gSelectingParty = FALSE;
        }
    }
    else if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        if (gSelectingParty == FALSE && (gSelectedDriveSlot64 % 8 == 0 || gSelectedDriveSlot64 == 0))
        {
            gSelectingParty = TRUE;
            gSelectedDriveSlot64 = RESET_MARKER;

        }
        else if (gSelectingParty == FALSE)
        {
            gSelectedDriveSlot64--;
            if (gSelectedDriveSlot64 == -1)
            {
                gSelectedDriveSlot64 = 63;
            }
        }
        else
        {
            switch (gSelectedPartySlot)     //only 6 slots but 8 rows
            {
                case 0:
                {
                    gSelectedDriveSlot64 = 7;
                    break;
                }
                case 1:
                {
                    //skip 15
                    gSelectedDriveSlot64 = 23;
                    break;
                }
                case 2:
                {
                    gSelectedDriveSlot64 = 31;
                    break;
                }
                case 3:
                {
                    gSelectedDriveSlot64 = 39;
                    break;
                }
                case 4:
                {
                    gSelectedDriveSlot64 = 47;
                    break;
                }
                case 5:
                {
                    //skip 55
                    gSelectedDriveSlot64 = 63;
                    break;
                }
            }
            gSelectingParty = FALSE;
        }
    }
    else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        if (gSelectingParty == FALSE)
        {
            gSelectedDriveSlot64 += 8;
            if (gSelectedDriveSlot64 > 63)
            {
                gSelectedDriveSlot64 -= 64;
            }
        }
        else
        {
            gSelectedPartySlot++;
            if (gSelectedPartySlot > 5)
            {
                gSelectedPartySlot = 0;
            }
        }
    }
    else if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        if (gSelectingParty == FALSE)
        {
            gSelectedDriveSlot64 -= 8;
            if (gSelectedDriveSlot64 < 0)
            {
                gSelectedDriveSlot64 += 64;
            }
        }
        else
        {
            gSelectedPartySlot--;
            if (gSelectedPartySlot < 0)
            {
                gSelectedPartySlot = 5;
            }
        }
        
    }

    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuChoose);

        ////TODO: make a menu here and call below from a function of that menu, example a "move" or "switch" menu button
        if (gMarkedDriveSlot64 == RESET_MARKER)
        {
            
            if (gSelectedDriveSlot64 == RESET_MARKER && gSelectingParty == TRUE)
            {
                if (gMarkedPartySlot == RESET_MARKER)
                {
                    //mark selected party slot
                    gMarkedPartySlot = gSelectedPartySlot;
                }
                else
                {
                    //switch party slots
                    struct Monster TempMonster = gPlayerParty[gMarkedPartySlot];

                    gPlayerParty[gMarkedPartySlot] = gPlayerParty[gSelectedPartySlot];
                    gPlayerParty[gSelectedPartySlot] = TempMonster; 
                    int8_t count = CompactPlayerPartySlots();
                    if (count != -1)
                    {
                        gPlayerPartyCount = count;
                    }
                    gMarkedPartySlot = RESET_MARKER;

                }
                
            }
            else if (gSelectedDriveSlot64 != RESET_MARKER && gMarkedPartySlot != RESET_MARKER)
            {
                //switch marked party with selected drive

                //TODO:
                struct Monster TempMonster = gPlayerParty[gMarkedPartySlot];

                ConvDriveMonsterAtToMonster(gCurrentDrive, gSelectedDriveSlot64, &gPlayerParty[gMarkedPartySlot]);
                uint8_t level = GetLevelFromMonsterExp(&gPlayerParty[gMarkedPartySlot]);
                SetMonsterData(&gPlayerParty[gMarkedPartySlot], MONSTER_DATA_LEVEL, &level);
                SetDriveMonsterAt(gCurrentDrive, gSelectedDriveSlot64, &TempMonster.DriveMonster);
                int8_t count = CompactPlayerPartySlots();
                if (count != -1)
                {
                    gPlayerPartyCount = count;
                }
                gMarkedPartySlot = RESET_MARKER;
            }
            else
            {
                //mark selected slot
                gMarkedDriveSlot64 = gSelectedDriveSlot64;
                gMarkedDrive = gCurrentDrive;
            }
        }
        else if (gMarkedDriveSlot64 != RESET_MARKER && gSelectingParty == TRUE)
        {
            //switch marked drive with selected party

            //TODO:

            struct Monster TempMonster = gPlayerParty[gSelectedPartySlot];

            ConvDriveMonsterAtToMonster(gMarkedDrive, gMarkedDriveSlot64, &gPlayerParty[gSelectedPartySlot]);
            uint8_t level = GetLevelFromMonsterExp(&gPlayerParty[gSelectedPartySlot]);
            SetMonsterData(&gPlayerParty[gSelectedPartySlot], MONSTER_DATA_LEVEL, &level);
            SetDriveMonsterAt(gMarkedDrive, gMarkedDriveSlot64, &TempMonster.DriveMonster);
            int8_t count = CompactPlayerPartySlots();
            if (count != -1)
            {
                gPlayerPartyCount = count;
            }
            gMarkedDriveSlot64 = RESET_MARKER;
            gMarkedDrive = RESET_MARKER;
        }
        else   //switches marked drive with selected drive
        {
            ////TODO: working but im sure it has problems
            struct DriveMonster TempMonster;

            CopyDriveMonsterAt(gCurrentDrive, gSelectedDriveSlot64, &TempMonster);  //copy selected into temp
            SetDriveMonsterAt(gCurrentDrive, gSelectedDriveSlot64, GetDriveMonPtr(gMarkedDrive, gMarkedDriveSlot64));  //set marked into selected
            ZeroDriveMonsterAt(gMarkedDrive, gMarkedDriveSlot64);  //clear marked
            SetDriveMonsterAt(gMarkedDrive, gMarkedDriveSlot64, &TempMonster);     //set temp into marked
            gMarkedDriveSlot64 = RESET_MARKER;
            gMarkedDrive = RESET_MARKER;
        }
    }
}