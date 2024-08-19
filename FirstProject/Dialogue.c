#include "Main.h"

#include "NPCData.h"
#include "Dialogue.h"

static const uint8_t dNullDialogue[] = "NULL!\nnull\nnull\nnull...";

static const uint8_t dRestHealing1[] = "It looks comfy enough to nap,\nyou decide to lie down.";
static const uint8_t dRestHealing2[] = "...";
static const uint8_t dRestHealing3[] = "...zzzz...zzzzzz...";
static const uint8_t dRestHealing4[] = "...zzzz...";
static const uint8_t dRestHealing5[] = "...zzz.....zzzzzzzz...";
static const uint8_t dRestHealing6[] = "...zzzzzzz...zzzz...";
static const uint8_t dRestHealing7[] = "...";
static const uint8_t dRestHealing8[] = "Your party feels refreshed,\nafter a quick nap!";

static const uint8_t dFirstMonster1[] = "Hello and welcome to my lab!\nI can see you are interested\nin aquiring an elemental\nmonster to help you on\nyour new journey!";
static const uint8_t dFirstMonster2[] = "Would you be interested in\nhelping me with my research\nin exchange for one of the\n four I have here?";
static const uint8_t dFirstMonster3[] = "Wonderful! I knew I could\n count on you to help me\nlearn more about them! Now,\nplease have this one!";

static const uint8_t dHospitalWorker1[] = "I can heal your party to full\n health, just one moment!";
static const uint8_t dHospitalWorker2[] = "...";
static const uint8_t dHospitalWorker3[] = "...";
static const uint8_t dHospitalWorker4[] = "...";
static const uint8_t dHospitalWorker5[] = "There! Your party has healed\n to their full health!";

static const uint8_t dJimmyBattle1[] = "Are you about to go into the\n forest too?";
static const uint8_t dJimmyBattle2[] = "Battle me first to see if you\nare ready for the wild\nmonsters in there!";
static const uint8_t dJimmyBattle3[] = "Maybe I am the one who needs\nto train more...";

static const uint8_t dJerryBattle1[] = "I see you! That means I am\nchallenging you to a battle!";
static const uint8_t dJerryBattle2[] = "Wow! You are much stronger\nthan you look!";

static const uint8_t dStorageSystem[] = "You booted up the monster DSS,\n accessing your party and caught monsters.";

static const uint8_t dSmallItem01[] = "You found some HP items!";
static const uint8_t dSmallItem02[] = "You put them into the USABLE\npocket of your bag.";

static const uint8_t dHiddenItem01[] = "You found some hidden HP items!";
static const uint8_t dHiddenItem02[] = "You put them into the USABLE\npocket of your bag.";

const uint8_t* const gDialoguePointers[NUM_OF_DIALOGUES] = {

	[DIALOGUE_NULL] = dNullDialogue,

	[DIALOGUE_REST_1] = dRestHealing1,
	[DIALOGUE_REST_2] = dRestHealing2,
	[DIALOGUE_REST_3] = dRestHealing3,
	[DIALOGUE_REST_4] = dRestHealing4,
	[DIALOGUE_REST_5] = dRestHealing5,
	[DIALOGUE_REST_6] = dRestHealing6,
	[DIALOGUE_REST_7] = dRestHealing7,
	[DIALOGUE_REST_8] = dRestHealing8,

	[DIALOGUE_LAB_1] = dFirstMonster1,
	[DIALOGUE_LAB_2] = dFirstMonster2,
	[DIALOGUE_LAB_3] = dFirstMonster3,

	[DIALOGUE_HEALER_1] = dHospitalWorker1,
	[DIALOGUE_HEALER_2] = dHospitalWorker2,
	[DIALOGUE_HEALER_3] = dHospitalWorker3,
	[DIALOGUE_HEALER_4] = dHospitalWorker4,
	[DIALOGUE_HEALER_5] = dHospitalWorker5,

	[DIALOGUE_JIMMY_1] = dJimmyBattle1,
	[DIALOGUE_JIMMY_2] = dJimmyBattle2,
	[DIALOGUE_JIMMY_3] = dJimmyBattle3,

	[DIALOGUE_JERRY_1] = dJerryBattle1,
	[DIALOGUE_JERRY_2] = dJerryBattle2,

	[DIALOGUE_STORAGE] = dStorageSystem,

	[DIALOGUE_ITEM_01] = dSmallItem01,
	[DIALOGUE_ITEM_02] = dSmallItem02,

	[DIALOGUE_HIDDENITEM_01] = dHiddenItem01,
	[DIALOGUE_HIDDENITEM_02] = dHiddenItem02,


};

