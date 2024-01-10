#pragma once



typedef struct MENUITEM
{
	char* Name;

	int16_t x;

	int16_t y;

	BOOL Enabled;

	void(*Action)(void);

} MENUITEM;

typedef struct MENU
{
	char* Name;

	uint8_t SelectedItem;

	uint8_t ItemCount;

	MENUITEM** Items;

} MENU;

void MenuItem_TitleScreen_Resume(void);
void MenuItem_TitleScreen_StartNew(void);
void MenuItem_TitleScreen_Options(void);
void MenuItem_TitleScreen_Exit(void);
void MenuItem_ExitYesNo_Yes(void);
void MenuItem_ExitYesNo_No(void);
void MenuItem_OptionsScreen_SfxVolume(void);
void MenuItem_OptionsScreen_MusicVolume(void);
void MenuItem_OptionsScreen_ScreenSize(void);
void MenuItem_OptionsScreen_Back(void);
void MenuItem_OptionsScreen_FullScreen(void);

void MenuItem_CharacterName_Add(void);
void MenuItem_CharacterName_Okay(void);
void MenuItem_CharacterName_Back(void);

//title screen

MENUITEM gMI_ResumeGame = { "Resume", (GAME_RES_WIDTH / 2) - (6 * 6 / 2), 112, FALSE, MenuItem_TitleScreen_Resume };

MENUITEM gMI_StartNewGame = { "Start New Game", (GAME_RES_WIDTH / 2) - (6 * 14 / 2), 128, TRUE, MenuItem_TitleScreen_StartNew };

MENUITEM gMI_Options = { "Options", (GAME_RES_WIDTH / 2) - (6 * 7 / 2), 144, TRUE, MenuItem_TitleScreen_Options };

MENUITEM gMI_Exit = { "Exit", (GAME_RES_WIDTH / 2) - (6 * 4 / 2), 160, TRUE, MenuItem_TitleScreen_Exit };

MENUITEM* gMI_TitleScreenItems[] = { &gMI_ResumeGame, &gMI_StartNewGame, &gMI_Options, &gMI_Exit };

MENU gMenu_TitleScreen = { "Title Screen Menu", 1, _countof(gMI_TitleScreenItems), gMI_TitleScreenItems };

////Exit yes/no screen


MENUITEM gMI_ExitYesNo_Yes = { "Yes", (GAME_RES_WIDTH/2) - (6 * 3 / 2), 100, TRUE, MenuItem_ExitYesNo_Yes };

MENUITEM gMI_ExitYesNo_No = { "No", (GAME_RES_WIDTH/2) - 6, 140, TRUE, MenuItem_ExitYesNo_No };

MENUITEM* gMI_ExitScreenItems[] = { &gMI_ExitYesNo_Yes, &gMI_ExitYesNo_No };

MENU gMenu_ExitYesNoScreen = { "Quit the game?", 0, _countof(gMI_ExitScreenItems), gMI_ExitScreenItems };


////// Options screen

MENUITEM gMI_OptionsScreen_SfxVolume = { "SFX Volume:", (GAME_RES_WIDTH/2) - (6 * 12 / 2), 96, TRUE, MenuItem_OptionsScreen_SfxVolume };

MENUITEM gMI_OptionsScreen_MusicVolume = { "Music Volume:", (GAME_RES_WIDTH / 2) - (6 * 14 / 2), 112, TRUE, MenuItem_OptionsScreen_MusicVolume };

MENUITEM gMI_OptionsScreen_ScreenSize = { "ScreenSize:", (GAME_RES_WIDTH / 2) - (6 * 12 / 2), 128, TRUE, MenuItem_OptionsScreen_ScreenSize };

MENUITEM gMI_OptionsScreen_FullScreen = { "Fullscreen:", (GAME_RES_WIDTH / 2) - (6 * 12 / 2), 144, TRUE, MenuItem_OptionsScreen_FullScreen };

MENUITEM gMI_OptionsScreen_Back = { "Back:", (GAME_RES_WIDTH / 2) - (6 * 6 / 2), 160, TRUE, MenuItem_OptionsScreen_Back };

MENUITEM* gMI_OptionsScreenItems[] = { &gMI_OptionsScreen_SfxVolume, &gMI_OptionsScreen_MusicVolume, &gMI_OptionsScreen_ScreenSize, &gMI_OptionsScreen_FullScreen, &gMI_OptionsScreen_Back };

MENU gMenu_OptionsScreen = { "Options", 0, _countof(gMI_OptionsScreenItems), gMI_OptionsScreenItems };

////////		Character Nameing Menu


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