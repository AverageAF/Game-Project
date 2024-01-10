#include "Main.h"
#include "LoadGameSave.h"

MENUITEM gMI_LoadGameSave_Back = { "Back",(GAME_RES_WIDTH / 2) - (6 * 5 / 2) - 150, (GAME_RES_HEIGHT - 14), TRUE, MenuItem_LoadGameSave_Back };

MENUITEM gMI_LoadGameSave_SaveSlot1 = { "Slot1",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (96), TRUE, MenuItem_LoadGameSave_Slot1 };

MENUITEM gMI_LoadGameSave_SaveSlot2 = { "Slot2",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (128), TRUE, MenuItem_LoadGameSave_Slot2 };

MENUITEM gMI_LoadGameSave_SaveSlot3 = { "Slot3",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (160), TRUE, MenuItem_LoadGameSave_Slot3 };

MENUITEM gMI_LoadGameSave_DeleteSaveSlot1 = { "[Delete 1]",(GAME_RES_WIDTH / 2) - (6 * 6 / 2) + 128, (96), TRUE, MenuItem_LoadGameSave_DeleteSlot1 };

MENUITEM gMI_LoadGameSave_DeleteSaveSlot2 = { "[Delete 2]",(GAME_RES_WIDTH / 2) - (6 * 6 / 2) + 128, (128), TRUE, MenuItem_LoadGameSave_DeleteSlot2 };

MENUITEM gMI_LoadGameSave_DeleteSaveSlot3 = { "[Delete 3]",(GAME_RES_WIDTH / 2) - (6 * 6 / 2) + 128, (160), TRUE, MenuItem_LoadGameSave_DeleteSlot3 };

MENUITEM* gMI_LoadGameSaveItems[] = { &gMI_LoadGameSave_Back, &gMI_LoadGameSave_SaveSlot1, &gMI_LoadGameSave_SaveSlot2, &gMI_LoadGameSave_SaveSlot3, &gMI_LoadGameSave_DeleteSaveSlot1, &gMI_LoadGameSave_DeleteSaveSlot2, &gMI_LoadGameSave_DeleteSaveSlot3 };

MENU gMenu_LoadGameSave = { "Load Save Slot", 1, _countof(gMI_LoadGameSaveItems), gMI_LoadGameSaveItems };

void DrawLoadGameSave(void)
{
    static PIXEL32 White = { 0x00, 0x00, 0x00, 0x00 };
    static PIXEL32 Red = { 0x00, 0x00, 0x00, 0x00 };
    //static PIXEL32 LimeGreen = { 0x00, 0x00, 0x00, 0x00 };

    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
        gInputEnabled = FALSE;
        gMenu_LoadGameSave.SelectedItem = 1;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));
    //memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

    if (LocalFrameCounter <= 4)
    {
        White.Colors.Red = 64;
        White.Colors.Blue = 64;
        White.Colors.Green = 64;

        Red.Colors.Red = 64;
    }
    if (LocalFrameCounter == 8)
    {
        White.Colors.Red = 128;
        White.Colors.Blue = 128;
        White.Colors.Green = 128;

        Red.Colors.Red = 128;
    }
    if (LocalFrameCounter == 12)
    {
        White.Colors.Red = 192;
        White.Colors.Blue = 192;
        White.Colors.Green = 192;

        Red.Colors.Red = 192;
    }
    if (LocalFrameCounter == 16)
    {
        White.Colors.Red = 255;
        White.Colors.Blue = 255;
        White.Colors.Green = 255;

        Red.Colors.Red = 255;

        gInputEnabled = TRUE;
    }


    BlitStringToBuffer(gMenu_LoadGameSave.Name, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(gMenu_LoadGameSave.Name) * 6 / 2), 60);

    for (uint8_t MenuItem = 0; MenuItem < gMenu_LoadGameSave.ItemCount; MenuItem++)
    {
        if (gMenu_LoadGameSave.Items[MenuItem]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMenu_LoadGameSave.Items[MenuItem]->Name, &g6x7Font, &White, gMenu_LoadGameSave.Items[MenuItem]->x, gMenu_LoadGameSave.Items[MenuItem]->y);
        }
    }
    BlitStringToBuffer("»", &g6x7Font, &White, gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->x - 6, gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_LoadGameSave(void)
{
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_LoadGameSave.SelectedItem != 0)
        {
            gMenu_LoadGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
	{
		gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->Action();
		PlayGameSound(&gSoundMenuChoose);
	}
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_LoadGameSave.SelectedItem == 0)
        {
            gMenu_LoadGameSave.SelectedItem = 3;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_LoadGameSave.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        if (gMenu_LoadGameSave.SelectedItem >= 3)
        {
            gMenu_LoadGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_LoadGameSave.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if ((gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed) || (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed))
    {
        if (gMenu_LoadGameSave.SelectedItem == 1 || gMenu_LoadGameSave.SelectedItem == 2 || gMenu_LoadGameSave.SelectedItem == 3)
        {
            gMenu_LoadGameSave.SelectedItem += 3;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_LoadGameSave.SelectedItem == 4 || gMenu_LoadGameSave.SelectedItem == 5 || gMenu_LoadGameSave.SelectedItem == 6)
        {
            gMenu_LoadGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
}

void MenuItem_LoadGameSave_Back(void)
{
	gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_TITLESCREEN;
}

void MenuItem_LoadGameSave_Slot1(void)
{

}


void MenuItem_LoadGameSave_Slot2(void)
{

}


void MenuItem_LoadGameSave_Slot3(void)
{

}

void MenuItem_LoadGameSave_DeleteSlot1(void)
{

}

void MenuItem_LoadGameSave_DeleteSlot2(void)
{

}

void MenuItem_LoadGameSave_DeleteSlot3(void)
{

}