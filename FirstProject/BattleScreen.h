#pragma once


typedef enum BATTLESTATE
{
	BATTLESTATE_OPENING_TEXT,
	BATTLESTATE_WAIT_INPUT1,
	BATTLESTATE_CALCULATE,
	BATTLESTATE_RUN_FIGHT,
	BATTLESTATE_SWITCH_FIGHT,				///////TODO harder difficulties where there is no running or item bag
	BATTLESTATE_CHOOSE_MOVE,
	BATTLESTATE_CHOOSE_MONSTER,
	BATTLESTATE_CHOOSE_ITEM,

} BATTLESTATE;

typedef enum NPC_AI_FLAG
{
	FLAG_NPCAI_RANDOM,
	FLAG_NPCAI_NEVERSTATUS,
	FLAG_NPCAI_HIGHESTPOWER,
} NPC_AI_FLAG;

BATTLESTATE gPreviousBattleState;
BATTLESTATE gCurrentBattleState;

uint8_t gCurrentPartyMember;
uint8_t gCurrentOpponentPartyMember;
uint8_t gSelectedPlayerMove;

void PPI_BattleScreen(void);

void DrawBattleScreen(void);

void DrawBattleButtons(void);

void DrawMoveButtons(void);

void DrawMonsterHpBar(uint16_t x, uint16_t y, uint8_t percentHp100, uint8_t percentExp100, uint8_t monsterLevel, char* monsterNickname);


void MenuItem_BattleScreen_FightButton(void);

void MenuItem_BattleScreen_EscapeButton(void);

void MenuItem_BattleScreen_SwitchButton(void);

void MenuItem_BattleScreen_ItemsButton(void);


void MenuItem_SwitchScreen_PartySlot0(void);

void MenuItem_SwitchScreen_PartySlot1(void);

void MenuItem_SwitchScreen_PartySlot2(void);

void MenuItem_SwitchScreen_PartySlot3(void);

void MenuItem_SwitchScreen_PartySlot4(void);

void MenuItem_SwitchScreen_PartySlot5(void);

void MenuItem_SwitchScreen_BackButton(void);


void MenuItem_MoveScreen_MoveSlot0(void);

void MenuItem_MoveScreen_MoveSlot1(void);

void MenuItem_MoveScreen_MoveSlot2(void);

void MenuItem_MoveScreen_MoveSlot3(void);

void MenuItem_MoveScreen_SignatureMove(void);

void MenuItem_MoveScreen_BackButton(void);

uint8_t CalculateOpponentMoveChoice(uint8_t npcaiFlag);

BOOL CalculateSpeedPriorityIfPlayerMovesFirst(uint16_t speedStatPlayer, uint16_t speedStatOpponent);

uint16_t CalcPotentialDamageToPlayerMonster(uint8_t oppLevel, uint16_t oppMonAtk, uint16_t playerMonDef, uint16_t oppMonPsi, uint16_t playerMonRes, uint8_t movePower1, uint8_t movePower2, uint8_t movePower3, uint8_t split);

uint16_t CalcPotentialDamageToOpponentMonster(uint8_t playerLevel, uint16_t playerMonAtk, uint16_t oppMonDef, uint16_t playerMonPsi, uint16_t oppMonRes, uint8_t movePower1, uint8_t movePower2, uint8_t movePower3, uint8_t split);

void ModiftyMonsterHealthValuesThisTurn(uint16_t damageToPlayer, uint16_t damageToOpponent, BOOL isPlayerGoingFirst);