NPC_DIALOGUE gNPCDialogue[NUM_CHAR_SPRITES] =
{
	[NPC_NULL] =
	{
		.DialogueFlag = DIALOGUE_NULL,
		.DialogueLoopReturn = DIALOGUE_NULL,
		.DialoguesBeforeLoop = DIALOGUE_NULL,
	},
	[NPC_BED_01] =
	{
		.DialogueFlag = DIALOGUE_REST_1,
		.DialogueLoopReturn = DIALOGUE_REST_1,
		.DialoguesBeforeLoop = DIALOGUE_REST_8,
	},
	[NPC_PROF] =
	{
		.DialogueFlag = DIALOGUE_LAB_1,
		.DialogueLoopReturn = DIALOGUE_LAB_1,
		.DialoguesBeforeLoop = DIALOGUE_LAB_3,
	},
	[NPC_HEAL_01] = 
	{
		.DialogueFlag = DIALOGUE_HEALER_1,
		.DialogueLoopReturn = DIALOGUE_HEALER_1,
		.DialoguesBeforeLoop = DIALOGUE_HEALER_5,
	},
	[NPC_JIMMY] =
	{
		.DialogueFlag = DIALOGUE_JIMMY_1,
		.DialogueLoopReturn = DIALOGUE_JIMMY_3,
		.DialoguesBeforeLoop = DIALOGUE_JIMMY_3,
	},
	[NPC_JERRY] =
	{
		.DialogueFlag = DIALOGUE_JERRY_1,
		.DialogueLoopReturn = DIALOGUE_JERRY_2,
		.DialoguesBeforeLoop = DIALOGUE_JERRY_2,
	},
	[NPC_STORAGE_01] =
	{
		.DialogueFlag = DIALOGUE_STORAGE,
		.DialogueLoopReturn = DIALOGUE_STORAGE,
		.DialoguesBeforeLoop = DIALOGUE_STORAGE,
	},
	[NPC_ITEM_01] =
	{
		.DialogueFlag = DIALOGUE_ITEM_01,
		.DialogueLoopReturn = DIALOGUE_ITEM_01,
		.DialoguesBeforeLoop = DIALOGUE_ITEM_02,
	},
	[NPC_HIDDENITEM_01] =
	{
		.DialogueFlag = DIALOGUE_HIDDENITEM_01,
		.DialogueLoopReturn = DIALOGUE_HIDDENITEM_01,
		.DialoguesBeforeLoop = DIALOGUE_HIDDENITEM_02,
	},

};

//string based on NPC's current dialogue position
char* GetCurrentDialogueFromNPC(uint8_t NPCIndex)
{
	return(gDialoguePointers[gNPCDialogue[NPCIndex].DialogueFlag]);
}

//advances to the next dialogue, if it is the last it will loop back to the return point
void GoToNextDialogueNPC(uint8_t NPCIndex)
{
	if (gNPCDialogue[NPCIndex].DialogueFlag >= gNPCDialogue[NPCIndex].DialoguesBeforeLoop)
	{
		gNPCDialogue[NPCIndex].DialogueFlag = gNPCDialogue[NPCIndex].DialogueLoopReturn;
	}
	else
	{
		gNPCDialogue[NPCIndex].DialogueFlag++;
	}
}

//returns true if current dialogue is >= loop point
BOOL CheckIfLastDialogueNPC(uint8_t NPCIndex)
{
	if (gNPCDialogue[NPCIndex].DialogueFlag >= gNPCDialogue[NPCIndex].DialoguesBeforeLoop)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}