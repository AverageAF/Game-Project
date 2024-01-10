#pragma once


typedef enum BATTLESTATE
{
	BATTLESTATE_OPENING_TEXT,
	BATTLESTATE_OPENING_WAIT,
	BATTLESTATE_TURNORDER_CALC,
	BATTLESTATE_FIRSTMOVE_TEXT,
	BATTLESTATE_FIRSTMOVE_WAIT,
	BATTLESTATE_FIRSTMOVE_CALC,
	BATTLESTATE_POSTFIRSTMOVE_TEXT,
	BATTLESTATE_SECONDMOVE_TEXT,
	BATTLESTATE_SECONDMOVE_WAIT,
	BATTLESTATE_SECONDMOVE_CALC,
	BATTLESTATE_POSTSECONDMOVE_TEXT,
	BATTLESTATE_KO,
	BATTLESTATE_KO_WAIT,
	BATTLESTATE_REWARD,
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


////////////MENU//BUTTONS//////////////

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

////////////MENU//BUTTONS//////////////

uint8_t CalculateOpponentMoveChoice(uint8_t npcaiFlag);

BOOL CalculateSpeedPriorityIfPlayerMovesFirst(uint16_t speedStatPlayer, uint16_t speedStatOpponent);

uint16_t CalcPotentialDamageToPlayerMonster(uint8_t oppLevel, uint16_t oppMonAtk, uint16_t playerMonDef, uint16_t oppMonPsi, uint16_t playerMonRes, uint8_t movePower1, uint8_t movePower2, uint8_t movePower3, uint8_t split);

uint16_t CalcPotentialDamageToOpponentMonster(uint8_t playerLevel, uint16_t playerMonAtk, uint16_t oppMonDef, uint16_t playerMonPsi, uint16_t oppMonRes, uint8_t movePower1, uint8_t movePower2, uint8_t movePower3, uint8_t split);

BOOL ModifyMonsterHealthValueGetKO(uint16_t damageToMonster, BOOL isPlayerSideMonster);