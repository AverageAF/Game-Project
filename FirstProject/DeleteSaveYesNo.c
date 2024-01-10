#include "Main.h"

#include "DeleteSaveYesNo.h"


MENUITEM gMI_DeleteSaveYesNo_Yes = { "Yes", (GAME_RES_WIDTH / 2) - (6 * 3 / 2), 100, TRUE, MenuItem_DeleteSaveYesNo_Yes };

MENUITEM gMI_DeleteSaveYesNo_No = { "No", (GAME_RES_WIDTH / 2) - 6, 140, TRUE, MenuItem_DeleteSaveYesNo_No };

MENUITEM* gMI_DeleteSaveScreenItems[] = { &gMI_DeleteSaveYesNo_Yes, &gMI_DeleteSaveYesNo_No };

MENU gMenu_DeleteSaveYesNoScreen = { "Delete selected Save Slot?", 1, _countof(gMI_DeleteSaveScreenItems), gMI_DeleteSaveScreenItems };

void DrawDeleteSaveYesNoScreen(void)
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
    ApplyFadeIn(LocalFrameCounter, (PIXEL32) { 0x00, 0x00, 0xFF, 0xFF }, & Red, NULL);
    ApplyFadeIn(LocalFrameCounter, (PIXEL32) { 0x00, 0xFF, 0x00, 0xFF }, & LimeGreen, NULL);

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    BlitStringToBuffer(gMenu_DeleteSaveYesNoScreen.Name, &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (strlen(gMenu_DeleteSaveYesNoScreen.Name) * 6 / 2), 60);                                                             //Quit the game?

    BlitStringToBuffer(gMenu_DeleteSaveYesNoScreen.Items[0]->Name, &g6x7Font, &LimeGreen, (GAME_RES_WIDTH / 2) - (strlen(gMenu_DeleteSaveYesNoScreen.Items[0]->Name) * 6 / 2), gMenu_DeleteSaveYesNoScreen.Items[0]->y);          //yes

    BlitStringToBuffer(gMenu_DeleteSaveYesNoScreen.Items[1]->Name, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(gMenu_DeleteSaveYesNoScreen.Items[1]->Name) * 6 / 2), gMenu_DeleteSaveYesNoScreen.Items[1]->y);                //no

    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_DeleteSaveYesNoScreen.Items[gMenu_DeleteSaveYesNoScreen.SelectedItem]->x - 6, gMenu_DeleteSaveYesNoScreen.Items[gMenu_DeleteSaveYesNoScreen.SelectedItem]->y);                     //Cursor


    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}


void PPI_DeleteSaveYesNoScreen(void)
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
        if ((gMenu_DeleteSaveYesNoScreen.SelectedItem == gMenu_DeleteSaveYesNoScreen.ItemCount - 1) && (gInputEnabled == TRUE))
        {
            gMenu_DeleteSaveYesNoScreen.Items[gMenu_DeleteSaveYesNoScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_DeleteSaveYesNoScreen.SelectedItem = gMenu_DeleteSaveYesNoScreen.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {

        if (gMenu_DeleteSaveYesNoScreen.SelectedItem < gMenu_DeleteSaveYesNoScreen.ItemCount - 1)
        {
            gMenu_DeleteSaveYesNoScreen.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_DeleteSaveYesNoScreen.SelectedItem > 0)
        {
            gMenu_DeleteSaveYesNoScreen.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        gMenu_DeleteSaveYesNoScreen.Items[gMenu_DeleteSaveYesNoScreen.SelectedItem]->Action();
        PlayGameSound(&gSoundMenuChoose);
    }
}

void MenuItem_DeleteSaveYesNo_Yes(void)
{
    if (gPreviousGameState == GAMESTATE_LOADGAMESAVE)
    {
        //TODO  delete selected saved game file
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_LOADGAMESAVE;
    }
    else if (gPreviousGameState == GAMESTATE_SAVEGAMESAVE)
    {
        ////TODO    delete/overwrite previously saved game file and replace it with current player information
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_SAVEGAMESAVE;
    }
    else
    {
        ASSERT(FALSE, "unknown gPreviousGameState when deleting save in DeleteSaveYesNo.c")
    }
}
void MenuItem_DeleteSaveYesNo_No(void)
{
    if (gPreviousGameState == GAMESTATE_LOADGAMESAVE)
    {
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_LOADGAMESAVE;
    }
    else if (gPreviousGameState == GAMESTATE_SAVEGAMESAVE)
    {
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_SAVEGAMESAVE;
    }
    else
    {
        ASSERT(FALSE, "unknown gPreviousGameState when deleting save in DeleteSaveYesNo.c")
    }
}
