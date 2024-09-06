#pragma once

#define DIALOGUE_NULL 0

#define DIALOGUE_REST_1 1
#define DIALOGUE_REST_2 2
#define DIALOGUE_REST_3 3
#define DIALOGUE_REST_4 4
#define DIALOGUE_REST_5 5
#define DIALOGUE_REST_6 6
#define DIALOGUE_REST_7 7
#define DIALOGUE_REST_8 8

#define DIALOGUE_LAB_1 9
#define DIALOGUE_LAB_2 10
#define DIALOGUE_LAB_3 11

#define DIALOGUE_HEALER_1 12
#define DIALOGUE_HEALER_2 13
#define DIALOGUE_HEALER_3 14
#define DIALOGUE_HEALER_4 15
#define DIALOGUE_HEALER_5 16

#define DIALOGUE_JIMMY_1 17
#define DIALOGUE_JIMMY_2 18
#define DIALOGUE_JIMMY_3 19

#define DIALOGUE_JERRY_1 20
#define DIALOGUE_JERRY_2 21

#define DIALOGUE_STORAGE 22

#define DIALOGUE_ITEM_01 23
#define DIALOGUE_ITEM_02 24

#define DIALOGUE_HIDDENITEM_01 25
#define DIALOGUE_HIDDENITEM_02 26

#define DIALOGUE_BLOCK_01 27

#define NUM_OF_DIALOGUES (28)

typedef struct NPC_DIALOGUE
{
	char* Dialogue;
	uint8_t DialogueFlag;
	uint8_t DialoguesBeforeLoop;
	uint8_t DialogueLoopReturn;

} NPC_DIALOGUE;

NPC_DIALOGUE gNPCDialogue[NUM_CHAR_SPRITES];	//one for every NPC

const uint8_t* const gDialoguePointers[NUM_OF_DIALOGUES];


char* GetCurrentDialogueFromNPC(uint8_t NPCIndex);

void GoToNextDialogueNPC(uint8_t NPCIndex);

BOOL CheckIfLastDialogueNPC(uint8_t NPCIndex);

