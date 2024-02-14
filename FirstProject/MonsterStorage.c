
#include "MonsterStorage.h"
#include "MonsterData.h"



uint8_t gDriveToSendMonster = 0;

struct MonsterStorage* gMonsterStoragePtr;
struct MonsterStorageASLR gMonsterStorageASLR = { 0 };

void SetDriveToSendMonster(uint8_t driveId)
{
	gDriveToSendMonster = driveId;
}

uint8_t GetDriveToSendMonster(void)
{
	return(gDriveToSendMonster);
}

uint8_t StorageGetCurrentDrive(void)
{
	return (gMonsterStoragePtr->CurrentDrive);
}

static void SetCurrentDrive(uint8_t DriveId)
{
	if (DriveId < TOTAL_STORAGE_DRIVES)
	{
		gMonsterStoragePtr->CurrentDrive = DriveId;
	}
}

uint32_t GetDriveMonsterDataAt(uint8_t driveId, uint8_t drivePos, int32_t field)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		return (GetDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos], field, NULL));
	}
	else
	{
		return (0);
	}
}

void SetDriveMonsterDataAt(uint8_t driveId, uint8_t drivePos, int32_t field, const void* value)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		SetDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos], field, value);
	}
}

uint32_t GetCurrentDriveMonsterData(uint8_t drivePos, int32_t field)
{
	return (GetDriveMonsterDataAt(gMonsterStoragePtr->CurrentDrive, drivePos, field));
}

void SetCurrentDriveMonsterData(uint8_t drivePos, int32_t field, const void *value)
{
	return (SetDriveMonsterDataAt(gMonsterStoragePtr->CurrentDrive, drivePos, field, value));
}

void GetDriveMonsterNicknameAt(uint8_t driveId, uint8_t drivePos, uint8_t *dest)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		GetDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos], MONSTER_DATA_NICKNAME, dest);
	}
	else
	{
		*dest = END_OF_STRING;
	}
}

void GetDriveMonsterOTNameAt(uint8_t driveId, uint8_t drivePos, uint8_t* dest)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		GetDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos], MONSTER_DATA_PLAYER_NAME, dest);
	}
	else
	{
		*dest = END_OF_STRING;
	}
}


uint32_t GetDriveMonsterLevelAt(uint8_t driveId, uint8_t drivePos)
{
	uint32_t lvl;

	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE && GetDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos], MONSTER_DATA_HAS_INDEX, NULL))
	{
		lvl = GetLevelFromDriveMonsterExp(&gMonsterStoragePtr->Drive[driveId][drivePos]);
	}
	else
	{
		lvl = 0;
	}
	return (lvl);
}

void SetDriveMonsterNicknameAt(uint8_t driveId, uint8_t drivePos, const uint8_t* nickname)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		SetDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos], MONSTER_DATA_NICKNAME, nickname);
	}
}

uint32_t GetAndCopyDriveMonsterDataAt(uint8_t driveId, uint8_t drivePos, int32_t field, void *destination)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		return (GetDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos], field, destination));
	}
	else
	{
		return (0);
	}
}

void SetDriveMonsterAt(uint8_t driveId, uint8_t drivePos, struct DriveMonster* source)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
		gMonsterStoragePtr->Drive[driveId][drivePos] = *source;
}

void CopyDriveMonsterAt(uint8_t driveId, uint8_t drivePos, struct DriveMonster* destination)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
		*destination = gMonsterStoragePtr->Drive[driveId][drivePos];
}

void CreateDriveMonsterAt(uint8_t driveId, uint8_t drivePos, uint16_t monsterIndex, uint8_t level, uint8_t fixedGenetics, uint8_t hasFixedMonsterSeed, uint32_t monsterSeed, uint32_t playerSeed)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		CreateDriveMonster(&gMonsterStoragePtr->Drive[driveId][drivePos], monsterIndex, level, fixedGenetics, hasFixedMonsterSeed, monsterSeed, playerSeed);
	}
}

