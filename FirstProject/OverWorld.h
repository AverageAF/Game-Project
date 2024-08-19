#pragma once

typedef struct PORTCOORDS
{
	//index for this portal, used for others to target this one
	//uint16_t PortIndex;

	////where is the source portal in world coordinates
	UPOINT WorldPos;

	////add this many world coords to current world position
	//POINT WorldPosAdd;

	//index to target another portal, 
	uint16_t PortDestIndex;

	////location of new camera upon teleporting to destination
	UPOINT CameraDest;

	////which area is the player in after teleporting to their dest
	GAMEAREA AreaDest;

} PORTCOORDS;

typedef struct TransitionArea
{
	uint16_t TransAreaDestIndex;

	//Area where player.worldpos must be standing to trigger a transition
	RECT TilesArea;

	//add this many screen coords to current screen position
	//POINT ScreenPosAdd;

	//add this many world coords to the current camera postition
	POINT CameraPosAdd;

} TransitionArea;

#define NUM_TRANS_AREAS 16
#define MAX_TRANS_PERMAP 4 + 1		//4 transitions + 1 for none

uint8_t gTransitionsPerMap[NUM_TRANS_AREAS];		//always between 0 - 4
TransitionArea gTransitionAreas[NUM_TRANS_AREAS][MAX_TRANS_PERMAP];	//5 is possible number of transitions in one map 0 - 4

BOOL gHasPlayerTransitioned;

#define UNIQUE_TELEPADS_COUNT 12

PORTCOORDS gPortCoords[UNIQUE_TELEPADS_COUNT];

PORTCOORDS gTeleport000;
PORTCOORDS gTeleport001;
PORTCOORDS gTeleport002;
PORTCOORDS gTeleport003;
PORTCOORDS gTeleport004;
PORTCOORDS gTeleport005;
PORTCOORDS gTeleport006;
PORTCOORDS gTeleport007;
PORTCOORDS gTeleport008;
PORTCOORDS gTeleport009;
PORTCOORDS gTeleport010;
PORTCOORDS gTeleport011;

#define NUM_GAME_AREAS 10

GAMEAREA gCurrentArea;

GAMEAREA gOverworldArea;	////TODO change to fit new gamemap size, used to know the size of the whole map


GAMEAREA gHomeArea;

GAMEAREA gStartingTownArea;

GAMEAREA gRoute01Area;

GAMEAREA gBattleTown01Area;

GAMEAREA gForest01Area;

GAMEAREA gRoute02Area;

GAMEAREA gBattleTown02Area;

GAMEAREA gGameAreas[NUM_GAME_AREAS];

GAMEAREA gStartingLabArea;

GAMEAREA gBedroomArea;

#define NUM_ENCOUNTER_AREAS 9

ENCOUNTERAREA gOverworld01EncounterArea;

ENCOUNTERAREA gOverworld02EncounterArea;

ENCOUNTERAREA gOverworld03EncounterArea;

ENCOUNTERAREA gOverworld04EncounterArea;

ENCOUNTERAREA gOverworld05EncounterArea;

ENCOUNTERAREA gOverworld06EncounterArea;

ENCOUNTERAREA gOverworld07EncounterArea;

ENCOUNTERAREA gOverworld08EncounterArea;

ENCOUNTERAREA gNullEncounterArea;

ENCOUNTERAREA gCurrentEncounterArea;

ENCOUNTERAREA gEncounterAreas[NUM_ENCOUNTER_AREAS];


void DrawOverworldScreen(void);

void PPI_Overworld(void);

void TeleportHandler(void);

void TriggerNPCMovement(_In_ uint64_t Counter);

uint8_t ScriptGiveMonster(uint8_t index, uint8_t level, uint16_t item);

void HealPlayerParty(void);

void TeleportPlayerBlackOut(void);

void GivePlayerEquipItem(uint16_t itemIndex, uint16_t amount);

void GivePlayerUseItem(uint16_t itemIndex, uint16_t amount);

void GivePlayerValueItem(uint16_t itemIndex, uint16_t amount);

void GivePlayerAdventureItem(uint16_t itemIndex, uint16_t amount);

void ModifyCharVisibility(void);

void DrawBuySellBackBox(void);
uint8_t PPI_BuySellBackBox(void);

BOOL MapTransition(void);

void LoadUnloadSpritesVIAGameArea(void);
void LoadUnloadTriggerVIAGameArea(void);

INGAMESPRITE CharSpriteSparkleAnim(INGAMESPRITE _Inout_ charactersprite, uint16_t _In_ counter, GAMEBITMAP _In_ sparkletype[]);

INGAMESPRITE GivePlayerItemFromCharSpriteEvent(INGAMESPRITE _Inout_ charactersprite, uint8_t _In_ itemtype);

void GiveItemAndRemoveSprite(uint8_t spriteIndex);

void GiveItemChangeNPCEvent(uint8_t spriteIndex);

INGAMESPRITE GivePlayerItemFromCharAndRemoveSprite(INGAMESPRITE _Inout_ charactersprite, uint8_t _In_ itemtype);

void RemoveCharSprite(uint8_t Index);

void CharSpriteDrawHandler(uint16_t BrightnessAdjustment);

BOOL CharSpriteInteractionHandler(uint64_t LocalFrameCounter);

void DisplayDebugTiles(void);

BOOL ApplyMovementScriptSprite(_In_ SCENE_SCRIPT scriptarray[], _In_ uint16_t xstart, _In_ uint16_t ystart, _In_ BOOL removeSpriteAfterScript);

BOOL CheckNPCHasLOSWithPlayer(uint8_t spriteId);

void InitiateDialogueAndCutscene(uint64_t counter);

void TurnNPCToPlayerStartDialogue(void);

void HandlePlayerCollision(void);

void HandleCameraPlayerMovementAndCharVisibility(void);

void HandleTileFunctions(void);

void HandleNPCEvent(void);

BOOL IsPlayerOnActiveTrigger(uint8_t index);

BOOL TriggerInteractionHandler(void);

BOOL InteractWithTrigger(void);

void PopulateSceneScriptArray(uint8_t triggertileindex);

void ClearSceneScriptArray(void);