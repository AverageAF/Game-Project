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
	RECT AreaDest;

} PORTCOORDS;

typedef struct GAMEAREA
{
	char* Name;

	RECT Area;

	GAMESOUND Music;


} GAMEAREA;

PORTCOORDS gPortCoords[2];

PORTCOORDS gTeleport001;
PORTCOORDS gTeleport002;

RECT gCurrentArea;

RECT gOverworldArea;

RECT gDungeon01Area;

void DrawOverworldScreen(void);

void PPI_Overworld(void);

void TeleportHandler(void);