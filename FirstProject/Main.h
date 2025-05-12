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
#define NUM_TRIGGERS 16							//total trigger tiles in the game
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
	GAMESTATE_DRIVE_STORAGE,

} GAMESTATE;

typedef enum EVENT_TYPE		////flags for gCharacterSprite.Event, creates events after dialogue
{
	EVENT_NULL,
	EVENT_TALK,
	EVENT_BATTLE,
	EVENT_HEAL,
	EVENT_EQUIPITEM_ONCE,
	EVENT_USEITEM_ONCE,
	EVENT_VALUEITEM_ONCE,
	EVENT_ADVENTUREITEM_ONCE,
	EVENT_EQUIPITEM_REPEAT,
	EVENT_USEITEM_REPEAT,
	EVENT_VALUEITEM_REPEAT,
	EVENT_ADVENTUREITEM_REPEAT,
	EVENT_MONSTER,
	EVENT_MOVEMENT,
	EVENT_STORE,
	EVENT_EQUIPITEM_NOSPRITE,
	EVENT_USEITEM_NOSPRITE,
	EVENT_VALUEITEM_NOSPRITE,
	EVENT_ADVENTUREITEM_NOSPRITE,
	EVENT_DRIVE_STORAGE,

} EVENT_TYPE;

typedef enum BATTLESTATE
{
	BATTLESTATE_OPENING_TEXT,
	BATTLESTATE_OPENING_WAIT,
	BATTLESTATE_TURNORDER_CALC,
	BATTLESTATE_FIRSTMOVE_TEXT,
	BATTLESTATE_FIRSTMOVE_WAIT,
	BATTLESTATE_FIRSTMOVE_CALC,
	BATTLESTATE_POSTFIRSTMOVE_TEXT,			//unused
	BATTLESTATE_SECONDMOVE_TEXT,
	BATTLESTATE_SECONDMOVE_WAIT,
	BATTLESTATE_SECONDMOVE_CALC,
	BATTLESTATE_POSTSECONDMOVE_TEXT,		//unused
	BATTLESTATE_KO,
	BATTLESTATE_KO_WAIT,
	BATTLESTATE_REWARD,
	BATTLESTATE_RUN_FIGHT,
	BATTLESTATE_SWITCH_FIGHT,				///////TOUSE: harder difficulties where there is no running or item bag
	BATTLESTATE_CHOOSE_MOVE,
	BATTLESTATE_CHOOSE_MONSTER,
	BATTLESTATE_CHOOSE_ITEM,
	BATTLESTATE_SWITCHING_TEXT,
	BATTLESTATE_SWITCHING_WAIT,
	BATTLESTATE_FIRSTMOVE_POSTTEXT,
	BATTLESTATE_FIRSTMOVE_POSTWAIT,
	BATTLESTATE_SECONDMOVE_POSTTEXT,
	BATTLESTATE_SECONDMOVE_POSTWAIT,
	BATTLESTATE_USEITEM_TEXT,
	BATTLESTATE_USEITEM_WAIT,
	BATTLESTATE_CATCH_TEXT,
	BATTLESTATE_CATCH_WAIT,
	BATTLESTATE_CATCHSUCCESS_TEXT,
	BATTLESTATE_CATCHSUCCESS_WAIT,
	BATTLESTATE_NOESCAPE_TEXT,
	BATTLESTATE_NOESCAPE_WAIT,
	BATTLESTATE_ESCAPEFAIL_TEXT,
	BATTLESTATE_ESCAPEFAIL_WAIT,
	BATTLESTATE_ESCAPESUCCESS_TEXT,
	BATTLESTATE_ESCAPESUCCESS_WAIT,
	BATTLESTATE_OPPONENTSWITCH_TEXT,
	BATTLESTATE_OPPONENTSWITCH_WAIT,
	BATTLESTATE_OPPONENTITEM_TEXT,
	BATTLESTATE_OPPONENTITEM_WAIT,

} BATTLESTATE;

typedef enum TRIGGER_TYPES
{
	//TODO:
	TRIGGER_TYPE_NULL,
	TRIGGER_TYPE_BLANK,
	TRIGGER_TYPE_ONCE,
	TRIGGER_TYPE_INFINITE,
	TRIGGER_TYPE_COOLDOWN,
	//TRIGGER_TYPE_STORY????

} TRIGGER_TYPES;

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

