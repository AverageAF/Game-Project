
#include "Main.h"
#include "BattleScreen.h"

void DrawBattleScreen(void)
{
	__stosd(gBackBuffer.Memory, 0xFF00FF00, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));
}


void PPI_BattleScreen(void)
{
	if (gGameInput.EscapeKeyPressed && gGameInput.EscapeKeyAlreadyPressed)
	{
		gPreviousGameState = gCurrentGameState;
		gCurrentGameState = GAMESTATE_OVERWORLD;
		gInputEnabled = FALSE;
	}
}
