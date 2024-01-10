#pragma once

#include <stdint.h>  

//// Might move this to its own Monster.h file
void ZeroPcMonsterData(struct PCMonster* pcMonster);

void ZeroMonsterData(struct Monster* monster);

void ZeroPlayerPartyMonsters(void);

void ZeroOpponentPartyMonsters(void);

void CreateMonster(struct Monster* monster, uint16_t Index, uint8_t Level, uint8_t FixedGenetics);

void CreatePCMonster(struct PCMonster* pcMonster, uint16_t Index, uint8_t Level, uint8_t FixedGenetics);

uint8_t GetLevelFromMonsterExp(struct Monster* monster);

uint8_t GetLevelFromPCMonsterExp(struct PCMonster* pcMonster);

void GiveMonsterInitialMoveset(struct Monster* monster);

void GivePCMonsterInitialMoveset(struct PCMonster* pcMonster);

uint16_t GiveMoveToMonster(struct Monster* monster, uint16_t move);

static uint16_t GiveMoveToPCMonster(struct PCMonster* pcMonster, uint16_t move);

void DeleteFirstMoveGiveMoveToMonster(struct Monster* monster, uint16_t move);

void DeleteFirstMoveGiveMoveToPCMonster(struct PCMonster* pcMonster, uint16_t move);

static void RemoveGenesIndexFromList(uint8_t* genes, uint8_t selectedGene);

void SetMonsterData(struct Monster* monster, int32_t field, const void* dataArgument);

void SetPCMonsterData(struct PCMonster* pcMonster, int32_t field, const void* dataArgument);

uint16_t GiveMoveToBattleMonster(struct BattleMonster* bMonster, uint16_t move);

void SetMonsterMoveSlot(struct Monster* monster, uint16_t move, uint8_t slot);

void SetBattleMonsterMoveSlot(struct BattleMonster* bMonster, uint16_t move, uint8_t slot);

uint16_t MonsterTryLearningNewMove(struct Monster* monster, BOOL firstMove);

void CopyMonster(void* destination, void* source, size_t size);

void PCMonsterToMonster(const struct PCMonster* source, struct Monster* destination);

uint32_t GetMonsterData(struct Monster* monster, int32_t field, uint8_t* data);

uint32_t GetPCMonsterData(struct PCMonster* pcMonster, int32_t field, uint8_t* data);

void CalculateMonsterStats(struct Monster* monster);

static void EncryptPCMonster(struct PCMonster* pcMonster);

static void DecryptPCMonster(struct PCMonster* pcMonster);

static union MonsterSubstruct* GetSubstruct(struct PCMonster* pcMonster, uint32_t MonsterSeed, uint8_t substructType);

static uint16_t CalculatePCMonsterCheckSum(struct PCMonster* pcMonster);
