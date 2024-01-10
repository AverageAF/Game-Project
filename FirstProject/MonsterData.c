

#include "BaseStats.h"
#include "SimpleConstants.h"
#include "LevelUpMoves.h"
#include "ExperienceTables.h"

#include "MonsterData.h"

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

/////////////////////////////////////////////////////////////////////MONSTER DATA FUNCTIONS/////////////////////
void ZeroPcMonsterData(struct PCMonster* pcMonster)
{
    uint8_t* raw = (uint8_t*)pcMonster;
    uint32_t i;
    for (i = 0; i < sizeof(struct PCMonster); i++)
    {
        raw[i] = 0;
    }
}

void ZeroMonsterData(struct Monster* monster)
{
    uint32_t argument;
    ZeroPcMonsterData(&monster->PcMonster);
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
    CreatePCMonster(&monster->PcMonster, Index, Level, FixedGenetics, hasFixedMonsterSeed, FixedMonsterSeed, FixedPlayerSeed);
    SetMonsterData(monster, MONSTER_DATA_LEVEL, &Level);
    CalculateMonsterStats(monster);
}

//
//TODO: FINISH
void CreatePCMonster(struct PCMonster* pcMonster, uint8_t Index, uint8_t Level, uint8_t FixedGenetics, uint8_t hasFixedMonsterSeed, uint32_t FixedMonsterSeed, uint32_t FixedPlayerSeed)
{
    uint8_t MonsterName[MAX_MONSTER_NAME_LENGTH + 1];
    uint32_t MonsterSeed;
    uint32_t Value;
    uint16_t checksum;
    uint8_t i;
    uint8_t AvailableGenetics[NUM_MONSTER_STATS];
    uint8_t SelectedGenetics[LEGENDARY_GENETICS_PERFECT_COUNT];

    ZeroPcMonsterData(pcMonster);

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
        rand_s((unsigned int*)&Value);      //TODO: make this gPlayer.seed or something similar
    }

    SetPCMonsterData(pcMonster, MONSTER_DATA_MONSTER_SEED, &MonsterSeed);
    SetPCMonsterData(pcMonster, MONSTER_DATA_PLAYER_SEED, &Value);
    checksum = CalculatePCMonsterCheckSum(pcMonster);
    SetPCMonsterData(pcMonster, MONSTER_DATA_CHECKSUM, &checksum);

    EncryptPCMonster(pcMonster);

    //getmonstername                                                                                    //TODO: requires own header file probably
    //SetPCMonsterData(pcMonster, MONSTER_DATA_NICKNAME, MonsterName);
    SetPCMonsterData(pcMonster, MONSTER_DATA_PLAYER_NAME, gPlayer.Name);
    SetPCMonsterData(pcMonster, MONSTER_DATA_INDEX, &Index);
    SetPCMonsterData(pcMonster, MONSTER_DATA_EXPERIENCE, &gExperienceTables[gBaseStats[Index].growthRate][Level]);
    SetPCMonsterData(pcMonster, MONSTER_DATA_FRIENDSHIP, &gBaseStats[Index].friendship);
    //Value = gCurrentMapArea??
    //SetPCMonsterData(pcMonster, MONSTER_DATA_MET_LOCATION, &Value);
    SetPCMonsterData(pcMonster, MONSTER_DATA_MET_LEVEL, &Level);

    if (FixedGenetics < USE_RANDOM_GENETICS)
    {
        SetPCMonsterData(pcMonster, MONSTER_DATA_HP_GENETICS, &FixedGenetics);
        SetPCMonsterData(pcMonster, MONSTER_DATA_ATTACK_GENETICS, &FixedGenetics);
        SetPCMonsterData(pcMonster, MONSTER_DATA_DEFENSE_GENETICS, &FixedGenetics);
        SetPCMonsterData(pcMonster, MONSTER_DATA_SPEED_GENETICS, &FixedGenetics);
        SetPCMonsterData(pcMonster, MONSTER_DATA_PSI_GENETICS, &FixedGenetics);
        SetPCMonsterData(pcMonster, MONSTER_DATA_RESOLVE_GENETICS, &FixedGenetics);
    }
    else
    {
        uint32_t Genes;
        uint16_t Random;
        rand_s((unsigned int*)&Value);
        Random = Value >> 16;

        Genes = Random & MAX_GENETICS_VALUE;
        SetPCMonsterData(pcMonster, MONSTER_DATA_HP_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 5)) >> 5;
        SetPCMonsterData(pcMonster, MONSTER_DATA_ATTACK_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 10)) >> 10;
        SetPCMonsterData(pcMonster, MONSTER_DATA_DEFENSE_GENETICS, &Genes);

        rand_s((unsigned int*)&Value);
        Random = Value >> 16;

        Genes = Random & MAX_GENETICS_VALUE;
        SetPCMonsterData(pcMonster, MONSTER_DATA_SPEED_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 5)) >> 5;
        SetPCMonsterData(pcMonster, MONSTER_DATA_PSI_GENETICS, &Genes);
        Genes = (Random & (MAX_GENETICS_VALUE << 10)) >> 10;
        SetPCMonsterData(pcMonster, MONSTER_DATA_RESOLVE_GENETICS, &Genes);

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
                        SetPCMonsterData(pcMonster, MONSTER_DATA_HP_GENETICS, &Genes);
                        break;
                    }
                    case 1:
                    {
                        SetPCMonsterData(pcMonster, MONSTER_DATA_ATTACK_GENETICS, &Genes);
                        break;
                    }
                    case 2:
                    {
                        SetPCMonsterData(pcMonster, MONSTER_DATA_DEFENSE_GENETICS, &Genes);
                        break;
                    }
                    case 3:
                    {
                        SetPCMonsterData(pcMonster, MONSTER_DATA_SPEED_GENETICS, &Genes);
                        break;
                    }
                    case 4:
                    {
                        SetPCMonsterData(pcMonster, MONSTER_DATA_PSI_GENETICS, &Genes);
                        break;
                    }
                    case 5:
                    {
                        SetPCMonsterData(pcMonster, MONSTER_DATA_RESOLVE_GENETICS, &Genes);
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
        SetPCMonsterData(pcMonster, MONSTER_DATA_ABILITY_NUMBER, &Value);
    }

    GivePCMonsterInitialMoveset(pcMonster);
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

