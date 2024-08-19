#pragma once

#define NPC_NULL 0
#define NPC_BED_01 1
#define NPC_PROF 2 
#define NPC_HEAL_01 3
#define NPC_JIMMY 4
#define NPC_JERRY 5
#define NPC_STORAGE_01 6 
#define NPC_ITEM_01 7
#define NPC_HIDDENITEM_01 8


typedef struct NPC_EVENT_DATA
{
	EVENT_FLAGS Event;
	uint16_t EventItemsIndex[MAX_ITEMS_GIVE];
	uint16_t EventItemsCount[MAX_ITEMS_GIVE];
	uint8_t EventMonsterIndex;
	uint8_t EventMonsterLevel;
	uint16_t EventMonsterItem;

} NPC_EVENT_DATA;


typedef struct NPC_BATTLE_DATA
{
	struct Monster Party[MAX_PARTY_SIZE];
	uint8_t BattleAiFlag;
	uint16_t BaseReward;

} NPC_BATTLE_DATA;

NPC_EVENT_DATA gNPCEventTable[NUM_CHAR_SPRITES];

//make this its own .c file???
NPC_BATTLE_DATA gNPCBattleTable[NUM_CHAR_SPRITES];


EVENT_FLAGS GetEventFlagFromSpriteIndex(uint8_t SpriteIndex);