void ZeroDriveMonsterAt(uint8_t driveId, uint8_t drivePos)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		ZeroDriveMonsterData(&gMonsterStoragePtr->Drive[driveId][drivePos]);
	}
}

void ConvDriveMonsterAtToMonster(uint8_t driveId, uint8_t drivePos, struct Monster* destination)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		DriveMonsterToMonster(&gMonsterStoragePtr->Drive[driveId][drivePos], destination);
	}
}

struct DriveMonster* GetDriveMonPtr(uint8_t driveId, uint8_t drivePos)
{
	if (driveId < TOTAL_STORAGE_DRIVES && drivePos < TOTAL_IN_DRIVE)
	{
		return (&gMonsterStoragePtr->Drive[driveId][drivePos]);
	}
	else
	{
		return(NULL);
	}
}

uint8_t* GetDriveNamePtr(uint8_t driveId)
{
	if (driveId < TOTAL_STORAGE_DRIVES)
	{
		return(gMonsterStoragePtr->DriveName[driveId]);
	}
	else
	{
		return(NULL);
	}
}

static uint8_t GetDriveBackground(uint8_t driveId)
{
	if (driveId < TOTAL_STORAGE_DRIVES)
	{
		return(gMonsterStoragePtr->DriveBackground[driveId]);
	}
	else
	{
		return(0);
	}
}

static void SetDriveBackground(uint8_t driveId, uint8_t backgroundId)
{
	if (driveId < TOTAL_STORAGE_DRIVES && backgroundId < DRIVE_BACKGROUND_COUNT)
	{
		gMonsterStoragePtr->DriveBackground[driveId] = backgroundId;
	}
}

int16_t AdvanceStorageMonsterIndex(struct DriveMonster* driveMonster, uint8_t currentIndex, uint8_t maxIndex, uint8_t mode)
{
	int16_t i;
	int16_t direction = -1;

	if (mode == 0 || mode == 1)
	{
		direction = 1;
	}
	if (mode == 1 || mode == 3)
	{
		for (i = (int8_t)currentIndex + direction; i >= 0 && i < maxIndex; i += direction)
		{
			if (GetDriveMonsterData(&driveMonster[i], MONSTER_DATA_INDEX, NULL) != MONSTER_NULL)
			{
				return (i);
			}
		}
	}
	return(-1);
}

BOOL CheckFreeMonsterDriveSpace(void)
{
	int32_t i, j;

	for (i = 0; i < TOTAL_STORAGE_DRIVES; i++)
	{
		for (j = 0; j < TOTAL_IN_DRIVE; j++)
		{
			if (!GetDriveMonsterData(&gMonsterStoragePtr->Drive[i][j], MONSTER_DATA_HAS_INDEX, NULL))
			{
				return(TRUE);
			}
		}
	}

	return(FALSE);
}

uint32_t CountAllStorageMonsters(void)
{
	int32_t i, j;
	uint32_t count = 0;

	for (i = 0; i < TOTAL_STORAGE_DRIVES; i++)
	{
		for (j = 0; j < TOTAL_IN_DRIVE; j++)
		{
			if (GetDriveMonsterData(&gMonsterStoragePtr->Drive[i][j], MONSTER_DATA_HAS_INDEX, NULL))
			{
				count++;
			}
		}
	}
	
	return (count);
}

void ResetDriveStorageSystem(void)
{
	uint8_t driveId, drivePos;

	SetCurrentDrive(0);
	for (driveId = 0 ; driveId < TOTAL_STORAGE_DRIVES; driveId++)
	{
		for (drivePos = 0; drivePos < TOTAL_IN_DRIVE; drivePos++)
		{
			ZeroDriveMonsterAt(driveId, drivePos);
		}
	}
	for (driveId = 0; driveId < TOTAL_STORAGE_DRIVES; driveId++)
	{
		uint8_t *drivename = StringCopy(GetDriveNamePtr(driveId), "A");
		SetDriveBackground(driveId, 1);
	}
	//ResetBackgroundUpgrades			//TODO: if I make any extra themes
}

