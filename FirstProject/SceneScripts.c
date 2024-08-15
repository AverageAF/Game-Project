#include "Main.h"

const SCENE_SCRIPT const gSceneScriptTable[TOTAL_SCENE_SCRIPTS][MAX_MOVEMENT_SCRIPTS] =
{
	[NULL_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[INTRO_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[HOME_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[PROFESSOR_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = FACE_UP, .Actor = 3}, {.Script = DELAY_64, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = DIALOGUE_TRIGGER, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = FACE_DOWN, .Actor = 3}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[RIVAL_1_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
};