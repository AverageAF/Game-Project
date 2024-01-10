
#include "Main.h"
#include "BattleScreen.h"

void DrawBattleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    GAMEBITMAP* BattleScene = NULL;

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
    }


    //////TODO: Add battle intro and battle music/////////////
    //if (LocalFrameCounter == 0)
    //{
    //    StopMusic();
    //    PlayGameMusic(&MusicBattleInto01, FALSE, TRUE);
    //    PlayGameMusic(&MusicBattle01, TRUE, FALSE);       ////queue full loop behind intro
    //}


    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawWindow(1, 1, 256, 128, &COLOR_NES_WHITE, NULL, &COLOR_NES_GRAY, WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);

    switch (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][gPlayer.WorldPos.x / 16])
    {
        case TILE_GRASS_01:
        {
            BattleScene = &gBattleScreen_Grass01;
            break;
        }
        case TILE_STONE_BRICKS_01:
        {
            BattleScene = &gBattleScreen_StoneBricks01;
            break;
        }
        default:
        {
            ASSERT(FALSE, "Player encountered a monster on an unknown tile!")
        }
    }

    if (BattleScene != 0)
    {
        Blit32BppBitmapToBuffer(BattleScene, (65), (57), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "BattleScene is NULL!");
    }

    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}


void PPI_BattleScreen(void)
{
	if (gGameInput.EscapeKeyPressed && gGameInput.EscapeKeyAlreadyPressed)
	{
		gPreviousGameState = gCurrentGameState;
		gCurrentGameState = GAMESTATE_OVERWORLD;
		gInputEnabled = FALSE;
        StopGameMusic();
	}
}
