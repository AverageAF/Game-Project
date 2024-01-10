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

#define MAX_LEVEL 100


#pragma warning(push, 3)

#include "cJSON.h"

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
#include "SimpleConstants.h"

#define NUMBER_OF_SFX_SOURCE_VOICES 8

#define FADE_DURATION_FRAMES 20

#define COLOR_BLACK (PIXEL32){ .Bytes = 0xFF000000 }
#define COLOR_DARK_RED (PIXEL32){ .Bytes = 0xFFa81000 }
#define COLOR_NES_RED (PIXEL32){ .Bytes = 0xFFB80020 }
#define COLOR_FORREST_GREEN (PIXEL32) { .Bytes = 0xFF007800 }
#define COLOR_DARK_BLUE (PIXEL32){ .Bytes = 0xFF0000bc }
#define COLOR_LIGHT_BLUE (PIXEL32){ .Bytes = 0xFF3cbcfc }
#define COLOR_NES_WHITE (PIXEL32){ .Bytes = 0xFFFCFCFC }
#define COLOR_DARK_WHITE (PIXEL32){ .Bytes = 0xFFDCDCDC }
#define COLOR_DARK_GRAY (PIXEL32){ .Bytes = 0xFF303030 }
#define COLOR_LIGHT_GRAY (PIXEL32){ .Bytes = 0xFFacacac }
#define COLOR_LIGHT_GREEN (PIXEL32){ .Bytes = 0xFF00b800 }
#define COLOR_DARK_GREEN (PIXEL32){ .Bytes = 0xFF005800 }
#define COLOR_NEON_BLUE (PIXEL32){ .Bytes = 0xFF3cbcfc }
#define COLOR_NES_PINK (PIXEL32){ .Bytes = 0xFFf85898 }
#define COLOR_GOLD (PIXEL32){ .Bytes = 0xFFFFD700 }
#define COLOR_NES_MAGENTA (PIXEL32){ .Bytes = 0xFFd800cc }

#define COLOR_NES_TAN (PIXEL32){ .Bytes = 0xFFfca044 }			//element none
#define COLOR_NES_BROWN (PIXEL32){ .Bytes = 0xFF503000 }		//earth
#define COLOR_NES_SKY_BLUE (PIXEL32){ .Bytes = 0xFFa4e4fc }		//air
#define COLOR_NES_ORANGE (PIXEL32){ .Bytes = 0xFFf83800 }		//fire
#define COLOR_NES_BLUE (PIXEL32){ .Bytes = 0xFF0000fc }			//water
#define COLOR_NES_YELLOW (PIXEL32){ .Bytes = 0xFFf8b800 }		//electric
#define COLOR_NES_GRAY (PIXEL32){ .Bytes = 0xFF7c7c7c }			//metal
#define COLOR_NES_PURPLE (PIXEL32){ .Bytes = 0xFF940084 }		//soul
#define COLOR_NES_LIGHT_GREEN (PIXEL32){ .Bytes = 0xFF00a844 }	//life
#define COLOR_NES_BLACK_RED (PIXEL32){ .Bytes = 0xFF160000}		//death


#define FONT_SHEET_CHARACTERS_PER_ROW 98

#define BATTLE_ENCOUNTER_GRACE_PERIOD 4				////grace period before triggering a battle

#define LEVEL_UP_END 0xFFFF

#define MAX_NAME_LENGTH 8							//8 characters + 1 null
#define MAX_MONSTER_NAME_LENGTH 12					//12 characters + 1 null
#define MAX_MOVE_NAME_LENGTH 23					//23 characters + 1 null
#define NUM_CHAR_SPRITES 16						//total sprites
#define MAX_DIALOGUE_BOXES 10
#define MAX_PARTY_SIZE 6
#define MAX_NONSIGNATURE_MOVES 4
#define MAX_INCSIGNATURE_MOVES 5
#define MAX_GENETICS_VALUE 31
#define LEGENDARY_GENETICS_PERFECT_COUNT 4
#define USE_RANDOM_GENETICS (MAX_GENETICS_VALUE + 1)
#define MONSTER_FLAG_LEGENDARY (1 << 0)
#define NUM_MONSTER_STATS 6
#define END_OF_STRING 0xFF
#define MAX_STAT_CHANGES 6
#define MAX_DIALOGUE_ROWS 7
#define MAX_BATTLECHAR_PER_ROW 40
#define MAX_ITEMS_GIVE 16



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

