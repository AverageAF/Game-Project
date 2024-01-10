#pragma once 

#include "Main.h"

void ZeroDriveMonsterData(struct DriveMonster* driveMonster);

void ZeroMonsterData(struct Monster* monster);

void ZeroPlayerPartyMonsters(void);

void ZeroOpponentPartyMonsters(void);

void CreateMonster(struct Monster* monster, uint8_t Index, uint8_t Level, uint8_t FixedGenetics, uint8_t hasFixedMonsterSeed, uint32_t FixedMonsterSeed, uint32_t FixedPlayerSeed);

void CreateDriveMonster(struct DriveMonster* driveMonster, uint8_t Index, uint8_t Level, uint8_t FixedGenetics, uint8_t hasFixedMonsterSeed, uint32_t FixedMonsterSeed, uint32_t FixedPlayerSeed);

uint8_t GetLevelFromMonsterExp(struct Monster* monster);

uint8_t GetLevelFromDriveMonsterExp(struct DriveMonster* driveMonster);

void GiveMonsterInitialMoveset(struct Monster* monster);

void GiveDriveMonsterInitialMoveset(struct DriveMonster* driveMonster);

uint16_t GiveMoveToMonster(struct Monster* monster, uint16_t move);

static uint16_t GiveMoveToDriveMonster(struct DriveMonster* driveMonster, uint16_t move);

void DeleteFirstMoveGiveMoveToMonster(struct Monster* monster, uint16_t move);

void DeleteFirstMoveGiveMoveToDriveMonster(struct DriveMonster* driveMonster, uint16_t move);

static void RemoveGenesIndexFromList(uint8_t* genes, uint8_t selectedGene);

void SetMonsterData(struct Monster* monster, int32_t field, const void* dataArgument);

void SetDriveMonsterData(struct DriveMonster* driveMonster, int32_t field, const void* dataArgument);

uint16_t GiveMoveToBattleMonster(struct BattleMonster* bMonster, uint16_t move);

void SetMonsterMoveSlot(struct Monster* monster, uint16_t move, uint8_t slot);

void SetBattleMonsterMoveSlot(struct BattleMonster* bMonster, uint16_t move, uint8_t slot);

uint16_t MonsterTryLearningNewMove(struct Monster* monster, BOOL firstMove);

void CopyMonster(struct Monster destination, struct Monster source);

void DriveMonsterToMonster(const struct DriveMonster* source, struct Monster* destination);

uint32_t GetMonsterData(struct Monster* monster, int32_t field, uint8_t* data);

uint32_t GetDriveMonsterData(struct DriveMonster* driveMonster, int32_t field, uint8_t* data);

void CalculateMonsterStats(struct Monster* monster);

//static void EncryptDriveMonster(struct DriveMonster* driveMonster);

//static void DecryptDriveMonster(struct DriveMonster* driveMonster);

//static union MonsterSubstruct* GetSubstruct(struct DriveMonster* driveMonster, uint32_t MonsterSeed, uint8_t substructType);

//static uint16_t CalculateDriveMonsterCheckSum(struct DriveMonster* driveMonster);

uint8_t CountAliveMonstersInBattle(uint8_t caseId);

uint8_t GetMonsterGender(struct Monster* monster);

uint8_t GetDriveMonsterGender(struct DriveMonster* driveMonster);

uint8_t GetGenderFromMonsterIndexAndSeed(uint8_t monsterIndex, uint32_t monsterSeed);

uint8_t GiveMonsterToPlayer(struct Monster* monster);

uint8_t SendMonsterToPC(struct Monster* monster);

void GetMonsterNameFromIndex(uint8_t* name, uint8_t index);

uint16_t GetAbilityByIndex(uint16_t index, uint8_t abilityNum);

uint16_t GetMonAbility(struct Monster* monster);

void MonsterToBattleMonster(struct Monster* source, struct BattleMonster* dest);

uint8_t* StringCopy_NickName(uint8_t* dest, const uint8_t* source);
