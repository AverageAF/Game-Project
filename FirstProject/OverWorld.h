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
	BOOL SpritesToLoad[MAX_SPRITE_LOAD];

	////sprites onscreen before teleporting
	BOOL SpritesToUnload[MAX_SPRITE_LOAD];

} PORTCOORDS;

PORTCOORDS gPortCoords[2];

PORTCOORDS gTeleport001;
PORTCOORDS gTeleport002;

GAMEAREA gCurrentArea;

GAMEAREA gOverworldArea;

GAMEAREA gDungeon01Area;

void DrawOverworldScreen(void);

void PPI_Overworld(void);

void TeleportHandler(void);

void TriggerNPCMovement(_In_ uint64_t Counter, _In_ MOVEMENTTYPE MovementFlag);

void ResetNPCs(void);
