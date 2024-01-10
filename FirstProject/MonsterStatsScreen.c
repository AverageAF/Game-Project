
#include "Main.h"

#include "MonsterStatsScreen.h"

int32_t gLearnableSlotOffSet;
uint16_t gLearnableMoveCount;
uint16_t gLearnableMoveSort[NUM_BATTLEMOVES];
uint16_t gLearnableSlotIndex[10];

///////////////////////////////////////////////////////////////////////////

MENUITEM gMI_MonsterStats_LearnableMoveSlot0 = { "Learn0", 82, 7 + (12 * 1), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot1 = { "Learn1", 82, 7 + (12 * 2), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot2 = { "Learn2", 82, 7 + (12 * 3), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot3 = { "Learn3", 82, 7 + (12 * 4), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot4 = { "Learn4", 82, 7 + (12 * 5), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot5 = { "Learn5", 82, 7 + (12 * 6), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot6 = { "Learn6", 82, 7 + (12 * 7), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot7 = { "Learn7", 82, 7 + (12 * 8), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot8 = { "Learn8", 82, 7 + (12 * 9), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM gMI_MonsterStats_LearnableMoveSlot9 = { "Learn9", 82, 7 + (12 * 10), TRUE, MenuItem_MonsterStats_LearnableSelected };

MENUITEM* gMI_MonsterStats_LearnableItems[] = { &gMI_MonsterStats_LearnableMoveSlot0, &gMI_MonsterStats_LearnableMoveSlot1, &gMI_MonsterStats_LearnableMoveSlot2, &gMI_MonsterStats_LearnableMoveSlot3, &gMI_MonsterStats_LearnableMoveSlot4, &gMI_MonsterStats_LearnableMoveSlot5, &gMI_MonsterStats_LearnableMoveSlot6, &gMI_MonsterStats_LearnableMoveSlot7, &gMI_MonsterStats_LearnableMoveSlot8, &gMI_MonsterStats_LearnableMoveSlot9 };

MENU gMenu_MonsterStats_LearnableMoves = { "Learnable Moves", 0, _countof(gMI_MonsterStats_LearnableItems), gMI_MonsterStats_LearnableItems };

///////////////////////////////////////////////////////////////////////////

MENUITEM gMI_MonsterStats_Move0 = { "Move0", 65, 71 + (12 * 5) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM gMI_MonsterStats_Move1 = { "Move1", 65, 71 + (12 * 6) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM gMI_MonsterStats_Move2 = { "Move2", 65, 71 + (12 * 7) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM gMI_MonsterStats_Move3 = { "Move3", 65, 71 + (12 * 8) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM* gMI_MonsterStats_MoveItems[] = { &gMI_MonsterStats_Move0, &gMI_MonsterStats_Move1, &gMI_MonsterStats_Move2, &gMI_MonsterStats_Move3 };

MENU gMenu_MonsterStats_Moves = { "MonsterStats Move Menu", 0, _countof(gMI_MonsterStats_MoveItems), gMI_MonsterStats_MoveItems };

///////////////////////////////////////////////////////////////////////////

MENUITEM gMI_MonsterStats_Switch = { "Switch", 11, 65 + (12 * 5) + 1, TRUE, MenuItem_MonsterStats_MoveSwitch };

MENUITEM gMI_MonsterStats_Remove = { "Remove", 11, 65 + (12 * 6) + 3, TRUE, MenuItem_MonsterStats_MoveRemove };

MENUITEM gMI_MonsterStats_Back = { "Back", 11, 65 + (12 * 7) + 5, TRUE, MenuItem_MonsterStats_MoveBack };

MENUITEM* gMI_MonsterStats_MoveOptionItems[] = { &gMI_MonsterStats_Switch, &gMI_MonsterStats_Remove, &gMI_MonsterStats_Back };

MENU gMenu_MonsterStats_MoveOptions = { "Move Options", 0, _countof(gMI_MonsterStats_MoveOptionItems), gMI_MonsterStats_MoveOptionItems };

///////////////////////////////////////////////////////////////////////////

uint8_t gMonsterToViewStats;

BOOL gIsMoveSelected = FALSE;

BOOL gIsSelectingMoveToSwitchWith = FALSE;

BOOL gDrawMoveRelearnScreen = FALSE;

uint8_t gMoveToSwitch = 0;

void DrawMonsterStatsScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    char StatsTextLine[14][40];                 //need more than 14 lines probably

    static PIXEL32 Element1Color = { 0x00, 0x00, 0x00, 0x00 };

    static PIXEL32 Element2Color = { 0x00, 0x00, 0x00, 0x00 };

    static char Element1Name[12];

    static char Element2Name[12];

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
    }

    switch (gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element1)
    {
        case ELEMENT_NONE:
        {
            Element1Color = COLOR_NES_TAN;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "NONE");

            break;
        }
        case ELEMENT_EARTH:
        {
            Element1Color = COLOR_NES_BROWN;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "EARTH");

            break;
        }
        case ELEMENT_AIR:
        {
            Element1Color = COLOR_NES_SKY_BLUE;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "AIR");

            break;
        }
        case ELEMENT_FIRE:
        {
            Element1Color = COLOR_NES_ORANGE;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "FIRE");

            break;
        }
        case ELEMENT_WATER:
        {
            Element1Color = COLOR_NES_BLUE;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "WATER");

            break;
        }
        case ELEMENT_ELECTRIC:
        {
            Element1Color = COLOR_NES_YELLOW;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "ELECTRIC");

            break;
        }
        case ELEMENT_METAL:
        {
            Element1Color = COLOR_NES_GRAY;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "METAL");

            break;
        }
        case ELEMENT_SOUL:
        {
            Element1Color = COLOR_NES_PURPLE;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "SOUL");

            break;
        }
        case ELEMENT_LIFE:
        {
            Element1Color = COLOR_NES_LIGHT_GREEN;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "LIFE");

            break;
        }
        case ELEMENT_DEATH:
        {
            Element1Color = COLOR_NES_BLACK_RED;

            sprintf_s((char*)Element1Name, sizeof(Element1Name), "DEATH");

            break;
        }
    }
    

    __stosd(gBackBuffer.Memory, 0xFF787878, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));                             //background

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    DrawWindow(63, 63 - (12 * 4), 64, 11, &COLOR_BLACK, &Element1Color, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //Element 1

    if (gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element2 != ELEMENT_NULL && gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element2 != gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element1)
    {
        DrawWindow(63 + 65, 63 - (12 * 4), 64, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //element 2
    }

    DrawWindow(63, 63 - (12 * 3), 150, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //lvl name

    DrawWindow(63, 63 - (12 * 2), 150, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //Health

    DrawWindow(63, 63 - (12 * 1), 196, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //Experience

    DrawWindow(63, 63, 66, 66, &COLOR_BLACK, &COLOR_FORREST_GREEN, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //sprite


    DrawWindow(159, 63, 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //atk

    DrawWindow(159, 63 + (12 * 1), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //def

    DrawWindow(159, 63 + (12 * 2), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //spd

    DrawWindow(159, 63 + (12 * 3), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //psi

    DrawWindow(159, 63 + (12 * 4), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //res

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_MonsterStats_MoveItems[gMenu_MonsterStats_Moves.SelectedItem]->x - 7, gMI_MonsterStats_MoveItems[gMenu_MonsterStats_Moves.SelectedItem]->y);


    Blit32BppBitmapToBuffer(&gBattleSpriteFront[gPlayerParty[gMonsterToViewStats].DriveMonster.Index], 64, 64, BrightnessAdjustment);   //monstersprite


    if (gIsMoveSelected == TRUE)        //move selection options
    {
        if (gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMenu_MonsterStats_Moves.SelectedItem] == BATTLEMOVE_NULL)
        {
            gMI_MonsterStats_Remove.Name = "Relearn";
        }
        else
        {
            gMI_MonsterStats_Remove.Name = "Remove";
        }

        DrawWindow(8, 63 + (12 * 5), 48, 13, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

        DrawWindow(8, 63 + (12 * 6) + 2, 48, 13, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

        DrawWindow(8, 63 + (12 * 7) + 4, 48, 13, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

        for (uint8_t i = 0; i < _countof(gMI_MonsterStats_MoveOptionItems); i++)
        {
            BlitStringToBuffer(gMI_MonsterStats_MoveOptionItems[i]->Name, &g6x7Font, &COLOR_BLACK, gMI_MonsterStats_MoveOptionItems[i]->x, gMI_MonsterStats_MoveOptionItems[i]->y);
        }

        BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_MonsterStats_MoveOptionItems[gMenu_MonsterStats_MoveOptions.SelectedItem]->x - 9, gMI_MonsterStats_MoveOptionItems[gMenu_MonsterStats_MoveOptions.SelectedItem]->y);
    }

    int level = (int*)gPlayerParty[gMonsterToViewStats].Level;

    sprintf_s((char*)StatsTextLine[0], sizeof(StatsTextLine[0]), "Lvl: %d %s", level, gPlayerParty[gMonsterToViewStats].DriveMonster.nickname);

    BlitStringToBuffer(StatsTextLine[0], &g6x7Font, &COLOR_NES_RED, 65, 63 - (12 * 3) + 2);


    int health = (int*)gPlayerParty[gMonsterToViewStats].Health;
    int maxHealth = (int*)gPlayerParty[gMonsterToViewStats].MaxHealth;

    sprintf_s((char*)StatsTextLine[1], sizeof(StatsTextLine[1]), "Health: %d / %d", health, maxHealth);

    if ((health * 100 / maxHealth) > 50)
    {
        BlitStringToBuffer(StatsTextLine[1], &g6x7Font, &COLOR_FORREST_GREEN, 65, 63 - (12 * 2) + 2);       //high hp
    }
    else if ((health * 100 / maxHealth) <= 50 && (health * 100 / maxHealth) > 10)
    {
        BlitStringToBuffer(StatsTextLine[1], &g6x7Font, &COLOR_GOLD, 65, 63 - (12 * 2) + 2);               //med hp
    }
    else if ((health * 100 / maxHealth) <= 10)
    {
        BlitStringToBuffer(StatsTextLine[1], &g6x7Font, &COLOR_DARK_RED, 65, 63 - (12 * 2) + 2);          //low hp
    }


    int expCurr = (int*)(gPlayerParty[gMonsterToViewStats].DriveMonster.Experience - gExperienceTables[gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].growthRate][gPlayerParty[gMonsterToViewStats].Level]);
    int expNextLvl = (int*)(gExperienceTables[gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].growthRate][gPlayerParty[gMonsterToViewStats].Level + 1] - gExperienceTables[gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].growthRate][gPlayerParty[gMonsterToViewStats].Level]);

    sprintf_s((char*)StatsTextLine[2], sizeof(StatsTextLine[2]), "Exp: %d / %d  Tot: %d", expCurr, expNextLvl, gPlayerParty[gMonsterToViewStats].DriveMonster.Experience);

    BlitStringToBuffer(StatsTextLine[2], &g6x7Font, &COLOR_NES_BLUE, 65, 63 - (12 * 1) + 2);


    int attack = (int*)gPlayerParty[gMonsterToViewStats].Attack;

    sprintf_s((char*)StatsTextLine[3], sizeof(StatsTextLine[3]), "Attack: %d", attack);

    BlitStringToBuffer(StatsTextLine[3], &g6x7Font, &COLOR_FORREST_GREEN, 161, 64 + (12 * 0) + 1);


    int defense = (int*)gPlayerParty[gMonsterToViewStats].Defense;

    sprintf_s((char*)StatsTextLine[4], sizeof(StatsTextLine[4]), "Defense: %d", defense);

    BlitStringToBuffer(StatsTextLine[4], &g6x7Font, &COLOR_FORREST_GREEN, 161, 64 + (12 * 1) + 1);


    int speed = (int*)gPlayerParty[gMonsterToViewStats].Speed;

    sprintf_s((char*)StatsTextLine[5], sizeof(StatsTextLine[5]), "Speed: %d", speed);

    BlitStringToBuffer(StatsTextLine[5], &g6x7Font, &COLOR_FORREST_GREEN, 161, 64 + (12 * 2) + 1);


    int psi = (int*)gPlayerParty[gMonsterToViewStats].Psi;

    sprintf_s((char*)StatsTextLine[6], sizeof(StatsTextLine[6]), "Psi: %d", psi);

    BlitStringToBuffer(StatsTextLine[6], &g6x7Font, &COLOR_FORREST_GREEN, 161, 64 + (12 * 3) + 1);


    int resolve = (int*)gPlayerParty[gMonsterToViewStats].Resolve;

    sprintf_s((char*)StatsTextLine[7], sizeof(StatsTextLine[7]), "Resolve: %d", resolve);

    BlitStringToBuffer(StatsTextLine[7], &g6x7Font, &COLOR_FORREST_GREEN, 161, 64 + (12 * 4) + 1);


    BlitStringToBuffer(Element1Name, &g6x7Font, &COLOR_NES_WHITE, 65, 63 - (12 * 4) + 2);

    if (gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element2 != ELEMENT_NULL && gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element2 != gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element1)
    {
        BlitStringToBuffer(Element2Name, &g6x7Font, &COLOR_NES_WHITE, 65 + 65, 63 - (12 * 4) + 2);
    }

    static PIXEL32 MoveColor = { 0x00, 0x00, 0x00, 0x00 };

    for (uint8_t Moves = 0; Moves < MAX_NONSIGNATURE_MOVES; Moves++)
    {
        switch (gBattleMoves[gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[Moves]].element)
        {
            case ELEMENT_NONE:
            {
                MoveColor = COLOR_NES_TAN;
                break;
            }
            case ELEMENT_EARTH:
            {
                MoveColor = COLOR_NES_BROWN;
                break;
            }
            case ELEMENT_AIR:
            {
                MoveColor = COLOR_NES_SKY_BLUE;
                break;
            }
            case ELEMENT_FIRE:
            {
                MoveColor = COLOR_NES_ORANGE;
                break;
            }
            case ELEMENT_WATER:
            {
                MoveColor = COLOR_NES_BLUE;
                break;
            }
            case ELEMENT_ELECTRIC:
            {
                MoveColor = COLOR_NES_YELLOW;
                break;
            }
            case ELEMENT_METAL:
            {
                MoveColor = COLOR_NES_GRAY;
                break;
            }
            case ELEMENT_SOUL:
            {
                MoveColor = COLOR_NES_PURPLE;
                break;
            }
            case ELEMENT_LIFE:
            {
                MoveColor = COLOR_NES_LIGHT_GREEN;
                break;
            }
            case ELEMENT_DEATH:
            {
                MoveColor = COLOR_NES_BLACK_RED;
                break;
            }
            case ELEMENT_STATUS:
            {
                MoveColor = COLOR_NES_PINK;
                break;
            }
            default:
            {
                MoveColor = COLOR_LIGHT_GRAY;
                break;
            }
        }
        DrawWindow(63, 71 + (12 * (5 + Moves)), 196, 11, &COLOR_BLACK, &MoveColor, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //move box

        BlitStringToBuffer(gBattleMoveNames[gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[Moves]], &g6x7Font, &COLOR_NES_WHITE, 65, 71 + (12 * (5 + Moves)) + 2);    //move text
    }
    
    //TODO: signature move

    //TODO: ability name
    //TODO: ability description     //might need a couple arrays of StatsTextLine[]

    //TODO: monster index name

    //TODO: original owner name



    if (gDrawMoveRelearnScreen == TRUE)
    {
        DrawMoveRelearnScreen();
    }

    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_MonsterStatsScreen(void)
{
    if (gIsMoveSelected == FALSE && gDrawMoveRelearnScreen == FALSE)
    {
        if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
        {
            gCurrentGameState = gPreviousGameState;
            gIsSelectingMoveToSwitchWith = FALSE;
        }

        if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
        {
            if (gMonsterToViewStats > 0)
            {
                gMonsterToViewStats--;
                gIsSelectingMoveToSwitchWith = FALSE;
                gMenu_MonsterStats_Moves.SelectedItem = 0;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMonsterToViewStats = gPlayerPartyCount - 1;
                gIsSelectingMoveToSwitchWith = FALSE;
                gMenu_MonsterStats_Moves.SelectedItem = 0;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
        if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
        {
            if (gMonsterToViewStats < gPlayerPartyCount - 1)
            {
                gMonsterToViewStats++;
                gIsSelectingMoveToSwitchWith = FALSE;
                gMenu_MonsterStats_Moves.SelectedItem = 0;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMonsterToViewStats = 0;
                gIsSelectingMoveToSwitchWith = FALSE;
                gMenu_MonsterStats_Moves.SelectedItem = 0;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
        if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_Moves.SelectedItem > 0)
            {
                gMenu_MonsterStats_Moves.SelectedItem--;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMenu_MonsterStats_Moves.SelectedItem = 3;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
        if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_Moves.SelectedItem < 3)
            {
                gMenu_MonsterStats_Moves.SelectedItem++;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMenu_MonsterStats_Moves.SelectedItem = 0;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            if (gIsSelectingMoveToSwitchWith == FALSE)
            {
                gMI_MonsterStats_MoveItems[gMenu_MonsterStats_Moves.SelectedItem]->Action();
                PlayGameSound(&gSoundMenuChoose);
            }
            else
            {
                uint16_t MoveA = gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMoveToSwitch];
                uint16_t MoveB = gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMenu_MonsterStats_Moves.SelectedItem];

                gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMoveToSwitch] = MoveB;
                gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMenu_MonsterStats_Moves.SelectedItem] = MoveA;

                gIsSelectingMoveToSwitchWith = FALSE;
                PlayGameSound(&gSoundMenuChoose);
            }
        }
    }
    else if (gIsMoveSelected == FALSE && gDrawMoveRelearnScreen == TRUE)
    {

        if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
        {
            gDrawMoveRelearnScreen = FALSE;
            PlayGameSound(&gSoundMenuNavigate);
        }

        if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_LearnableMoves.SelectedItem > 0)
            {
                gMenu_MonsterStats_LearnableMoves.SelectedItem--;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gMenu_MonsterStats_LearnableMoves.SelectedItem == 0 && gLearnableMoveCount - 1 > _countof(gMI_MonsterStats_LearnableItems))
            {
                gLearnableSlotOffSet--;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }

        if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_LearnableMoves.SelectedItem < _countof(gMI_MonsterStats_LearnableItems) && gMenu_MonsterStats_LearnableMoves.SelectedItem < gLearnableMoveCount - 1)
            {
                gMenu_MonsterStats_LearnableMoves.SelectedItem++;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gMenu_MonsterStats_LearnableMoves.SelectedItem == _countof(gMI_MonsterStats_LearnableItems) && gLearnableMoveCount - 1 > _countof(gMI_MonsterStats_LearnableItems))
            {
                gLearnableSlotOffSet--;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            gMI_MonsterStats_LearnableItems[gMenu_MonsterStats_LearnableMoves.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    else            //gIsmoveSelected == TRUE
    {
        if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_MoveOptions.SelectedItem > 0)
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem--;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem = 2;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
        if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_MoveOptions.SelectedItem < 2)
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem++;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem = 0;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }

        if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
        {
            gIsMoveSelected = FALSE;
            PlayGameSound(&gSoundMenuNavigate);
        }

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            gIsMoveSelected = FALSE;
            gMI_MonsterStats_MoveOptionItems[gMenu_MonsterStats_MoveOptions.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    
}


void DrawMoveRelearnScreen(void)
{
    //background window
    DrawWindow(57, 7, 270, 138, &COLOR_BLACK, &COLOR_DARK_WHITE, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE );


    DrawWindow(31, 5 + (12 * 1), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 2), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 3), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 4), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 5), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 6), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 7), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 8), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 9), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    DrawWindow(31, 5 + (12 * 10), 224, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_HORIZ_CENTERED);

    
    //move description
    DrawWindow(42, 5 + (12 * 12), 249, 66, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE );

    //move data (power, element, usage amount, etc.)
    DrawWindow(293, 5 + (12 * 12), 48, 66, &COLOR_BLACK, &COLOR_LIGHT_GRAY, &COLOR_LIGHT_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE );


    ReSortLearnableMovesFromMonster(&gPlayerParty[gMonsterToViewStats].DriveMonster);

    for (uint8_t LearnableBox = 0; LearnableBox < 10; LearnableBox++)
    {
        uint16_t SlotLoop = 0x800A;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:

        SlotOrder = LearnableBox + gLearnableSlotOffSet + SlotLoop;

        if (gLearnableMoveCount != 0)
        {
            SlotOrder %= gLearnableMoveCount;
        }

        SlotIndex = gLearnableMoveSort[SlotOrder];

        if (gLearnableMoveSort[LearnableBox] == 0xFFFF)
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

        gLearnableSlotIndex[LearnableBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_MonsterStats_LearnableItems[LearnableBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            BlitStringToBuffer(gBattleMoveNames[SlotIndex], &g6x7Font, &COLOR_BLACK, gMI_MonsterStats_LearnableItems[LearnableBox]->x, gMI_MonsterStats_LearnableItems[LearnableBox]->y);
        }
    }
    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_MonsterStats_LearnableItems[gMenu_MonsterStats_LearnableMoves.SelectedItem]->x - 7, gMI_MonsterStats_LearnableItems[gMenu_MonsterStats_LearnableMoves.SelectedItem]->y);
}


void MenuItem_MonsterStats_MoveSelected(void)
{
    gIsMoveSelected = TRUE;
}


void MenuItem_MonsterStats_MoveSwitch(void)
{
    gMoveToSwitch = gMenu_MonsterStats_Moves.SelectedItem;
    gIsSelectingMoveToSwitchWith = TRUE;
    gIsMoveSelected = FALSE;
}


void MenuItem_MonsterStats_MoveRemove(void)
{
    //TODO: remove and relearn moves here
    if (gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMenu_MonsterStats_Moves.SelectedItem] == BATTLEMOVE_NULL)
    {
        gDrawMoveRelearnScreen = TRUE;
        gIsMoveSelected = FALSE;
    }
    else
    {
        gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMenu_MonsterStats_Moves.SelectedItem] = BATTLEMOVE_NULL;
    }
}

void MenuItem_MonsterStats_MoveBack(void)
{
    gIsMoveSelected = FALSE;
}

void MenuItem_MonsterStats_LearnableSelected(void)
{
    gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[gMenu_MonsterStats_Moves.SelectedItem] = gLearnableSlotIndex[gMenu_MonsterStats_LearnableMoves.SelectedItem];
    gDrawMoveRelearnScreen = FALSE;
}