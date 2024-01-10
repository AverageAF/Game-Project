#include "Main.h"

#include "ExitYesNoScreen.h"


MENUITEM gMI_ExitYesNo_Yes = { "Yes", (GAME_RES_WIDTH / 2) - (6 * 3 / 2), 100, TRUE, MenuItem_ExitYesNo_Yes };

MENUITEM gMI_ExitYesNo_No = { "No", (GAME_RES_WIDTH / 2) - 6, 140, TRUE, MenuItem_ExitYesNo_No };

MENUITEM* gMI_ExitScreenItems[] = { &gMI_ExitYesNo_Yes, &gMI_ExitYesNo_No };

MENU gMenu_ExitYesNoScreen = { "Quit the game?", 0, _countof(gMI_ExitScreenItems), gMI_ExitScreenItems };

void DrawExitYesNoScreen(void)
{
    static PIXEL32 TextColor = { 0x00, 0x00, 0x00, 0x00 };
    static PIXEL32 Red = { 0x00, 0x00, 0x00, 0x00 };
    static PIXEL32 LimeGreen = { 0x00, 0x00, 0x00, 0x00 };

    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
        gInputEnabled = FALSE;
    }



    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, NULL);
    ApplyFadeIn(LocalFrameCounter, (PIXEL32) { 0x00, 0x00, 0xFF, 0xFF }, &Red, NULL);
    ApplyFadeIn(LocalFrameCounter, (PIXEL32) { 0x00, 0xFF, 0x00, 0xFF }, &LimeGreen, NULL);

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    BlitStringToBuffer(gMenu_ExitYesNoScreen.Name, &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (strlen(gMenu_ExitYesNoScreen.Name) * 6 / 2), 60);                                                             //Quit the game?

    BlitStringToBuffer(gMenu_ExitYesNoScreen.Items[0]->Name, &g6x7Font, &LimeGreen, (GAME_RES_WIDTH / 2) - (strlen(gMenu_ExitYesNoScreen.Items[0]->Name) * 6 / 2), gMenu_ExitYesNoScreen.Items[0]->y);          //yes

    BlitStringToBuffer(gMenu_ExitYesNoScreen.Items[1]->Name, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(gMenu_ExitYesNoScreen.Items[1]->Name) * 6 / 2), gMenu_ExitYesNoScreen.Items[1]->y);                //no

    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->x - 6, gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->y);                     //Cursor


    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}


void PPI_ExitYesNoScreen(void)
{

    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gDesiredGameState = gPreviousGameState;
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = gDesiredGameState;
        PlayGameSound(&gSoundMenuChoose);
    }

    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if ((gMenu_ExitYesNoScreen.SelectedItem == gMenu_ExitYesNoScreen.ItemCount - 1) && (gInputEnabled == TRUE))
        {
            gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_ExitYesNoScreen.SelectedItem = gMenu_ExitYesNoScreen.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {

        if (gMenu_ExitYesNoScreen.SelectedItem < gMenu_ExitYesNoScreen.ItemCount - 1)
        {
            gMenu_ExitYesNoScreen.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_ExitYesNoScreen.SelectedItem > 0)
        {
            gMenu_ExitYesNoScreen.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        if (gMenu_ExitYesNoScreen.SelectedItem == 1)
        {
            gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);

        }
        else
        {
            gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);

        }
    }
}

void MenuItem_ExitYesNo_Yes(void)
{
    SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
}
void MenuItem_ExitYesNo_No(void)
{
    gCurrentGameState = gPreviousGameState;
    gPreviousGameState = GAMESTATE_EXITYESNO;
    gMenu_ExitYesNoScreen.SelectedItem = 0;
}