uint8_t GetLevelFromPCMonsterExp(struct PCMonster* pcMonster)
{
    uint8_t MonsterIndex = GetPCMonsterData(pcMonster, MONSTER_DATA_INDEX, NULL);
    uint32_t Exp = GetPCMonsterData(pcMonster, MONSTER_DATA_EXPERIENCE, NULL);
    int32_t Level = 1;

    while (gExperienceTables[gBaseStats[MonsterIndex].growthRate][Level] <= Exp)        //TODO:     (gExperienceTables[gBaseStats[MonsterIndex].growthRate][Level] <= Exp && Level <= GetPartyLevelCap)??
    {                                                                                   //Idea is to make level cap for endless mode or maybe hard difficulty
        Level++;
    }
    return(Level - 1);
}

void GiveMonsterInitialMoveset(struct Monster* monster)
{
    GivePCMonsterInitialMoveset(&monster->PcMonster);
}

void GivePCMonsterInitialMoveset(struct PCMonster* pcMonster)
{
    uint16_t MonsterIndex = GetPCMonsterData(pcMonster, MONSTER_DATA_INDEX, NULL);
    int32_t level = GetLevelFromPCMonsterExp(pcMonster);
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
        if (GiveMoveToPCMonster(pcMonster, gLevelUpMoves[MonsterIndex][i].move) == MONSTER_HAS_MAX_MOVES)
        {
            DeleteFirstMoveGiveMoveToPCMonster(pcMonster, gLevelUpMoves[MonsterIndex][i].move);
        }
    }
}

uint16_t GiveMoveToMonster(struct Monster* monster, uint16_t move)
{
    return GiveMoveToPCMonster(&monster->PcMonster, move);
}

