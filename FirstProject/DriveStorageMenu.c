
#include "DriveStorageMenu.h"

#define X_MARGIN_1 32
#define X_MARGIN_2 2
#define Y_MARGIN 3
#define X_GAP 11
#define Y_GAP 5

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

    //TODO: organize storage boxes size and shape based on how many ROWS_IN_DRIVE and COLUMNS_IN_DRIVE      //////////////////////////////////////////////////////////////////////

    for (uint8_t drive = 0; drive < TOTAL_STORAGE_DRIVES; drive++)
    {
        //name for each drive A:, B:, C:... etc
        char* drivename = malloc(16);
        if (drivename != NULL)
        {
            //drive discs
            if (gCurrentDrive == drive)
            {
                //selected drive
                DrawWindow(6, 9 + (drive * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_NES_YELLOW, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
            }
            else
            {
                //grayed out drives
                DrawWindow(6, 9 + (drive * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
            }

            sprintf_s(drivename, 16, "%c:", 65 + drive);

            BlitStringToBuffer(drivename, &g6x7Font, &COLOR_NES_RED, 10, 15 + (drive * 29));
        }
    }

    uint16_t xbox = CalcXBoxLength(256, X_MARGIN_1, X_MARGIN_2, X_GAP);
    uint16_t ybox = CalcYBoxLength(227, Y_MARGIN, Y_GAP);

    uint16_t xspace = xbox + (X_GAP);
    uint16_t yspace = ybox + (Y_GAP);

    for (uint8_t row = 0; row < ROWS_IN_DRIVE; row++)
    {
        for (uint8_t column = 0; column < COLUMNS_IN_DRIVE; column++)
        {
            ////sprite boxes
            if (gSelectedDriveSlot64 == (row) + (column * ROWS_IN_DRIVE) && gMarkedDriveSlot64 == (row) + (column * ROWS_IN_DRIVE) && gSelectingParty == FALSE && gMarkedDrive == gCurrentDrive)
            {
                //selected AND marked slot
            DrawWindow(
            (row * xspace) + 4 + X_MARGIN_1, //4 is x position of large box
            (column * yspace) + 6 + Y_MARGIN, //6 is y pos of large box
            18, //enough to fit a 16x16 sprite, but could use xbox or ybox to create bigger boxes
            18,
            &COLOR_NES_MAGENTA,
            &COLOR_DARK_WHITE,
            &COLOR_DARK_GRAY,
            WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else if (gSelectedDriveSlot64 == (row) + (column * ROWS_IN_DRIVE) && gSelectingParty == FALSE)
            {
                //selected slot
            DrawWindow(
            (row * xspace) + 4 + X_MARGIN_1, //4 is x position of large box
            (column * yspace) + 6 + Y_MARGIN, //6 is y pos of large box
            18, //enough to fit a 16x16 sprite, but could use xbox or ybox to create bigger boxes
            18,
            &COLOR_LIGHT_BLUE,
            &COLOR_DARK_WHITE,
            &COLOR_DARK_GRAY,
            WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else if (gMarkedDriveSlot64 == (row) + (column * ROWS_IN_DRIVE) && gMarkedDrive == gCurrentDrive)
            {
                //marked for interaction
            DrawWindow(
            (row * xspace) + 4 + X_MARGIN_1, //4 is x position of large box
            (column * yspace) + 6 + Y_MARGIN, //6 is y pos of large box
            18, //enough to fit a 16x16 sprite, but could use xbox or ybox to create bigger boxes
            18,
            &COLOR_NES_PURPLE,
            &COLOR_DARK_WHITE,
            &COLOR_DARK_GRAY,
            WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }
            else
            {
                //grayed out slots
            DrawWindow(
            (row * xspace) + 4 + X_MARGIN_1, //4 is x position of large box
            (column * yspace) + 6 + Y_MARGIN, //6 is y pos of large box
            18, //enough to fit a 16x16 sprite, but could use xbox or ybox to create bigger boxes
            18,
            &COLOR_NES_WHITE,
            &COLOR_DARK_WHITE,
            &COLOR_DARK_GRAY,
            WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
            }

            //display monster level on each slot
            if (GetDriveMonsterDataAt(gCurrentDrive, (row) + (column * ROWS_IN_DRIVE), MONSTER_DATA_INDEX) != MONSTER_NULL)
            {
                char* slotlvl = malloc(16);
                if (slotlvl != NULL)
                {
                    uint8_t level = GetLevelFromDriveMonsterExp(GetDriveMonPtr(gCurrentDrive, (row)+(column * ROWS_IN_DRIVE)));
                    sprintf_s(slotlvl, 16, "%d", level);
                    BlitStringToBuffer(slotlvl, &g6x7Font, &COLOR_FORREST_GREEN, (row * xspace) + 6 + X_MARGIN_1, (column * yspace) + 8 + Y_MARGIN);
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //char drivename = 'A';
    //for (uint8_t row = 0; row < ROWS_IN_DRIVE; row++)
    //{
    //    for (uint8_t column = 0; column < COLUMNS_IN_DRIVE; column++)
    //    {
    //        //drive discs
    //        if (gCurrentDrive == column)    
    //        {
    //            //selected drive
    //            DrawWindow(6, 9 + (column * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_NES_YELLOW, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    //        }
    //        else        
    //        {
    //            //grayed out drives
    //            DrawWindow(6, 9 + (column * 29), 15, 17, &COLOR_DARK_GRAY, &COLOR_LIGHT_GRAY, &COLOR_BLACK, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);
    //        }

    //        //name for each drive A:, B:, C:... etc
    //        char* drivename = malloc(16);
    //        sprintf_s(drivename, 16, "%c:", 65 + column);
    //        BlitStringToBuffer(drivename, &g6x7Font, &COLOR_NES_RED, 10, 15 + (column * 29));

    //        //sprite boxes
    //        if (gSelectedDriveSlot64 == (row) + (column * 8) && gMarkedDriveSlot64 == (row) + (column * 8) && gSelectingParty == FALSE && gMarkedDrive == gCurrentDrive)
    //        {
    //            //selected AND marked slot
    //            DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_MAGENTA, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
    //        }
    //        else if (gSelectedDriveSlot64 == (row) + (column * 8) && gSelectingParty == FALSE)
    //        {
    //            //selected slot
    //            DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_LIGHT_BLUE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
    //        }
    //        else if (gMarkedDriveSlot64 == (row) + (column * 8) && gMarkedDrive == gCurrentDrive)
    //        {
    //            //marked for interaction
    //            DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_PURPLE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
    //        }
    //        else
    //        {
    //            //grayed out slots
    //            DrawWindow(36 + (row * 29), 9 + (column * 29), 17, 17, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);
    //        }

    //        //display monster level on each slot
    //        if (GetDriveMonsterDataAt(gCurrentDrive, (row) + (column * 8), MONSTER_DATA_INDEX) != MONSTER_NULL)
    //        {
    //            char* slotlvl = malloc(16);
    //            uint8_t level = GetLevelFromDriveMonsterExp(GetDriveMonPtr(gCurrentDrive, (row)+(column * 8)));
    //            sprintf_s(slotlvl, 16, "%d", level);
    //            BlitStringToBuffer(slotlvl, &g6x7Font, &COLOR_FORREST_GREEN, 40 + (row * 29), 12 + (column * 29));
    //        }
    //    }
    //}

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
        if (gCurrentDrive >= TOTAL_STORAGE_DRIVES)
        {
            gCurrentDrive = 0;
        }
    }

    if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        if (gSelectingParty == FALSE && (gSelectedDriveSlot64 + 1) % ROWS_IN_DRIVE == 0)
        {
            gSelectingParty = TRUE;
            //gSelectedPartySlot = ??? TODO
            gSelectedPartySlot = ConvSelectedDriveSlotToSelectedPartySlot();
            gSelectedDriveSlot64 = RESET_MARKER;

        }
        else if (gSelectingParty == FALSE)
        {
            gSelectedDriveSlot64++;
            if (gSelectedDriveSlot64 == TOTAL_IN_DRIVE)
            {
                gSelectedDriveSlot64 = 0;
            }
        }
        else
        {
            //transfer marker from party to storage based on previous location of cursor
            gSelectedDriveSlot64 = ROWS_IN_DRIVE * ConvSelectedPartySlotToSelectedColumn();
            gSelectingParty = FALSE;
        }
    }
    else if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        if (gSelectingParty == FALSE && (gSelectedDriveSlot64 % ROWS_IN_DRIVE == 0 || gSelectedDriveSlot64 == 0))
        {
            gSelectingParty = TRUE;
            //gSelectedPartySlot = ??? TODO
            gSelectedPartySlot = ConvSelectedDriveSlotToSelectedPartySlot();
            gSelectedDriveSlot64 = RESET_MARKER;

        }
        else if (gSelectingParty == FALSE)
        {
            gSelectedDriveSlot64--;
            if (gSelectedDriveSlot64 == -1)
            {
                gSelectedDriveSlot64 = TOTAL_IN_DRIVE - 1;
            }
        }
        else
        {
            //transfer marker from party to storage based on previous location of cursor
            gSelectedDriveSlot64 = (ROWS_IN_DRIVE * (ConvSelectedPartySlotToSelectedColumn() + 1)) - 1;
            gSelectingParty = FALSE;
        }
    }
    else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuNavigate);
        if (gSelectingParty == FALSE)
        {
            gSelectedDriveSlot64 += ROWS_IN_DRIVE;
            if (gSelectedDriveSlot64 > TOTAL_IN_DRIVE - 1)
            {
                gSelectedDriveSlot64 -= TOTAL_IN_DRIVE;
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
            gSelectedDriveSlot64 -= ROWS_IN_DRIVE;
            if (gSelectedDriveSlot64 < 0)
            {
                gSelectedDriveSlot64 += TOTAL_IN_DRIVE;
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


uint16_t CalcXBoxLength(uint16_t xTotal, uint16_t xMargin1, uint16_t xMargin2, uint16_t xGap)
{
    uint16_t retValue;

    //calculate the length of all boxes to fill total minus both margins

    retValue = xTotal - xMargin1;
    retValue -= xMargin2;
    retValue -= (ROWS_IN_DRIVE - 1) * xGap;
    retValue /= ROWS_IN_DRIVE;

    return(retValue);
}

uint16_t CalcYBoxLength(uint16_t yTotal, uint16_t yMargin, uint16_t yGap)
{
    uint16_t retValue;

    //calculate the length of all boxes to fill total minus both margins

    retValue = yTotal - (2 * yMargin);
    retValue -= (COLUMNS_IN_DRIVE - 1) * yGap;
    retValue /= COLUMNS_IN_DRIVE;

    return(retValue);
}

uint8_t ConvSelectedPartySlotToSelectedColumn(void)
{
    //converts selected party member to a column when columns are not equal to party slots
    uint8_t convertedcolumn[MAX_PARTY_SIZE];

    if (COLUMNS_IN_DRIVE >= MAX_PARTY_SIZE)
    {
        //large inventory, no overlap
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; i++)
        {
            convertedcolumn[i] = i;
        }
    }
    else
    {
        //small inventory, overlapping

        //TODO: Only works for columns that divide evenly into MAX_PARTY_SIZE
        uint8_t ratio = MAX_PARTY_SIZE / COLUMNS_IN_DRIVE;

        for (uint8_t i = 0, j = 0; i < MAX_PARTY_SIZE; i++)
        {

            convertedcolumn[i] = j;
            if ((i + 1) % ratio == 0)
            {
                j++;
            }
        }
    }
    return(convertedcolumn[gSelectedPartySlot]);
}

uint8_t ConvSelectedDriveSlotToSelectedPartySlot(void)
{
    //converts selected column to a party member when uneven
    uint8_t convertedparty[COLUMNS_IN_DRIVE];

    if (COLUMNS_IN_DRIVE >= MAX_PARTY_SIZE)
    {
        //large inventory, no overlap
        for (uint8_t i = 0; i < COLUMNS_IN_DRIVE; i++)
        {
            if (i >= MAX_PARTY_SIZE)
            {
                convertedparty[i] = MAX_PARTY_SIZE - 1;
            }
            else
            {
                convertedparty[i] = i;
            }
        }
    }
    else
    {
        //small inventory, overlapping

        //TODO: Only works for columns that divide evenly into MAX_PARTY_SIZE
        uint8_t ratio = MAX_PARTY_SIZE / COLUMNS_IN_DRIVE;

        for (uint8_t i = 0, j = 0; i < COLUMNS_IN_DRIVE; i++, j += ratio)
        {
            convertedparty[i] = j;
        }
    }

    //TODO probably a better way to find the current selected column than dividing selected by rows
    return(convertedparty[(gSelectedDriveSlot64 / ROWS_IN_DRIVE)]);
}