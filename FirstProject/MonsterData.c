

#include "BaseStats.h"
#include "SimpleConstants.h"
#include "LevelUpMoves.h"
#include "ExperienceTables.h"

#include "MonsterData.h"

#include "OverWorld.h"

const uint32_t gBitTable[] =
{
    1 << 0,
    1 << 1,
    1 << 2,
    1 << 3,
    1 << 4,
    1 << 5,
    1 << 6,
    1 << 7,
    1 << 8,
    1 << 9,
    1 << 10,
    1 << 11,
    1 << 12,
    1 << 13,
    1 << 14,
    1 << 15,
    1 << 16,
    1 << 17,
    1 << 18,
    1 << 19,
    1 << 20,
    1 << 21,
    1 << 22,
    1 << 23,
    1 << 24,
    1 << 25,
    1 << 26,
    1 << 27,
    1 << 28,
    1 << 29,
    1 << 30,
    1 << 31,
};

uint16_t gLastAbilityUsed = 0;

/////////////////////////////////////////////////////////////////////MONSTER DATA FUNCTIONS/////////////////////
void ZeroDriveMonsterData(struct DriveMonster* driveMonster)
{
    uint8_t* raw = (uint8_t*)driveMonster;
    uint32_t i;
    for (i = 0; i < sizeof(struct DriveMonster); i++)
    {
        raw[i] = 0;
    }
}

void ZeroMonsterData(struct Monster* monster)
{
    uint32_t argument;
    ZeroDriveMonsterData(&monster->DriveMonster);
    argument = 0;
    SetMonsterData(monster, MONSTER_DATA_STATUS, &argument);
    SetMonsterData(monster, MONSTER_DATA_LEVEL, &argument);
    SetMonsterData(monster, MONSTER_DATA_HEALTH, &argument);
    SetMonsterData(monster, MONSTER_DATA_MAX_HEALTH, &argument);
    SetMonsterData(monster, MONSTER_DATA_ATTACK, &argument);
    SetMonsterData(monster, MONSTER_DATA_DEFENSE, &argument);
    SetMonsterData(monster, MONSTER_DATA_SPEED, &argument);
    SetMonsterData(monster, MONSTER_DATA_PSI, &argument);
    SetMonsterData(monster, MONSTER_DATA_RESOLVE, &argument);
}

void ZeroPlayerPartyMonsters(void)
{
    int32_t i;
    for (i = 0; i < MAX_PARTY_SIZE; i++)
    {
        ZeroMonsterData(&gPlayerParty[i]);
    }
}

void ZeroOpponentPartyMonsters(void)
{
    int32_t i;
    for (i = 0; i < MAX_PARTY_SIZE; i++)
    {
        ZeroMonsterData(&gOpponentParty[i]);
    }
}

//
//
void CreateMonster(struct Monster* monster, uint8_t Index, uint8_t Level, uint8_t FixedGenetics, uint8_t hasFixedMonsterSeed, uint32_t FixedMonsterSeed, uint32_t FixedPlayerSeed)
{
    ZeroMonsterData(monster);
    CreateDriveMonster(&monster->DriveMonster, Index, Level, FixedGenetics, hasFixedMonsterSeed, FixedMonsterSeed, FixedPlayerSeed);
    SetMonsterData(monster, MONSTER_DATA_LEVEL, &Level);
    CalculateMonsterStats(monster);
}

