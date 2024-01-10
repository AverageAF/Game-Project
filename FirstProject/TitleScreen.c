#include "Main.h"

#include "TitleScreen.h"


MENUITEM gMI_StartNewGame = { "Start New Game", (GAME_RES_WIDTH / 2) - (6 * 14 / 2), 112, TRUE, MenuItem_TitleScreen_StartNew };

MENUITEM gMI_ResumeGame = { "Resume", (GAME_RES_WIDTH / 2) - (6 * 6 / 2), 112, FALSE, MenuItem_TitleScreen_Resume };

MENUITEM gMI_LoadSavedGame = { "Load Saved Game", (GAME_RES_WIDTH / 2) - (6 * 16 / 2), 128, TRUE, MenuItem_TitleScreen_LoadSave };

MENUITEM gMI_Options = { "Options", (GAME_RES_WIDTH / 2) - (6 * 7 / 2), 144, TRUE, MenuItem_TitleScreen_Options };

MENUITEM gMI_Exit = { "Exit", (GAME_RES_WIDTH / 2) - (6 * 4 / 2), 160, TRUE, MenuItem_TitleScreen_Exit };

MENUITEM* gMI_TitleScreenItems[] = { &gMI_StartNewGame, &gMI_ResumeGame, &gMI_LoadSavedGame, &gMI_Options, &gMI_Exit };

MENU gMenu_TitleScreen = { "Title Screen Menu", 1, _countof(gMI_TitleScreenItems), gMI_TitleScreenItems };




void DrawTitleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 Red = { 0x00, 0x00, 0xFF, 0xFF };

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;

        if (gPlayer.Active)
        {
            gMenu_TitleScreen.SelectedItem = 1;
            gMI_ResumeGame.Enabled = TRUE;              ////TODO: dont change title screen here, change once save game exists and likewise restore if saves are deleted
            gMI_StartNewGame.Enabled = FALSE;
        }
        else
        {
            gMenu_TitleScreen.SelectedItem = 0;
        }

        gInputEnabled = FALSE;
    }

    if (LocalFrameCounter <= 5)
    {
        TextColor.Colors.Red = 64;
        TextColor.Colors.Blue = 64;
        TextColor.Colors.Green = 64;
    }
    if (LocalFrameCounter == 10)
    {
        TextColor.Colors.Red = 128;
        TextColor.Colors.Blue = 128;
        TextColor.Colors.Green = 128;
    }
    if (LocalFrameCounter == 15)
    {
        TextColor.Colors.Red = 192;
        TextColor.Colors.Blue = 192;
        TextColor.Colors.Green = 192;
    }
    if (LocalFrameCounter == 20)
    {
        TextColor.Colors.Red = 255;
        TextColor.Colors.Blue = 255;
        TextColor.Colors.Green = 255;
        gInputEnabled = TRUE;
    }

    //memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);
                            //    AARRGGBB
    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    BlitStringToBuffer(GAME_NAME, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(GAME_NAME) * 6 / 2), 60);

    for (uint8_t MenuItem = 0; MenuItem < gMenu_TitleScreen.ItemCount; MenuItem++)
    {
        if (gMenu_TitleScreen.Items[MenuItem]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMenu_TitleScreen.Items[MenuItem]->Name, &g6x7Font, &TextColor, gMenu_TitleScreen.Items[MenuItem]->x, gMenu_TitleScreen.Items[MenuItem]->y);
        }
    }
    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->x - 6, gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}



void PPI_TitleScreen(void)
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
        if (gMenu_TitleScreen.SelectedItem == gMenu_TitleScreen.ItemCount - 1)
        {
            gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_TitleScreen.SelectedItem = gMenu_TitleScreen.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        if (gPlayer.Active)
        {
            if (gMenu_TitleScreen.SelectedItem < gMenu_TitleScreen.ItemCount - 1)
            {
                gMenu_TitleScreen.SelectedItem++;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
        else
        {
            if (gMenu_TitleScreen.SelectedItem == 0)    //jump over "resume" when so save game
            {
                gMenu_TitleScreen.SelectedItem += 2;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if ((gMenu_TitleScreen.SelectedItem >= 2) && (gMenu_TitleScreen.SelectedItem < gMenu_TitleScreen.ItemCount - 1))
            {
                gMenu_TitleScreen.SelectedItem++;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gPlayer.Active)                                //allow navigation to "resume" when save file present
        {
            if (gMenu_TitleScreen.SelectedItem > 1)
            {
                gMenu_TitleScreen.SelectedItem --;
                PlayGameSound(&gSoundMenuNavigate);
            }
            
        }
        else                                                //prevent navigation to "Resume" with no save file
        {
            if (gMenu_TitleScreen.SelectedItem == 2)    //jump over "resume" when so save game
            {
                gMenu_TitleScreen.SelectedItem -= 2;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gMenu_TitleScreen.SelectedItem > 2)
            {
                gMenu_TitleScreen.SelectedItem--;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
    }

    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Action();
        PlayGameSound(&gSoundMenuChoose);
    }
}


void MenuItem_TitleScreen_Resume(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_OVERWORLD;
}
void MenuItem_TitleScreen_StartNew(void)
{
    //if a save exists ask "are you sure you want to start a new game?"
    //otherwise just go to the character naming screen

    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_CHARACTERNAME;
}
void MenuItem_TitleScreen_Options(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_OPTIONS;
}
void MenuItem_TitleScreen_Exit(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_EXITYESNO;
}


void MenuItem_TitleScreen_LoadSave(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_LOADGAMESAVE;
}