
#include "Main.h"
#include "BattleScreen.h"

void DrawBattleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
    }

    if (LocalFrameCounter == 0)
    {
        //StopMusic();
        //PlayGameMusic(&MusicBattleInto01, FALSE, TRUE);
        //PlayGameMusic(&MusicBattle01, TRUE, FALSE);       ////queue full loop behind intro
    }

    if (LocalFrameCounter == 5)
    {
        TextColor.Colors.Red = 64;
        TextColor.Colors.Blue = 64;
        TextColor.Colors.Green = 64;
        BrightnessAdjustment = -128;

    }
    if (LocalFrameCounter == 10)
    {
        TextColor.Colors.Red = 128;
        TextColor.Colors.Blue = 128;
        TextColor.Colors.Green = 128;
        BrightnessAdjustment = -64;
    }
    if (LocalFrameCounter == 15)
    {
        TextColor.Colors.Red = 192;
        TextColor.Colors.Blue = 192;
        TextColor.Colors.Green = 192;
        BrightnessAdjustment = -32;
    }
    if (LocalFrameCounter == 20)
    {
        TextColor.Colors.Red = 255;
        TextColor.Colors.Blue = 255;
        TextColor.Colors.Green = 255;
        BrightnessAdjustment = 0;
        gInputEnabled = TRUE;
    }
	//__stosd(gBackBuffer.Memory, 0xFF00FF00, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));


    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

	DrawWindow(1, 1, 256, 128, (PIXEL32){ 0xFF000000}, WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_VERT_CENTERED); 


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