//
//TODO: FINISH
void CreateDriveMonster(struct DriveMonster* driveMonster, uint8_t Index, uint8_t Level, uint8_t FixedGenetics, uint8_t hasFixedMonsterSeed, uint32_t FixedMonsterSeed, uint32_t FixedPlayerSeed)
{
    uint8_t MonsterName[MAX_MONSTER_NAME_LENGTH + 1] = { 0 };
    uint32_t MonsterSeed;
    uint16_t MetLocation;
    uint32_t Value;
    uint16_t checksum;
    uint8_t i;
    uint8_t AvailableGenetics[NUM_MONSTER_STATS];
    uint8_t SelectedGenetics[LEGENDARY_GENETICS_PERFECT_COUNT];

    ZeroDriveMonsterData(driveMonster);

    if (hasFixedMonsterSeed)
    {
        MonsterSeed = FixedMonsterSeed;
    }
    else
    {
        rand_s((unsigned int*)&MonsterSeed);
    }

    if (FixedPlayerSeed)
    {
        Value = FixedPlayerSeed;
    }
    else
    {
        Value = gPlayer.Seed;
    }

    SetDriveMonsterData(driveMonster, MONSTER_DATA_MONSTER_SEED, &MonsterSeed);
    SetDriveMonsterData(driveMonster, MONSTER_DATA_PLAYER_SEED, &Value);

    GetMonsterNameFromIndex(MonsterName, Index);
    SetDriveMonsterData(driveMonster, MONSTER_DATA_NICKNAME, MonsterName);
    SetDriveMonsterData(driveMonster, MONSTER_DATA_PLAYER_NAME, gPlayer.Name);
    SetDriveMonsterData(driveMonster, MONSTER_DATA_INDEX, &Index);
    SetDriveMonsterData(driveMonster, MONSTER_DATA_EXPERIENCE, &gExperienceTables[gBaseStats[Index].growthRate][Level]);
    SetDriveMonsterData(driveMonster, MONSTER_DATA_FRIENDSHIP, &gBaseStats[Index].friendship);
    MetLocation = gCurrentArea.Index;
    SetDriveMonsterData(driveMonster, MONSTER_DATA_MET_LOCATION, &MetLocation);
    SetDriveMonsterData(driveMonster, MONSTER_DATA_MET_LEVEL, &Level);

    if (FixedGenetics < USE_RANDOM_GENETICS)
    {
        SetDriveMonsterData(driveMonster, MONSTER_DATA_HP_GENETICS, &FixedGenetics);
        SetDriveMonsterData(driveMonster, MONSTER_DATA_ATTACK_GENETICS, &FixedGenetics);
        SetDriveMonsterData(driveMonster, MONSTER_DATA_DEFENSE_GENETICS, &FixedGenetics);
        SetDriveMonsterData(driveMonster, MONSTER_DATA_SPEED_GENETICS, &FixedGenetics);
        SetDriveMonsterData(driveMonster, MONSTER_DATA_PSI_GENETICS, &FixedGenetics);
        SetDriveMonsterData(driveMonster, MONSTER_DATA_RESOLVE_GENETICS, &FixedGenetics);
    }
    else
    {
        uint32_t Genes;
        uint16_t Random;
        rand_s((unsigned int*)&Value);
        Random = Value >> 16;

        Genes = Random & MAX_GENETICS_VALUE;
        SetDriveMonsterData(driveMonster, MONSTER_DATA_HP_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 5)) >> 5;
        SetDriveMonsterData(driveMonster, MONSTER_DATA_ATTACK_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 10)) >> 10;
        SetDriveMonsterData(driveMonster, MONSTER_DATA_DEFENSE_GENETICS, &Genes);

        rand_s((unsigned int*)&Value);
        Random = Value >> 16;

        Genes = Random & MAX_GENETICS_VALUE;
        SetDriveMonsterData(driveMonster, MONSTER_DATA_SPEED_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 5)) >> 5;
        SetDriveMonsterData(driveMonster, MONSTER_DATA_PSI_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 10)) >> 10;
        SetDriveMonsterData(driveMonster, MONSTER_DATA_RESOLVE_GENETICS, &Genes);

        if (gBaseStats[Index].flags & (MONSTER_FLAG_LEGENDARY))
        {
            Genes = MAX_GENETICS_VALUE;
            for (i = 0; i < NUM_MONSTER_STATS; i++)
            {
                AvailableGenetics[i] = i;
            }

            rand_s((unsigned int*)&Value);
            Random = Value >> 16;

            for (i = 0; i < LEGENDARY_GENETICS_PERFECT_COUNT; i++)
            {
                uint8_t Counter = Random % (NUM_MONSTER_STATS - i);
                SelectedGenetics[i] = AvailableGenetics[Counter];
                RemoveGenesIndexFromList(AvailableGenetics, Index);
            }

            for (i = 0; i < LEGENDARY_GENETICS_PERFECT_COUNT; i++)
            {
                switch (SelectedGenetics[i])
                {
                    case 0:
                    {
                        SetDriveMonsterData(driveMonster, MONSTER_DATA_HP_GENETICS, &Genes);
                        break;
                    }
                    case 1:
                    {
                        SetDriveMonsterData(driveMonster, MONSTER_DATA_ATTACK_GENETICS, &Genes);
                        break;
                    }
                    case 2:
                    {
                        SetDriveMonsterData(driveMonster, MONSTER_DATA_DEFENSE_GENETICS, &Genes);
                        break;
                    }
                    case 3:
                    {
                        SetDriveMonsterData(driveMonster, MONSTER_DATA_SPEED_GENETICS, &Genes);
                        break;
                    }
                    case 4:
                    {
                        SetDriveMonsterData(driveMonster, MONSTER_DATA_PSI_GENETICS, &Genes);
                        break;
                    }
                    case 5:
                    {
                        SetDriveMonsterData(driveMonster, MONSTER_DATA_RESOLVE_GENETICS, &Genes);
                        break;
                    }
                }
            }
        }
    }

    if (gBaseStats[Index].abilities[1])
    {
        uint32_t Random;
        rand_s((unsigned int*)&Random);
        Random = Random >> 16;

        Value = Random & 1;
        SetDriveMonsterData(driveMonster, MONSTER_DATA_ABILITY_NUMBER, &Value);
    }


    GiveDriveMonsterInitialMoveset(driveMonster);
}

void CreateMonsterWithGeneticsMonsterSeed(struct Monster *monster, uint8_t monsterIndex, uint8_t level, uint32_t genes, uint32_t monsterSeed)
{
    CreateMonster(monster, monsterIndex, level, 0, TRUE, monsterSeed, 0, 0);
    SetMonsterData(monster, MONSTER_DATA_GENETICS, &genes);
    CalculateMonsterStats(monster);
}

void CreateMonsterWithPtrGeneticsAndPresetPlayerSeed(struct Monster* monster, uint8_t monsterIndex, uint8_t level, uint8_t* genes, uint32_t PresetPlayerSeed)
{
    CreateMonster(monster, monsterIndex, level, 0, FALSE, 0, PresetPlayerSeed);
    SetMonsterData(monster, MONSTER_DATA_HP_GENETICS, &genes[0]);
    SetMonsterData(monster, MONSTER_DATA_ATTACK_GENETICS, &genes[1]);
    SetMonsterData(monster, MONSTER_DATA_DEFENSE_GENETICS, &genes[2]);
    SetMonsterData(monster, MONSTER_DATA_SPEED_GENETICS, &genes[3]);
    SetMonsterData(monster, MONSTER_DATA_PSI_GENETICS, &genes[4]);
    SetMonsterData(monster, MONSTER_DATA_RESOLVE_GENETICS, &genes[5]);
    CalculateMonsterStats(monster);
}

uint8_t GetLevelFromMonsterExp(struct Monster* monster)
{
    uint8_t MonsterIndex = GetMonsterData(monster, MONSTER_DATA_INDEX, NULL);
    uint32_t Exp = GetMonsterData(monster, MONSTER_DATA_EXPERIENCE, NULL);
    int32_t Level = 1;

    while (gExperienceTables[gBaseStats[MonsterIndex].growthRate][Level] <= Exp)        //TODO:     (gExperienceTables[gBaseStats[MonsterIndex].growthRate][Level] <= Exp && Level <= GetPartyLevelCap)??
    {                                                                                   //Idea is to make level cap for endless mode or maybe hard difficulty
        Level++;
    }
    return(Level - 1);
}

uint8_t GetLevelFromDriveMonsterExp(struct DriveMonster* driveMonster)
{
    uint8_t MonsterIndex = GetDriveMonsterData(driveMonster, MONSTER_DATA_INDEX, NULL);
    uint32_t Exp = GetDriveMonsterData(driveMonster, MONSTER_DATA_EXPERIENCE, NULL);
    int32_t Level = 1;

    while (gExperienceTables[gBaseStats[MonsterIndex].growthRate][Level] <= Exp)        //TODO:     (gExperienceTables[gBaseStats[MonsterIndex].growthRate][Level] <= Exp && Level <= GetPartyLevelCap)??
    {                                                                                   //Idea is to make level cap for endless mode or maybe hard difficulty
        Level++;
    }
    return(Level - 1);
}

