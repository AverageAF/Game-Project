#include "Main.h"

const SCENE_SCRIPT const gSceneScriptTable[TOTAL_SCENE_SCRIPTS][MAX_MOVEMENT_SCRIPTS] =
{
	[NULL_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[INTRO_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[HOME_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[PROFESSOR_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = FACE_UP, .Actor = 3}, {.Script = DELAY_64, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = DIALOGUE_TRIGGER, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = FACE_DOWN, .Actor = 3}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
	[PREVENT_PROFESSOR_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = FACE_DOWN, .Actor = 9}, {.Script = DIALOGUE_TRIGGER, .Actor = 9}, {.Script = DELAY_32, .Actor = 9}, {.Script = FACE_RIGHT, .Actor = 9 }, {.Script = WALK_BW_LEFT, .Actor = 0xFF }, { .Script = END_OF_SCRIPT, .Actor = 0xFF } },
	[RIVAL_1_SCRIPT] = { {.Script = START_OF_SCRIPT, .Actor = 0xFF}, {.Script = END_OF_SCRIPT, .Actor = 0xFF} },
};

const SCENE_XYPOS const gSceneStartingPosition[TOTAL_SCENE_SCRIPTS][MAX_NPCS_PER_SCRIPT] =
{
	[NULL_SCRIPT] = { {.Actor = 0xFF, .Pos.x = 0, .Pos.y = 0} },
	[INTRO_SCRIPT] = { {.Actor = 0xFF, .Pos.x = 0, .Pos.y = 0} },
	[HOME_SCRIPT] = { {.Actor = 0xFF, .Pos.x = 0, .Pos.y = 0} },
	[PROFESSOR_SCRIPT] = { {.Actor = 3, .Pos.x = 480, .Pos.y = 4544} },
	[PREVENT_PROFESSOR_SCRIPT] = { {.Actor = 9, .Pos.x = 528, .Pos.y = 4640} },
	[RIVAL_1_SCRIPT] = { {.Actor = 0xFF, .Pos.x = 0, .Pos.y = 0} },
};