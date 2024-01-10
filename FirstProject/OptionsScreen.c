#include "Main.h"

#include "OptionsScreen.h"


MENUITEM gMI_OptionsScreen_SfxVolume = { "SFX Volume:", (GAME_RES_WIDTH / 2) - (6 * 12 / 2), 96, TRUE, MenuItem_OptionsScreen_SfxVolume };

MENUITEM gMI_OptionsScreen_MusicVolume = { "Music Volume:", (GAME_RES_WIDTH / 2) - (6 * 14 / 2), 112, TRUE, MenuItem_OptionsScreen_MusicVolume };

MENUITEM gMI_OptionsScreen_ScreenSize = { "ScreenSize:", (GAME_RES_WIDTH / 2) - (6 * 12 / 2), 128, TRUE, MenuItem_OptionsScreen_ScreenSize };

MENUITEM gMI_OptionsScreen_FullScreen = { "Fullscreen:", (GAME_RES_WIDTH / 2) - (6 * 12 / 2), 144, TRUE, MenuItem_OptionsScreen_FullScreen };

MENUITEM gMI_OptionsScreen_Back = { "Back:", (GAME_RES_WIDTH / 2) - (6 * 6 / 2), 160, TRUE, MenuItem_OptionsScreen_Back };

MENUITEM* gMI_OptionsScreenItems[] = { &gMI_OptionsScreen_SfxVolume, &gMI_OptionsScreen_MusicVolume, &gMI_OptionsScreen_ScreenSize, &gMI_OptionsScreen_FullScreen, &gMI_OptionsScreen_Back };

MENU gMenu_OptionsScreen = { "Options", 0, _countof(gMI_OptionsScreenItems), gMI_OptionsScreenItems };