void GiveMonsterInitialMoveset(struct Monster* monster)
{
    GiveDriveMonsterInitialMoveset(&monster->DriveMonster);
}

void GiveDriveMonsterInitialMoveset(struct DriveMonster* driveMonster)
{
    uint16_t MonsterIndex = GetDriveMonsterData(driveMonster, MONSTER_DATA_INDEX, NULL);
    int32_t level = GetLevelFromDriveMonsterExp(driveMonster);
    int32_t i;

    for (i = 0; gLevelUpMoves[MonsterIndex][i].move != LEVEL_UP_END; i++)
    {
        if (gLevelUpMoves[MonsterIndex][i].level > level)
        {
            break;
        }
        if (gLevelUpMoves[MonsterIndex][i].level == 0)
        {
            continue;
        }
        if (GiveMoveToDriveMonster(driveMonster, gLevelUpMoves[MonsterIndex][i].move) == MONSTER_HAS_MAX_MOVES)
        {
            DeleteFirstMoveGiveMoveToDriveMonster(driveMonster, gLevelUpMoves[MonsterIndex][i].move);
        }
    }
}

uint16_t GiveMoveToMonster(struct Monster* monster, uint16_t move)
{
    return GiveMoveToDriveMonster(&monster->DriveMonster, move);
}