static uint16_t GiveMoveToPCMonster(struct PCMonster* pcMonster, uint16_t move)
{
    int32_t i;
    for (i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
    {
        uint16_t existingMove = GetPCMonsterData(pcMonster, MONSTER_DATA_MOVE_1 + i, NULL);
        if (existingMove == 0)
        {
            SetPCMonsterData(pcMonster, MONSTER_DATA_MOVE_1 + i, &move);
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

void DeleteFirstMoveGiveMoveToPCMonster(struct PCMonster* pcMonster, uint16_t move)
{
    int32_t i;
    uint16_t moves[MAX_NONSIGNATURE_MOVES];
    for (i = 0; i < MAX_NONSIGNATURE_MOVES - 1; i++)
    {
        moves[i] = GetPCMonsterData(pcMonster, MONSTER_DATA_MOVE_2 + i, NULL);
    }

    moves[MAX_NONSIGNATURE_MOVES - 1] = move;

    for (i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
    {
        SetPCMonsterData(pcMonster, MONSTER_DATA_MOVE_1 + i, &moves[i]);
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
            SetPCMonsterData(&monster->PcMonster, field, data);
            break;
        }
    }
}

void SetPCMonsterData(struct PCMonster* pcMonster, int32_t field, const void* dataArgument)
{
    const uint8_t* data = dataArgument;

    struct MonsterSubstruct0* Substruct0 = NULL;
    struct MonsterSubstruct1* Substruct1 = NULL;
    struct MonsterSubstruct2* Substruct2 = NULL;
    struct MonsterSubstruct3* Substruct3 = NULL;

    if (field > MONSTER_DATA_ENCRYPT)
    {
        Substruct0 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 0)->type0);
        Substruct1 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 1)->type1);
        Substruct2 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 2)->type2);
        Substruct3 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 3)->type3);

        DecryptPCMonster(pcMonster);

        if (CalculatePCMonsterCheckSum(pcMonster) != pcMonster->checkSum)
        {
            LogMessageA(LL_WARNING, "[%s] CalculateMonsterCheckSum did not return same value as pcMonster->checkSum in SetPCMonsterData!", __FUNCTION__);
            EncryptPCMonster(pcMonster);
        }
    }

    switch (field)
    {
        case MONSTER_DATA_MONSTER_SEED:
        {
            SET32(pcMonster->MonsterSeed);
            break;
        }
        case MONSTER_DATA_PLAYER_SEED:
        {
            SET32(pcMonster->PlayerSeed);
            break;
        }
        case MONSTER_DATA_NICKNAME:
        {
            int32_t i;
            for (i = 0; i < MAX_MONSTER_NAME_LENGTH; i++)
            {
                pcMonster->nickname[i] = data[i];
            }
            break;
        }
        case MONSTER_DATA_PLAYER_NAME:
        {
            int32_t i;
            for (i = 0; i < MAX_NAME_LENGTH; i++)
            {
                pcMonster->playerName[i] = data[i];
            }
            break;
        }
        case MONSTER_DATA_HAS_INDEX:
        {
            SET8(pcMonster->hasIndex);
            break;
        }
        case MONSTER_DATA_CHECKSUM:
        {
            SET16(pcMonster->checkSum);
            break;
        }
        case MONSTER_DATA_ENCRYPT:
        {
            SET16(pcMonster->unknown);
            break;
        }
        case MONSTER_DATA_INDEX:
        {
            SET16(Substruct0->Index);
            if (Substruct0->Index)
            {
                pcMonster->hasIndex = TRUE;
            }
            else
            {
                pcMonster->hasIndex = FALSE;
            }
            break;
        }
        case MONSTER_DATA_HELDITEM:
        {
            SET16(Substruct0->HeldItem);
            break;
        }
        case MONSTER_DATA_EXPERIENCE:
        {
            SET32(Substruct0->Experience);
            break;
        }
        case MONSTER_DATA_FRIENDSHIP:
        {
            SET8(Substruct0->Friendship);
            break;
        }
        case MONSTER_DATA_MOVE_1:
        case MONSTER_DATA_MOVE_2:
        case MONSTER_DATA_MOVE_3:
        case MONSTER_DATA_MOVE_4:
        {
            SET16(Substruct1->Moves[field - MONSTER_DATA_MOVE_1]);
            break;
        }
        case MONSTER_DATA_HP_TRAINING:
        {
            SET8(Substruct2->HpTraining);
            break;
        }
        case MONSTER_DATA_ATTACK_TRAINING:
        {
            SET8(Substruct2->AttackTraining);
            break;
        }
        case MONSTER_DATA_DEFENSE_TRAINING:
        {
            SET8(Substruct2->DefenseTraining);
            break;
        }
        case MONSTER_DATA_SPEED_TRAINING:
        {
            SET8(Substruct2->SpeedTraining);
            break;
        }
        case MONSTER_DATA_PSI_TRAINING:
        {
            SET8(Substruct2->PsiTraining);
            break;
        }
        case MONSTER_DATA_RESOLVE_TRAINING:
        {
            SET8(Substruct2->ResolveTraining);
            break;
        }
        case MONSTER_DATA_MET_LOCATION:
        {
            SET8(Substruct3->MetLocation);
            break;
        }
        case MONSTER_DATA_MET_LEVEL:
        {
            uint8_t Met_Level = *data;
            Substruct3->MetLevel = Met_Level;
            break;
        }
        case MONSTER_DATA_HP_GENETICS:
        {
            SET8(Substruct3->HpGenetics);
            break;
        }
        case MONSTER_DATA_ATTACK_GENETICS:
        {
            SET8(Substruct3->AttackGenetics);
            break;
        }
        case MONSTER_DATA_DEFENSE_GENETICS:
        {
            SET8(Substruct3->DefenseGenetics);
            break;
        }
        case MONSTER_DATA_SPEED_GENETICS:
        {
            SET8(Substruct3->SpeedGenetics);
            break;
        }
        case MONSTER_DATA_PSI_GENETICS:
        {
            SET8(Substruct3->PsiGenetics);
            break;
        }
        case MONSTER_DATA_RESOLVE_GENETICS:
        {
            SET8(Substruct3->ResolveGenetics);
            break;
        }
        case MONSTER_DATA_ABILITY_NUMBER:
        {
            SET8(Substruct3->AbilityNumber);
            break;
        }
        case MONSTER_DATA_GENETICS:
        {
            uint32_t Genes = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

            Substruct3->HpGenetics = Genes & MAX_GENETICS_VALUE;
            Substruct3->AttackGenetics = (Genes >> 5) & MAX_GENETICS_VALUE;
            Substruct3->DefenseGenetics = (Genes >> 10) & MAX_GENETICS_VALUE;
            Substruct3->SpeedGenetics = (Genes >> 15) & MAX_GENETICS_VALUE;
            Substruct3->PsiGenetics = (Genes >> 20) & MAX_GENETICS_VALUE;
            Substruct3->ResolveGenetics = (Genes >> 25) & MAX_GENETICS_VALUE;
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
            if (gLevelUpMoves[monsterIndex][sLearningMoveTableID].move == LEVEL_UP_END);
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

void CopyMonster(void* destination, void* source, size_t size)
{
    memcpy(destination, source, size);
}

void PCMonsterToMonster(const struct PCMonster* source, struct Monster* destination)
{
    uint32_t value = 0;
    destination->PcMonster = *source;
    SetMonsterData(destination, MONSTER_DATA_STATUS, &value);
    SetMonsterData(destination, MONSTER_DATA_HEALTH, &value);
    SetMonsterData(destination, MONSTER_DATA_MAX_HEALTH, &value);
    CalculateMonsterStats(destination);
}

uint32_t GetMonsterData(struct Monster* monster, int32_t field, uint8_t* data)
{
    uint32_t GetValue;

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
            GetPCMonsterData(&monster->PcMonster, field, data);
            break;
        }
    }

    return (GetValue);
}

