#pragma once



typedef struct PORTCOORDS
{
	////where is the portal in world coordinates
	UPOINT WorldPos;

	////where is the player in world coordinates after teleporting
	UPOINT WorldDest;

	////where is the player in screen coordinates after teleporting
	UPOINT ScreenPos;

	////where is the camera be in world coords after teleporting
	UPOINT CameraPos;

	////which area is the player in after teleporting
	GAMEAREA AreaDest;

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

