
#include "Main.h"

#include "MonsterStatsScreen.h"

uint8_t gMonsterToViewStats;

void DrawMonsterStatsScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    char StatsTextLine[14][40];                 //6 stats + maxhp + ability + 4 moves + name + sig move = 14

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
    }

    __stosd(gBackBuffer.Memory, 0xFF008888, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);


    DrawWindow(63, 63 - 12, 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //lvl name

    DrawWindow(63, 63, 66, 66, &COLOR_BLACK, &COLOR_FORREST_GREEN, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //sprite


    DrawWindow(159, 63, 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //hp

    DrawWindow(159, 63 + (12 * 1), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //atk

    DrawWindow(159, 63 + (12 * 2), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //def

    DrawWindow(159, 63 + (12 * 3), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //spd

    DrawWindow(159, 63 + (12 * 4), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //psi

    DrawWindow(159, 63 + (12 * 5), 100, 11, &COLOR_BLACK, &COLOR_LIGHT_GRAY, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);   //res


    Blit32BppBitmapToBuffer(&gBattleSpriteFront[gPlayerParty[gMonsterToViewStats].DriveMonster.Index], 64, 64, BrightnessAdjustment);

    int health = (int*)gPlayerParty[gMonsterToViewStats].Health;
    int maxHealth = (int*)gPlayerParty[gMonsterToViewStats].MaxHealth;

    sprintf_s((char*)StatsTextLine[0], sizeof(StatsTextLine[0]), "Health: %d / %d", health, maxHealth);

    if ((health * 100 / maxHealth) > 50)
    {
        BlitStringToBuffer(StatsTextLine[0], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + 1);
    }
    else if ((health * 100 / maxHealth) <= 50 && (health * 100 / maxHealth) > 10)
    {
        BlitStringToBuffer(StatsTextLine[0], &g6x7Font, &COLOR_GOLD, 161, 64+ 1);
    }
    else if ((health * 100 / maxHealth) <= 10)
    {
        BlitStringToBuffer(StatsTextLine[0], &g6x7Font, &COLOR_DARK_RED, 161, 64 + 1);
    }

    int attack = (int*)gPlayerParty[gMonsterToViewStats].Attack;

    sprintf_s((char*)StatsTextLine[1], sizeof(StatsTextLine[1]), "Attack: %d", attack);

    BlitStringToBuffer(StatsTextLine[1], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 1) + 1);

    int defense = (int*)gPlayerParty[gMonsterToViewStats].Defense;

    sprintf_s((char*)StatsTextLine[2], sizeof(StatsTextLine[2]), "Defense: %d", defense);

    BlitStringToBuffer(StatsTextLine[2], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 2) + 1);

    int speed = (int*)gPlayerParty[gMonsterToViewStats].Speed;

    sprintf_s((char*)StatsTextLine[3], sizeof(StatsTextLine[3]), "Speed: %d", speed);

    BlitStringToBuffer(StatsTextLine[3], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 3) + 1);

    int psi = (int*)gPlayerParty[gMonsterToViewStats].Psi;

    sprintf_s((char*)StatsTextLine[4], sizeof(StatsTextLine[4]), "Psi: %d", psi);

    BlitStringToBuffer(StatsTextLine[4], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 4) + 1);

    int resolve = (int*)gPlayerParty[gMonsterToViewStats].Resolve;

    sprintf_s((char*)StatsTextLine[5], sizeof(StatsTextLine[5]), "Res: %d", resolve);

    BlitStringToBuffer(StatsTextLine[5], &g6x7Font, &COLOR_LIGHT_GREEN, 161, 64 + (12 * 5) + 1);

    int level = (int*)gPlayerParty[gMonsterToViewStats].Level;

    sprintf_s((char*)StatsTextLine[6], sizeof(StatsTextLine[6]), "Lvl: %d %s", level, gPlayerParty[gMonsterToViewStats].DriveMonster.nickname);

    BlitStringToBuffer(StatsTextLine[6], &g6x7Font, &COLOR_NES_RED, 65, 63 - 10);

    //TODO: element 1
    //TODO: element 2

    //TODO: moves 1-4
    
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
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        gCurrentGameState = gPreviousGameState;
    }

    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gMonsterToViewStats > 0)
        {
            gMonsterToViewStats--;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMonsterToViewStats = gPlayerPartyCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
    {
        if (gMonsterToViewStats < gPlayerPartyCount - 1)
        {
            gMonsterToViewStats++;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMonsterToViewStats = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
}