uint32_t GetPCMonsterData(struct PCMonster* pcMonster, int32_t field, uint8_t* data)
{
    int32_t i;
    uint32_t GetValue;

    struct MonsterSubstruct0* substruct0 = NULL;
    struct MonsterSubstruct1* substruct1 = NULL;
    struct MonsterSubstruct2* substruct2 = NULL;
    struct MonsterSubstruct3* substruct3 = NULL;

    if (field > MONSTER_DATA_ENCRYPT)
    {
        substruct0 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 0)->type0);
        substruct1 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 1)->type1);
        substruct2 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 2)->type2);
        substruct3 = &(GetSubstruct(pcMonster, pcMonster->MonsterSeed, 3)->type3);

        DecryptPCMonster(pcMonster);

        if (CalculatePCMonsterCheckSum(pcMonster) != pcMonster->checkSum)
        {
            LogMessageA(LL_WARNING, "[%s] CalculateMonsterCheckSum did not return same value as pcMonster->checkSum in GetPCMonsterData!", __FUNCTION__);
        }
    }

    switch (field)
    {
        case MONSTER_DATA_MONSTER_SEED:
        {
            GetValue = pcMonster->MonsterSeed;
            break;
        }
        case MONSTER_DATA_PLAYER_SEED:
        {
            GetValue = pcMonster->PlayerSeed;
            break;
        }
        case MONSTER_DATA_NICKNAME:
        {
            for (GetValue = 0; GetValue < MAX_MONSTER_NAME_LENGTH; data[GetValue] = pcMonster->nickname[GetValue], GetValue++)
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
                data[GetValue] = pcMonster->playerName[GetValue];
                GetValue++;
            }
            data[GetValue] = END_OF_STRING;
            break;
        }
        case MONSTER_DATA_CHECKSUM:
        {
            GetValue = pcMonster->checkSum;
            break;
        }
        case MONSTER_DATA_ENCRYPT:
        {
            GetValue = pcMonster->unknown;
            break;
        }
        case MONSTER_DATA_INDEX:
        {
            GetValue = substruct0->Index;
            break;
        }
        case MONSTER_DATA_HELDITEM:
        {
            GetValue = substruct0->HeldItem;
            break;
        }
        case MONSTER_DATA_EXPERIENCE:
        {
            GetValue = substruct0->Experience;
            break;
        }
        case MONSTER_DATA_FRIENDSHIP:
        {
            GetValue = substruct0->Friendship;
            break;
        }
        case MONSTER_DATA_MOVE_1:
        case MONSTER_DATA_MOVE_2:
        case MONSTER_DATA_MOVE_3:
        case MONSTER_DATA_MOVE_4:
        {
            GetValue = substruct1->Moves[field - MONSTER_DATA_MOVE_1];
            break;
        }
        case MONSTER_DATA_HP_TRAINING:
        {
            GetValue = substruct2->HpTraining;
            break;
        }
        case MONSTER_DATA_ATTACK_TRAINING:
        {
            GetValue = substruct2->AttackTraining;
            break;
        }
        case MONSTER_DATA_DEFENSE_TRAINING:
        {
            GetValue = substruct2->DefenseTraining;
            break;
        }
        case MONSTER_DATA_SPEED_TRAINING:
        {
            GetValue = substruct2->SpeedTraining;
            break;
        }
        case MONSTER_DATA_PSI_TRAINING:
        {
            GetValue = substruct2->PsiTraining;
            break;
        }
        case MONSTER_DATA_RESOLVE_TRAINING:
        {
            GetValue = substruct2->ResolveTraining;
            break;
        }
        case MONSTER_DATA_MET_LOCATION:
        {
            GetValue = substruct3->MetLocation;
            break;
        }
        case MONSTER_DATA_MET_LEVEL:
        {
            GetValue = substruct3->MetLevel;
            break;
        }
        case MONSTER_DATA_HP_GENETICS:
        {
            GetValue = substruct3->HpGenetics;
            break;
        }
        case MONSTER_DATA_ATTACK_GENETICS:
        {
            GetValue = substruct3->AttackGenetics;
            break;
        }
        case MONSTER_DATA_DEFENSE_GENETICS:
        {
            GetValue = substruct3->DefenseGenetics;
            break;
        }
        case MONSTER_DATA_SPEED_GENETICS:
        {
            GetValue = substruct3->SpeedGenetics;
            break;
        }
        case MONSTER_DATA_PSI_GENETICS:
        {
            GetValue = substruct3->PsiGenetics;
            break;
        }
        case MONSTER_DATA_RESOLVE_GENETICS:
        {
            GetValue = substruct3->ResolveGenetics;
            break;
        }
        case MONSTER_DATA_ABILITY_NUMBER:
        {
            GetValue = substruct3->AbilityNumber;
            break;
        }
        case MONSTER_DATA_GENETICS:
        {
            GetValue = substruct3->HpGenetics
                | (substruct3->AttackGenetics << 5)
                | (substruct3->DefenseGenetics << 10)
                | (substruct3->SpeedGenetics << 15)
                | (substruct3->PsiGenetics << 20)
                | (substruct3->ResolveGenetics << 25);
            break;
        }
        case MONSTER_DATA_KNOWN_MOVES:
        {
            if (substruct0->Index)
            {
                uint16_t* moves = (uint16_t*)data;
                int32_t i = 0;

                while (moves[i] != MAX_NONSIGNATURE_MOVES)
                {
                    uint16_t move = moves[i];
                    if (substruct1->Moves[0] == move || substruct1->Moves[1] == move || substruct1->Moves[2] == move || substruct1->Moves[3] == move)
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

    if (field > MONSTER_DATA_ENCRYPT)
    {
        EncryptPCMonster(pcMonster);
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

static void EncryptPCMonster(struct PCMonster* pcMonster)
{
    uint32_t i;
    for (i = 0; i < (size_t)(sizeof(pcMonster->secure.raw) / sizeof((pcMonster->secure.raw)[0])); i++)
    {
        pcMonster->secure.raw[i] ^= pcMonster->MonsterSeed;
        pcMonster->secure.raw[i] ^= pcMonster->PlayerSeed;
    }
}

static void DecryptPCMonster(struct PCMonster* pcMonster)
{
    uint32_t i;
    for (i = 0; i < (size_t)(sizeof(pcMonster->secure.raw) / sizeof((pcMonster->secure.raw)[0])); i++)
    {
        pcMonster->secure.raw[i] ^= pcMonster->PlayerSeed;
        pcMonster->secure.raw[i] ^= pcMonster->MonsterSeed;
    }
}

#define SUBSTRUCT_CASE(n, v1, v2, v3, v4)                               \
case n:                                                                 \
    {                                                                   \
    union MonsterSubstruct *substructs0 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs1 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs2 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs3 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs4 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs5 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs6 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs7 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs8 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs9 = pcMonster->secure.substructs;    \
    union MonsterSubstruct *substructs10 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs11 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs12 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs13 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs14 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs15 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs16 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs17 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs18 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs19 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs20 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs21 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs22 = pcMonster->secure.substructs;   \
    union MonsterSubstruct *substructs23 = pcMonster->secure.substructs;   \
                                                                        \
        switch (substructType)                                          \
        {                                                               \
        case 0:                                                         \
            substruct = &substructs ## n [v1];                          \
            break;                                                      \
        case 1:                                                         \
            substruct = &substructs ## n [v2];                          \
            break;                                                      \
        case 2:                                                         \
            substruct = &substructs ## n [v3];                          \
            break;                                                      \
        case 3:                                                         \
            substruct = &substructs ## n [v4];                          \
            break;                                                      \
        }                                                               \
        break;                                                          \
    } 

static union MonsterSubstruct* GetSubstruct(struct PCMonster* pcMonster, uint32_t MonsterSeed, uint8_t substructType)
{
    union MonsterSubstruct* substruct = NULL;

    switch (MonsterSeed % 24)
    {
        SUBSTRUCT_CASE(0, 0, 1, 2, 3)
            SUBSTRUCT_CASE(1, 0, 1, 3, 2)
            SUBSTRUCT_CASE(2, 0, 2, 1, 3)
            SUBSTRUCT_CASE(3, 0, 3, 1, 2)
            SUBSTRUCT_CASE(4, 0, 2, 3, 1)
            SUBSTRUCT_CASE(5, 0, 3, 2, 1)
            SUBSTRUCT_CASE(6, 1, 0, 2, 3)
            SUBSTRUCT_CASE(7, 1, 0, 3, 2)
            SUBSTRUCT_CASE(8, 2, 0, 1, 3)
            SUBSTRUCT_CASE(9, 3, 0, 1, 2)
            SUBSTRUCT_CASE(10, 2, 0, 3, 1)
            SUBSTRUCT_CASE(11, 3, 0, 2, 1)
            SUBSTRUCT_CASE(12, 1, 2, 0, 3)
            SUBSTRUCT_CASE(13, 1, 3, 0, 2)
            SUBSTRUCT_CASE(14, 2, 1, 0, 3)
            SUBSTRUCT_CASE(15, 3, 1, 0, 2)
            SUBSTRUCT_CASE(16, 2, 3, 0, 1)
            SUBSTRUCT_CASE(17, 3, 2, 0, 1)
            SUBSTRUCT_CASE(18, 1, 2, 3, 0)
            SUBSTRUCT_CASE(19, 1, 3, 2, 0)
            SUBSTRUCT_CASE(20, 2, 1, 3, 0)
            SUBSTRUCT_CASE(21, 3, 1, 2, 0)
            SUBSTRUCT_CASE(22, 2, 3, 1, 0)
            SUBSTRUCT_CASE(23, 3, 2, 1, 0)
    }

    return (substruct);
}

static uint16_t CalculatePCMonsterCheckSum(struct PCMonster* pcMonster)
{
    uint16_t checksum = 0;
    union MonsterSubstruct* substruct0 = GetSubstruct(pcMonster, pcMonster->MonsterSeed, 0);
    union MonsterSubstruct* substruct1 = GetSubstruct(pcMonster, pcMonster->MonsterSeed, 1);
    union MonsterSubstruct* substruct2 = GetSubstruct(pcMonster, pcMonster->MonsterSeed, 2);
    union MonsterSubstruct* substruct3 = GetSubstruct(pcMonster, pcMonster->MonsterSeed, 3);
    int32_t i;

    for (i = 0; i < (int32_t)(size_t)(sizeof(substruct0->raw) / sizeof((substruct0->raw)[0])); i++)
    {
        checksum += substruct0->raw[i];
    }
    for (i = 0; i < (int32_t)(size_t)(sizeof(substruct1->raw) / sizeof((substruct1->raw)[0])); i++)
    {
        checksum += substruct1->raw[i];
    }
    for (i = 0; i < (int32_t)(size_t)(sizeof(substruct2->raw) / sizeof((substruct2->raw)[0])); i++)
    {
        checksum += substruct2->raw[i];
    }
    for (i = 0; i < (int32_t)(size_t)(sizeof(substruct3->raw) / sizeof((substruct3->raw)[0])); i++)
    {
        checksum += substruct3->raw[i];
    }

    return (checksum);
}

uint8_t CountAliveMonstersInBattle(uint8_t caseId)
{

}

uint8_t GetMonsterGender(struct Monster* monster)
{
    return (GetPCMonsterGender(&monster->PcMonster));
}

uint8_t GetPCMonsterGender(struct PCMonster* pcMonster)
{
    uint8_t monsterIndex = GetPCMonsterData(pcMonster, MONSTER_DATA_INDEX, NULL);
    uint32_t monsterSeed = GetPCMonsterData(pcMonster, MONSTER_DATA_MONSTER_SEED, NULL);

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
        //return(SendMonsterToPC(monster));
    }

    CopyMonster(&gPlayerParty[i], monster, sizeof(monster));
    gPlayerPartyCount = i + 1;
    return(0);
}

uint8_t SendMonsterToPC(struct Monster* monster)            //TODO: FINISH
{
    //int32_t ;
    //SetPCStorageToSendMonster();

}