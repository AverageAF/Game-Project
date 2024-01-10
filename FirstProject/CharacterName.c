#include "Main.h"

#include "CharacterName.h"


/////////		upper case letters 

MENUITEM gMI_CharacterName_A = { "A", 36, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_B = { "B", 48, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_C = { "C", 60, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_D = { "D", 72, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_E = { "E", 84, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_F = { "F", 96, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_G = { "G", 108, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_H = { "H", 120, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_I = { "I", 132, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_J = { "J", 144, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_K = { "K", 156, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_L = { "L", 168, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_M = { "M", 180, 112, TRUE, MenuItem_CharacterName_Add };



MENUITEM gMI_CharacterName_N = { "N", 192, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_O = { "O", 204, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_P = { "P", 216, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_Q = { "Q", 228, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_R = { "R", 240, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_S = { "S", 252, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_T = { "T", 264, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_U = { "U", 276, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_V = { "V", 288, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_W = { "W", 300, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_X = { "X", 312, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_Y = { "Y", 324, 112, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_Z = { "Z", 336, 112, TRUE, MenuItem_CharacterName_Add };

///////////////		lower case


MENUITEM gMI_CharacterName_a = { "a", 36, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_b = { "b", 48, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_c = { "c", 60, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_d = { "d", 72, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_e = { "e", 84, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_f = { "f", 96, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_g = { "g", 108, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_h = { "h", 120, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_i = { "i", 132, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_j = { "j", 144, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_k = { "k", 156, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_l = { "l", 168, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_m = { "m", 180, 144, TRUE, MenuItem_CharacterName_Add };


MENUITEM gMI_CharacterName_n = { "n", 192, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_o = { "o", 204, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_p = { "p", 216, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_q = { "q", 228, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_r = { "r", 240, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_s = { "s", 252, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_t = { "t", 264, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_u = { "u", 276, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_v = { "v", 288, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_w = { "w", 300, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_x = { "x", 312, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_y = { "y", 324, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_z = { "z", 336, 144, TRUE, MenuItem_CharacterName_Add };

MENUITEM gMI_CharacterName_Back = { "Back", 36, 192, TRUE, MenuItem_CharacterName_Back };

MENUITEM gMI_CharacterName_OK = { "OK", 336, 192, TRUE, MenuItem_CharacterName_Okay };

MENUITEM* gMI_CharacterNameItems[] = {
	&gMI_CharacterName_A, &gMI_CharacterName_B, &gMI_CharacterName_C, &gMI_CharacterName_D, &gMI_CharacterName_E, &gMI_CharacterName_F,
	&gMI_CharacterName_G, &gMI_CharacterName_H, &gMI_CharacterName_I, &gMI_CharacterName_J, &gMI_CharacterName_K, &gMI_CharacterName_L,
	&gMI_CharacterName_M, &gMI_CharacterName_N, &gMI_CharacterName_O, &gMI_CharacterName_P, &gMI_CharacterName_Q, &gMI_CharacterName_R,
	&gMI_CharacterName_S, &gMI_CharacterName_T, &gMI_CharacterName_U, &gMI_CharacterName_V, &gMI_CharacterName_W, &gMI_CharacterName_X,
	&gMI_CharacterName_Y, &gMI_CharacterName_Z,
	&gMI_CharacterName_a, &gMI_CharacterName_b, &gMI_CharacterName_c, &gMI_CharacterName_d, &gMI_CharacterName_e, &gMI_CharacterName_f,
	&gMI_CharacterName_g, &gMI_CharacterName_h, &gMI_CharacterName_i, &gMI_CharacterName_j, &gMI_CharacterName_k, &gMI_CharacterName_l,
	&gMI_CharacterName_m, &gMI_CharacterName_n, &gMI_CharacterName_o, &gMI_CharacterName_p, &gMI_CharacterName_q, &gMI_CharacterName_r,
	&gMI_CharacterName_s, &gMI_CharacterName_t, &gMI_CharacterName_u, &gMI_CharacterName_v, &gMI_CharacterName_w, &gMI_CharacterName_x,
	&gMI_CharacterName_y, &gMI_CharacterName_z, &gMI_CharacterName_OK, &gMI_CharacterName_Back };

MENU gMenu_CharacterName = { "Name your Character!", 53, _countof(gMI_CharacterNameItems), gMI_CharacterNameItems };


void DrawCharacterNaming(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 Red = { 0x00, 0x00, 0xFF, 0xFF };

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static uint8_t PlayerSprite;

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
        gMenu_CharacterName.SelectedItem = 0;
        TextColor.Red = 0;
        TextColor.Blue = 0;
        TextColor.Green = 0;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    if (LocalFrameCounter <= 10)
    {
        TextColor.Red = 64;
        TextColor.Blue = 64;
        TextColor.Green = 64;
    }
    if (LocalFrameCounter == 20)
    {
        TextColor.Red = 128;
        TextColor.Blue = 128;
        TextColor.Green = 128;
    }
    if (LocalFrameCounter == 30)
    {
        TextColor.Red = 192;
        TextColor.Blue = 192;
        TextColor.Green = 192;
    }
    if (LocalFrameCounter == 40)
    {
        TextColor.Red = 255;
        TextColor.Blue = 255;
        TextColor.Green = 255;
    }

    BlitStringToBuffer(gMenu_CharacterName.Name, &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (strlen(gMenu_CharacterName.Name) * 6 / 2), 16);

    if (LocalFrameCounter % 120 == 0)
    {
        if (PlayerSprite >= 9)
        {
            PlayerSprite = 0;
        }
        else
        {
            PlayerSprite += 2;
        }
    }

    Blit32BppBitmapToBuffer(&gPlayer.Sprite[SUIT_0][PlayerSprite], ((GAME_RES_WIDTH / 2) - (MAX_NAME_LENGTH * 6) / 2) - 24, 49);

    for (uint8_t Letter = 0; Letter < MAX_NAME_LENGTH; Letter++)
    {
        if (gPlayer.Name[Letter] == '\0')
        {
            BlitStringToBuffer("_", &g6x7Font, &TextColor, (((GAME_RES_WIDTH / 2) - (MAX_NAME_LENGTH * 6) / 2) + (Letter * 6)), 64);
        }
        else
        {
            BlitStringToBuffer(&gPlayer.Name[Letter], &g6x7Font, &TextColor, (((GAME_RES_WIDTH / 2) - (MAX_NAME_LENGTH * 6) / 2) + (Letter * 6)), 62);
        }
    }

    for (uint8_t Counter = 0; Counter < gMenu_CharacterName.ItemCount; Counter++)
    {
        BlitStringToBuffer(gMenu_CharacterName.Items[Counter]->Name, &g6x7Font, &TextColor, gMenu_CharacterName.Items[Counter]->x, gMenu_CharacterName.Items[Counter]->y);
    }

    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->x - 6, gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}


void PPI_CharacterName(void)
{
    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->Action();
        PlayGameSound(&gSoundMenuChoose);
    }


    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem > 25 && gMenu_CharacterName.SelectedItem < 52)
        {
            gMenu_CharacterName.SelectedItem -= 26;
        }
        else if (gMenu_CharacterName.SelectedItem < 25 && gMenu_CharacterName.SelectedItem > 0)
        {
            gMenu_CharacterName.SelectedItem += 26;
        }
        else if (gMenu_CharacterName.SelectedItem == 0)         //akward movement to the "back" and "ok" buttons
        {
            gMenu_CharacterName.SelectedItem = 53;
        }
        else if (gMenu_CharacterName.SelectedItem == 25)
        {
            gMenu_CharacterName.SelectedItem = 52;
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 26;
        }
        else if (gMenu_CharacterName.SelectedItem == 52)
        {
            gMenu_CharacterName.SelectedItem = 51;
        }
        PlayGameSound(&gSoundMenuNavigate);
    }
    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem < 26)
        {
            gMenu_CharacterName.SelectedItem += 26;
        }
        else if (gMenu_CharacterName.SelectedItem > 27 && gMenu_CharacterName.SelectedItem < 51)
        {
            gMenu_CharacterName.SelectedItem -= 26;
        }
        else if (gMenu_CharacterName.SelectedItem == 26)            //akward movement to the "back" and "ok" buttons
        {
            gMenu_CharacterName.SelectedItem = 53;
        }
        else if (gMenu_CharacterName.SelectedItem == 51)
        {
            gMenu_CharacterName.SelectedItem = 52;
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 0;
        }
        else if (gMenu_CharacterName.SelectedItem == 52)
        {
            gMenu_CharacterName.SelectedItem = 25;
        }
        PlayGameSound(&gSoundMenuNavigate);
    }

    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem == 0 || gMenu_CharacterName.SelectedItem == 26)
        {
            gMenu_CharacterName.SelectedItem += 25;
        }
        else if (gMenu_CharacterName.SelectedItem == 52)
        {
            gMenu_CharacterName.SelectedItem = 53;
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 52;
        }
        else
        {
            gMenu_CharacterName.SelectedItem--;
        }
        PlayGameSound(&gSoundMenuNavigate);
    }

    if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem == 25 || gMenu_CharacterName.SelectedItem == 51)
        {
            gMenu_CharacterName.SelectedItem -= 25;
        }
        else if (gMenu_CharacterName.SelectedItem == 52)
        {
            gMenu_CharacterName.SelectedItem = 53;
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 52;
        }
        else
        {
            gMenu_CharacterName.SelectedItem++;
        }
        PlayGameSound(&gSoundMenuNavigate);
    }

    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gDesiredGameState = gPreviousGameState;
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = gDesiredGameState;
        PlayGameSound(&gSoundMenuChoose);
    }

    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem == gMenu_CharacterName.ItemCount - 1)
        {
            gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->Action();
            gMenu_CharacterName.SelectedItem = 53;
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_CharacterName.SelectedItem = gMenu_CharacterName.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
}

void MenuItem_CharacterName_Add(void)
{
    if (strlen(gPlayer.Name) < MAX_NAME_LENGTH)
    {
        gPlayer.Name[strlen(gPlayer.Name)] = gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->Name[0];
    }
}

void MenuItem_CharacterName_Okay(void)
{
    if (strlen(gPlayer.Name) > 0)
    {
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_OVERWORLD;
        gPlayer.Active = TRUE;
    }
}

void MenuItem_CharacterName_Back(void)
{
    if (strlen(gPlayer.Name) < 1)
    {
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_TITLESCREEN;
        gMenu_CharacterName.SelectedItem = 53;
    }
    else
    {
        gPlayer.Name[strlen(gPlayer.Name) - 1] = '\0';
    }
}

