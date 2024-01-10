#pragma once

//for rand_s()
#define _CRT_RAND_S 

#ifdef _DEBUG
	#define ASSERT(Expression, Message) if (!(Expression)) { *(int*)0 = 0; }
#else
	#define ASSERT(Expression, Message) ((void),0);
#endif

#define GAME_NAME "A_Game"
#define GAME_VERSION "0.9a"
#define ASSET_FILE "C:\\Users\\Frankenstein\\source\\repos\\FirstProject\\x64\\Debug\\Assets.dat"		////a fullyqualified directory, TODO: change somehow to a relative directory
#define LOG_FILE_NAME GAME_NAME ".log"
#define GAME_CODE_MODULE "FirstProjectDLL.dll"
#define GAME_CODE_MODULE_TMP "FirstProjectDLL.tmp"

#define GAME_RES_WIDTH 384		//maybe 400
#define GAME_RES_HEIGHT 240
#define GAME_BPP 32
#define GAME_DRAWING_AREA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP/8))

#define CALCULATE_AVG_FPS_EVERY_X_FRAMES 25			//goal of 120fps
#define GOAL_MICROSECONDS_PER_FRAME 16667ULL		//16667 = 60fps <8000 for 120fps


#pragma warning(push, 3)

#include <Windows.h>

#include <xaudio2.h>                // Audio library

#pragma comment(lib, "XAudio2.lib") // Audio library.

#include <stdio.h>                  // String manipulation functions such as sprintf, etc.

#include <psapi.h>                  // Process Status API, e.g. GetProcessMemoryInfo

#include <stdint.h>                 // Nicer data types, e.g., uint8_t, int32_t, etc.

#pragma comment(lib, "Winmm.lib")   // Windows Multimedia library, we use it for timeBeginPeriod to adjust the global system timer resolution.

#define AVX                         // Valid options are SSE2, AVX, or nothing.

#ifdef AVX

#include <immintrin.h>              // AVX (Advanced Vector Extensions)

#elif defined SSE2

#include <emmintrin.h>              // SSE2 (Streaming SIMD Extensions)

#endif

#pragma warning(pop)

#include "Tiles.h"

#define NUMBER_OF_SFX_SOURCE_VOICES 8

#define FONT_SHEET_CHARACTERS_PER_ROW 98

#define MAX_NAME_LENGTH 8							//8 characters + 1 null

#define SUIT_0 0
#define SUIT_1 1
#define SUIT_2 2

#define FACING_DOWN_0 0
#define FACING_DOWN_1 1
#define FACING_DOWN_2 2
#define FACING_LEFT_0 3
#define FACING_LEFT_1 4
#define FACING_LEFT_2 5
#define FACING_RIGHT_0 6
#define FACING_RIGHT_1 7
#define FACING_RIGHT_2 8
#define FACING_UP_0 9
#define FACING_UP_1 10
#define FACING_UP_2 11

typedef enum DIRECTION
{
	DOWN = 0,
	LEFT = 3,
	RIGHT = 6,
	UP = 9

} DIRECTION;



typedef enum LOGLEVEL
{
	LL_NONE = 0,
	LL_ERROR = 1,
	LL_WARNING = 2,
	LL_INFO = 3,
	LL_DEBUG = 4

} LOGLEVEL;


typedef enum GAMESTATE
{
	GAMESTATE_OPENINGSPLASH,
	GAMESTATE_TITLESCREEN,
	GAMESTATE_OVERWORLD,
	GAMESTATE_BATTLE,
	GAMESTATE_OPTIONS,
	GAMESTATE_EXITYESNO,
	GAMESTATE_CHARACTERNAME,
	GAMESTATE_LOADGAMESAVE,
	GAMESTATE_DELETESAVEYESNO

} GAMESTATE;

typedef enum RESOURCE_TYPE
{
	RESOURCE_TYPE_WAV,
	RESOURCE_TYPE_OGG,
	RESOURCE_TYPE_TILEMAP,
	RESOURCE_TYPE_BMPX

} RESOURCE_TYPE;