///////////////////////////////////////////////////////////////////////////////////////////////
//possibly give this its own flags.c
BOOL gGameFlags[0x80];

#define START_OF_FLAGS 0x0
#define TEMP_FLAGS_START 0x0
#define FLAG_TEMP_1 0x1
#define FLAG_TEMP_2 0x2
#define FLAG_TEMP_3 0x3
#define FLAG_TEMP_4 0x4
#define FLAG_TEMP_5 0x5
#define FLAG_TEMP_6 0x6
#define FLAG_TEMP_7 0x7
#define FLAG_TEMP_8 0x8
#define FLAG_TEMP_9 0x9
#define FLAG_TEMP_A 0xA
#define FLAG_TEMP_B 0xB
#define FLAG_TEMP_C 0xC
#define FLAG_TEMP_D 0xD
#define FLAG_TEMP_E 0xE
#define FLAG_TEMP_F 0xF
#define FLAG_TEMP_10 0x10
#define FLAG_TEMP_11 0x11
#define FLAG_TEMP_12 0x12
#define FLAG_TEMP_13 0x13
#define FLAG_TEMP_14 0x14
#define FLAG_TEMP_15 0x15
#define FLAG_TEMP_16 0x16
#define FLAG_TEMP_17 0x17
#define FLAG_TEMP_18 0x18
#define FLAG_TEMP_19 0x19
#define FLAG_TEMP_1A 0x1A
#define FLAG_TEMP_1B 0x1B
#define FLAG_TEMP_1C 0x1C
#define FLAG_TEMP_1D 0x1D
#define FLAG_TEMP_1E 0x1E
#define FLAG_TEMP_1F 0x1F
#define TEMP_FLAGS_END FLAG_TEMP_1F

