#pragma once


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

} BATTLESTATE;

typedef enum NPC_AI_FLAG
{
	FLAG_NPCAI_RANDOM,
	FLAG_NPCAI_NEVERSTATUS,
	FLAG_NPCAI_HIGHESTPOWER,
} NPC_AI_FLAG;

BATTLESTATE gPreviousBattleState;
BATTLESTATE gCurrentBattleState;

uint8_t gPartyMemberToSwitchIn;
uint8_t gCurrentPartyMember;
uint8_t gCurrentOpponentPartyMember;
uint16_t gSelectedPlayerMove;
uint16_t gSelectedOpponentMove;

char gBattleTextLine[MAX_DIALOGUE_ROWS + 1][MAX_BATTLECHAR_PER_ROW];      //first line of dialogue in combat text

void PPI_BattleScreen(void);

void DrawBattleScreen(void);

void DrawBattleButtons(void);

void DrawMoveButtons(void);

void DrawMonsterPartyButtons(void);

void DrawUseableItemsButtons(void);


////////////MENU//BUTTONS//////////////

void MenuItem_BattleScreen_FightButton(void);

void MenuItem_BattleScreen_EscapeButton(void);

void MenuItem_BattleScreen_SwitchButton(void);

void MenuItem_BattleScreen_ItemsButton(void);


void MenuItem_SwitchScreen_PartySelected(void);
//
//void MenuItem_SwitchScreen_PartySlot1(void);
//
//void MenuItem_SwitchScreen_PartySlot2(void);
//
//void MenuItem_SwitchScreen_PartySlot3(void);
//
//void MenuItem_SwitchScreen_PartySlot4(void);
//
//void MenuItem_SwitchScreen_PartySlot5(void);

void MenuItem_SwitchScreen_BackButton(void);


void MenuItem_MoveScreen_MoveSlot0(void);

void MenuItem_MoveScreen_MoveSlot1(void);

void MenuItem_MoveScreen_MoveSlot2(void);

void MenuItem_MoveScreen_MoveSlot3(void);

void MenuItem_MoveScreen_SignatureMove(void);

void MenuItem_MoveScreen_BackButton(void);


void MenuItem_UseableScreen_SlotSelected(void);

void MenuItem_UseableScreen_BackButton(void);

////////////MENU//BUTTONS//////////////

uint8_t CalculateOpponentMoveChoice(uint8_t npcaiFlag);

BOOL CalculateSpeedPriorityIfPlayerMovesFirst(uint16_t speedStatPlayer, uint16_t speedStatOpponent);

uint16_t CalcDmgFromMonsterAToMonsterB(uint8_t AMonLevel, uint16_t AMonAtk, uint16_t BMonDef, uint16_t AMonPsi, uint16_t BMonRes, uint8_t AMovePower1, uint8_t AMovePower2, uint8_t AMovePower3, uint8_t AMoveSplit, BOOL IsMonAPlayer);

void ModifyMonsterHealthValueGetKO(uint16_t damageToMonster, BOOL isPlayerSideMonster, BOOL wasLastMoveCrit);

uint16_t GetElementaBonusDamage(uint16_t damageBeforeElement, BOOL isPlayerMonsterMoveTarget);

void GenerateOpponentMove(uint8_t Opponent);