typedef enum WINDOW_FLAGS
{
	WINDOW_FLAG_BORDERED = 1,
	WINDOW_FLAG_HORIZ_CENTERED = 2,
	WINDOW_FLAG_VERT_CENTERED = 4,
	WINDOW_FLAG_SHADOWED = 8,

} WINDOW_FLAGS;


typedef struct UPOINT		//used for character screen position
{
	uint16_t x;
	uint16_t y;

} UPOINT;

typedef struct GAMEINPUT
{
	int16_t EscapeKeyPressed;		////key inputs
	int16_t DebugKeyPressed;
	int16_t SDownKeyPressed;
	int16_t ALeftKeyPressed;
	int16_t DRightKeyPressed;
	int16_t WUpKeyPressed;
	int16_t ChooseKeyPressed;
	int16_t TabKeyPressed;

	int16_t EscapeKeyAlreadyPressed;	////for pulse responces
	int16_t DebugKeyAlreadyPressed;
	int16_t SDownKeyAlreadyPressed;
	int16_t ALeftKeyAlreadyPressed;
	int16_t DRightKeyAlreadyPressed;
	int16_t WUpKeyAlreadyPressed;
	int16_t ChooseKeyAlreadyPressed;
	int16_t TabKeyAlreadyPressed;

} GAMEINPUT;

typedef struct GAMEBITMAP
{
	BITMAPINFO BitmapInfo;
	void* Memory;

} GAMEBITMAP;

typedef struct GAMESOUND
{
	WAVEFORMATEX WaveFormat;
	XAUDIO2_BUFFER Buffer;

} GAMESOUND;

typedef struct GAMEAREA
{
	char* Name;

	RECT Area;

	GAMESOUND* Music;

} GAMEAREA;

//typedef struct PIXEL32
//{
//	uint8_t Blue;
//	uint8_t Green;
//	uint8_t Red;
//	uint8_t Alpha;
//
//} PIXEL32;

typedef union PIXEL32 
{
	struct Colors {

	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;

	} Colors;

	DWORD Bytes;

} PIXEL32;

typedef struct GAME_PERFORMANCE_DATA
{
	uint64_t TotalFramesRendered;
	float RawFPSAverage;
	float CookedFPSAverage;
	int64_t PerfFrequency;

	MONITORINFO MonitorInfo;
	BOOL DisplayDebugInfo;
	LONG MinimumTimerResolution;		//if warning C4057 make ULONG instead
	LONG MaximumTimerResolution;		//c4057
	LONG CurrentTimerResolution;		//C4057

	DWORD HandleCount;
	PROCESS_MEMORY_COUNTERS_EX MemInfo;
	SYSTEM_INFO SystemInfo;
	int64_t CurrentSystemTime;
	int64_t PreviousSystemTime;
	double CPUPercent;

	uint8_t MaxScaleFactor;
	uint8_t CurrentScaleFactor;

} GAME_PERFORMANCE_DATA;

typedef struct TILEMAP
{
	
	uint16_t Width;

	uint16_t Height;

	uint8_t** Map;

} TILEMAP;

typedef struct GAMEMAP
{
	TILEMAP TileMap;

	GAMEBITMAP GameBitmap;

} GAMEMAP;

typedef struct PLAYER
{
	char Name[MAX_NAME_LENGTH + 1];
	GAMEBITMAP Sprite[3][12]; 
	BOOL Active;
	UPOINT ScreenPos;
	UPOINT WorldPos;
	BOOL HasMovedSincePort;
	uint8_t MovementRemaining;
	DIRECTION Direction;
	uint8_t CurrentSuit;
	uint8_t SpriteIndex;

	//10 = 1% chance, 1000 = 100% chance, 0 = 0% chance
	uint16_t RandomEncounterPercent;

} PLAYER;

typedef struct REGISTRYPARAMS
{
	DWORD LogLevel;

	DWORD SFXVolume;

	DWORD MusicVolume;

	DWORD ScaleFactor;

	BOOL FullScreen;

} REGISTRYPARAMS;


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


HMODULE gGameCodeModule;

FILETIME gGameCodeLastWriteTime;

