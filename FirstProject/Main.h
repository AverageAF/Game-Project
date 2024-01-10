#pragma once

//for rand_s()
#define _CRT_RAND_S 

#ifdef _DEBUG
	#define ASSERT(Expression, Message) if (!(Expression)) { __ud2(); }
#else
	//#define ASSERT(Expression, Message) if (!(Expression)) { __ud2(); }
	#define ASSERT(Expression, Message) ((void),0);
#endif



#define GAME_NAME "A_Game"
#define GAME_VERSION "0.9a"
#define ASSET_FILE "C:\\Users\\Frankenstein\\source\\repos\\FirstProject\\x64\\Debug\\Assets.dat"		////a fullyqualified directory, TODO: change somehow to a relative directory

#define GAME_RES_WIDTH 384		//maybe 400
#define GAME_RES_HEIGHT 240
#define GAME_BPP 32
#define GAME_DRAWING_AREA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP/8))

#define CALCULATE_AVG_FPS_EVERY_X_FRAMES 30			//goal of 120fps
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

#define FADE_DURATION_FRAMES 20

#define COLOR_BLACK (PIXEL32){ .Bytes = 0xFF000000 }
#define COLOR_NES_TAN (PIXEL32){ .Bytes = 0xFFfca044 }
#define COLOR_DARK_RED (PIXEL32){ .Bytes = 0xFFa81000 }
#define COLOR_FORREST_GREEN (PIXEL32) { .Bytes = 0xFF007800 }
#define COLOR_DARK_BLUE (PIXEL32){ .Bytes = 0xFF0000bc }
#define COLOR_LIGHT_BLUE (PIXEL32){ .Bytes = 0xFF3cbcfc }
#define COLOR_NES_WHITE (PIXEL32){ .Bytes = 0xFFFCFCFC }
#define COLOR_DARK_WHITE (PIXEL32){ .Bytes = 0xFFDCDCDC }
#define COLOR_NES_GRAY (PIXEL32){ .Bytes = 0xFF202020 }

#define FONT_SHEET_CHARACTERS_PER_ROW 98

#define BATTLE_ENCOUNTER_GRACE_PERIOD 4				////grace period before triggering a battle

#define MAX_NAME_LENGTH 8							//8 characters + 1 null
#define MAX_MONSTER_NAME_LENGTH 12					//12 characters + 1 null
#define MAX_MOVE_NAME_LENGTH 23					//23 characters + 1 null
#define MAX_SPRITE_LOAD 10
#define MAX_DIALOGUE_BOXES 10
#define MAX_PARTY_SIZE 6
#define MAX_NONSIGNATURE_MOVES 4



#define TOTAL_MONSTERS 10
#define TOTAL_MOVES 10



#define SUIT_0 0
#define SUIT_1 1
#define SUIT_2 2

#define FACING_DOWN_0 0
#define FACING_DOWN_1 1
#define FACING_DOWN_2 2
#define FACING_LEFT_0 3
#define FACING_LEFT_1 4
#define FACING_LEFT_2 5
#define FACING_UP_0 6
#define FACING_UP_1 7
#define FACING_UP_2 8
#define FACING_RIGHT_0 9
#define FACING_RIGHT_1 10
#define FACING_RIGHT_2 11

typedef enum DIRECTION
{
	DOWN = 0,
	LEFT = 3,
	UP = 6,
	RIGHT = 9

} DIRECTION;

typedef enum LOGLEVEL
{
	LL_NONE = 0,
	LL_ERROR = 1,
	LL_WARNING = 2,
	LL_INFO = 3,
	LL_DEBUG = 4

} LOGLEVEL;

#define LOG_FILE_NAME GAME_NAME ".log"

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
	WINDOW_FLAG_OPAQUE = 2,
	WINDOW_FLAG_HORIZ_CENTERED = 4,
	WINDOW_FLAG_VERT_CENTERED = 8,
	WINDOW_FLAG_SHADOWED = 16,
	WINDOW_FLAG_ROUNDED = 32,
	WINDOW_FLAG_THICK = 64

} WINDOW_FLAGS;


///////////////////TODO maybe remove?
typedef enum DIALOGUE_FLAGS
{
	DIALOGUE_FLAG_BRIEF = 1

} DIALOGUE_FLAGS;