#define FLAG_UNUSED_0x20 0x20
#define FLAG_UNUSED_0x21 0x21
#define FLAG_UNUSED_0x22 0x22
#define FLAG_UNUSED_0x23 0x23
#define FLAG_UNUSED_0x24 0x24
#define FLAG_UNUSED_0x25 0x25
#define FLAG_UNUSED_0x26 0x26
#define FLAG_UNUSED_0x27 0x27
#define FLAG_UNUSED_0x28 0x28
#define FLAG_UNUSED_0x29 0x29
#define FLAG_UNUSED_0x2A 0x2A
#define FLAG_UNUSED_0x2B 0x2B
#define FLAG_UNUSED_0x2C 0x2C
#define FLAG_UNUSED_0x2D 0x2D
#define FLAG_UNUSED_0x2E 0x2E
#define FLAG_UNUSED_0x2F 0x2F
#define FLAG_UNUSED_0x30 0x30
#define FLAG_UNUSED_0x31 0x31
#define FLAG_UNUSED_0x32 0x32
#define FLAG_UNUSED_0x33 0x33
#define FLAG_UNUSED_0x34 0x34
#define FLAG_UNUSED_0x35 0x35
#define FLAG_UNUSED_0x36 0x36
#define FLAG_UNUSED_0x37 0x37
#define FLAG_UNUSED_0x38 0x38
#define FLAG_UNUSED_0x39 0x39
#define FLAG_UNUSED_0x3A 0x3A
#define FLAG_UNUSED_0x3B 0x3B
#define FLAG_UNUSED_0x3C 0x3C
#define FLAG_UNUSED_0x3D 0x3D
#define FLAG_UNUSED_0x3E 0x3E
#define FLAG_UNUSED_0x3F 0x3F
#define FLAG_UNUSED_0x40 0x40
#define FLAG_UNUSED_0x41 0x41
#define FLAG_UNUSED_0x42 0x42
#define FLAG_UNUSED_0x43 0x43
#define FLAG_UNUSED_0x44 0x44
#define FLAG_UNUSED_0x45 0x45
#define FLAG_UNUSED_0x46 0x46
#define FLAG_UNUSED_0x47 0x47
#define FLAG_UNUSED_0x48 0x48
#define FLAG_UNUSED_0x49 0x49
#define FLAG_UNUSED_0x4A 0x4A
#define FLAG_UNUSED_0x4B 0x4B
#define FLAG_UNUSED_0x4C 0x4C
#define FLAG_UNUSED_0x4D 0x4D
#define FLAG_UNUSED_0x4E 0x4E
#define FLAG_UNUSED_0x4F 0x4F
#define FLAG_UNUSED_0x50 0x50
#define FLAG_UNUSED_0x51 0x51
#define FLAG_UNUSED_0x52 0x52
#define FLAG_UNUSED_0x53 0x53
#define FLAG_UNUSED_0x54 0x54
#define FLAG_UNUSED_0x55 0x55
#define FLAG_UNUSED_0x56 0x56
#define FLAG_UNUSED_0x57 0x57
#define FLAG_UNUSED_0x58 0x58
#define FLAG_UNUSED_0x59 0x59
#define FLAG_UNUSED_0x5A 0x5A
#define FLAG_UNUSED_0x5B 0x5B
#define FLAG_UNUSED_0x5C 0x5C
#define FLAG_UNUSED_0x5D 0x5D
#define FLAG_UNUSED_0x5E 0x5E
#define FLAG_UNUSED_0x5F 0x5F
#define FLAG_UNUSED_0x60 0x60
#define FLAG_UNUSED_0x61 0x61
#define FLAG_UNUSED_0x62 0x62
#define FLAG_UNUSED_0x63 0x63
#define FLAG_UNUSED_0x64 0x64
#define FLAG_UNUSED_0x65 0x65
#define FLAG_UNUSED_0x66 0x66
#define FLAG_UNUSED_0x67 0x67
#define FLAG_UNUSED_0x68 0x68
#define FLAG_UNUSED_0x69 0x69
#define FLAG_UNUSED_0x6A 0x6A
#define FLAG_UNUSED_0x6B 0x6B
#define FLAG_UNUSED_0x6C 0x6C
#define FLAG_UNUSED_0x6D 0x6D
#define FLAG_UNUSED_0x6E 0x6E
#define FLAG_UNUSED_0x6F 0x6F
#define FLAG_UNUSED_0x70 0x70
#define FLAG_UNUSED_0x71 0x71
#define FLAG_UNUSED_0x72 0x72
#define FLAG_UNUSED_0x73 0x73
#define FLAG_UNUSED_0x74 0x74
#define FLAG_UNUSED_0x75 0x75
#define FLAG_UNUSED_0x76 0x76
#define FLAG_UNUSED_0x77 0x77
#define FLAG_UNUSED_0x78 0x78
#define FLAG_UNUSED_0x79 0x79
#define FLAG_UNUSED_0x7A 0x7A
#define FLAG_UNUSED_0x7B 0x7B
#define FLAG_UNUSED_0x7C 0x7C
#define FLAG_UNUSED_0x7D 0x7D
#define FLAG_UNUSED_0x7E 0x7E
#define FLAG_UNUSED_0x7F 0x7F

#define END_OF_FLAGS FLAG_UNUSED_0x7F	

///////////////////////////////////////////////////////////////////////////////////////////////

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
	GAMESTATE_BATTLE_MONSTER,
	GAMESTATE_BATTLE_TRAINER,
	GAMESTATE_OPTIONS,
	GAMESTATE_EXITYESNO,
	GAMESTATE_CHARACTERNAME,
	GAMESTATE_LOADGAMESAVE,
	GAMESTATE_SAVEGAMESAVE,
	GAMESTATE_DELETESAVEYESNO,
	GAMESTATE_INVENTORYSCREEN,
	GAMESTATE_MONSTERSTATS,
	GAMESTATE_STORE,

} GAMESTATE;