static uint16_t GiveMoveToDriveMonster(struct DriveMonster* driveMonster, uint16_t move)
{
    int32_t i;
    for (i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
    {
        uint16_t existingMove = GetDriveMonsterData(driveMonster, MONSTER_DATA_MOVE_1 + i, NULL);
        if (existingMove == 0)
        {
            SetDriveMonsterData(driveMonster, MONSTER_DATA_MOVE_1 + i, &move);
            return (move);
        }
        if (existingMove == move)
        {
            return (MONSTER_ALREADY_KNOWS_MOVE);
        }
    }
    return(MONSTER_HAS_MAX_MOVES);
}

void DeleteFirstMoveGiveMoveToMonster(struct Monster* monster, uint16_t move)
{
    int32_t i;
    uint16_t moves[MAX_NONSIGNATURE_MOVES];
    for (i = 0; i < MAX_NONSIGNATURE_MOVES - 1; i++)
    {
        moves[i] = GetMonsterData(monster, MONSTER_DATA_MOVE_2 + i, NULL);
    }

    moves[MAX_NONSIGNATURE_MOVES - 1] = move;

    for (i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
    {
        SetMonsterData(monster, MONSTER_DATA_MOVE_1 + i, &moves[i]);
    }
}

void DeleteFirstMoveGiveMoveToDriveMonster(struct DriveMonster* driveMonster, uint16_t move)
{
    int32_t i;
    uint16_t moves[MAX_NONSIGNATURE_MOVES];
    for (i = 0; i < MAX_NONSIGNATURE_MOVES - 1; i++)
    {
        moves[i] = GetDriveMonsterData(driveMonster, MONSTER_DATA_MOVE_2 + i, NULL);
    }

    moves[MAX_NONSIGNATURE_MOVES - 1] = move;

    for (i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
    {
        SetDriveMonsterData(driveMonster, MONSTER_DATA_MOVE_1 + i, &moves[i]);
    }
}

static void RemoveGenesIndexFromList(uint8_t* genes, uint8_t selectedGene)
{
    int32_t i, j;
    uint8_t temp[NUM_MONSTER_STATS];

    genes[selectedGene] = 0xFF;
    for (i = 0; i < NUM_MONSTER_STATS; i++)
    {
        temp[i] = genes[i];
    }

    j = 0;
    for (i = 0; i < NUM_MONSTER_STATS; i++)
    {
        if (temp[i] != 0xFF)
        {
            genes[j++] = temp[i];
        }
    }
}


#define SET8(lhs) (lhs) = *data
#define SET16(lhs) (lhs) = data[0] + (data[1] << 8)
#define SET32(lhs) (lhs) = data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24)

void SetMonsterData(struct Monster* monster, int32_t field, const void* dataArgument)
{
    const uint8_t* data = dataArgument;

    switch (field)
    {
        case MONSTER_DATA_STATUS:
        {
            SET32(monster->Status);
            break;
        }
        case MONSTER_DATA_LEVEL:
        {
            SET8(monster->Level);
            break;
        }
        case MONSTER_DATA_HEALTH:
        {
            SET16(monster->Health);
            break;
        }
        case MONSTER_DATA_MAX_HEALTH:
        {
            SET16(monster->MaxHealth);
            break;
        }
        case MONSTER_DATA_ATTACK:
        {
            SET16(monster->Attack);
            break;
        }
        case MONSTER_DATA_DEFENSE:
        {
            SET16(monster->Defense);
            break;
        }
        case MONSTER_DATA_SPEED:
        {
            SET16(monster->Speed);
            break;
        }
        case MONSTER_DATA_PSI:
        {
            SET16(monster->Psi);
            break;
        }
        case MONSTER_DATA_RESOLVE:
        {
            SET16(monster->Resolve);
            break;
        }
        default:
        {
            SetDriveMonsterData(&monster->DriveMonster, field, data);
            break;
        }
    }
}

void SetDriveMonsterData(struct DriveMonster* driveMonster, int32_t field, const void* dataArgument)
{
    const uint8_t* data = dataArgument;

    uint16_t CalculatedCheck = 0;

    switch (field)
    {
        case MONSTER_DATA_MONSTER_SEED:
        {
            SET32(driveMonster->MonsterSeed);
            break;
        }
        case MONSTER_DATA_PLAYER_SEED:
        {
            SET32(driveMonster->PlayerSeed);
            break;
        }
        case MONSTER_DATA_NICKNAME:
        {
            int32_t i;
            for (i = 0; i < MAX_MONSTER_NAME_LENGTH; i++)
            {
                driveMonster->nickname[i] = data[i];
            }
            break;
        }
        case MONSTER_DATA_PLAYER_NAME:
        {
            int32_t i;
            for (i = 0; i < MAX_NAME_LENGTH; i++)
            {
                driveMonster->playerName[i] = data[i];
            }
            break;
        }
        case MONSTER_DATA_HAS_INDEX:
        {
            SET8(driveMonster->hasIndex);
            break;
        }
        case MONSTER_DATA_CHECKSUM:
        {
            SET16(driveMonster->checkSum);
            break;
        }
        case MONSTER_DATA_ENCRYPT:
        {
            SET16(driveMonster->unknown);
            break;
        }
        case MONSTER_DATA_INDEX:
        {
            SET16(driveMonster->Index);
            if (driveMonster->Index)
            {
                driveMonster->hasIndex = TRUE;
            }
            else
            {
                driveMonster->hasIndex = FALSE;
            }
            break;
        }
        case MONSTER_DATA_HELDITEM:
        {
            SET16(driveMonster->HeldItem);
            break;
        }
        case MONSTER_DATA_EXPERIENCE:
        {
            SET32(driveMonster->Experience);
            break;
        }
        case MONSTER_DATA_FRIENDSHIP:
        {
            SET8(driveMonster->Friendship);
            break;
        }
        case MONSTER_DATA_MOVE_1:
        case MONSTER_DATA_MOVE_2:
        case MONSTER_DATA_MOVE_3:
        case MONSTER_DATA_MOVE_4:
        {
            SET16(driveMonster->Moves[field - MONSTER_DATA_MOVE_1]);
            break;
        }
        case MONSTER_DATA_HP_TRAINING:
        {
            SET8(driveMonster->HpTraining);
            break;
        }
        case MONSTER_DATA_ATTACK_TRAINING:
        {
            SET8(driveMonster->AttackTraining);
            break;
        }
        case MONSTER_DATA_DEFENSE_TRAINING:
        {
            SET8(driveMonster->DefenseTraining);
            break;
        }
        case MONSTER_DATA_SPEED_TRAINING:
        {
            SET8(driveMonster->SpeedTraining);
            break;
        }
        case MONSTER_DATA_PSI_TRAINING:
        {
            SET8(driveMonster->PsiTraining);
            break;
        }
        case MONSTER_DATA_RESOLVE_TRAINING:
        {
            SET8(driveMonster->ResolveTraining);
            break;
        }
        case MONSTER_DATA_MET_LOCATION:
        {
            SET8(driveMonster->MetLocation);
            break;
        }
        case MONSTER_DATA_MET_LEVEL:
        {
            uint8_t Met_Level = *data;
            driveMonster->MetLevel = Met_Level;
            break;
        }
        case MONSTER_DATA_HP_GENETICS:
        {
            SET8(driveMonster->HpGenetics);
            break;
        }
        case MONSTER_DATA_ATTACK_GENETICS:
        {
            SET8(driveMonster->AttackGenetics);
            break;
        }
        case MONSTER_DATA_DEFENSE_GENETICS:
        {
            SET8(driveMonster->DefenseGenetics);
            break;
        }
        case MONSTER_DATA_SPEED_GENETICS:
        {
            SET8(driveMonster->SpeedGenetics);
            break;
        }
        case MONSTER_DATA_PSI_GENETICS:
        {
            SET8(driveMonster->PsiGenetics);
            break;
        }
        case MONSTER_DATA_RESOLVE_GENETICS:
        {
            SET8(driveMonster->ResolveGenetics);
            break;
        }
        case MONSTER_DATA_ABILITY_NUMBER:
        {
            SET8(driveMonster->AbilityNumber);
            break;
        }
        case MONSTER_DATA_GENETICS:
        {
            uint32_t Genes = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

            driveMonster->HpGenetics = Genes & MAX_GENETICS_VALUE;
            driveMonster->AttackGenetics = (Genes >> 5) & MAX_GENETICS_VALUE;
            driveMonster->DefenseGenetics = (Genes >> 10) & MAX_GENETICS_VALUE;
            driveMonster->SpeedGenetics = (Genes >> 15) & MAX_GENETICS_VALUE;
            driveMonster->PsiGenetics = (Genes >> 20) & MAX_GENETICS_VALUE;
            driveMonster->ResolveGenetics = (Genes >> 25) & MAX_GENETICS_VALUE;
        }
        default:
        {
            break;
        }
    }
}

uint16_t GiveMoveToBattleMonster(struct BattleMonster* bMonster, uint16_t move)
{
    int32_t i;

    for (i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
    {
        if (bMonster->Moves[i] == MONSTER_MOVE_NONE)
        {
            bMonster->Moves[i] = move;
            return (move);
        }
    }
    return (MONSTER_HAS_MAX_MOVES);
}

//slot index starts at 0 ends at 3
void SetMonsterMoveSlot(struct Monster* monster, uint16_t move, uint8_t slot)
{
    SetMonsterData(monster, MONSTER_DATA_MOVE_1 + slot, &move);
}

//slot index starts at 0 ends at 3
void SetBattleMonsterMoveSlot(struct BattleMonster* bMonster, uint16_t move, uint8_t slot)
{
    bMonster->Moves[slot] = move;
}

static uint8_t sLearningMoveTableID = 0;

uint16_t gMoveToLearn = 0;

uint16_t MonsterTryLearningNewMove(struct Monster* monster, BOOL firstMove)
{
    uint32_t value = MONSTER_MOVE_NONE;
    uint16_t monsterIndex = GetMonsterData(monster, MONSTER_DATA_INDEX, NULL);
    uint8_t level = GetMonsterData(monster, MONSTER_DATA_LEVEL, NULL);


    if (firstMove)
    {

        sLearningMoveTableID = 0;
        while (gLevelUpMoves[monsterIndex][sLearningMoveTableID].level != level)
        {
            sLearningMoveTableID++;
            if (gLevelUpMoves[monsterIndex][sLearningMoveTableID].move == LEVEL_UP_END)
            {
                return (MONSTER_MOVE_NONE);
            }
        }
    }

    if (gLevelUpMoves[monsterIndex][sLearningMoveTableID].level == level)
    {
        gMoveToLearn = gLevelUpMoves[monsterIndex][sLearningMoveTableID].move;
        sLearningMoveTableID++;
        value = GiveMoveToMonster(monster, gMoveToLearn);
    }

    return(value);
}

void CopyMonsterToPlayerParty(uint8_t partyIndex, struct Monster source)
{
    gPlayerParty[partyIndex] = source;
}

void CopyMonsterToOpponentParty(uint8_t partyIndex, struct Monster source)
{
    gOpponentParty[partyIndex] = source;
}

void DriveMonsterToMonster(const struct DriveMonster* source, struct Monster* destination)
{
    uint32_t value = 0;
    destination->DriveMonster = *source;
    SetMonsterData(destination, MONSTER_DATA_STATUS, &value);
    SetMonsterData(destination, MONSTER_DATA_HEALTH, &value);
    SetMonsterData(destination, MONSTER_DATA_MAX_HEALTH, &value);
    CalculateMonsterStats(destination);
}

uint32_t GetMonsterData(struct Monster* monster, int32_t field, uint8_t* data)
{
    uint32_t GetValue = 0;

    switch (field)
    {
        case MONSTER_DATA_STATUS:
        {
            GetValue = (monster->Status);
            break;
        }
        case MONSTER_DATA_LEVEL:
        {
            GetValue = (monster->Level);
            break;
        }
        case MONSTER_DATA_HEALTH:
        {
            GetValue = (monster->Health);
            break;
        }
        case MONSTER_DATA_MAX_HEALTH:
        {
            GetValue = (monster->MaxHealth);
            break;
        }
        case MONSTER_DATA_ATTACK:
        {
            GetValue = (monster->Attack);
            break;
        }
        case MONSTER_DATA_DEFENSE:
        {
            GetValue = (monster->Defense);
            break;
        }
        case MONSTER_DATA_SPEED:
        {
            GetValue = (monster->Speed);
            break;
        }
        case MONSTER_DATA_PSI:
        {
            GetValue = (monster->Psi);
            break;
        }
        case MONSTER_DATA_RESOLVE:
        {
            GetValue = (monster->Resolve);
            break;
        }
        default:
        {
            GetValue = GetDriveMonsterData(&monster->DriveMonster, field, data);
            break;
        }
    }

    return (GetValue);
}

uint32_t GetDriveMonsterData(struct DriveMonster* driveMonster, int32_t field, uint8_t* data)
{
    int32_t i;
    uint32_t GetValue = 0;

    uint16_t CalculatedCheck = 0;

    switch (field)
    {
        case MONSTER_DATA_MONSTER_SEED:
        {
            GetValue = driveMonster->MonsterSeed;
            break;
        }
        case MONSTER_DATA_PLAYER_SEED:
        {
            GetValue = driveMonster->PlayerSeed;
            break;
        }
        case MONSTER_DATA_NICKNAME:
        {
            for (GetValue = 0; GetValue < MAX_MONSTER_NAME_LENGTH; data[GetValue] = driveMonster->nickname[GetValue], GetValue++)
            {
            }
            data[GetValue] = END_OF_STRING;
            break;
        }
        case MONSTER_DATA_PLAYER_NAME:
        {
            GetValue = 0;
            while (GetValue < MAX_NAME_LENGTH)
            {
                data[GetValue] = driveMonster->playerName[GetValue];
                GetValue++;
            }
            data[GetValue] = END_OF_STRING;
            break;
        }
        case MONSTER_DATA_CHECKSUM:
        {
            GetValue = driveMonster->checkSum;
            break;
        }
        case MONSTER_DATA_ENCRYPT:
        {
            GetValue = driveMonster->unknown;
            break;
        }
        case MONSTER_DATA_INDEX:
        {
            GetValue = driveMonster->Index;
            break;
        }
        case MONSTER_DATA_HELDITEM:
        {
            GetValue = driveMonster->HeldItem;
            break;
        }
        case MONSTER_DATA_EXPERIENCE:
        {
            GetValue = driveMonster->Experience;
            break;
        }
        case MONSTER_DATA_FRIENDSHIP:
        {
            GetValue = driveMonster->Friendship;
            break;
        }
        case MONSTER_DATA_MOVE_1:
        case MONSTER_DATA_MOVE_2:
        case MONSTER_DATA_MOVE_3:
        case MONSTER_DATA_MOVE_4:
        {
            GetValue = driveMonster->Moves[field - MONSTER_DATA_MOVE_1];
            break;
        }
        case MONSTER_DATA_HP_TRAINING:
        {
            GetValue = driveMonster->HpTraining;
            break;
        }
        case MONSTER_DATA_ATTACK_TRAINING:
        {
            GetValue = driveMonster->AttackTraining;
            break;
        }
        case MONSTER_DATA_DEFENSE_TRAINING:
        {
            GetValue = driveMonster->DefenseTraining;
            break;
        }
        case MONSTER_DATA_SPEED_TRAINING:
        {
            GetValue = driveMonster->SpeedTraining;
            break;
        }
        case MONSTER_DATA_PSI_TRAINING:
        {
            GetValue = driveMonster->PsiTraining;
            break;
        }
        case MONSTER_DATA_RESOLVE_TRAINING:
        {
            GetValue = driveMonster->ResolveTraining;
            break;
        }
        case MONSTER_DATA_MET_LOCATION:
        {
            GetValue = driveMonster->MetLocation;
            break;
        }
        case MONSTER_DATA_MET_LEVEL:
        {
            GetValue = driveMonster->MetLevel;
            break;
        }
        case MONSTER_DATA_HP_GENETICS:
        {
            GetValue = driveMonster->HpGenetics;
            break;
        }
        case MONSTER_DATA_ATTACK_GENETICS:
        {
            GetValue = driveMonster->AttackGenetics;
            break;
        }
        case MONSTER_DATA_DEFENSE_GENETICS:
        {
            GetValue = driveMonster->DefenseGenetics;
            break;
        }
        case MONSTER_DATA_SPEED_GENETICS:
        {
            GetValue = driveMonster->SpeedGenetics;
            break;
        }
        case MONSTER_DATA_PSI_GENETICS:
        {
            GetValue = driveMonster->PsiGenetics;
            break;
        }
        case MONSTER_DATA_RESOLVE_GENETICS:
        {
            GetValue = driveMonster->ResolveGenetics;
            break;
        }
        case MONSTER_DATA_ABILITY_NUMBER:
        {
            GetValue = driveMonster->AbilityNumber;
            break;
        }
        case MONSTER_DATA_GENETICS:
        {
            GetValue = driveMonster->HpGenetics
                | (driveMonster->AttackGenetics << 5)
                | (driveMonster->DefenseGenetics << 10)
                | (driveMonster->SpeedGenetics << 15)
                | (driveMonster->PsiGenetics << 20)
                | (driveMonster->ResolveGenetics << 25);
            break;
        }
        case MONSTER_DATA_KNOWN_MOVES:
        {
            if (driveMonster->Index)
            {
                uint16_t* moves = (uint16_t*)data;
                int32_t i = 0;

                while (moves[i] != MAX_NONSIGNATURE_MOVES)
                {
                    uint16_t move = moves[i];
                    if (driveMonster->Moves[0] == move || driveMonster->Moves[1] == move || driveMonster->Moves[2] == move || driveMonster->Moves[3] == move)
                    {
                        GetValue |= gBitTable[i];
                    }
                    i++;
                }

            }
            break;
        }
        default:
            break;
    }

    return (GetValue);
}

void CalculateMonsterStats(struct Monster* monster)
{
    int32_t PrevMaxHP = GetMonsterData(monster, MONSTER_DATA_MAX_HEALTH, NULL);
    int32_t CurrentHP = GetMonsterData(monster, MONSTER_DATA_HEALTH, NULL);
    int32_t HPTraining = GetMonsterData(monster, MONSTER_DATA_HP_TRAINING, NULL);
    int32_t HPGenetics = GetMonsterData(monster, MONSTER_DATA_HP_GENETICS, NULL);
    int32_t AttackTraining = GetMonsterData(monster, MONSTER_DATA_ATTACK_TRAINING, NULL);
    int32_t AttackGenetics = GetMonsterData(monster, MONSTER_DATA_ATTACK_GENETICS, NULL);
    int32_t DefenseTraining = GetMonsterData(monster, MONSTER_DATA_DEFENSE_TRAINING, NULL);
    int32_t DefenseGenetics = GetMonsterData(monster, MONSTER_DATA_DEFENSE_GENETICS, NULL);
    int32_t SpeedTraining = GetMonsterData(monster, MONSTER_DATA_SPEED_TRAINING, NULL);
    int32_t SpeedGenetics = GetMonsterData(monster, MONSTER_DATA_SPEED_GENETICS, NULL);
    int32_t PsiTraining = GetMonsterData(monster, MONSTER_DATA_PSI_TRAINING, NULL);
    int32_t PsiGenetics = GetMonsterData(monster, MONSTER_DATA_PSI_GENETICS, NULL);
    int32_t ResolveTraining = GetMonsterData(monster, MONSTER_DATA_RESOLVE_TRAINING, NULL);
    int32_t ResolveGenetics = GetMonsterData(monster, MONSTER_DATA_RESOLVE_GENETICS, NULL);
    uint8_t MonsterIndex = GetMonsterData(monster, MONSTER_DATA_INDEX, NULL);
    int32_t Level = GetMonsterData(monster, MONSTER_DATA_LEVEL, NULL);
    int32_t NewMaxHP;

    SetMonsterData(monster, MONSTER_DATA_LEVEL, &Level);

    if (MonsterIndex == 0)      //if null then one hit point TODO: make interesting monster exceptions here
    {
        NewMaxHP = 1;
    }
    else
    {
        int32_t Number = 2 * gBaseStats[MonsterIndex].baseHP + HPGenetics;              ////pokemon's exact calculation, TODO: invent new calculation
        NewMaxHP = (((Number + HPTraining / 4) * Level) / 100) + Level + 10;
    }
    SetMonsterData(monster, MONSTER_DATA_MAX_HEALTH, &NewMaxHP);

    //start calculating                                                                         ////pokemon's formula, TODO: make my own (make training more impactful than genes??)

    uint8_t baseStat = gBaseStats[MonsterIndex].baseAttack;
    int32_t Number = (((2 * baseStat + AttackGenetics + AttackTraining / 4) * Level) / 100) + 5;
    SetMonsterData(monster, MONSTER_DATA_ATTACK, &Number);

    baseStat = gBaseStats[MonsterIndex].baseDefense;
    Number = (((2 * baseStat + DefenseGenetics + DefenseTraining / 4) * Level) / 100) + 5;
    SetMonsterData(monster, MONSTER_DATA_DEFENSE, &Number);

    baseStat = gBaseStats[MonsterIndex].baseSpeed;
    Number = (((2 * baseStat + SpeedGenetics + SpeedTraining / 4) * Level) / 100) + 5;
    SetMonsterData(monster, MONSTER_DATA_SPEED, &Number);

    baseStat = gBaseStats[MonsterIndex].basePsi;
    Number = (((2 * baseStat + PsiGenetics + PsiTraining / 4) * Level) / 100) + 5;
    SetMonsterData(monster, MONSTER_DATA_PSI, &Number);

    baseStat = gBaseStats[MonsterIndex].baseResolve;
    Number = (((2 * baseStat + ResolveGenetics + ResolveTraining / 4) * Level) / 100) + 5;
    SetMonsterData(monster, MONSTER_DATA_RESOLVE, &Number);

    if (MonsterIndex == 0)
    {
        if (CurrentHP != 0 || PrevMaxHP == 0)
        {
            CurrentHP = 1;
        }
        else
        {
            return;
        }
    }
    else
    {
        if (CurrentHP == 0 && PrevMaxHP == 0)
        {
            CurrentHP = NewMaxHP;
        }
        else if (CurrentHP != 0)
        {
            CurrentHP += NewMaxHP - PrevMaxHP;
            if (CurrentHP <= 0)
                CurrentHP = 1;
        }
        else
        {
            return;
        }
    }

    SetMonsterData(monster, MONSTER_DATA_HEALTH, &CurrentHP);

}

uint8_t CountAliveMonstersInBattle(uint8_t caseId)
{

}

uint8_t GetMonsterGender(struct Monster* monster)
{
    return (GetDriveMonsterGender(&monster->DriveMonster));
}

uint8_t GetDriveMonsterGender(struct DriveMonster* driveMonster)
{
    uint8_t monsterIndex = GetDriveMonsterData(driveMonster, MONSTER_DATA_INDEX, NULL);
    uint32_t monsterSeed = GetDriveMonsterData(driveMonster, MONSTER_DATA_MONSTER_SEED, NULL);

    switch (gBaseStats[monsterIndex].genderRatio)
    {
        case MONSTER_MALE:
        case MONSTER_FEMALE:
        case MONSTER_GENDERLESS:
        {
            return(gBaseStats[monsterIndex].genderRatio);
        }
    }

    if (gBaseStats[monsterIndex].genderRatio > (monsterSeed & 0xFF))
    {
        return(MONSTER_FEMALE);
    }
    else
    {
        return(MONSTER_MALE);
    }
}

uint8_t GetGenderFromMonsterIndexAndSeed(uint8_t monsterIndex, uint32_t monsterSeed)
{
    switch (gBaseStats[monsterIndex].genderRatio)
    {
        case MONSTER_MALE:
        case MONSTER_FEMALE:
        case MONSTER_GENDERLESS:
        {
            return(gBaseStats[monsterIndex].genderRatio);
        }
    }

    if (gBaseStats[monsterIndex].genderRatio > (monsterSeed & 0xFF))
    {
        return(MONSTER_FEMALE);
    }
    else
    {
        return(MONSTER_MALE);
    }
}

////TEMPORARY WILL BE BASED ON MAP LATER
struct Monster GenerateScriptedMonsterForWildEncounter(uint8_t index, uint8_t maxLevel, uint8_t minLevel, uint16_t item)
{
    uint8_t heldItem[2];
    struct Monster monster;
    //uint16_t targetIndex;

    DWORD Random;
    uint8_t randLevel;
    rand_s((unsigned int*)&Random);
    randLevel = (uint8_t*)Random;
    randLevel = ((randLevel % (maxLevel - minLevel + 1)) + minLevel);

    CreateMonster(&monster, index, randLevel, USE_RANDOM_GENETICS, FALSE, 0, 0, 0);
    heldItem[0] = item;
    heldItem[1] = item >> 8;
    SetMonsterData(&monster, MONSTER_DATA_HELDITEM, heldItem);

    return(monster);
}

////TODO: TEMPORARY WILL BE BASED ON MAP LATER
struct Monster GenerateRandMonsterForWildEncounter(uint8_t maxLevel, uint8_t minLevel, uint16_t item)
{
    uint8_t heldItem[2];
    struct Monster monster;
    //uint16_t targetIndex;

    DWORD Random;
    uint8_t randIndex;
    uint8_t randLevel;
    rand_s((unsigned int*)&Random);
    randIndex = (uint8_t*)Random;
    randIndex %= (NUM_MONSTERS - 1);
    rand_s((unsigned int*)&Random);
    randLevel = (uint8_t*)Random;
    randLevel = ((randLevel % (maxLevel - minLevel + 1)) + minLevel);

    CreateMonster(&monster, randIndex + 1, randLevel, USE_RANDOM_GENETICS, FALSE, 0, 0, 0);
    heldItem[0] = item;
    heldItem[1] = item >> 8;
    SetMonsterData(&monster, MONSTER_DATA_HELDITEM, heldItem);

    return(monster);
}

struct Monster GenerateMonsterForCharacterSpriteBattle(uint8_t index, uint8_t level, uint16_t item)
{
    uint8_t heldItem[2];
    struct Monster monster;
    //uint16_t targetIndex;         //TODO: monsters with multiple forms/states

    CreateMonster(&monster, index, level, USE_RANDOM_GENETICS, FALSE, 0, 0, 0);
    heldItem[0] = item;
    heldItem[1] = item >> 8;
    SetMonsterData(&monster, MONSTER_DATA_HELDITEM, heldItem);

    return(monster);
}

uint8_t GiveMonsterToPlayer(struct Monster* monster)
{
    int32_t i;

    SetMonsterData(monster, MONSTER_DATA_PLAYER_NAME, &gPlayer.Name);
    SetMonsterData(monster, MONSTER_DATA_PLAYER_SEED, &gPlayer.Seed);

    for (i = 0; i < MAX_PARTY_SIZE; i++)
    {
        if (GetMonsterData(&gPlayerParty[i], MONSTER_DATA_INDEX, NULL) == MONSTER_NULL)
        {
            break;
        }
    }

    if (i > MAX_PARTY_SIZE)         //TODO:FINISH
    {
        ASSERT(FALSE, "MonsterDrive system not added yet! player added too many party members!")
        //return(SendMonsterToPC(monster));
    }

    CopyMonsterToPlayerParty(i, *monster);
    gPlayerPartyCount = i + 1;
    return(0);
}

uint8_t SendMonsterToPC(struct Monster* monster)            //TODO: FINISH
{
    //int32_t ;
    //SetPCStorageToSendMonster();

}

void GetMonsterNameFromIndex(uint8_t* name, uint8_t index)
{
    int32_t i;

    for (i = 0; i <= MAX_MONSTER_NAME_LENGTH; i++)
    {
        if (index > NUM_MONSTERS)
        {
            name[i] = gMonsterNames[MONSTER_NULL][i];
        }
        else
        {
            name[i] = gMonsterNames[index][i];
        }
        if (name[i] == END_OF_STRING)
        {
            break;
        }
    }
}

uint16_t GetAbilityByIndex(uint16_t index, uint8_t abilityNum)
{
    int i;

    if (abilityNum < NUM_ABILITY_SLOTS)
    {
        gLastAbilityUsed = gBaseStats[index].abilities[abilityNum];
    }
    else
    {
        gLastAbilityUsed = ABILITY_NONE;
    }

    if (abilityNum >= NORMAL_ABILITY_SLOTS)     //if empty hidden ability, look for other hidden abilities
    {
        for (i = NORMAL_ABILITY_SLOTS; i < NUM_ABILITY_SLOTS && gLastAbilityUsed == ABILITY_NONE; i++)
        {
            gLastAbilityUsed = gBaseStats[index].abilities[i];
        }
    }

    for (i = 0; i < NUM_ABILITY_SLOTS && gLastAbilityUsed == ABILITY_NONE; i++)     //look for any non-empty hidden ability
    {
        gLastAbilityUsed = gBaseStats[index].abilities[i];
    }

    return(gLastAbilityUsed);
}

uint16_t GetMonAbility(struct Monster* monster)
{
    uint16_t index = GetMonsterData(monster, MONSTER_DATA_INDEX, NULL);
    uint8_t abilityNum = GetMonsterData(monster, MONSTER_DATA_ABILITY_NUMBER, NULL);
    return(GetAbilityByIndex(index, abilityNum));
}

void MonsterToBattleMonster(struct Monster* source, struct BattleMonster* dest)
{
    int32_t i;
    uint8_t nickname[MAX_MONSTER_NAME_LENGTH + 1];

    for (i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
    {
        dest->Moves[i] = GetMonsterData(source, MONSTER_DATA_MOVE_1 + i, NULL);
    }

    dest->Index = GetMonsterData(source, MONSTER_DATA_INDEX, NULL);
    dest->EquippedItem = GetMonsterData(source, MONSTER_DATA_HELDITEM, NULL);
    dest->Friendship = GetMonsterData(source, MONSTER_DATA_FRIENDSHIP, NULL);
    dest->Experience = GetMonsterData(source, MONSTER_DATA_EXPERIENCE, NULL);
    dest->HpGenetics = GetMonsterData(source, MONSTER_DATA_HP_GENETICS, NULL);
    dest->AttackGenetics = GetMonsterData(source, MONSTER_DATA_ATTACK_GENETICS, NULL);
    dest->DefenseGenetics = GetMonsterData(source, MONSTER_DATA_DEFENSE_GENETICS, NULL);
    dest->SpeedGenetics = GetMonsterData(source, MONSTER_DATA_SPEED_GENETICS, NULL);
    dest->PsiGenetics = GetMonsterData(source, MONSTER_DATA_PSI_GENETICS, NULL);
    dest->ResolveGenetics = GetMonsterData(source, MONSTER_DATA_RESOLVE_GENETICS, NULL);
    dest->MonsterSeed = GetMonsterData(source, MONSTER_DATA_MONSTER_SEED, NULL);
    dest->PlayerSeed = GetMonsterData(source, MONSTER_DATA_PLAYER_SEED, NULL);
    dest->Status1 = GetMonsterData(source, MONSTER_DATA_STATUS, NULL);
    dest->Level = GetMonsterData(source, MONSTER_DATA_LEVEL, NULL);
    dest->Health = GetMonsterData(source, MONSTER_DATA_HEALTH, NULL);
    dest->MaxHealth = GetMonsterData(source, MONSTER_DATA_MAX_HEALTH, NULL);
    dest->Attack = GetMonsterData(source, MONSTER_DATA_ATTACK, NULL);
    dest->Defense = GetMonsterData(source, MONSTER_DATA_DEFENSE, NULL);
    dest->Speed = GetMonsterData(source, MONSTER_DATA_SPEED, NULL);
    dest->Psi = GetMonsterData(source, MONSTER_DATA_PSI, NULL);
    dest->Resolve = GetMonsterData(source, MONSTER_DATA_RESOLVE, NULL);
    dest->AbilityNumber = GetMonsterData(source, MONSTER_DATA_ABILITY_NUMBER, NULL);
    dest->Element1 = gBaseStats[dest->Index].element1;
    dest->Element2 = gBaseStats[dest->Index].element2;
    dest->Ability = GetAbilityByIndex(dest->Index, dest->AbilityNumber);
    GetMonsterData(source, MONSTER_DATA_NICKNAME, nickname);
    StringCopy_NickName(dest->Nickname, nickname);
    GetMonsterData(source, MONSTER_DATA_PLAYER_NAME, dest->PlayerName);

    for (i = 0; i < 6; i++)     //6 battle stats, hp atk def spd psi res
    {
        dest->StatChanges[i] = 6;
    }
    dest->Status2 = 0;
}

uint8_t* StringCopy_NickName(uint8_t* dest, const uint8_t* source)
{
    uint8_t i;
    uint32_t limit = MAX_MONSTER_NAME_LENGTH;
    
    for (i = 0; i < limit; i++)
    {
        dest[i] = source[i];
        if (dest[i] == END_OF_STRING)
        {
            return(&dest[i]);
        }
    }

    dest[i] = END_OF_STRING;
    return(&dest[i]);
}

BOOL TryIncrementMonsterLevel(struct Monster* monster)
{
    uint8_t Index = GetMonsterData(monster, MONSTER_DATA_INDEX, NULL);
    uint8_t NextLevel = GetMonsterData(monster, MONSTER_DATA_LEVEL, NULL) + 1;
    uint32_t ExpPoints = GetMonsterData(monster, MONSTER_DATA_EXPERIENCE, NULL);

    //if (ExpPoints > gExperienceTables[gBaseStats[Index].growthRate][GetPartyLevelCap()])      //TODO: levelcap function for harder difficulties
    //{
    //    ExpPoints = gExperienceTables[gBaseStats[Index].growthRate][GetPartyLevelCap()];
    //    SetMonsterData(monster, MONSTER_DATA_EXPERIENCE, &ExpPoints);
    //}
    if (/*NextLevel > GetPartyLevelCap() ||*/ ExpPoints < gExperienceTables[gBaseStats[Index].growthRate][NextLevel])
    {
        return(FALSE);
    }
    else
    {
        SetMonsterData(monster, MONSTER_DATA_LEVEL, &NextLevel);
        return(TRUE);
    }
}