void DrawOptionsScreen(void)
{

    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 Red = { 0x00, 0x00, 0xFF, 0xFF };
    static PIXEL32 Gray = { 0x50, 0x50, 0x50, 0x50 };
    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    char ScreenSizeString[64] = { 0 };

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;

        gMenu_OptionsScreen.SelectedItem = 0;
        gInputEnabled = FALSE;
    }


    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, NULL);

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_GRAY, & Gray, NULL);

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    for (uint8_t MenuItem = 0; MenuItem < gMenu_OptionsScreen.ItemCount; MenuItem++)
    {
        if (gMenu_OptionsScreen.Items[MenuItem]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMenu_OptionsScreen.Items[MenuItem]->Name, &g6x7Font, &TextColor, gMenu_OptionsScreen.Items[MenuItem]->x, gMenu_OptionsScreen.Items[MenuItem]->y);
        }
    }

    for (uint8_t Volume = 0; Volume < 10; Volume++)
    {
        if (Volume >= (gSFXVolume / 10))
        {
            BlitStringToBuffer("\xf2", &g6x7Font, &Gray, 240 + (Volume * 6), gMI_OptionsScreen_SfxVolume.y);
        }
        else
        {
            BlitStringToBuffer("\xf2", &g6x7Font, &TextColor, 240 + (Volume * 6), gMI_OptionsScreen_SfxVolume.y);
        }
    }

    for (uint8_t Volume = 0; Volume < 10; Volume++)
    {
        if (Volume >= (gMusicVolume / 10))
        {
            BlitStringToBuffer("\xf2", &g6x7Font, &Gray, 240 + (Volume * 6), gMI_OptionsScreen_MusicVolume.y);
        }
        else
        {
            BlitStringToBuffer("\xf2", &g6x7Font, &TextColor, 240 + (Volume * 6), gMI_OptionsScreen_MusicVolume.y);
        }
    }

    if (gRegistryParams.FullScreen == TRUE)
    {
        BlitStringToBuffer("x", &g6x7Font, &TextColor, 269, gMI_OptionsScreen_FullScreen.y);
    }

    snprintf(ScreenSizeString, sizeof(ScreenSizeString), "%dx%d", GAME_RES_WIDTH * gGamePerformanceData.CurrentScaleFactor, GAME_RES_HEIGHT * gGamePerformanceData.CurrentScaleFactor);

    BlitStringToBuffer(ScreenSizeString, &g6x7Font, &TextColor, 240, gMI_OptionsScreen_ScreenSize.y);

    BlitStringToBuffer("(Req:Game Exit)", &g6x7Font, &TextColor, gMI_OptionsScreen_ScreenSize.x - (16 * 6) - 12, gMI_OptionsScreen_ScreenSize.y);

    BlitStringToBuffer("(Req:Game Exit)", &g6x7Font, &TextColor, gMI_OptionsScreen_FullScreen.x - (16 * 6) - 12, gMI_OptionsScreen_FullScreen.y);

    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->x - 6, gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_OptionsScreen(void)
{
    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gDesiredGameState = gPreviousGameState;
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = gDesiredGameState;
        PlayGameSound(&gSoundMenuChoose);
        if (SaveRegistryParameters() != ERROR_SUCCESS)
        {
            LogMessageA(LL_ERROR, "[%s] Save Registry Parameters failed from Options>Tabbed!", __FUNCTION__);
        }
    }

    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_OptionsScreen.SelectedItem == gMenu_OptionsScreen.ItemCount - 1)
        {
            gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_OptionsScreen.SelectedItem = gMenu_OptionsScreen.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {

        if (gMenu_OptionsScreen.SelectedItem < gMenu_OptionsScreen.ItemCount - 1)
        {
            gMenu_OptionsScreen.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_OptionsScreen.SelectedItem > 0)
        {
            gMenu_OptionsScreen.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gMenu_OptionsScreen.SelectedItem >= gMenu_OptionsScreen.ItemCount - 1)              ////dont allow for sideways navigation on the 'back' button and 'FullScreen'
    {
        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    else
    {
        if ((gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed) ||
            (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed) ||
            (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed))
        {
            gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
}


void MenuItem_OptionsScreen_SfxVolume(void)
{
    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gSFXVolume < 10)
        {
            gSFXVolume = 100;
        }
        else
        {
            gSFXVolume -= 10;
        }
    }
    else
    {
        if (gSFXVolume >= 100)
        {
            gSFXVolume = 0;
        }
        else
        {
            gSFXVolume += 10;
        }
    }

    float SFXVolume = gSFXVolume / 100.0f;

    for (uint8_t Counter = 0; Counter < NUMBER_OF_SFX_SOURCE_VOICES; Counter++)
    {
        gXAudioSFXSourceVoice[Counter]->lpVtbl->SetVolume(gXAudioSFXSourceVoice[Counter], SFXVolume, XAUDIO2_COMMIT_NOW);
    }
}
void MenuItem_OptionsScreen_MusicVolume(void)
{

    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gMusicVolume < 10)
        {
            gMusicVolume = 100;
        }
        else
        {
            gMusicVolume -= 10;
        }
    }
    else
    {
        if (gMusicVolume >= 100)
        {
            gMusicVolume = 0;
        }
        else
        {
            gMusicVolume += 10;
        }
    }

    float MusicVolume = gMusicVolume / 100.0f;

    gXAudioMusicSourceVoice->lpVtbl->SetVolume(gXAudioMusicSourceVoice, MusicVolume, XAUDIO2_COMMIT_NOW);
}
void MenuItem_OptionsScreen_ScreenSize(void)
{
    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gGamePerformanceData.CurrentScaleFactor > 1)
        {
            gGamePerformanceData.CurrentScaleFactor--;
        }
        else
        {
            gGamePerformanceData.CurrentScaleFactor = gGamePerformanceData.MaxScaleFactor;
        }
    }
    else
    {
        if (gGamePerformanceData.CurrentScaleFactor < gGamePerformanceData.MaxScaleFactor)
        {
            gGamePerformanceData.CurrentScaleFactor++;
        }
        else
        {
            gGamePerformanceData.CurrentScaleFactor = 1;
        }
    }

    InvalidateRect(gGameWindow, NULL, TRUE);
}

void MenuItem_OptionsScreen_FullScreen(void)
{
    if (gRegistryParams.FullScreen == FALSE)
    {
        gRegistryParams.FullScreen++;
    }
    else
    {
        gRegistryParams.FullScreen = FALSE;
    }
}

void MenuItem_OptionsScreen_Back(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_TITLESCREEN;

    if (SaveRegistryParameters() != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Save Registry Parameters failed from Options>Back!", __FUNCTION__);
    }
}