typedef enum MOVEMENTTYPE		//////describes how npcs move within the game
{
	MOVEMENT_STILL,
	MOVEMENT_SPIN,
	MOVEMENT_LOOK_AROUND,
	MOVEMENT_WANDER,
	MOVEMENT_WALK_UP_DOWN,
	MOVEMENT_WALK_LEFT_RIGHT

} MOVEMENTTYPE;

typedef enum MONSTER_ELEMENTS
{
	ELEMENT_NONE = 0,
	ELEMENT_EARTH = 1,
	ELEMENT_AIR = 2,
	ELEMENT_FIRE = 4,
	ELEMENT_WATER = 8,
	ELEMENT_METAL = 16,
	ELEMENT_ELECTRIC = 32,
	ELEMENT_SPIRIT = 64,
	ELEMENT_LIFE = 128,
	ELEMENT_DEATH = 256

} MONSTER_ELEMENTS;

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

typedef LONG(NTAPI* _NtQueryTimerResolution) (OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);

_NtQueryTimerResolution NtQueryTimerResolution;

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


typedef struct ELEMENTINFO
{
	uint8_t ElementIndex;

	char Name[10];

	PIXEL32 PrimaryColor;
	PIXEL32 SecondaryColor;

	MONSTER_ELEMENTS ElementFlag;

	//weakness		TODO
	//strengths


} ELEMENTINFO;



typedef struct BATTLEMOVEINFO
{
	uint16_t MoveIndex;
	char Name[MAX_MOVE_NAME_LENGTH + 1];
	////	Attack power
	uint8_t SlashPower;
	uint8_t BluntPower;
	uint8_t PiercePower;
	//// Psi power
	uint8_t ThermalPower;
	uint8_t MentalPower;
	uint8_t AstralPower;

	ELEMENTINFO MoveElement;

	////MOVE_EFFECT MoveEffect;		TODO

	BOOL MoveSignature;

} BATTLEMOVEINFO;


typedef struct MONSTERINFO			/////this structure will contain all info for all monsters from a general POV, including sprites and base-stats
{
	uint8_t MonsterIndex;
	char* Name;
	GAMEBITMAP MonsterBattleSpriteFront;
	GAMEBITMAP MonsterBattleSpriteBack;

	uint8_t MonsterBaseHealthStat;
	uint8_t MonsterBaseAttackStat;
	uint8_t MonsterBasePsiStat;
	uint8_t MonsterBaseDefenseStat;
	uint8_t MonsterBaseResolveStat;
	uint8_t MonsterBaseSpeedStat;
	uint16_t MonsterBaseStatTotal;

	MONSTER_ELEMENTS MonsterElement1;
	MONSTER_ELEMENTS MonsterElement2;

	BATTLEMOVEINFO MonsterMovePool[TOTAL_MOVES];

} MONSTERINFO;



typedef struct UNIQUEMONSTER
{
	MONSTERINFO MonsterBaseInfo;

	uint8_t MonsterCurrentLevel;
	uint8_t MonsterCurrentXp;

	//TODO
	///////EQUIPPED_ITEM MonsterEquippedItem; ????
	// 
	BATTLEMOVEINFO LearnedMoveSlot[MAX_NONSIGNATURE_MOVES];
	BATTLEMOVEINFO SignatureMove;

	uint16_t MonsterCurrentHealth;
	uint16_t MonsterMaxHealth;
	float MonsterPercetHealth;

	uint8_t MonsterHealthStat;
	uint8_t MonsterAttackStat;
	uint8_t MonsterPsiStat;
	uint8_t MonsterDefenseStat;
	uint8_t MonsterResolveStat;
	uint8_t MonsterSpeedStat;

	uint8_t HealthTrained;
	uint8_t AttackTrained;
	uint8_t PsiTrained;
	uint8_t DefenseTrained;
	uint8_t ResolveTrained;
	uint8_t SpeedTrained;

	uint8_t GeneticHealth;
	uint8_t GeneticAttack;
	uint8_t GeneticPsi;
	uint8_t GeneticDefense;
	uint8_t GeneticResolve;
	uint8_t GeneticSpeed;

} UNIQUEMONSTER;



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
	uint64_t StepsTaken;
	uint64_t StepsSinceLastEncounter;

	//10 = 1% chance, 1000 = 100% chance, 0 = 0% chance
	uint16_t RandomEncounterPercent;
	UNIQUEMONSTER Party[MAX_PARTY_SIZE];

} PLAYER;

