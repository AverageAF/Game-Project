#include "Main.h"

#include "OpeningSplashScreen.h"


void DrawOpeningSplashScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    if (LocalFrameCounter >= 60)
    {
        if (LocalFrameCounter == 60)
        {
            TextColor.Red = 255;
            TextColor.Blue = 255;
            TextColor.Green = 255;
            PlayGameSound(&gSoundSplashScreen);
        }

        if ((LocalFrameCounter >= 120 && LocalFrameCounter < 150) && (LocalFrameCounter % 10 == 0))
        {
            TextColor.Red -= 63;
            TextColor.Blue -= 63;
            TextColor.Green -= 63;
        }

        if (LocalFrameCounter == 150)
        {
            TextColor.Red = 0;
            TextColor.Blue = 0;
            TextColor.Green = 0;
        }

        if (LocalFrameCounter > 160)
        {
            gPreviousGameState = gCurrentGameState;
            gCurrentGameState = GAMESTATE_TITLESCREEN;
        }

        BlitStringToBuffer("A game by me :)", &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (6 * 16 / 2), 112);

        BlitStringToBuffer("Matthew Warriner", &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (6 * 17 / 2), 128);
    }



    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}



void PPI_OpeningSplashScreen(void)      //skip splash screen
{
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed ||
        gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed ||
        gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed ||
        gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed ||
        gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed ||
        gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_TITLESCREEN;
    }
}