typedef enum EVENT_FLAGS		////flags for gCharacterSprite.Event, creates events after dialogue
{
	EVENT_FLAG_NONE,
	EVENT_FLAG_BATLLE,
	EVENT_FLAG_HEAL,
	EVENT_FLAG_CUTSCENE,
	EVENT_FLAG_ITEM_ONCE,
	EVENT_FLAG_ITEM_REPEAT,
	EVENT_FLAG_MONSTER,
	EVENT_FLAG_MOVEMENT,
	EVENT_FLAG_STORE,
} EVENT_FLAGS;

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

typedef enum DIALOGUE_FLAGS
{
	DIALOGUE_FLAG_0,
	DIALOGUE_FLAG_1,
	DIALOGUE_FLAG_2,
	DIALOGUE_FLAG_3,
	DIALOGUE_FLAG_4,
	DIALOGUE_FLAG_5,
	DIALOGUE_FLAG_6,
	DIALOGUE_FLAG_7,
	DIALOGUE_FLAG_8,
	DIALOGUE_FLAG_9,

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


//labels for GetMonsterData/SetMonsterData
enum MONSTER_DATA
{
	MONSTER_DATA_MONSTER_SEED,
	MONSTER_DATA_PLAYER_SEED,
	MONSTER_DATA_NICKNAME,
	MONSTER_DATA_PLAYER_NAME,
	MONSTER_DATA_HAS_INDEX,
	MONSTER_DATA_CHECKSUM,
	MONSTER_DATA_ENCRYPT,
	MONSTER_DATA_INDEX,
	MONSTER_DATA_HELDITEM,
	MONSTER_DATA_EXPERIENCE,
	MONSTER_DATA_FRIENDSHIP,
	MONSTER_DATA_MOVE_1,
	MONSTER_DATA_MOVE_2,
	MONSTER_DATA_MOVE_3,
	MONSTER_DATA_MOVE_4,
	MONSTER_DATA_HP_TRAINING,
	MONSTER_DATA_ATTACK_TRAINING,
	MONSTER_DATA_DEFENSE_TRAINING,
	MONSTER_DATA_SPEED_TRAINING,
	MONSTER_DATA_PSI_TRAINING,
	MONSTER_DATA_RESOLVE_TRAINING,
	MONSTER_DATA_SIGNATURE_MOVE,
	MONSTER_DATA_MET_LOCATION,
	MONSTER_DATA_MET_LEVEL,
	MONSTER_DATA_HP_GENETICS,
	MONSTER_DATA_ATTACK_GENETICS,
	MONSTER_DATA_DEFENSE_GENETICS,
	MONSTER_DATA_SPEED_GENETICS,
	MONSTER_DATA_PSI_GENETICS,
	MONSTER_DATA_RESOLVE_GENETICS,
	MONSTER_DATA_ABILITY_NUMBER,
	MONSTER_DATA_STATUS,
	MONSTER_DATA_LEVEL,
	MONSTER_DATA_HEALTH,
	MONSTER_DATA_MAX_HEALTH,
	MONSTER_DATA_ATTACK,
	MONSTER_DATA_DEFENSE,
	MONSTER_DATA_SPEED,
	MONSTER_DATA_PSI,
	MONSTER_DATA_RESOLVE,
	MONSTER_DATA_GENETICS,
	MONSTER_DATA_KNOWN_MOVES,

};

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
	int16_t QKeyPressed;
	int16_t CtrlKeyPressed;

	int16_t EscapeKeyAlreadyPressed;	////for pulse responces
	int16_t DebugKeyAlreadyPressed;
	int16_t SDownKeyAlreadyPressed;
	int16_t ALeftKeyAlreadyPressed;
	int16_t DRightKeyAlreadyPressed;
	int16_t WUpKeyAlreadyPressed;
	int16_t ChooseKeyAlreadyPressed;
	int16_t TabKeyAlreadyPressed;
	int16_t QKeyAlreadyPressed;
	int16_t CtrlKeyAlreadyPressed;

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
	uint16_t Index;

	char* Name;

	RECT Area;

	GAMESOUND* Music;

} GAMEAREA;

