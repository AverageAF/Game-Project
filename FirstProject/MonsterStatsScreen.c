
#include "Main.h"

#include "MonsterStatsScreen.h"


MENUITEM gMI_MonsterStats_Move0 = { "Move0", 65, 71 + (12 * 5) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM gMI_MonsterStats_Move1 = { "Move1", 65, 71 + (12 * 6) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM gMI_MonsterStats_Move2 = { "Move2", 65, 71 + (12 * 7) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM gMI_MonsterStats_Move3 = { "Move3", 65, 71 + (12 * 8) + 2, TRUE, MenuItem_MonsterStats_MoveSelected };

MENUITEM* gMI_MonsterStats_MoveItems[] = { &gMI_MonsterStats_Move0, &gMI_MonsterStats_Move1, &gMI_MonsterStats_Move2, &gMI_MonsterStats_Move3 };

MENU gMenu_MonsterStats_Moves = { "MonsterStats Move Menu", 0, _countof(gMI_MonsterStats_MoveItems), gMI_MonsterStats_MoveItems };


MENUITEM gMI_MonsterStats_Switch = { "Switch", 11, 65 + (12 * 5) + 1, TRUE, MenuItem_MonsterStats_MoveSwitch };

MENUITEM gMI_MonsterStats_Remove = { "Remove", 11, 65 + (12 * 6) + 3, TRUE, MenuItem_MonsterStats_MoveRemove };

MENUITEM gMI_MonsterStats_Back = { "Back", 11, 65 + (12 * 7) + 5, TRUE, MenuItem_MonsterStats_MoveBack };

MENUITEM* gMI_MonsterStats_MoveOptionItems[] = { &gMI_MonsterStats_Switch, &gMI_MonsterStats_Remove, &gMI_MonsterStats_Back };

MENU gMenu_MonsterStats_MoveOptions = { "Move Options", 0, _countof(gMI_MonsterStats_MoveOptionItems), gMI_MonsterStats_MoveOptionItems };


uint8_t gMonsterToViewStats;

BOOL gIsMoveSelected = FALSE;

BOOL gIsSelectingMoveToSwitchWith = FALSE;

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
    

    __stosd(gBackBuffer.Memory, 0xFF008888, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

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


    DrawWindow(63, 71 + (12 * 5), 196, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //move 0

    DrawWindow(63, 71 + (12 * 6), 196, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //move 1

    DrawWindow(63, 71 + (12 * 7), 196, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //move 2

    DrawWindow(63, 71 + (12 * 8), 196, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //move 3

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
        BlitStringToBuffer(StatsTextLine[1], &g6x7Font, &COLOR_LIGHT_GREEN, 65, 63 - (12 * 2) + 2);       //high hp
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

    BlitStringToBuffer(StatsTextLine[2], &g6x7Font, &COLOR_NEON_BLUE, 65, 63 - (12 * 1) + 2);


    int attack = (int*)gPlayerParty[gMonsterToViewStats].Attack;

    sprintf_s((char*)StatsTextLine[3], sizeof(StatsTextLine[3]), "Attack: %d", attack);

    BlitStringToBuffer(StatsTextLine[3], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 0) + 1);


    int defense = (int*)gPlayerParty[gMonsterToViewStats].Defense;

    sprintf_s((char*)StatsTextLine[4], sizeof(StatsTextLine[4]), "Defense: %d", defense);

    BlitStringToBuffer(StatsTextLine[4], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 1) + 1);


    int speed = (int*)gPlayerParty[gMonsterToViewStats].Speed;

    sprintf_s((char*)StatsTextLine[5], sizeof(StatsTextLine[5]), "Speed: %d", speed);

    BlitStringToBuffer(StatsTextLine[5], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 2) + 1);


    int psi = (int*)gPlayerParty[gMonsterToViewStats].Psi;

    sprintf_s((char*)StatsTextLine[6], sizeof(StatsTextLine[6]), "Psi: %d", psi);

    BlitStringToBuffer(StatsTextLine[6], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 3) + 1);


    int resolve = (int*)gPlayerParty[gMonsterToViewStats].Resolve;

    sprintf_s((char*)StatsTextLine[7], sizeof(StatsTextLine[7]), "Res: %d", resolve);

    BlitStringToBuffer(StatsTextLine[7], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 4) + 1);


    BlitStringToBuffer(Element1Name, &g6x7Font, &COLOR_NES_WHITE, 65, 63 - (12 * 4) + 2);

    if (gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element2 != ELEMENT_NULL && gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element2 != gBaseStats[gPlayerParty[gMonsterToViewStats].DriveMonster.Index].element1)
    {
        BlitStringToBuffer(Element2Name, &g6x7Font, &COLOR_NES_WHITE, 65 + 65, 63 - (12 * 4) + 2);
    }


    BlitStringToBuffer(gBattleMoveNames[gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[0]], &g6x7Font, &COLOR_NES_WHITE, 65, 71 + (12 * 5) + 2);

    BlitStringToBuffer(gBattleMoveNames[gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[1]], &g6x7Font, &COLOR_NES_WHITE, 65, 71 + (12 * 6) + 2);

    BlitStringToBuffer(gBattleMoveNames[gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[2]], &g6x7Font, &COLOR_NES_WHITE, 65, 71 + (12 * 7) + 2);

    BlitStringToBuffer(gBattleMoveNames[gPlayerParty[gMonsterToViewStats].DriveMonster.Moves[3]], &g6x7Font, &COLOR_NES_WHITE, 65, 71 + (12 * 8) + 2);
    
    //TODO: signature move

    //TODO: ability name
    //TODO: ability description     //might need a couple arrays of StatsTextLine[]

    //TODO: monster index name

    //TODO: original owner name



    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_MonsterStatsScreen(void)
{
    if (gIsMoveSelected == FALSE)
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
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMonsterToViewStats = gPlayerPartyCount - 1;
                gIsSelectingMoveToSwitchWith = FALSE;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
        if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
        {
            if (gMonsterToViewStats < gPlayerPartyCount - 1)
            {
                gMonsterToViewStats++;
                gIsSelectingMoveToSwitchWith = FALSE;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else
            {
                gMonsterToViewStats = 0;
                gIsSelectingMoveToSwitchWith = FALSE;
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
    else            //gIsmoveSelected == TRUE
    {
        if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_MoveOptions.SelectedItem > 0)
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem--;
            }
            else
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem = 2;
            }
        }
        if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
        {
            if (gMenu_MonsterStats_MoveOptions.SelectedItem < 2)
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem++;
            }
            else
            {
                gMenu_MonsterStats_MoveOptions.SelectedItem = 0;
            }
        }

        if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
        {
            gIsMoveSelected = FALSE;
        }

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            gMI_MonsterStats_MoveOptionItems[gMenu_MonsterStats_MoveOptions.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    
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
}


void MenuItem_MonsterStats_MoveBack(void)
{
    gIsMoveSelected = FALSE;
}