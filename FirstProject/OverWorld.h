#pragma once



typedef struct PORTCOORDS
{
	////where is the source portal in world coordinates
	UPOINT WorldPos;

	////add this many world coords to current position
	POINT WorldPosAdd;

	////add this many screen coords to current position
	POINT ScreenPosAdd;

	////add this many coords to current camera position
	POINT CameraPosAdd;

	////which area is the player in after teleporting to their dest
	GAMEAREA AreaDest;

	////sprites onscreen after teleporting
	BOOL SpritesToLoad[NUM_CHAR_SPRITES];

	////sprites onscreen before teleporting
	BOOL SpritesToUnload[NUM_CHAR_SPRITES];

} PORTCOORDS;

PORTCOORDS gPortCoords[UNIQUE_TELEPADS_COUNT];

PORTCOORDS gTeleport001;
PORTCOORDS gTeleport002;
PORTCOORDS gTeleport003;
PORTCOORDS gTeleport004;

GAMEAREA gCurrentArea;

GAMEAREA gOverworldArea;

GAMEAREA gDungeon01Area;

GAMEAREA gHome01Area;

void DrawOverworldScreen(void);

void PPI_Overworld(void);

void TeleportHandler(void);

void TriggerNPCMovement(_In_ uint64_t Counter);

uint8_t ScriptGiveMonster(uint8_t index, uint8_t level, uint16_t item);

void HealPlayerParty(void);

void TeleportPlayerBlackOut(void);