#define NUM_MONSTER_ENCOUNTER_CHANCE_SLOTS 16

typedef struct ENCOUNTERAREA
{
	char* Name;

	RECT Area;

	uint16_t EncounterRate;

	uint8_t MinLevel;

	uint8_t MaxLevel;

	uint8_t MonsterIndexChanceSlots[NUM_MONSTER_ENCOUNTER_CHANCE_SLOTS];

} ENCOUNTERAREA;

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct DriveMonster
{
	uint32_t MonsterSeed;
	uint32_t PlayerSeed;
	uint8_t nickname[MAX_MONSTER_NAME_LENGTH + 1];
	uint8_t playerName[MAX_NAME_LENGTH + 1];
	uint8_t hasIndex : 1;
	uint8_t Filler : 7;
	uint16_t checkSum;		//UNUSED: remove??
	uint16_t unknown;		//UNUSED: remove??

	////////////////////////////////////////

	uint8_t Index;			//0x00
	uint16_t HeldItem;			//0x01
	uint32_t Experience;		//0x03
	uint8_t Friendship;			//0x07
	//size = 0x08

	uint16_t Moves[MAX_NONSIGNATURE_MOVES];	//0x00
	//size = 0x08 		(uint16_t x 4 = 2 bytes x 4 = 8 bytes)

	uint8_t HpTraining;			//0x00
	uint8_t AttackTraining;		//0x01
	uint8_t DefenseTraining;	//0x02
	uint8_t SpeedTraining;		//0x03
	uint8_t PsiTraining;		//0x04
	uint8_t ResolveTraining;	//0x05
	uint16_t SignatureMove;		//0x06
	//size = 0x08


	uint8_t MetLocation;		//0x00
	uint8_t MetLevel;			//0x01

	uint32_t HpGenetics : 5;		//0x02
	uint32_t AttackGenetics : 5;	//0x02
	uint32_t DefenseGenetics : 5;	//0x03
	uint32_t SpeedGenetics : 5;	//0x03
	uint32_t PsiGenetics : 5;		//0x03
	uint32_t ResolveGenetics : 5;	//0x04
	uint32_t AbilityNumber : 2;	//0x05

	uint8_t Filler1Sub3;		//0x06		////TOUSE: make into interesting stats, ex. favorite food, height & weight, ...
	uint8_t Filler2Sub3;		//0x07
	//size = 0x08


};

struct Monster
{
	struct DriveMonster DriveMonster;
	uint32_t Status;
	uint8_t Level;
	uint16_t Health;
	uint16_t MaxHealth;
	uint16_t Attack;
	uint16_t Defense;
	uint16_t Speed;
	uint16_t Psi;
	uint16_t Resolve;
};

struct BattleMonster
{
	uint8_t Index;
	uint16_t Health;
	uint16_t MaxHealth;
	uint16_t Attack;
	uint16_t Defense;
	uint16_t Speed;
	uint16_t Psi;
	uint16_t Resolve;
	uint16_t Moves[MAX_NONSIGNATURE_MOVES];
	uint16_t SignatureMove;
	uint32_t HpGenetics : 5;
	uint32_t AttackGenetics : 5;
	uint32_t DefenseGenetics : 5;
	uint32_t SpeedGenetics : 5;
	uint32_t PsiGenetics : 5;
	uint32_t ResolveGenetics : 5;
	uint32_t AbilityNumber : 2;
	int8_t StatChanges[MAX_STAT_CHANGES];
	uint16_t Ability;
	uint8_t Element1;
	uint8_t Element2;
	uint8_t Level;
	uint8_t Friendship;
	uint16_t EquippedItem;
	uint8_t Nickname[MAX_MONSTER_NAME_LENGTH + 1];
	uint8_t PlayerName[MAX_MONSTER_NAME_LENGTH + 1];
	uint32_t Experience;
	uint32_t Status1;
	uint32_t Status2;
	uint32_t MonsterSeed;
	uint32_t PlayerSeed;
};

struct LevelUpMove
{
	uint16_t move;
	uint16_t level;
};