typedef struct INGAMESPRITE			///// for sprites other than the player "NPCs Sprites"
{
	char Name[MAX_NAME_LENGTH + 1];
	UPOINT WorldPos;
	POINT ScreenPos;
	DIRECTION Direction;
	uint8_t SpriteIndex;
	GAMEBITMAP Sprite[12];
	MOVEMENTTYPE Movement;
	BOOL Visible;
	BOOL Exists;
	BOOL Loaded;
	UPOINT ResetWorldPos;
	POINT ResetScreenPos;
	DIRECTION ResetDirection;
	uint8_t ResetSightRange;
	uint8_t SightRange;
	UPOINT MovementRange;
	char* Dialogue[MAX_DIALOGUE_BOXES];
	BOOL InteractedWith;
	BOOL WantsToBattle;
	UNIQUEMONSTER Party[MAX_PARTY_SIZE];

} INGAMESPRITE;

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


IXAudio2SourceVoice* gXAudioSFXSourceVoice[NUMBER_OF_SFX_SOURCE_VOICES];
IXAudio2SourceVoice* gXAudioMusicSourceVoice;

uint8_t gPassableTiles[4];
uint8_t gEncounterTiles[2];

REGISTRYPARAMS gRegistryParams;

GAMEBITMAP gBackBuffer;
GAMEBITMAP g6x7Font;
GAMEBITMAP gBattleScreen_Grass01;
GAMEBITMAP gBattleScreen_StoneBricks01;

GAMEBITMAP gBattleSpriteBack[TOTAL_MONSTERS];	//////temp while still working on MONSTERINFO struct
GAMEBITMAP gBattleSpriteFront[TOTAL_MONSTERS];

GAMEMAP gOverWorld01;

HWND gGameWindow;

BOOL gGameIsRunning;                //when set to FALSE ends the game, controls main game loop in winmain

GAME_PERFORMANCE_DATA gGamePerformanceData;

BOOL gInputEnabled;
BOOL gGamePaused;
BOOL gDialogueControls;
BOOL gMenuControls;

INGAMESPRITE gCharacterSprite[MAX_SPRITE_LOAD];

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


LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);

DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);

//DWORD Load32BppBitmapFromFile(_In_ char* FileName, _Inout_ GAMEBITMAP* GameBitmap);

DWORD Load32BppBitmapFromMem(_In_ void* Buffer, _Inout_ GAMEBITMAP* GameBitmap);

DWORD InitializeSprites(void);

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

//DWORD LoadWaveFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound);

DWORD LoadWaveFromMem(_In_ void* Buffer, _Inout_ GAMESOUND* GameSound);


void PlayGameSound(_In_ GAMESOUND* GameSound);
void PlayGameMusic(_In_ GAMESOUND* GameSound, _In_ BOOL Looping, _In_ BOOL Immediate);

void PauseGameMusic(void);
void StopGameMusic(void);

BOOL MusicIsPlaying(void);

DWORD LoadAssetFromArchive(_In_ char* Archive, _In_ char* AssetFileName, _In_ RESOURCE_TYPE ResourceType, _Inout_ void* Resource);


//DWORD LoadTileMapFromFile(_In_ char* FileName, _Inout_ TILEMAP* TileMap);

DWORD LoadTileMapFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ TILEMAP* TileMap);

//DWORD LoadOggFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound);

DWORD LoadOggFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ GAMESOUND* GameSound);


DWORD AssetLoadingThreadProc(_In_ LPVOID lpParam);

void InitializeGlobals(void);

void RandomMonsterEncounter(_In_ GAMESTATE* PreviousGameState, _Inout_ GAMESTATE* CurrentGameState);

void DrawWindow( _In_opt_ uint16_t x, _In_opt_ uint16_t y, _In_ int16_t Width, _In_ int16_t Height, _In_opt_ PIXEL32* BorderColor, _In_opt_ PIXEL32* BackgroundColor, _In_opt_ PIXEL32* ShadowColor, _In_ DWORD Flags);

void ApplyFadeIn(_In_ uint64_t FrameCounter, _In_ PIXEL32 DefaultTextColor, _Inout_ PIXEL32* TextColor, _Inout_opt_ int16_t* BrightnessAdjustment);

void EnterDialogue(void);

void DrawDialogueBox(_In_ char* Dialogue, _In_opt_ uint64_t Counter, _In_opt_ DWORD Flags);
