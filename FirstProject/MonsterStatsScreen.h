#pragma once




uint16_t gLearnableMoveCount;
uint16_t gLearnableMoveSort[NUM_BATTLEMOVES];

uint8_t gMonsterToViewStats;

void DrawMonsterStatsScreen(void);

void PPI_MonsterStatsScreen(void);


void DrawMoveRelearnScreen(void);


void MenuItem_MonsterStats_MoveSelected(void);

void MenuItem_MonsterStats_MoveSwitch(void);

void MenuItem_MonsterStats_MoveRemove(void);

void MenuItem_MonsterStats_MoveBack(void);

void MenuItem_MonsterStats_LearnableSelected(void);