struct BaseStats
{
	uint8_t baseHP;
	uint8_t baseAttack;
	uint8_t baseDefense;
	uint8_t baseSpeed;
	uint8_t basePsi;
	uint8_t baseResolve;
	uint8_t element1;
	uint8_t element2;
	uint8_t catchrate;
	uint16_t expYield;
	uint16_t trainingYieldHp : 2;
	uint16_t trainingYieldAttack : 2;
	uint16_t trainingYieldDefense : 2;
	uint16_t trainingYieldSpeed : 2;
	uint16_t trainingYieldPsi : 2;
	uint16_t trainingYieldResolve : 2;
	uint16_t itemCommon;
	uint16_t itemRare;
	uint8_t genderRatio;
	uint8_t eggHatch;
	uint8_t friendship;
	uint8_t growthRate;
	uint8_t eggGroup1;
	uint8_t eggGroup2;
	uint16_t abilities[NUM_ABILITY_SLOTS];
	uint16_t flags;

};


struct BattleMove
{
	uint16_t effect;

	uint8_t power1;
	uint8_t power2;
	uint8_t power3;
	uint8_t element;
	uint8_t secondEffectChance;
	uint16_t target;
	int8_t speedPriority;
	uint32_t flags;
	uint8_t split;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



typedef struct PLAYER
{
	char Name[MAX_NAME_LENGTH + 1];
	uint32_t Currency;
	uint32_t Seed;
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

	//// Adventure items
	BOOL SprintingShoes;
	BOOL Sprinting;

	//10 = 1% chance, 1000 = 100% chance, 0 = 0% chance
	uint16_t RandomEncounterPercent;

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
	uint8_t MovementRemaining;
	BOOL Visible;
	BOOL Exists;
	BOOL Loaded;
	UPOINT ResetWorldPos;
	POINT ResetScreenPos;
	DIRECTION ResetDirection;
	UPOINT ResetOriginWorldPos;
	POINT ResetOriginScreenPos;
	DIRECTION ResetOriginDirection;
	uint8_t ResetSightRange;
	uint8_t SightRange;
	UPOINT MovementRange;
	char* Dialogue[MAX_DIALOGUE_BOXES];
	uint8_t DialogueFlag;
	uint8_t DialoguesBeforeLoop;
	uint8_t DialogueLoopReturn;
	//uint8_t DialogueLoopType;				//possibly not needed?
	BOOL InteractedWith;
	uint8_t Event;
	uint16_t EventItemsIndex[MAX_ITEMS_GIVE];
	uint16_t EventItemsCount[MAX_ITEMS_GIVE];
	uint8_t EventMonsterIndex;
	uint8_t EventMonsterLevel;
	uint16_t EventMonsterItem;
	struct Monster MonsterParty[MAX_PARTY_SIZE];
	uint8_t BattleAiFlag;

} INGAMESPRITE;

typedef struct REGISTRYPARAMS
{
	DWORD LogLevel;

	DWORD SFXVolume;

	DWORD MusicVolume;

	DWORD ScaleFactor;

	BOOL FullScreen;

	DWORD TextSpeed;

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

uint8_t gPassableTiles[NUM_PASSABLE_TILES];
uint8_t gEncounterTiles[NUM_ENCOUNTER_TILES];

REGISTRYPARAMS gRegistryParams;

GAMEBITMAP gBackBuffer;
GAMEBITMAP g6x7Font;
GAMEBITMAP gBattleScreen_Grass01;
GAMEBITMAP gBattleScreen_StoneBricks01;

uint8_t gPlayerPartyCount;				//number of monsters in player party from 0 to 6
uint8_t gOpponentPartyCount;

struct Monster gPlayerParty[MAX_PARTY_SIZE];
struct Monster gOpponentParty[MAX_PARTY_SIZE];

GAMEBITMAP gBattleSpriteBack[NUM_MONSTERS];	//////temp while still working on MONSTERINFO struct
GAMEBITMAP gBattleSpriteFront[NUM_MONSTERS];

BOOL gSellingItems;					//so inventory knows the difference between regular inventory and selling to a shop
uint8_t gStoreSpriteIndex;			//so storescreen.c knows what npc you are buying from | USE uint16_t?????

GAMEMAP gOverWorld01;

HWND gGameWindow;

BOOL gGameIsRunning;                //when set to FALSE ends the game, controls main game loop in winmain

GAME_PERFORMANCE_DATA gGamePerformanceData;

BOOL gInputEnabled;							//reads player inputs but does not apply them until enabled
BOOL gGamePaused;							//pauses sprites in background
BOOL gDialogueControls;						//enables input confirmation after dialogue
BOOL gOverWorldControls;					//enables overworld controls
BOOL gFinishedDialogueTextAnimation;		//allows for dialogue text animation to reset between boxes
BOOL gFinishedBattleTextAnimation;			//same as above but for battles

INGAMESPRITE gCharacterSprite[NUM_CHAR_SPRITES];

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

//hopefully this works and I dont need a header for strings
const uint8_t gMonsterNames[NUM_MONSTERS][MAX_MONSTER_NAME_LENGTH + 1];
const uint8_t gBattleMoveNames[NUM_BATTLEMOVES][MAX_MOVE_NAME_LENGTH + 1];
const uint8_t* const gMoveDescriptionPointers[NUM_BATTLEMOVES];
const struct BattleMove gBattleMoves[NUM_BATTLEMOVES];
const struct BaseStats gBaseStats[];
const uint32_t gExperienceTables[][MAX_LEVEL + 1];
const struct LevelUpMove* const gLevelUpMoves[NUM_MONSTERS];


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

DWORD LoadAssetFromArchive(_In_ char* Archive, _In_ char* AssetFileName, _Inout_ void* Resource);


//DWORD LoadTileMapFromFile(_In_ char* FileName, _Inout_ TILEMAP* TileMap);

DWORD LoadTileMapFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ TILEMAP* TileMap);

//DWORD LoadOggFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound);

DWORD LoadOggFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ GAMESOUND* GameSound);


