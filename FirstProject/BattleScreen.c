
#include "Main.h"
#include "BattleScreen.h"

void DrawBattleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    GAMEBITMAP* BattleScene = NULL;

    GAMEBITMAP* PlayerMonsterSprite = NULL;

    GAMEBITMAP* OpponentMonsterSprite = NULL;

    uint8_t Opponent = 0;

    for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
    {
        if ((gCharacterSprite[Index].InteractedWith == TRUE) && (gCharacterSprite[Index].WantsToBattle == TRUE))
        {
            Opponent = Index;
            break;
        }
    }

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
    }


    //////TODO: Add battle intro and battle music/////////////
    //if (LocalFrameCounter == 0)
    //{
    //    StopGameMusic();
    //    /*PlayGameMusic(&MusicBattleInto01, FALSE, TRUE);
    //    PlayGameMusic(&MusicBattle01, TRUE, FALSE);*/       ////queue full loop behind intro
    //}

    for (uint8_t Counter = 0; Counter < TOTAL_MONSTERS; Counter++)
    {
        if (gPlayer.Party[0] == Counter)
        {
            PlayerMonsterSprite = &gBattleSpriteBack[Counter];
        }

        if (gCharacterSprite[Opponent].Party[0] == Counter)
        {
            //////////TODO obviously make front facing sprites
            OpponentMonsterSprite = &gBattleSpriteFront[Counter];
        }
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawWindow(1, 1, 256, 128, &COLOR_NES_WHITE, NULL, &COLOR_NES_GRAY, WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);

    switch (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][gPlayer.WorldPos.x / 16])
    {
        case TILE_GRASS_01:
        {
            /////////TODO make battlescene floor higher for sprites to stand on
            BattleScene = &gBattleScreen_Grass01;
            break;
        }
        case TILE_TALLGRASS_01:
        {
            BattleScene = &gBattleScreen_Grass01;
            break;
        }
        case TILE_STONEBRICKS_FLOOR_01:
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

    if (PlayerMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(PlayerMonsterSprite, (65), (119), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Battle monster is NULL!");
    }

    if (OpponentMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(OpponentMonsterSprite, (255), (87), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Battle monster is NULL!");
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

        for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
        {
            if ((gCharacterSprite[Index].InteractedWith == TRUE) && (gCharacterSprite[Index].WantsToBattle == TRUE))
            {
                gCharacterSprite[Index].InteractedWith = FALSE;
                gCharacterSprite[Index].WantsToBattle = FALSE;
                gCharacterSprite[Index].SightRange = 0;
                break;
            }
        }
	}
}