IXAudio2SourceVoice* gXAudioSFXSourceVoice[NUMBER_OF_SFX_SOURCE_VOICES];
IXAudio2SourceVoice* gXAudioMusicSourceVoice;

uint8_t gPassableTiles[3];

REGISTRYPARAMS gRegistryParams;

GAMEBITMAP gBackBuffer;

GAMEBITMAP g6x7Font;
GAMEMAP gOverWorld01;

HWND gGameWindow;

BOOL gGameIsRunning;                //when set to FALSE ends the game, controls main game loop in winmain

GAME_PERFORMANCE_DATA gGamePerformanceData;

BOOL gInputEnabled;

PLAYER gPlayer;
UPOINT gCamera;

GAMESTATE gCurrentGameState;
GAMESTATE gPreviousGameState;
GAMESTATE gDesiredGameState;

GAMEINPUT gGameInput;

uint8_t gSFXVolume;
uint8_t gMusicVolume;

BOOL gMusicPaused;

HANDLE gAssetLoadingThreadHandle;

HANDLE gEssentialAssetsLoadedEvent;     ////event gets signaled after essential assets have been loaded (mostly req for splash screen)

GAMESOUND gSoundMenuNavigate;
GAMESOUND gSoundMenuChoose;
GAMESOUND gSoundSplashScreen;

GAMESOUND gMusicOverWorld01;
GAMESOUND gMusicDungeon01;

////imports from ntdll /////

typedef LONG(NTAPI* _NtQueryTimerResolution) (OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);

_NtQueryTimerResolution NtQueryTimerResolution;

////imports gamecode from dll////

typedef int(_cdecl* _RandomMonsterEncounter) (_In_ GAMESTATE* PreviousGameState, _Inout_ GAMESTATE* CurrentGameState);

_RandomMonsterEncounter RandomMonsterEncounter;

/////////////Function declarations//////////////////

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);

BOOL LoadGameCode(_In_ char* ModuleFileName);

DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);

DWORD Load32BppBitmapFromFile(_In_ char* FileName, _Inout_ GAMEBITMAP* GameBitmap);

DWORD Load32BppBitmapFromMem(_In_ void* Buffer, _Inout_ GAMEBITMAP* GameBitmap);

DWORD InitializePlayer(void);

void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ int16_t x, _In_ int16_t y, _In_ int16_t BrightnessAdjustment);

void BlitBackgroundToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ int16_t BrightnessAdjustment);

void BlitStringToBuffer(_In_ char* String, _In_ GAMEBITMAP* FontSheet, _In_ PIXEL32* Color, _In_ uint16_t x, _In_ uint16_t y);

void RenderFrameGraphics(void);

DWORD LoadRegistryParameters(void);

DWORD SaveRegistryParameters(void);

void LogMessageA(_In_ LOGLEVEL LogLevel, _In_ char* Message, _In_ ...);

void DrawDebugInfo(void);

HRESULT InitializeSoundEngine(void);

DWORD LoadWaveFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound);

DWORD LoadWaveFromMem(_In_ void* Buffer, _Inout_ GAMESOUND* GameSound);


void PlayGameSound(_In_ GAMESOUND* GameSound);
void PlayGameMusic(_In_ GAMESOUND* GameSound, _In_ BOOL Looping, _In_ BOOL Immediate);

void PauseGameMusic(void);
void StopGameMusic(void);

BOOL MusicIsPlaying(void);

DWORD LoadAssetFromArchive(_In_ char* Archive, _In_ char* AssetFileName, _In_ RESOURCE_TYPE ResourceType, _Inout_ void* Resource);


DWORD LoadTileMapFromFile(_In_ char* FileName, _Inout_ TILEMAP* TileMap);

DWORD LoadTileMapFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ TILEMAP* TileMap);

DWORD LoadOggFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound);

DWORD LoadOggFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ GAMESOUND* GameSound);


DWORD AssetLoadingThreadProc(_In_ LPVOID lpParam);

void InitializeGlobals(void);

void DrawWindow(_In_ int16_t x, _In_ int16_t y, _In_ int16_t Width, _In_ int16_t Height, _In_ PIXEL32 BackgroundColor, _In_ DWORD Flags);