DWORD AssetLoadingThreadProc(_In_ LPVOID lpParam);

void InitializeGlobals(void);

void RandomMonsterEncounter(_In_ GAMESTATE* PreviousGameState, _Inout_ GAMESTATE* CurrentGameState);
void TrainerEncounter(_In_ GAMESTATE* PreviousGameState, _Inout_ GAMESTATE* CurrentGameState);

void DrawWindow( _In_opt_ uint16_t x, _In_opt_ uint16_t y, _In_ int16_t Width, _In_ int16_t Height, _In_opt_ PIXEL32* BorderColor, _In_opt_ PIXEL32* BackgroundColor, _In_opt_ PIXEL32* ShadowColor, _In_ DWORD Flags);

void ApplyFadeIn(_In_ uint64_t FrameCounter, _In_ PIXEL32 DefaultTextColor, _Inout_ PIXEL32* TextColor, _Inout_opt_ int16_t* BrightnessAdjustment);

void EnterDialogue(void);

void DrawDialogueBox(_In_ char* Dialogue, _In_opt_ uint64_t Counter, _In_opt_ DWORD Flags);

void DrawMonsterHpBar(uint16_t x, uint16_t y, uint8_t percentHp100, uint8_t percentExp100, uint8_t monsterLevel, char* monsterNickname, BOOL showExpBar);

BOOL BlitBattleStateTextBox_Text(enum BATTLESTATE battlestate_wait, uint8_t battleTextLineCount, uint64_t frameCounter);

void BlitBattleStateTextBox_Wait(uint8_t battleTextLineCount);

void ReSortUsableitems(void);

void ReSortEquipableitems(void);

void ReSortValuableitems(void);

void ReSortAdventureitems(void);

void ReSortLearnableMovesFromMonster(struct DriveMonster* driveMonster);

void SetGameFlag(uint8_t flag);
void ClearGameFlag(uint8_t flag);
void ClearTempGameFlags(void);

void BlitItemDescription(char* description);

void DrawBuySellBackBox(void);

uint8_t PPI_BuySellBackBox(void);

BOOL CanPlayerAffordCurrencyCost(uint32_t currencycost);