//TOREMOVE: dont need these when we have Dialogue.c
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
//

typedef enum MOVEMENTTYPE		//////describes how npcs move within the game
{
	MOVEMENT_STILL,
	MOVEMENT_SPIN,
	MOVEMENT_LOOK_AROUND,
	MOVEMENT_WANDER,
	MOVEMENT_WALK_UP_DOWN,
	MOVEMENT_WALK_LEFT_RIGHT,
	MOVEMENT_SPARKLE,
	MOVEMENT_ITEMPICKUP,
	//MOVEMENT_TRIGGER,

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

typedef enum MOVEMENT_SCRIPT
{
	MOVE_NULL,
	FACE_DOWN,	//only changes direction
	FACE_LEFT,
	FACE_UP,
	FACE_RIGHT,
	WALK_DOWN,	//changes direction and adds movement for 1 tile
	WALK_LEFT,
	WALK_UP,
	WALK_RIGHT,
	WALK_BW_DOWN,	//does NOT change direction but applies movement
	WALK_BW_LEFT,
	WALK_BW_UP,
	WALK_BW_RIGHT,
	DELAY_16,		//does nothing for specified number of frames (60fps locked)
	DELAY_32,
	DELAY_48,
	DELAY_64,
	DIALOGUE_TRIGGER,	//triggers a dialogue box to appear, when dialogue is finished allow player to press 'E' to advance to next dialogue, or if it the last, advance to the next script
	START_OF_SCRIPT,
	END_OF_SCRIPT,

}MOVEMENT_SCRIPT;

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

#define MAX_MOVEMENT_SCRIPTS 255

typedef struct SCENE_SCRIPT
{
	MOVEMENT_SCRIPT Script;
	uint8_t Actor;

} SCENE_SCRIPT;

typedef struct SCENE_XYPOS
{
	uint8_t Actor;
	UPOINT Pos;

} SCENE_XYPOS;

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
	uint32_t AbilityNumber : 2;		//??? remove?
	int8_t StatChanges[MAX_STAT_CHANGES];
	uint16_t Ability;	//ability index
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
	BOOL EarnedExp;		//tags the monster so it may earn experience at the end of combat
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
	uint8_t upgradeType;
	uint8_t upgradeLvl;
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

#define NUM_SPRITES_PALLET 12

typedef struct INGAMESPRITE			///// for sprites other than the player "NPCs Sprites"
{
	char Name[MAX_NAME_LENGTH + 1];
	UPOINT WorldPos;
	POINT ScreenPos;
	DIRECTION Direction;
	uint8_t SpriteIndex;
	GAMEBITMAP Sprite[NUM_SPRITES_PALLET];
	MOVEMENTTYPE Movement;
	uint8_t MovementRemaining;
	BOOL Visible;
	BOOL Exists;
	BOOL Loaded;
	uint16_t GameAreaIndex;
	UPOINT ResetWorldPos;
	DIRECTION ResetDirection;
	UPOINT ResetOriginWorldPos;
	DIRECTION ResetOriginDirection;
	uint8_t ResetSightRange;
	uint8_t SightRange;
	UPOINT MovementRange;
	uint8_t CheckFlag;				//Checks gGameFlags[] for a required flag before setting a new flag
	uint8_t GameFlag;				//Does not exist when gGameFlags[.GameFlag] is TRUE
	uint8_t SetFlag;				//flag set upon a successful interaction and a required flag

	uint8_t AnimationFrame;
	BOOL InteractedWith;

	//TODO: get rid of these and reference a table instead
	/*char* Dialogue[MAX_DIALOGUE_BOXES];
	uint8_t DialogueFlag;
	uint8_t DialoguesBeforeLoop;
	uint8_t DialogueLoopReturn;*/

	//TODO: move these to a reference table
	/*EVENT_FLAGS Event;
	uint16_t EventItemsIndex[MAX_ITEMS_GIVE];
	uint16_t EventItemsCount[MAX_ITEMS_GIVE];
	uint8_t EventMonsterIndex;
	uint8_t EventMonsterLevel;
	uint16_t EventMonsterItem;*/

	//TODO: get rid of this and instead have a giant array of all opponent parties in the game and reference that list when battling
	//struct Monster MonsterParty[MAX_PARTY_SIZE];
	//uint8_t BattleAiFlag;


} INGAMESPRITE;

typedef struct TRIGGER_TILE
{
	UPOINT WorldPos;
	BOOL Exists;
	BOOL Loaded;
	BOOL Interactive;			//can the player press "e" on it? or do they walk over it to trigger the event
	uint16_t GameAreaIndex;
	BOOL InteractedWith;
	TRIGGER_TYPES Type;
	uint8_t GameFlag;				//Does not exist when gGameFlags[.GameFlag] is TRUE
	uint8_t SetFlag;				//flag set upon a successful interaction

} TRIGGER_TILE;

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
uint8_t gCliffEdgeTiles[NUM_CLIFF_TILES];
uint8_t gTeleportTiles[NUM_TELEPORT_TILES];

REGISTRYPARAMS gRegistryParams;

GAMEBITMAP gBackBuffer;
GAMEBITMAP g6x7Font;
GAMEBITMAP gBattleScreen_Grass01;
GAMEBITMAP gBattleScreen_StoneBricks01;

uint8_t gPlayerPartyCount;				//number of monsters in player party from 0 to 6
uint8_t gOpponentPartyCount;

struct Monster gPlayerParty[MAX_PARTY_SIZE];
struct Monster gOpponentParty[MAX_PARTY_SIZE];

struct BattleMonster gPlayerBattleParty[MAX_PARTY_SIZE];
struct BattleMonster gOpponentBattleParty[MAX_PARTY_SIZE];

GAMEBITMAP gBattleSpriteBack[NUM_MONSTERS];	//////temp while still working on MONSTERINFO struct
GAMEBITMAP gBattleSpriteFront[NUM_MONSTERS];

uint16_t gSelectedUseItem;

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
INGAMESPRITE gInitSprite[NUM_CHAR_SPRITES];
TRIGGER_TILE gTriggerTiles[NUM_TRIGGERS];

GAMEBITMAP gLootPickup;

GAMEBITMAP gSparkle01[4];
GAMEBITMAP gSparkle02[4];
GAMEBITMAP gSparkle03[4];

GAMEBITMAP gEmptySprite;

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
const SCENE_SCRIPT const gSceneScriptTable[TOTAL_SCENE_SCRIPTS][MAX_MOVEMENT_SCRIPTS];
const SCENE_XYPOS const gSceneStartingPosition[TOTAL_SCENE_SCRIPTS][MAX_NPCS_PER_SCRIPT];


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

void DrawWindow( _In_opt_ uint16_t x, _In_opt_ uint16_t y, _In_ int16_t Width, _In_ int16_t Height, _In_opt_ PIXEL32* BorderColor, _In_opt_ PIXEL32* BackgroundColor, _In_opt_ PIXEL32* ShadowColor, _In_ DWORD Flags);

void ApplyFadeIn(_In_ uint64_t FrameCounter, _In_ PIXEL32 DefaultTextColor, _Inout_ PIXEL32* TextColor, _Inout_opt_ int16_t* BrightnessAdjustment);

void EnterDialogue(void);

void ExitDialogue(void);

void DrawDialogueBox(_In_ char* Dialogue, _In_opt_ uint64_t Counter, _In_opt_ DWORD Flags);

void DrawMonsterHpBar(uint16_t x, uint16_t y, uint8_t percentHp100, uint8_t percentExp100, uint8_t monsterLevel, char* monsterNickname, BOOL showExpBar);

BOOL BlitBattleStateTextBox_Text(BATTLESTATE battlestate_wait, uint8_t battleTextLineCount, uint64_t frameCounter);

void BlitBattleStateTextBox_Wait(uint8_t battleTextLineCount);

void ReSortUsableitems(void);

void ReSortEquipableitems(void);

void ReSortValuableitems(void);

void ReSortAdventureitems(void);

void ReSortLearnableMovesFromMonster(struct DriveMonster* driveMonster);

void BlitItemDescription(char* description);

BOOL CanPlayerAffordCurrencyCost(uint32_t currencycost);

uint8_t* StringCopy(uint8_t* dest, const uint8_t* source);

uint16_t Random16(void);

void GoToDestGamestate(GAMESTATE destination);

void GoToPrevGamestate(void);