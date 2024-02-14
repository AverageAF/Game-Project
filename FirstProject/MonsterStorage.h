#pragma once

#include "Main.h"


#define TOTAL_STORAGE_DRIVES 8
#define ROWS_IN_DRIVE 8
#define COLUMNS_IN_DRIVE 8
#define TOTAL_IN_DRIVE (ROWS_IN_DRIVE * COLUMNS_IN_DRIVE)
#define DRIVE_NAME_LENGTH 8

#define DRIVE_BACKGROUND_COUNT 8

#define MOVE_BLOCK_RANGE 0x80

struct MonsterStorage
{
	uint8_t CurrentDrive;
	struct DriveMonster Drive[TOTAL_STORAGE_DRIVES][TOTAL_IN_DRIVE];
	uint8_t DriveName[TOTAL_STORAGE_DRIVES][DRIVE_NAME_LENGTH + 1];
	uint8_t DriveBackground[TOTAL_STORAGE_DRIVES];

};

//address space layout randomization
struct MonsterStorageASLR
{
	struct MonsterStorage block;
	uint8_t aslr[MOVE_BLOCK_RANGE];
};

extern struct MonsterStorage *gMonsterStoragePtr;
extern struct MonsterStorageASLR gMonsterStorageASLR;

void SetDriveToSendMonster(uint8_t driveId);
uint8_t GetDriveToSendMonster(void);

uint8_t StorageGetCurrentDrive(void);

static void SetCurrentDrive(uint8_t DriveId);

uint32_t GetDriveMonsterDataAt(uint8_t driveId, uint8_t drivePos, int32_t field);

void SetDriveMonsterDataAt(uint8_t driveId, uint8_t drivePos, int32_t field, const void* value);

uint32_t GetCurrentDriveMonsterData(uint8_t drivePos, int32_t field);

void SetCurrentDriveMonsterData(uint8_t drivePos, int32_t field, const void* value);

void GetDriveMonsterNicknameAt(uint8_t driveId, uint8_t drivePos, uint8_t* dest);

void GetDriveMonsterOTNameAt(uint8_t driveId, uint8_t drivePos, uint8_t* dest);

uint32_t GetDriveMonsterLevelAt(uint8_t driveId, uint8_t drivePos);

void SetDriveMonsterNicknameAt(uint8_t driveId, uint8_t drivePos, const uint8_t* nickname);

uint32_t GetAndCopyDriveMonsterDataAt(uint8_t driveId, uint8_t drivePos, int32_t field, void* destination);

void SetDriveMonsterAt(uint8_t driveId, uint8_t drivePos, struct DriveMonster* source);

void CopyDriveMonsterAt(uint8_t driveId, uint8_t drivePos, struct DriveMonster* destination);

void CreateDriveMonsterAt(uint8_t driveId, uint8_t drivePos, uint16_t monsterIndex, uint8_t level, uint8_t fixedGenetics, uint8_t hasFixedMonsterSeed, uint32_t monsterSeed, uint32_t playerSeed);

void ZeroDriveMonsterAt(uint8_t driveId, uint8_t drivePos);

void ConvDriveMonsterAtToMonster(uint8_t driveId, uint8_t drivePos, struct Monster* destination);

struct DriveMonster* GetDriveMonPtr(uint8_t driveId, uint8_t drivePos);

uint8_t* GetDriveNamePtr(uint8_t driveId);

static uint8_t GetDriveBackground(uint8_t driveId);

static void SetDriveBackground(uint8_t driveId, uint8_t backgroundId);

int16_t AdvanceStorageMonsterIndex(struct DriveMonster* driveMonster, uint8_t currentIndex, uint8_t maxIndex, uint8_t mode);

BOOL CheckFreeMonsterDriveSpace(void);

uint32_t CountAllStorageMonsters(void);

void ResetDriveStorageSystem(void);
