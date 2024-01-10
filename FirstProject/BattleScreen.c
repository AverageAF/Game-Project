
#include "Main.h"

#include "BattleScreen.h"

#include "MonsterData.h"

#include "InventoryItems.h"
#include "Inventory.h"
#include "OverWorld.h"

///////// combat variables

//uint8_t StatusEffectPlayer = 0;           ////shouldnt store these here, because we want to retain these out of combat
//uint8_t StatusEffectOpponent = 0;

uint8_t gStatChangesCurrentPlayerMon[5] = { 7, 7, 7, 7, 7 };       //0Atk 1Def 2Spe 3Psi 4Res       7 = neutral, 1-6 = lowered, 8-13 = hightened
uint8_t gStatChangesCurrentOpponentMon[5] = { 7, 7, 7, 7, 7 };
BOOL gStatsChangedFromPlayerMove = FALSE;
BOOL gStatsChangedFromOpponentMove = FALSE;

///////// escape variables

uint8_t gEscapeTriesThisBattle = 0;

///////// capture monster variables

BOOL gCaptureMonsterSuccess = FALSE;
BOOL gCaptureCalculationFinished = FALSE;
uint8_t gCaptureNumDotsWait = 1;
uint8_t gCaptureHpHelp = 0;

///////// item variables

uint8_t gCaptureDeviceHelp = 0;
uint8_t gUsableItemSelectedPartyMember = 0;
uint8_t gUseableItemEffect_SelectedItem = ITEM_USE_EFFECT_NULL;

///////// load starting monster
BOOL gWasMonsterKOed = FALSE;
BOOL gWasLastMoveCriticalHit = FALSE;
uint8_t gLastMoveElementalBonus = 0;

uint8_t gPartyMemberToSwitchIn = 0;
uint8_t gCurrentPartyMember = 0;
uint8_t gCurrentOpponentPartyMember = 0;
uint16_t gSelectedPlayerMove = 0;
uint8_t gSelectedPlayerMoveSlot = 0;
uint16_t gSelectedOpponentMove = 0;
uint8_t gSelectedOpponentMoveSlot = 0;

////////Initial starting choices at the start of battle

MENUITEM gMI_BattleScreen_FightButton = { "Fight!", (GAME_RES_WIDTH / 2) - (6 * 7 / 2) + 2, 200, TRUE, MenuItem_BattleScreen_FightButton };

MENUITEM gMI_BattleScreen_EscapeButton = { "Escape", (GAME_RES_WIDTH / 2) - (6 * 7 / 2) + 2, 226, TRUE, MenuItem_BattleScreen_EscapeButton };

MENUITEM gMI_BattleScreen_SwitchButton = { "Switch", 64 + 12, 210, TRUE, MenuItem_BattleScreen_SwitchButton };

MENUITEM gMI_BattleScreen_ItemsButton = { "Items", 256 + 14, 210, TRUE, MenuItem_BattleScreen_ItemsButton };

MENUITEM* gMI_BattleScreen_Items[] = { &gMI_BattleScreen_FightButton, &gMI_BattleScreen_EscapeButton, &gMI_BattleScreen_SwitchButton, &gMI_BattleScreen_ItemsButton };

MENU gMenu_BattleScreen = { "Battle Menu", 0 , _countof(gMI_BattleScreen_Items), gMI_BattleScreen_Items };

////////Menu choices for switching monsters in battle

MENUITEM gMI_SwitchScreen_PartySlot0 = { &gPlayerParty[0].DriveMonster.nickname, 70, 187, FALSE, MenuItem_SwitchScreen_PartySelected };
MENUITEM gMI_SwitchScreen_PartySlot1 = { &gPlayerParty[1].DriveMonster.nickname, 200, 187, FALSE, MenuItem_SwitchScreen_PartySelected };
MENUITEM gMI_SwitchScreen_PartySlot2 = { &gPlayerParty[2].DriveMonster.nickname, 70, 207, FALSE, MenuItem_SwitchScreen_PartySelected };
MENUITEM gMI_SwitchScreen_PartySlot3 = { &gPlayerParty[3].DriveMonster.nickname, 200, 207, FALSE, MenuItem_SwitchScreen_PartySelected };
MENUITEM gMI_SwitchScreen_PartySlot4 = { &gPlayerParty[4].DriveMonster.nickname, 70, 230, FALSE, MenuItem_SwitchScreen_PartySelected };
MENUITEM gMI_SwitchScreen_PartySlot5 = { &gPlayerParty[5].DriveMonster.nickname, 200, 230, FALSE, MenuItem_SwitchScreen_PartySelected };

MENUITEM gMI_SwitchScreen_BackButton = { "Back", 28, 190, TRUE, MenuItem_SwitchScreen_BackButton };

MENUITEM* gMI_SwitchScreen_Items[] = { &gMI_SwitchScreen_PartySlot0, &gMI_SwitchScreen_PartySlot1, &gMI_SwitchScreen_PartySlot2, &gMI_SwitchScreen_PartySlot3, &gMI_SwitchScreen_PartySlot4, &gMI_SwitchScreen_PartySlot5, &gMI_SwitchScreen_BackButton };

MENU gMenu_SwitchScreen = { "Switch Menu", 0, _countof(gMI_SwitchScreen_Items), gMI_SwitchScreen_Items };

//////////Menu choices for selecting moves

MENUITEM gMI_MoveScreen_MoveSlot0 = { "Slot1", 72, 187, TRUE, MenuItem_MoveScreen_MoveSlot0};

MENUITEM gMI_MoveScreen_MoveSlot1 = { "Slot2", 72, 201, TRUE, MenuItem_MoveScreen_MoveSlot1};

MENUITEM gMI_MoveScreen_MoveSlot2 = { "Slot3", 72, 215, TRUE, MenuItem_MoveScreen_MoveSlot2};

MENUITEM gMI_MoveScreen_MoveSlot3 = { "Slot4", 72, 229, TRUE, MenuItem_MoveScreen_MoveSlot3};

MENUITEM gMI_MoveScreen_MoveSlotSignature = { "Signature", 316 , 206, TRUE, MenuItem_MoveScreen_SignatureMove};

MENUITEM gMI_MoveScreen_BackButton = { "Back", 1 + (6 * 4 / 2), 206, TRUE, MenuItem_MoveScreen_BackButton };

MENUITEM* gMI_MoveScreen_Items[] = { &gMI_MoveScreen_BackButton, &gMI_MoveScreen_MoveSlot0, &gMI_MoveScreen_MoveSlot1, &gMI_MoveScreen_MoveSlot2, &gMI_MoveScreen_MoveSlot3, &gMI_MoveScreen_MoveSlotSignature };

MENU gMenu_MoveScreen = { "Move Menu", 0, _countof(gMI_MoveScreen_Items), gMI_MoveScreen_Items };

//////////Menu choices for usable items

MENUITEM gMI_UseableScreen_ItemSlot0 = { "ItemSlot0", 66, 180, TRUE, MenuItem_UseableScreen_SlotSelected };

MENUITEM gMI_UseableScreen_ItemSlot1 = { "ItemSlot1", 66, 180 + (12 * 1), TRUE, MenuItem_UseableScreen_SlotSelected };

MENUITEM gMI_UseableScreen_ItemSlot2 = { "ItemSlot2", 66, 180 + (12 * 2), TRUE, MenuItem_UseableScreen_SlotSelected };

MENUITEM gMI_UseableScreen_ItemSlot3 = { "ItemSlot3", 66, 180 + (12 * 3), TRUE, MenuItem_UseableScreen_SlotSelected };

MENUITEM gMI_UseableScreen_ItemSlot4 = { "ItemSlot4", 66, 180 + (12 * 4), TRUE, MenuItem_UseableScreen_SlotSelected };

MENUITEM gMI_UseableScreen_BackButton = { "Back", 34, 180, TRUE, MenuItem_UseableScreen_BackButton };

MENUITEM* gMI_UseableScreen_Items[] = { &gMI_UseableScreen_ItemSlot0, &gMI_UseableScreen_ItemSlot1, &gMI_UseableScreen_ItemSlot2, &gMI_UseableScreen_ItemSlot3, &gMI_UseableScreen_ItemSlot4, &gMI_UseableScreen_BackButton };

MENU gMenu_UseableScreen = { "Useable Items", 0, _countof(gMI_UseableScreen_Items), gMI_UseableScreen_Items };

//////////

char gBattleTextLine[MAX_DIALOGUE_ROWS + 1][MAX_BATTLECHAR_PER_ROW];      //first line of dialogue in combat text

GAMEBITMAP* gBattleScene = NULL;

GAMEBITMAP* gPlayerMonsterSprite = NULL;

GAMEBITMAP* gOpponentMonsterSprite = NULL;


void DrawBattleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    static uint8_t BattleTextLineCharactersToShow = 0;
    static uint16_t BattleTextLineCharactersWritten = 0;
    static uint8_t BattleTextRowsToShow = 0;
    static uint8_t BattleTextLineCount = 0;
    static uint16_t DamageToPlayer = 0;
    static uint16_t DamageToOpponent = 0;
    static uint32_t CalculatedExpReward = 0;
    static BOOL IsPlayerMovingFirst = FALSE;
    static BOOL MonsterHasKOed = FALSE;

    uint8_t Opponent = NULL;

    BOOL TextHasFinished = FALSE;



    char BattleTextLineScratch[40] = { 0 };

    for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
    {
        if ((gCharacterSprite[Index].InteractedWith == TRUE) && (gCharacterSprite[Index].Event == EVENT_FLAG_BATLLE))
        {
            Opponent = Index;
            break;
        }
    }

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        gCurrentBattleState = 0;
        gCurrentPartyMember = 0;
        gCurrentOpponentPartyMember = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
        BattleTextLineCharactersToShow = 0;
        BattleTextLineCharactersWritten = 0;
        BattleTextRowsToShow = 0;
        CalculatedExpReward = 0;
        gMenu_BattleScreen.SelectedItem = 0;

        ReSortUsableitems();

        gMI_MoveScreen_MoveSlot0.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[0]];
        gMI_MoveScreen_MoveSlot1.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[1]];
        gMI_MoveScreen_MoveSlot2.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[2]];
        gMI_MoveScreen_MoveSlot3.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[3]];

        for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
        {
            for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
            {
                gBattleTextLine[i][j] = 0;
            }
        }

        if (Opponent == NULL)
        {
            DWORD Random;
            uint8_t MonsterIndex;

        ReRandomizeIndex:

            rand_s((unsigned int*)&Random);
            Random %= NUM_MONSTER_ENCOUNTER_CHANCE_SLOTS;
            MonsterIndex = gCurrentEncounterArea.MonsterIndexChanceSlots[(uint8_t)Random];

            if (MonsterIndex == MONSTER_NULL)
            {
                goto ReRandomizeIndex;
            }

            BattleTextLineCount = 0;
            CopyMonsterToOpponentParty(0, GenerateScriptedMonsterForWildEncounter(MonsterIndex, gCurrentEncounterArea.MaxLevel, gCurrentEncounterArea.MinLevel, 0));
            //CopyMonsterToOpponentParty(0, GenerateRandMonsterForWildEncounter(6, 4, 0));
            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s encountered a %s!", gPlayer.Name, &gMonsterNames[gOpponentParty[0].DriveMonster.Index]);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s Sent out %s!", gPlayer.Name, &gPlayerParty[0].DriveMonster.nickname);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "What will %s do?", &gPlayerParty[0].DriveMonster.nickname);
            BattleTextLineCount++;

        }
        else
        {
            for (uint8_t i = 0; i < MAX_PARTY_SIZE; i++)
            {
                if (&gCharacterSprite[Opponent].MonsterParty[i].DriveMonster.Index != MONSTER_NULL)
                {
                    CopyMonsterToOpponentParty(i, gCharacterSprite[Opponent].MonsterParty[i]);
                }
            }
            BattleTextLineCount = 0;
            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s was challenged by %s", gPlayer.Name, gCharacterSprite[Opponent].Name);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s Sent out %s!", gCharacterSprite[Opponent].Name, &gMonsterNames[gOpponentParty[0].DriveMonster.Index]);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "%s Sent out %s!", gPlayer.Name, &gPlayerParty[0].DriveMonster.nickname);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[4], sizeof(gBattleTextLine[4]), "What will %s do?", &gPlayerParty[0].DriveMonster.nickname);
            BattleTextLineCount++;
        }

        for (uint8_t Counter = 0; Counter < NUM_MONSTERS; Counter++)
        {
            if (gPlayerParty[0].DriveMonster.Index == Counter)
            {
                gPlayerMonsterSprite = &gBattleSpriteBack[Counter];
            }
            if (gOpponentParty[0].DriveMonster.Index == Counter)
            {
                gOpponentMonsterSprite = &gBattleSpriteFront[Counter];
            }
        }
    }

    //////TODO: Add battle intro and battle music/////////////
    //if (LocalFrameCounter == 0)
    //{
    //    StopGameMusic();
    //    /*PlayGameMusic(&MusicBattleInto01, FALSE, TRUE);
    //    PlayGameMusic(&MusicBattle01, TRUE, FALSE);*/       ////queue full loop behind intro
    //}


    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawWindow(64, 48, 256, 128, &COLOR_NES_WHITE, NULL, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);

    switch (gCurrentBattleState)
    {
        case BATTLESTATE_OPENING_TEXT:
        {
        PlayerStartBattle:
            if (gPlayerParty[gCurrentPartyMember].Health == 0)
            {
                if (gCurrentPartyMember < gPlayerPartyCount - 1)
                {
                    gCurrentPartyMember++;
                    goto PlayerStartBattle;
                }
                else
                {
                    gCurrentBattleState = BATTLESTATE_KO;
                }

                goto PlayerNoHP;
            }

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_OPENING_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitOpening;
            }

            break;
        }
        case BATTLESTATE_OPENING_WAIT:
        {
    WaitOpening:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_TURNORDER_CALC:
        {

            GenerateOpponentMove(Opponent);

            IsPlayerMovingFirst = CalculateSpeedPriorityIfPlayerMovesFirst(gPlayerParty[gCurrentPartyMember].Speed, gOpponentParty[gCurrentOpponentPartyMember].Speed);

            gCurrentBattleState = BATTLESTATE_FIRSTMOVE_TEXT;
            break;
        }
        case BATTLESTATE_ESCAPESUCCESS_TEXT:
        {
            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;

            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s managed to escape!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
            BattleTextLineCount++;

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_ESCAPESUCCESS_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitEscapeSuccess;
            }

            break;
        }
        case BATTLESTATE_ESCAPESUCCESS_WAIT:
        {

        WaitEscapeSuccess:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_ESCAPEFAIL_TEXT:
        {

            GenerateOpponentMove(Opponent);
            IsPlayerMovingFirst = TRUE;

            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;

            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s couldn't escape!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
            BattleTextLineCount++;

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_ESCAPEFAIL_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitEscapeFail;
            }

            break;
        }
        case BATTLESTATE_ESCAPEFAIL_WAIT:
        {

        WaitEscapeFail:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_NOESCAPE_TEXT:
        {
            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;

            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "You've been challenged by %s!", &gCharacterSprite[Opponent].Name);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "There is no turning back now!");
            BattleTextLineCount++;

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_NOESCAPE_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitEscapeFail;
            }

            break;
        }
        case BATTLESTATE_NOESCAPE_WAIT:
        {

        WaitNoEscape:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_FIRSTMOVE_TEXT:
        {

            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;
            if (IsPlayerMovingFirst == TRUE)
            {
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname, &gBattleMoveNames[gSelectedPlayerMove]);
                BattleTextLineCount++;
            }
            else
            {
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname, &gBattleMoveNames[gSelectedOpponentMove]);
                BattleTextLineCount++;
            }

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_FIRSTMOVE_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitFirstMove;
            }

            break;
        }
        case BATTLESTATE_FIRSTMOVE_WAIT:
        {
        WaitFirstMove:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_FIRSTMOVE_CALC:
        {
            gLastMoveElementalBonus = ELEMENT_NEUTRAL;

            if (IsPlayerMovingFirst == TRUE)
            {
                DamageToOpponent =
                    CalcDmgFromMonsterAToMonsterB(
                        gPlayerParty[gCurrentPartyMember].Level,
                        gPlayerParty[gCurrentPartyMember].Attack,
                        gOpponentParty[gCurrentOpponentPartyMember].Defense,
                        gPlayerParty[gCurrentPartyMember].Psi,
                        gOpponentParty[gCurrentOpponentPartyMember].Resolve,
                        gBattleMoves[gSelectedPlayerMove].power1,
                        gBattleMoves[gSelectedPlayerMove].power2,
                        gBattleMoves[gSelectedPlayerMove].power3,
                        gBattleMoves[gSelectedPlayerMove].split,
                        TRUE
                    );

                if (gBattleMoves[gSelectedPlayerMove].split != SPLIT_STATUS)
                {
                    DamageToOpponent = GetElementaBonusDamage(DamageToOpponent, FALSE);
                }
                else
                {
                    if (gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_SELF)
                    {
                        switch (gBattleMoves[gSelectedPlayerMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentPlayerMon[0]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[0]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentPlayerMon[1]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[1]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                        }
                    }
                    else if (gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_OPPONENT)
                    {
                        switch (gBattleMoves[gSelectedPlayerMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentOpponentMon[0]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[0]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentOpponentMon[1]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[1]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                        }
                    }
                }

                ModifyMonsterHealthValueGetKO(DamageToOpponent, FALSE, IsPlayerMovingFirst);
            }
            else
            {
                DamageToPlayer =
                    CalcDmgFromMonsterAToMonsterB(
                        gOpponentParty[gCurrentOpponentPartyMember].Level,
                        gOpponentParty[gCurrentOpponentPartyMember].Attack,
                        gPlayerParty[gCurrentPartyMember].Defense,
                        gOpponentParty[gCurrentOpponentPartyMember].Psi,
                        gPlayerParty[gCurrentPartyMember].Resolve,
                        gBattleMoves[gSelectedOpponentMove].power1,
                        gBattleMoves[gSelectedOpponentMove].power2,
                        gBattleMoves[gSelectedOpponentMove].power3,
                        gBattleMoves[gSelectedOpponentMove].split,
                        FALSE
                    );

                if (gBattleMoves[gSelectedOpponentMove].split != SPLIT_STATUS)
                {
                    DamageToPlayer = GetElementaBonusDamage(DamageToPlayer, TRUE);
                }
                else
                {
                    if (gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_OPPONENT)
                    {
                        switch (gBattleMoves[gSelectedOpponentMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentPlayerMon[0]++;      //bugfix currently no overflow system, can go below min and max values of StatChanges
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[0]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentPlayerMon[1]++;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[1]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                        }
                    }
                    else if (gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_SELF)
                    {
                        switch (gBattleMoves[gSelectedOpponentMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentOpponentMon[0]++;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[0]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentOpponentMon[1]++;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[1]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                        }
                    }
                }

                ModifyMonsterHealthValueGetKO(DamageToPlayer, TRUE, IsPlayerMovingFirst);
            }

            break;
        }
        case BATTLESTATE_FIRSTMOVE_POSTTEXT:
        {
            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;
            if (IsPlayerMovingFirst == TRUE)
            {
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {

                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move dealt bonus", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                    sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "elemental damage!");
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move was resisted...", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_IMMUNE)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move had no effect!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }

                if (gWasLastMoveCriticalHit == TRUE)
                {
                    sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "It dealt extra devastating damage!");
                    BattleTextLineCount++;
                }

                if (gStatsChangedFromPlayerMove == TRUE && gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_SELF)
                {
                    switch (gBattleMoves[gSelectedPlayerMove].effect)
                    {
                        case EFFECT_ATTACK_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went up!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went up!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
                else if (gStatsChangedFromPlayerMove == TRUE && gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_OPPONENT)
                {
                    switch (gBattleMoves[gSelectedPlayerMove].effect)
                    {

                        case EFFECT_ATTACK_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went down!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went down!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
            }
            else
            {
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {

                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move dealt bonus", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                    sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "elemental damage!");
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move was resisted...", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_IMMUNE)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move had no effect!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }

                if (gWasLastMoveCriticalHit == TRUE)
                {
                    sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "It dealt extra devastating damage!");
                    BattleTextLineCount++;
                }

                if (gStatsChangedFromOpponentMove == TRUE && gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_SELF)
                {
                    switch (gBattleMoves[gSelectedOpponentMove].effect)
                    {
                        case EFFECT_ATTACK_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went up!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went up!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
                else if (gStatsChangedFromOpponentMove == TRUE && gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_OPPONENT)
                {
                    switch (gBattleMoves[gSelectedOpponentMove].effect)
                    {

                        case EFFECT_ATTACK_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went down!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went down!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
            }

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_FIRSTMOVE_POSTWAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitPostFirstMove;
            }

            break;
        }
        case BATTLESTATE_FIRSTMOVE_POSTWAIT:
        {

        WaitPostFirstMove:

            if (IsPlayerMovingFirst && gStatsChangedFromPlayerMove)
            {
                gStatsChangedFromPlayerMove = FALSE;
            }
            else if (!IsPlayerMovingFirst && gStatsChangedFromOpponentMove)
            {
                gStatsChangedFromOpponentMove = FALSE;
            }

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_SWITCHING_TEXT:
        {

            GenerateOpponentMove(Opponent);
            IsPlayerMovingFirst = TRUE;


            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;
            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "Come back %s!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "Go %s!", &gPlayerParty[gPartyMemberToSwitchIn].DriveMonster.nickname);
            BattleTextLineCount++;

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_SWITCHING_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitSwitching;
            }

            break;
        }
        case BATTLESTATE_SWITCHING_WAIT:
        {

        WaitSwitching:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            gCurrentPartyMember = gPartyMemberToSwitchIn;                                               ////instant monsters are swapped
            for (uint8_t stats = 0; stats < 4; stats++)
            {
                gStatChangesCurrentPlayerMon[stats] = 7;
            }

            for (uint8_t Counter = 0; Counter < NUM_MONSTERS; Counter++)
            {
                if (gPlayerParty[gCurrentPartyMember].DriveMonster.Index == Counter)
                {
                    gPlayerMonsterSprite = &gBattleSpriteBack[Counter];
                }
            }

            gMI_MoveScreen_MoveSlot0.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[0]];
            gMI_MoveScreen_MoveSlot1.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[1]];
            gMI_MoveScreen_MoveSlot2.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[2]];
            gMI_MoveScreen_MoveSlot3.Name = gBattleMoveNames[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[3]];
            
            break;
        }
        case BATTLESTATE_USEITEM_TEXT:
        {

            GenerateOpponentMove(Opponent);
            IsPlayerMovingFirst = TRUE;


            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;
            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gPlayer.Name, gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Name);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s was healed!", &gPlayerParty[gUsableItemSelectedPartyMember].DriveMonster.nickname);
            BattleTextLineCount++;


            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_SWITCHING_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitUseItem;
            }

            break;
        }
        case BATTLESTATE_USEITEM_WAIT:
        {

        WaitUseItem:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            //TODO: maybe move healing calculation to right here??

            break;
        }
        case BATTLESTATE_CATCH_TEXT:
        {

            GenerateOpponentMove(Opponent);
            IsPlayerMovingFirst = TRUE;

            ReSortUsableitems();

            if (gCaptureCalculationFinished == FALSE)
            {
                gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Count--;         //remove one of selected item from inventory

                uint8_t CatchDifficulty = 0;
                DWORD Random = 0;
                uint8_t Random8 = 0;

                if (gCaptureDeviceHelp == 255)
                {
                    CatchDifficulty = 0;
                }
                else if (gBaseStats[gOpponentParty[0].DriveMonster.Index].catchrate > gCaptureDeviceHelp + gCaptureHpHelp + 16)
                {
                    CatchDifficulty = gBaseStats[gOpponentParty[0].DriveMonster.Index].catchrate - gCaptureDeviceHelp - gCaptureHpHelp;
                }
                else if (gBaseStats[gOpponentParty[0].DriveMonster.Index].catchrate <= gCaptureDeviceHelp + gCaptureHpHelp + 16)
                {
                    CatchDifficulty = 16;
                }

                rand_s((unsigned int*)&Random);
                Random8 = (uint8_t*)Random;

                if (Random8 == 0)
                {
                    Random8++;
                }

                if (Random8 > CatchDifficulty)      //success
                {
                    gCaptureNumDotsWait = 0;
                }
                else if (Random8 > (CatchDifficulty * 2) / 3)
                {
                    gCaptureNumDotsWait = 3;
                }
                else if (Random8 > (CatchDifficulty * 1) / 3 && Random8 <= (CatchDifficulty * 2) / 3)
                {
                    gCaptureNumDotsWait = 2;
                }
                else
                {
                    gCaptureNumDotsWait = 1;
                }

                gCaptureCalculationFinished = TRUE;
            }

            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;
            if (Opponent == NULL)           ////NULL if wild encounter, since Opponent is the charactersprite.index
            {
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gPlayer.Name, gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Name);
                BattleTextLineCount++;
                switch (gCaptureNumDotsWait)
                {
                    case 0:         //successful capture
                    {
                        sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[4], sizeof(gBattleTextLine[4]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[5], sizeof(gBattleTextLine[5]), "%s was caught!", gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                        BattleTextLineCount++;
                        break;
                    }
                    case 1:
                    {
                        sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "%s broke free!", gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                        BattleTextLineCount++;
                        break;
                    }
                    case 2:
                    {
                        sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[4], sizeof(gBattleTextLine[3]), "%s broke free after a while!", gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                        BattleTextLineCount++;
                        break;
                    }
                    case 3:
                    {
                        sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[4], sizeof(gBattleTextLine[4]), ".....");
                        BattleTextLineCount++;
                        sprintf_s((char*)gBattleTextLine[5], sizeof(gBattleTextLine[3]), "%s broke free, so close!", gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                        BattleTextLineCount++;
                        break;
                    }
                }

                TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_CATCH_WAIT, BattleTextLineCount, LocalFrameCounter);

                if (TextHasFinished == TRUE)
                {
                    goto WaitCatch;
                }
            }
            else
            {
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gPlayer.Name, gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Name);
                BattleTextLineCount++;
                sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s blocked the device!", &gCharacterSprite[Opponent].Name);
                BattleTextLineCount++;
                sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "You can't capture their monster!");
                BattleTextLineCount++;

                gCaptureNumDotsWait = 1;

                TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_CATCH_WAIT, BattleTextLineCount, LocalFrameCounter);

                if (TextHasFinished == TRUE)
                {
                    goto WaitCatch;
                }
            }


            break;
        }
        case BATTLESTATE_CATCH_WAIT:
        {


        WaitCatch:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            gCaptureCalculationFinished = FALSE;

            break;
        }
        case BATTLESTATE_CATCHSUCCESS_TEXT:
        {
            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            CalculatedExpReward = (gBaseStats[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Index].expYield * gOpponentParty[gCurrentOpponentPartyMember].Level) / 4;

            BattleTextLineCount = 0;
            sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s caught %s!", &gPlayer.Name, gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s was added to the party!", gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "%s gained %d Exp!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname, CalculatedExpReward);
            BattleTextLineCount++;

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_CATCHSUCCESS_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitCatchSuccess;
            }

            break;
        }
        case BATTLESTATE_CATCHSUCCESS_WAIT:
        {

        WaitCatchSuccess:

            if (CalculatedExpReward != 0)
            {

                GiveMonsterToPlayer(&gOpponentParty[gCurrentOpponentPartyMember]);

                BOOL DidMonsterLevelUp = 0;

                gPlayerParty[gCurrentPartyMember].DriveMonster.Experience = gPlayerParty[gCurrentPartyMember].DriveMonster.Experience + CalculatedExpReward;
                CalculatedExpReward = 0;

                DidMonsterLevelUp = TryIncrementMonsterLevel(&gPlayerParty[gCurrentPartyMember]);

                if (DidMonsterLevelUp == TRUE)
                {
                    CalculateMonsterStats(&gPlayerParty[gCurrentPartyMember]);

                    MonsterTryLearningNewMove(&gPlayerParty[gCurrentPartyMember], TRUE);
                }
            }

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_SECONDMOVE_TEXT:
        {

            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;
            
            if (IsPlayerMovingFirst == FALSE)
            {   
                //player moving second bc FALSE
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname, &gBattleMoveNames[gSelectedPlayerMove]);
                BattleTextLineCount++;
            }
            else
            {
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname, &gBattleMoveNames[gSelectedOpponentMove]);
                BattleTextLineCount++;
            }

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_SECONDMOVE_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitSecondMove;
            }

            break;
        }
        case BATTLESTATE_SECONDMOVE_WAIT:
        {
        WaitSecondMove:

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_SECONDMOVE_CALC:
        {
            gLastMoveElementalBonus = ELEMENT_NEUTRAL;

            if (IsPlayerMovingFirst == FALSE)
            {
                DamageToOpponent =
                    CalcDmgFromMonsterAToMonsterB(
                        gPlayerParty[gCurrentPartyMember].Level,
                        gPlayerParty[gCurrentPartyMember].Attack,
                        gOpponentParty[gCurrentOpponentPartyMember].Defense,
                        gPlayerParty[gCurrentPartyMember].Psi,
                        gOpponentParty[gCurrentOpponentPartyMember].Resolve,
                        gBattleMoves[gSelectedPlayerMove].power1,
                        gBattleMoves[gSelectedPlayerMove].power2,
                        gBattleMoves[gSelectedPlayerMove].power3,
                        gBattleMoves[gSelectedPlayerMove].split,
                        TRUE
                    );

                if (gBattleMoves[gSelectedPlayerMove].split != SPLIT_STATUS)
                {

                    DamageToOpponent = GetElementaBonusDamage(DamageToOpponent, FALSE);
                }
                else
                {
                    if (gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_SELF)
                    {
                        switch (gBattleMoves[gSelectedPlayerMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentPlayerMon[0]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[0]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentPlayerMon[1]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[1]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                        }
                    }
                    else if (gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_OPPONENT)
                    {
                        switch (gBattleMoves[gSelectedPlayerMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentOpponentMon[0]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[0]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentOpponentMon[1]++;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[1]--;
                                gStatsChangedFromPlayerMove = TRUE;
                                break;
                            }
                        }
                    }
                }

                ModifyMonsterHealthValueGetKO(DamageToOpponent, FALSE, IsPlayerMovingFirst);
            }
            else
            {
                DamageToPlayer =
                    CalcDmgFromMonsterAToMonsterB(
                        gOpponentParty[gCurrentOpponentPartyMember].Level,
                        gOpponentParty[gCurrentOpponentPartyMember].Attack,
                        gPlayerParty[gCurrentPartyMember].Defense,
                        gOpponentParty[gCurrentOpponentPartyMember].Psi,
                        gPlayerParty[gCurrentPartyMember].Resolve,
                        gBattleMoves[gSelectedOpponentMove].power1,
                        gBattleMoves[gSelectedOpponentMove].power2,
                        gBattleMoves[gSelectedOpponentMove].power3,
                        gBattleMoves[gSelectedOpponentMove].split,
                        FALSE
                    );

                if (gBattleMoves[gSelectedOpponentMove].split != SPLIT_STATUS)
                {
                    DamageToPlayer = GetElementaBonusDamage(DamageToPlayer, TRUE);
                }
                else
                {
                    if (gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_OPPONENT)
                    {
                        switch (gBattleMoves[gSelectedOpponentMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentPlayerMon[0]++;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[0]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentPlayerMon[1]++;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentPlayerMon[1]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                        }
                    }
                    else if (gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_SELF)
                    {
                        switch (gBattleMoves[gSelectedOpponentMove].effect)
                        {
                            case EFFECT_ATTACK_UP:
                            {
                                gStatChangesCurrentOpponentMon[0]++;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_ATTACK_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[0]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_UP:
                            {
                                gStatChangesCurrentOpponentMon[1]++;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                            case EFFECT_DEFENSE_DOWN:
                            {
                                gStatChangesCurrentOpponentMon[1]--;
                                gStatsChangedFromOpponentMove = TRUE;
                                break;
                            }
                        }
                    }
                }

                ModifyMonsterHealthValueGetKO(DamageToPlayer, TRUE, IsPlayerMovingFirst);
            }

            break;
        }
        case BATTLESTATE_SECONDMOVE_POSTTEXT:
        {


            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            BattleTextLineCount = 0;
            if (IsPlayerMovingFirst == FALSE)
            {
                //player moving second bc above FALSE
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {

                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move dealt bonus", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                    sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "elemental damage!");
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move was resisted...", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_IMMUNE)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move had no effect!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }

                if (gWasLastMoveCriticalHit == TRUE)
                {
                    sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "It dealt extra devastating damage!");
                    BattleTextLineCount++;
                }

                if (gStatsChangedFromPlayerMove == TRUE && gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_SELF)
                {
                    switch (gBattleMoves[gSelectedPlayerMove].effect)
                    {
                        case EFFECT_ATTACK_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went up!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went up!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
                else if (gStatsChangedFromPlayerMove == TRUE && gBattleMoves[gSelectedPlayerMove].target == MOVE_TARGET_OPPONENT)
                {
                    switch (gBattleMoves[gSelectedPlayerMove].effect)
                    {

                        case EFFECT_ATTACK_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went down!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went down!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
            }
            else
            {

                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {

                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move dealt bonus", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                    sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "elemental damage!");
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move was resisted...", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }
                else if (gLastMoveElementalBonus == ELEMENT_IMMUNE)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s's move had no effect!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                }

                if (gWasLastMoveCriticalHit == TRUE)
                {
                    sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "It dealt extra devastating damage!");
                    BattleTextLineCount++;
                }

                if (gStatsChangedFromOpponentMove == TRUE && gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_SELF)
                {
                    switch (gBattleMoves[gSelectedOpponentMove].effect)
                    {
                        case EFFECT_ATTACK_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went up!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_UP:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went up!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
                else if (gStatsChangedFromOpponentMove == TRUE && gBattleMoves[gSelectedOpponentMove].target == MOVE_TARGET_OPPONENT)
                {
                    switch (gBattleMoves[gSelectedOpponentMove].effect)
                    {

                        case EFFECT_ATTACK_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's ATTACK went down!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                        case EFFECT_DEFENSE_DOWN:
                        {
                            sprintf_s((char*)gBattleTextLine[BattleTextLineCount + 1], sizeof(gBattleTextLine[BattleTextLineCount + 1]), "%s's DEFENSE went down!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                            BattleTextLineCount++;
                            break;
                        }
                    }
                }
            }

            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_SECONDMOVE_POSTWAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitPostSecondMove;
            }

            break;
        }
        case BATTLESTATE_SECONDMOVE_POSTWAIT:
        {

        WaitPostSecondMove:

            if (!IsPlayerMovingFirst && gStatsChangedFromPlayerMove)
            {
                gStatsChangedFromPlayerMove = FALSE;
            }
            else if (IsPlayerMovingFirst && gStatsChangedFromOpponentMove)
            {
                gStatsChangedFromOpponentMove = FALSE;
            }

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            break;
        }
        case BATTLESTATE_KO:
        {
            BOOL IsPlayerOutOfMonsters = FALSE;
            BOOL IsPlayerMonsterKOed = FALSE;

        PlayerNoHP:             //if starting combat with no healthy monsters

            if (gPlayerParty[gCurrentPartyMember].Health == 0)
            {
                IsPlayerMonsterKOed = TRUE;
                IsPlayerOutOfMonsters = TRUE;
                for (uint8_t partyMembers = 0; partyMembers < MAX_PARTY_SIZE; partyMembers++)
                {
                    if (gPlayerParty[partyMembers].Health != 0)
                    {
                        IsPlayerOutOfMonsters = FALSE;
                        break;
                    }
                }
            }
            else if (gOpponentParty[gCurrentOpponentPartyMember].Health == 0)
            {
                IsPlayerMonsterKOed = FALSE;
            }
            else
            {
                ASSERT(FALSE, "BATTLESTATE_KO reached while both in-battle monsters have non-zero HP!");
            }

            for (uint8_t i = 0; i < MAX_DIALOGUE_ROWS; i++)
            {
                for (uint8_t j = 0; j < MAX_DIALOGUE_ROWS; j++)
                {
                    gBattleTextLine[i][j] = 0;
                }
            }

            if (IsPlayerMonsterKOed == FALSE)
            {
                CalculatedExpReward = (gBaseStats[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Index].expYield * gOpponentParty[gCurrentOpponentPartyMember].Level) / 4;
            }

            BattleTextLineCount = 0;
            if (IsPlayerMonsterKOed == TRUE)
            {
                if (IsPlayerOutOfMonsters)
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s was KO'ed!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                    sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s blacked out!", &gPlayer.Name);
                    BattleTextLineCount++;
                }
                else
                {
                    sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s was KO'ed!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                    BattleTextLineCount++;
                    sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "Choose another monster!");
                    BattleTextLineCount++;
                }
                
            }
            else
            {
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s was KO'ed!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                BattleTextLineCount++;
                sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s gained %d Exp!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname, CalculatedExpReward);
                BattleTextLineCount++;
            }


            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_KO_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitKO;
            }

            break;
        }
        case BATTLESTATE_KO_WAIT:
        {

        WaitKO:
                
            if (CalculatedExpReward != 0)
            {
                BOOL DidMonsterLevelUp = 0;

                gPlayerParty[gCurrentPartyMember].DriveMonster.Experience = gPlayerParty[gCurrentPartyMember].DriveMonster.Experience + CalculatedExpReward;
                CalculatedExpReward = 0;

                DidMonsterLevelUp = TryIncrementMonsterLevel(&gPlayerParty[gCurrentPartyMember]);

                if (DidMonsterLevelUp == TRUE)
                {
                    CalculateMonsterStats(&gPlayerParty[gCurrentPartyMember]);

                    MonsterTryLearningNewMove(&gPlayerParty[gCurrentPartyMember], TRUE);
                }
            }

            BlitBattleStateTextBox_Wait(BattleTextLineCount);

            gWasMonsterKOed = FALSE;

            break;
        }
        case BATTLESTATE_RUN_FIGHT:
        {
            DrawBattleButtons();
            break;
        }
        case BATTLESTATE_SWITCH_FIGHT:
        {
            break;
        }
        case BATTLESTATE_CHOOSE_MOVE:
        {
            DrawMoveButtons();
            break;
        }
        case BATTLESTATE_CHOOSE_MONSTER:
        {
            DrawMonsterPartyButtons();
            break;
        }
        case BATTLESTATE_CHOOSE_ITEM:
        {
            DrawUseableItemsButtons();
            break;
        }
    }



    switch (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][gPlayer.WorldPos.x / 16])
    {
        case TILE_GRASS_01:
        {
            gBattleScene = &gBattleScreen_Grass01;
            break;
        }
        case TILE_TALLGRASS_01:
        {
            gBattleScene = &gBattleScreen_Grass01;
            break;
        }
        case TILE_STONEBRICKS_FLOOR_01:
        {
            gBattleScene = &gBattleScreen_StoneBricks01;
            break;
        }
        default:
        {
            ASSERT(FALSE, "Player encountered a monster on an unknown tile!")
        }
    }

    if (gBattleScene != 0)
    {
        Blit32BppBitmapToBuffer(gBattleScene, (65), (49), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "BattleScene is NULL!");
    }

    if (gPlayerMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(gPlayerMonsterSprite, (65), (111), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Player battle monster is NULL!");
    }

    if (gOpponentMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(gOpponentMonsterSprite, (255), (79), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Opponent battle monster is NULL!");
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    uint16_t HpPercent = 100 - ((gPlayerParty[gCurrentPartyMember].Health * 100) / (gPlayerParty[gCurrentPartyMember].MaxHealth));

    uint16_t ExpPercent = 100 - (((gPlayerParty[gCurrentPartyMember].DriveMonster.Experience - gExperienceTables[gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].growthRate][gPlayerParty[gCurrentPartyMember].Level]) * 100) / (gExperienceTables[gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].growthRate][gPlayerParty[gCurrentPartyMember].Level + 1] - gExperienceTables[gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].growthRate][gPlayerParty[gCurrentPartyMember].Level]));

    DrawMonsterHpBar(65 + 1, 111 - 8, HpPercent, ExpPercent, gPlayerParty[gCurrentPartyMember].Level, gPlayerParty[gCurrentPartyMember].DriveMonster.nickname, TRUE);

    HpPercent = 100 - ((gOpponentParty[gCurrentOpponentPartyMember].Health * 100) / (gOpponentParty[gCurrentOpponentPartyMember].MaxHealth));

    if (HpPercent > 75)
    {
        gCaptureHpHelp = 0;
    }
    else if (HpPercent > 50 && HpPercent <= 75)
    {
        gCaptureHpHelp = 10;
    }
    else if (HpPercent > 25 && HpPercent <= 50)
    {
        gCaptureHpHelp = 25;
    }
    else if (HpPercent > 0 && HpPercent <= 25)
    {
        gCaptureHpHelp = 45;
    }
    else
    {
        gCaptureHpHelp = 0;
    }

    DrawMonsterHpBar(255 - 46, 79 - 8, HpPercent, 0, gOpponentParty[gCurrentOpponentPartyMember].Level, gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname, FALSE);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}


void PPI_BattleScreen(void)
{
    switch (gCurrentBattleState)
    {
        case BATTLESTATE_OPENING_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_OPENING_WAIT;
            }
            break;
        }
        case BATTLESTATE_OPENING_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_TURNORDER_CALC:
        {
            break;
        }
        case BATTLESTATE_ESCAPESUCCESS_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_ESCAPESUCCESS_WAIT;
            }
            break;
        }
        case BATTLESTATE_ESCAPESUCCESS_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gPreviousGameState = gCurrentGameState;
                gCurrentGameState = GAMESTATE_OVERWORLD;
                gEscapeTriesThisBattle = 0;
                gInputEnabled = FALSE;
                StopGameMusic();
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_ESCAPEFAIL_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_ESCAPEFAIL_WAIT;
            }
            break;
        }
        case BATTLESTATE_ESCAPEFAIL_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_SECONDMOVE_TEXT;
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_NOESCAPE_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_NOESCAPE_WAIT;
            }
            break;
        }
        case BATTLESTATE_NOESCAPE_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_FIRSTMOVE_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_FIRSTMOVE_WAIT;
            }
            break;
        }
        case BATTLESTATE_FIRSTMOVE_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_FIRSTMOVE_CALC;
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_FIRSTMOVE_CALC:
        {
            break;
        }
        case BATTLESTATE_FIRSTMOVE_POSTTEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_FIRSTMOVE_POSTWAIT;
            }
            break;
        }
        case BATTLESTATE_FIRSTMOVE_POSTWAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gFinishedBattleTextAnimation = FALSE;
                if (gWasMonsterKOed == TRUE)
                {
                    gCurrentBattleState = BATTLESTATE_KO;
                }
                else
                {
                    gCurrentBattleState = BATTLESTATE_SECONDMOVE_TEXT;
                }
            }
            break;
        }
        case BATTLESTATE_SWITCHING_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_SWITCHING_WAIT;
            }
            break;
        }
        case BATTLESTATE_SWITCHING_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                if (gPreviousBattleState == BATTLESTATE_KO_WAIT)
                {
                    gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
                    gFinishedBattleTextAnimation = FALSE;
                }
                else
                {
                    gCurrentBattleState = BATTLESTATE_SECONDMOVE_TEXT;
                    gFinishedBattleTextAnimation = FALSE;
                }
            }
            break;
        }
        case BATTLESTATE_USEITEM_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_USEITEM_WAIT;
            }
            break;
        }
        case BATTLESTATE_USEITEM_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_SECONDMOVE_TEXT;
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_CATCH_TEXT:
        {
            break;
        }
        case BATTLESTATE_CATCH_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                if (gCaptureNumDotsWait > 0)
                {
                    gCurrentBattleState = BATTLESTATE_SECONDMOVE_TEXT;
                }
                else
                {
                    gCurrentBattleState = BATTLESTATE_CATCHSUCCESS_TEXT;
                }
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_CATCHSUCCESS_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_CATCHSUCCESS_WAIT;
            }
            break;
        }
        case BATTLESTATE_CATCHSUCCESS_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gFinishedBattleTextAnimation = FALSE;
                MenuItem_BattleScreen_EscapeButton();
            }
            break;
        }
        case BATTLESTATE_SECONDMOVE_TEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_SECONDMOVE_WAIT;
            }
            break;
        }
        case BATTLESTATE_SECONDMOVE_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_SECONDMOVE_CALC;
                gFinishedBattleTextAnimation = FALSE;
            }
            break;
        }
        case BATTLESTATE_SECONDMOVE_CALC:
        {
            break;
        }

        case BATTLESTATE_SECONDMOVE_POSTTEXT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_SECONDMOVE_POSTWAIT;
            }
            break;
        }
        case BATTLESTATE_SECONDMOVE_POSTWAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gFinishedBattleTextAnimation = FALSE;
                if (gWasMonsterKOed == TRUE)
                {
                    gCurrentBattleState = BATTLESTATE_KO;
                }
                else
                {
                    gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
                }
            }
            break;
        }
        case BATTLESTATE_KO:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gCurrentBattleState = BATTLESTATE_KO_WAIT;
            }
            break;
        }
        case BATTLESTATE_KO_WAIT:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                BOOL ContinueBattle = FALSE;
                gFinishedBattleTextAnimation = FALSE;

                if (gPlayerParty[gCurrentPartyMember].Health == 0)
                {
                    for (uint8_t party = 0; party < MAX_PARTY_SIZE; party++)
                    {
                        if (gPlayerParty[party].Health == 0)
                        {
                            ContinueBattle = FALSE;
                        }
                        else
                        {
                            ContinueBattle = TRUE;
                            break;
                        }
                    }

                    if (ContinueBattle == FALSE)    //check if player has more monsters in party
                    {
                        MenuItem_BattleScreen_EscapeButton();
                    }
                    else
                    {
                        gPreviousBattleState = BATTLESTATE_KO_WAIT;
                        gCurrentBattleState = BATTLESTATE_CHOOSE_MONSTER;
                        gMenu_SwitchScreen.SelectedItem = 0;
                        goto PlayerChooseNewMonster;
                    }
                }

                if (gOpponentParty[gCurrentOpponentPartyMember].Health == 0)
                {
                    for (uint8_t enemy = 0; enemy < MAX_PARTY_SIZE; enemy++)
                    {
                        if (gOpponentParty[enemy].Health == 0)
                        {
                            ContinueBattle = FALSE;
                        }
                        else
                        {
                            ContinueBattle = TRUE;
                            break;
                        }
                    }

                    if (ContinueBattle == FALSE)        //check for more opponent monsters in party
                    {
                        MenuItem_BattleScreen_EscapeButton();
                    }
                    else                //TODO: make BATTLESTATE_ENEMYSWITCH_TEXT and BATTLESTATE_ENEMYSWITCH_WAIT for enemy switching monsters
                    {

                    FindNewOpponentMonster:

                        gCurrentOpponentPartyMember++;
                        if (gOpponentParty[gCurrentOpponentPartyMember].Health == 0)
                        {
                            goto FindNewOpponentMonster;
                        }
                        else
                        {
                            gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
                        }
                    }
                }
                else
                {
                    if (gPlayerParty[gCurrentPartyMember].Health != 0)
                    {
                        ASSERT(FALSE, "PPI case BATTLESTATE_KO_WAIT reached while both battlemonsters are at non-zero HP!");
                    }
                }
            }
            break;
        }
        case BATTLESTATE_RUN_FIGHT:
        {
            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                gMenu_BattleScreen.SelectedItem = 0;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
            {
                gMenu_BattleScreen.SelectedItem = 2;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
            {
                gMenu_BattleScreen.SelectedItem = 3;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                gMenu_BattleScreen.SelectedItem = 1;
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gMenu_BattleScreen.Items[gMenu_BattleScreen.SelectedItem]->Action();
                PlayGameSound(&gSoundMenuChoose);
            }
            else if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                if (gMenu_BattleScreen.SelectedItem == 1)
                {
                    gMenu_BattleScreen.Items[gMenu_BattleScreen.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
                else
                {
                gMenu_BattleScreen.SelectedItem = 1;
                PlayGameSound(&gSoundMenuNavigate);
                }
            }
            break;
        }
        case BATTLESTATE_SWITCH_FIGHT:
        {
            break;
        }
        case BATTLESTATE_CHOOSE_MOVE:
        {
            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                if (gMenu_MoveScreen.SelectedItem > 1 && gMenu_MoveScreen.SelectedItem < 5)
                {
                    gMenu_MoveScreen.SelectedItem--;
                }
                else
                {
                    gMenu_MoveScreen.SelectedItem = 1;
                }
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
            {
                if (gMenu_MoveScreen.SelectedItem == 5)
                {
                    gMenu_MoveScreen.SelectedItem = 4;
                }
                else if (gMenu_MoveScreen.SelectedItem > 0 && gMenu_MoveScreen.SelectedItem < 5)
                {
                    gMenu_MoveScreen.SelectedItem = 0;
                }
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
            {
                if (gMenu_MoveScreen.SelectedItem == 0)
                {
                    gMenu_MoveScreen.SelectedItem = 1;
                }
                else if (gMenu_MoveScreen.SelectedItem > 0 && gMenu_MoveScreen.SelectedItem < 5)
                {
                    gMenu_MoveScreen.SelectedItem = 5;
                }
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                if (gMenu_MoveScreen.SelectedItem > 0 && gMenu_MoveScreen.SelectedItem < 4)
                {
                    gMenu_MoveScreen.SelectedItem++;
                }
                else
                {
                    gMenu_MoveScreen.SelectedItem = 4;
                }
                PlayGameSound(&gSoundMenuNavigate);
            }
            else if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gMenu_MoveScreen.Items[gMenu_MoveScreen.SelectedItem]->Action();
                PlayGameSound(&gSoundMenuChoose);
            }
            else if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                if (gMenu_MoveScreen.SelectedItem == 0)
                {
                    gMenu_MoveScreen.Items[gMenu_MoveScreen.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
                else
                {
                    gMenu_MoveScreen.SelectedItem = 0;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }
            break;
        }
        case BATTLESTATE_CHOOSE_MONSTER:
        {

    PlayerChooseNewMonster:

            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed && gPreviousBattleState != BATTLESTATE_KO_WAIT)
            {
                if (gMenu_SwitchScreen.SelectedItem == 6)
                {
                    gMenu_SwitchScreen.Items[gMenu_SwitchScreen.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
                else
                {
                    gMenu_SwitchScreen.SelectedItem = 6;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                if (gMenu_SwitchScreen.SelectedItem > 0 && gMenu_SwitchScreen.SelectedItem != 6)
                {
                    gMenu_SwitchScreen.SelectedItem--;
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_SwitchScreen.SelectedItem == 0 && gPreviousBattleState != BATTLESTATE_KO_WAIT)
                {
                    gMenu_SwitchScreen.SelectedItem = 6;
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_SwitchScreen.SelectedItem == 6)
                {
                    gMenu_SwitchScreen.SelectedItem = gPlayerPartyCount - 1;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }
            else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                if (gMenu_SwitchScreen.SelectedItem < gPlayerPartyCount - 1)
                {
                    gMenu_SwitchScreen.SelectedItem++;
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_SwitchScreen.SelectedItem == 6)
                {
                    gMenu_SwitchScreen.SelectedItem = 0;
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_SwitchScreen.SelectedItem == gPlayerPartyCount - 1 && gPreviousBattleState != BATTLESTATE_KO_WAIT)
                {
                    gMenu_SwitchScreen.SelectedItem = 6;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                if (gPreviousBattleState == BATTLESTATE_KO && gMenu_SwitchScreen.SelectedItem == 6)
                {

                }
                else
                {
                    gMenu_SwitchScreen.Items[gMenu_SwitchScreen.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
            }
            break;
        }
        case BATTLESTATE_CHOOSE_ITEM:
        {
            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
            {
                if (gMenu_UseableScreen.SelectedItem == 5)
                {
                    gMenu_UseableScreen.Items[gMenu_UseableScreen.SelectedItem]->Action();
                    PlayGameSound(&gSoundMenuChoose);
                }
                else
                {
                    gMenu_UseableScreen.SelectedItem = 5;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                if (gMenu_UseableScreen.SelectedItem > 0)
                {
                    gMenu_UseableScreen.SelectedItem--;
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_UseableScreen.SelectedItem == 0 && gUseableItemCount > _countof(gMI_UseableScreen_Items) - 1)
                {
                    gUseableSlotOffset--;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                if (gMenu_UseableScreen.SelectedItem < _countof(gMI_UseableScreen_Items) - 2)
                {
                    gMenu_UseableScreen.SelectedItem++;
                    PlayGameSound(&gSoundMenuNavigate);
                }
                else if (gMenu_UseableScreen.SelectedItem == 4 && gUseableItemCount > _countof(gMI_UseableScreen_Items) - 1)
                {
                    gUseableSlotOffset++;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.DRightKeyPressed && gGameInput.DRightKeyAlreadyPressed)
            {
                if (gMenu_UseableScreen.SelectedItem == 5)
                {
                    gMenu_UseableScreen.SelectedItem = 0;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.ALeftKeyPressed && gGameInput.ALeftKeyAlreadyPressed)
            {
                if (gMenu_UseableScreen.SelectedItem < 5)
                {
                    gMenu_UseableScreen.SelectedItem = 5;
                    PlayGameSound(&gSoundMenuNavigate);
                }
            }

            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gMenu_UseableScreen.Items[gMenu_UseableScreen.SelectedItem]->Action();
                PlayGameSound(&gSoundMenuChoose);
            }

            break;
        }
    }
}

void DrawBattleButtons(void)
{
    DrawWindow(1, 190, 64, 28, &COLOR_BLACK, &COLOR_DARK_RED, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    DrawWindow(1, 222, 64, 16, &COLOR_BLACK, &COLOR_LIGHT_BLUE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    DrawWindow(64, 190, 64, 48, &COLOR_BLACK, &COLOR_FORREST_GREEN, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    DrawWindow(256, 190, 64, 48, &COLOR_BLACK, &COLOR_NES_TAN, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    for (uint8_t Counter = 0; Counter < _countof(gMI_BattleScreen_Items); Counter++)
    {
        if (gMI_BattleScreen_Items[Counter]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMI_BattleScreen_Items[Counter]->Name, &g6x7Font, &COLOR_BLACK, gMI_BattleScreen_Items[Counter]->x, gMI_BattleScreen_Items[Counter]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_BattleScreen_Items[gMenu_BattleScreen.SelectedItem]->x - 6, gMI_BattleScreen_Items[gMenu_BattleScreen.SelectedItem]->y);
}

void DrawMoveButtons(void)
{

    static PIXEL32 MoveColor = { 0x00, 0x00, 0x00, 0x00 };

    for (uint8_t Moves = 0; Moves < MAX_NONSIGNATURE_MOVES; Moves++)
    {
        switch (gBattleMoves[gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[Moves]].element)
        {
            case ELEMENT_NONE:
            {
                MoveColor = COLOR_NES_TAN;
                break;
            }
            case ELEMENT_EARTH:
            {
                MoveColor = COLOR_NES_BROWN;
                break;
            }
            case ELEMENT_AIR:
            {
                MoveColor = COLOR_NES_SKY_BLUE;
                break;
            }
            case ELEMENT_FIRE:
            {
                MoveColor = COLOR_NES_ORANGE;
                break;
            }
            case ELEMENT_WATER:
            {
                MoveColor = COLOR_NES_BLUE;
                break;
            }
            case ELEMENT_ELECTRIC:
            {
                MoveColor = COLOR_NES_YELLOW;
                break;
            }
            case ELEMENT_METAL:
            {
                MoveColor = COLOR_NES_GRAY;
                break;
            }
            case ELEMENT_SOUL:
            {
                MoveColor = COLOR_NES_PURPLE;
                break;
            }
            case ELEMENT_LIFE:
            {
                MoveColor = COLOR_NES_LIGHT_GREEN;
                break;
            }
            case ELEMENT_DEATH:
            {
                MoveColor = COLOR_NES_BLACK_RED;
                break;
            }
            case ELEMENT_STATUS:
            {
                MoveColor = COLOR_NES_PINK;
                break;
            }
            default:
            {
                MoveColor = COLOR_LIGHT_GRAY;
                break;
            }
        }

        DrawWindow(64, 185 + (Moves * 14), 240, 11, &COLOR_BLACK, &MoveColor, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);

    }

    ////slot 1
    //DrawWindow(64, 185, 240, 11, &COLOR_BLACK, &MoveColor, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);

    ////slot 2
    //DrawWindow(64, 187 + 12, 240, 11, &COLOR_BLACK, &MoveColor, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);
    //                   
    ////slot 3
    //DrawWindow(64, 189 + 24, 240, 11, &COLOR_BLACK, &MoveColor, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);
    //             
    ////slot 4
    //DrawWindow(64, 191 + 36, 240, 11, &COLOR_BLACK, &MoveColor, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);
                            
    //back button
    DrawWindow(1, 186, 48, 51, &COLOR_BLACK, &COLOR_DARK_RED, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);
                            
    //signature move button
    DrawWindow(306, 186, 48 + 12 + 16, 51, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    for (uint8_t Counter = 0; Counter < _countof(gMI_MoveScreen_Items); Counter++)
    {
        if (gMI_MoveScreen_Items[Counter]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMI_MoveScreen_Items[Counter]->Name, &g6x7Font, &COLOR_BLACK, gMI_MoveScreen_Items[Counter]->x, gMI_MoveScreen_Items[Counter]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_MoveScreen_Items[gMenu_MoveScreen.SelectedItem]->x - 6, gMI_MoveScreen_Items[gMenu_MoveScreen.SelectedItem]->y);
}

void DrawMonsterPartyButtons(void)
{
    DrawWindow(gMI_SwitchScreen_Items[6]->x - 3, gMI_SwitchScreen_Items[6]->y - 5, 32, 16, &COLOR_BLACK, &COLOR_DARK_RED, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    for (uint8_t playerParty = 0; playerParty < gPlayerPartyCount; playerParty++)
    {
        uint16_t HpPercent = 100 - ((gPlayerParty[playerParty].Health * 100) / (gPlayerParty[playerParty].MaxHealth));

        uint16_t ExpPercent = 100 - (((gPlayerParty[playerParty].DriveMonster.Experience - gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level]) * 100) / (gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level + 1] - gExperienceTables[gBaseStats[gPlayerParty[playerParty].DriveMonster.Index].growthRate][gPlayerParty[playerParty].Level]));

        DrawMonsterHpBar(gMI_SwitchScreen_Items[playerParty]->x, gMI_SwitchScreen_Items[playerParty]->y + 5, HpPercent, ExpPercent, gPlayerParty[playerParty].Level, gPlayerParty[playerParty].DriveMonster.nickname, TRUE);
    }

    BlitStringToBuffer(gMI_SwitchScreen_BackButton.Name, &g6x7Font, &COLOR_BLACK, gMI_SwitchScreen_BackButton.x, gMI_SwitchScreen_BackButton.y);

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_SwitchScreen_Items[gMenu_SwitchScreen.SelectedItem]->x - 9, gMI_SwitchScreen_Items[gMenu_SwitchScreen.SelectedItem]->y);
}

void DrawUseableItemsButtons(void)
{
    //slot 1
    DrawWindow(64, 178, 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(64, 178 + (12 * 1), 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(64, 178 + (12 * 2), 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(64, 178 + (12 * 3), 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(64, 178 + (12 * 4), 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    DrawWindow(32, 178, 26, 11, &COLOR_BLACK, &COLOR_DARK_RED, NULL, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    for (uint8_t UseableBox = 0; UseableBox < _countof(gMI_UseableScreen_Items) - 1; UseableBox++)
    {
        uint16_t SlotLoop = 0x800C;

        int32_t SlotOrder = 0;

        uint16_t SlotIndex = 0;

    SlotIndexing:



        SlotOrder = UseableBox + gUseableSlotOffset + SlotLoop;

        if (gUseableItemCount != 0)
        {
            SlotOrder %= gUseableItemCount;
        }

        SlotIndex = gUseableHasItemSort[SlotOrder];

        if (gUseableHasItemSort[UseableBox] == 0xFFFF)
        {
            SlotIndex = 0xFFFF;
            goto SlotIndexed;
        }
        if (SlotIndex == 0xFFFF)
        {
            SlotLoop++;
            goto SlotIndexing;
        }

    SlotIndexed:

        gUseableSlotIndex[UseableBox] = SlotIndex;  //used for PPI_InventoryScreen to know what item is in what menubox and prevent using blank items

        if (gMI_UseableScreen_Items[UseableBox]->Enabled == TRUE && SlotIndex != 0xFFFF)
        {
            uint16_t ItemCountSize = snprintf(NULL, 0, "x%d", gUseableItems[SlotIndex].Count);
            char* ItemCountString = malloc(ItemCountSize + 1);
            snprintf(ItemCountString, ItemCountSize + 1, "x%d", gUseableItems[SlotIndex].Count);

            BlitStringToBuffer(gUseableItems[SlotIndex].Name, &g6x7Font, &COLOR_BLACK, gMI_UseableScreen_Items[UseableBox]->x, gMI_UseableScreen_Items[UseableBox]->y);

            BlitStringToBuffer(ItemCountString, &g6x7Font, &COLOR_BLACK, gMI_UseableScreen_Items[UseableBox]->x + 149, gMI_UseableScreen_Items[UseableBox]->y);
        }
    }

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_UseableScreen_Items[gMenu_UseableScreen.SelectedItem]->x - 7, gMI_UseableScreen_Items[gMenu_UseableScreen.SelectedItem]->y);

    BlitStringToBuffer("Back", &g6x7Font, &COLOR_BLACK, gMI_UseableScreen_Items[5]->x, gMI_UseableScreen_Items[5]->y);

}

//////// initial choice menu

void MenuItem_BattleScreen_FightButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_CHOOSE_MOVE;
}

void MenuItem_BattleScreen_EscapeButton(void)
{
    if ((gCurrentGameState == GAMESTATE_BATTLE_MONSTER) && (gCurrentBattleState == BATTLESTATE_RUN_FIGHT))
    {
        uint16_t EscapeChance = 0;

        EscapeChance = ((gPlayerParty[gCurrentPartyMember].Speed * 1.0f / gOpponentParty[gCurrentOpponentPartyMember].Speed * 1.0f) * 400) + (50 * gEscapeTriesThisBattle);        //if player.speed = opp.speed then chance is about 40%

        EscapeChance = min(EscapeChance, 990);      //max of 99% chance to escape

        gEscapeTriesThisBattle++;    //Increases every attempt by aprox 5%

        DWORD Random = 0;
        rand_s((unsigned int*)&Random);
        Random %= 1000;

        if (Random > (1000 - EscapeChance))
        {
            gCurrentBattleState = BATTLESTATE_ESCAPESUCCESS_TEXT;
            gEscapeTriesThisBattle = 0;
        }
        else
        {
            gCurrentBattleState = BATTLESTATE_ESCAPEFAIL_TEXT;
        }
    }
    else if ((gCurrentGameState == GAMESTATE_BATTLE_MONSTER) && (gCurrentBattleState != BATTLESTATE_RUN_FIGHT))
    {
        for (uint8_t stats = 0; stats < 4; stats++)
        {
            gStatChangesCurrentPlayerMon[stats] = 7;
            gStatChangesCurrentOpponentMon[stats] = 7;
        }
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_OVERWORLD;
        gEscapeTriesThisBattle = 0;
        gInputEnabled = FALSE;
        StopGameMusic();
    }
    else if ((gCurrentGameState == GAMESTATE_BATTLE_TRAINER) && (gCurrentBattleState == BATTLESTATE_KO_WAIT))
    {
        BOOL PlayerLostBattle = 0;
        for (uint8_t party = 0; party < MAX_PARTY_SIZE - 1; party++)
        {
            if (gPlayerParty[party].Health != 0)
            {
                PlayerLostBattle = FALSE;
                break;
            }
            else
            {
                PlayerLostBattle = TRUE;
            }
        }

        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
        {
            if ((gCharacterSprite[Index].InteractedWith == TRUE) && (gCharacterSprite[Index].Event == EVENT_FLAG_BATLLE))
            {
                gCharacterSprite[Index].InteractedWith = FALSE;

                if (PlayerLostBattle == FALSE)      //only reset EVENT_FLAG_BATTLE and SightRange when player has won the fight, otherwise allow for re-battle
                {
                    gCharacterSprite[Index].Event = EVENT_FLAG_NONE;
                    gCharacterSprite[Index].SightRange = 0;
                    gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                }
                break;
            }
        }
        for (uint8_t stats = 0; stats < 4; stats++)
        {
            gStatChangesCurrentPlayerMon[stats] = 7;
            gStatChangesCurrentOpponentMon[stats] = 7;
        }
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_OVERWORLD;
        gInputEnabled = FALSE;
        StopGameMusic();
    }
    else if(gCurrentGameState == GAMESTATE_BATTLE_TRAINER && gCurrentBattleState == BATTLESTATE_RUN_FIGHT)
    {
        gCurrentBattleState = BATTLESTATE_NOESCAPE_TEXT;
    }
    
}

void MenuItem_BattleScreen_SwitchButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_CHOOSE_MONSTER;
}

void MenuItem_BattleScreen_ItemsButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_CHOOSE_ITEM;
}

/////// switching monsters screen
void MenuItem_SwitchScreen_PartySelected(void)
{
    if (gPreviousBattleState == BATTLESTATE_RUN_FIGHT || gPreviousBattleState == BATTLESTATE_KO_WAIT)
    {
        if (gCurrentPartyMember == gMenu_SwitchScreen.SelectedItem || gPlayerParty[gMenu_SwitchScreen.SelectedItem].Health == gMenu_SwitchScreen.SelectedItem)
        {
            //trying to swap to monster already in battle
        }
        else
        {
            gPartyMemberToSwitchIn = gMenu_SwitchScreen.SelectedItem;
            gCurrentBattleState = BATTLESTATE_SWITCHING_TEXT;
        }
    }
    else if (gPreviousBattleState == BATTLESTATE_CHOOSE_ITEM && gPlayerParty[gMenu_SwitchScreen.SelectedItem].DriveMonster.hasIndex)
    {
        gUsableItemSelectedPartyMember = gMenu_SwitchScreen.SelectedItem;

        switch (gUseableItemEffect_SelectedItem)
        {
            case ITEM_USE_EFFECT_HEAL_MONSTER:
            {
                if (gPlayerParty[gUsableItemSelectedPartyMember].Health == gPlayerParty[gUsableItemSelectedPartyMember].MaxHealth)      //selected monster has full health
                {
                    gCurrentBattleState = BATTLESTATE_CHOOSE_ITEM;
                    gPreviousBattleState = BATTLESTATE_CHOOSE_MONSTER;
                }
                else
                {
                    uint16_t healthBeforeHeal = gPlayerParty[gUsableItemSelectedPartyMember].Health;
                    uint16_t healAmount = 0;
                    if (gUseableSlotIndex[gMenu_UseableScreen.SelectedItem] == INV_USABLE_ITEM_0)
                    {
                        healAmount = 20;
                    }
                    else if (gUseableSlotIndex[gMenu_UseableScreen.SelectedItem] == INV_USABLE_ITEM_1)
                    {
                        healAmount = 50;
                    }
                    else if (gUseableSlotIndex[gMenu_UseableScreen.SelectedItem] == INV_USABLE_ITEM_2)
                    {
                        healAmount = 100;
                    }
                    else if (gUseableSlotIndex[gMenu_UseableScreen.SelectedItem] == INV_USABLE_ITEM_3)
                    {
                        healAmount = 250;
                    }
                    else if (gUseableSlotIndex[gMenu_UseableScreen.SelectedItem] == INV_USABLE_ITEM_4)
                    {
                        healAmount = gPlayerParty[gUsableItemSelectedPartyMember].MaxHealth;
                    }

                    for (uint16_t healedHealth = gPlayerParty[gUsableItemSelectedPartyMember].Health; healedHealth <= healthBeforeHeal + healAmount; healedHealth++)
                    {
                        gPlayerParty[gUsableItemSelectedPartyMember].Health = healedHealth;
                        if (gPlayerParty[gUsableItemSelectedPartyMember].Health == gPlayerParty[gUsableItemSelectedPartyMember].MaxHealth)
                        {
                            break;
                        }
                    }

                    gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Count--;

                    gCurrentBattleState = BATTLESTATE_USEITEM_TEXT;
                    gPreviousBattleState = BATTLESTATE_CHOOSE_MONSTER;

                    ReSortUsableitems();
                }
                break;
            }
            case ITEM_USE_EFFECT_CAPTURE:
            {
                break;
            }
            default:
            {
                ASSERT(FALSE, "Usable item in battle had an unknown gUsableItemEffect_SelectedItem!");
            }
        }
    }
}

void MenuItem_SwitchScreen_BackButton(void)
{
    if (gPreviousBattleState == BATTLESTATE_CHOOSE_ITEM || gPreviousBattleState == BATTLESTATE_RUN_FIGHT)
    {
        gCurrentBattleState = gPreviousBattleState;
        gPreviousBattleState = BATTLESTATE_CHOOSE_MONSTER;
    }
}

///////////// selecting moves screen

void MenuItem_MoveScreen_MoveSlot0(void)
{
    if (gMI_MoveScreen_MoveSlot0.Name == gBattleMoveNames[BATTLEMOVE_NULL])
    {

    }
    else          //end player turn start calculating
    {
        gSelectedPlayerMove = gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[0];
        gSelectedPlayerMoveSlot = 0;
        gCurrentBattleState = BATTLESTATE_TURNORDER_CALC;
    }
}

void MenuItem_MoveScreen_MoveSlot1(void)
{
    if (gMI_MoveScreen_MoveSlot1.Name == gBattleMoveNames[BATTLEMOVE_NULL])
    {

    }
    else          //end player turn start calculating
    {
        gSelectedPlayerMove = gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[1];
        gSelectedPlayerMoveSlot = 1;
        gCurrentBattleState = BATTLESTATE_TURNORDER_CALC;
    }
}

void MenuItem_MoveScreen_MoveSlot2(void)
{

    if (gMI_MoveScreen_MoveSlot2.Name == gBattleMoveNames[BATTLEMOVE_NULL])
    {

    }
    else          //end player turn start calculating
    {
        gSelectedPlayerMove = gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[2];
        gSelectedPlayerMoveSlot = 2;
        gCurrentBattleState = BATTLESTATE_TURNORDER_CALC;
    }
}

void MenuItem_MoveScreen_MoveSlot3(void)
{

    if (gMI_MoveScreen_MoveSlot3.Name == gBattleMoveNames[BATTLEMOVE_NULL])
    {

    }
    else          //end player turn start calculating
    {
        gSelectedPlayerMove = gPlayerParty[gCurrentPartyMember].DriveMonster.Moves[3];
        gSelectedPlayerMoveSlot = 3;
        gCurrentBattleState = BATTLESTATE_TURNORDER_CALC;
    }
}

void MenuItem_MoveScreen_SignatureMove(void)
{
    //end player turn start calculating
}

void MenuItem_MoveScreen_BackButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
}

void MenuItem_UseableScreen_SlotSelected(void)
{
    if (gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Effect == ITEM_USE_EFFECT_HEAL_MONSTER && gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Count > 0)
    {
        gUseableItemEffect_SelectedItem = ITEM_USE_EFFECT_HEAL_MONSTER;
        gPreviousBattleState = gCurrentBattleState;
        gCurrentBattleState = BATTLESTATE_CHOOSE_MONSTER;
    }
    else if (gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Effect == ITEM_USE_EFFECT_CAPTURE && gUseableItems[gUseableSlotIndex[gMenu_UseableScreen.SelectedItem]].Count > 0)
    {
        gPreviousBattleState = gCurrentBattleState;
        gCurrentBattleState = BATTLESTATE_CATCH_TEXT;
        
        if (gUseableSlotIndex[gMenu_UseableScreen.SelectedItem] == INV_USABLE_ITEM_6)
        {
            gCaptureDeviceHelp = 50;
        }
        else if (gUseableSlotIndex[gMenu_UseableScreen.SelectedItem] == INV_USABLE_ITEM_7)
        {
            gCaptureDeviceHelp = 75;
        }
        else
        {
            gCaptureDeviceHelp = 0;
        }
    }

}

void MenuItem_UseableScreen_BackButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
}

uint8_t CalculateOpponentMoveChoice(uint8_t npcaiFlag)
{
    DWORD Random;
    uint8_t moveChoice;

    
    if (npcaiFlag && FLAG_NPCAI_NEVERSTATUS == npcaiFlag)
    {

    ReRandomizeStatus:

        rand_s((unsigned int*)&Random);
        Random = Random % (MAX_NONSIGNATURE_MOVES);
        moveChoice = (uint8_t*)Random;

        if (BATTLEMOVE_NULL == gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[moveChoice] || gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[moveChoice]].split == SPLIT_STATUS)
        {
            goto ReRandomizeStatus;
        }
    }
    else if (npcaiFlag && FLAG_NPCAI_HIGHESTPOWER == npcaiFlag)
    {
        uint8_t power1 = 0;
        uint8_t power2 = 0;
        uint8_t power3 = 0;
        uint8_t powerTot = 0;
        uint8_t powerHighest = 0;

        for (uint8_t i = 0; i < MAX_NONSIGNATURE_MOVES; i++)
        {
            power1 = gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[i]].power1;
            power2 = gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[i]].power2;
            power3 = gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[i]].power3;

            powerTot = power1 + power2 + power3;
            if (powerTot >= powerHighest)
            {
                powerHighest = powerTot;
                moveChoice = i;
            }
        }
    }
    else
    {

    ReRandomize:

        rand_s((unsigned int*)&Random);
        Random = Random % (MAX_NONSIGNATURE_MOVES);
        moveChoice = (uint8_t*)Random;

        if (BATTLEMOVE_NULL == gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[moveChoice])
        {
            goto ReRandomize;
        }
    }

    return(moveChoice);
}

BOOL CalculateSpeedPriorityIfPlayerMovesFirst(uint16_t speedStatPlayer, uint16_t speedStatOpponent)
{
    if (gPlayerParty[gCurrentPartyMember].Speed > gOpponentParty[gCurrentOpponentPartyMember].Speed)
    {
        return(TRUE);
    }
    else if (gPlayerParty[gCurrentPartyMember].Speed < gOpponentParty[gCurrentOpponentPartyMember].Speed)
    {
        return(FALSE);
    }
    else        //speed tie
    {
        DWORD Random;
        BOOL coinFlip;

        rand_s((unsigned int*)&Random);
        Random = Random % 2;
        coinFlip = (BOOL*)Random;

        return(coinFlip);
    }
}

uint16_t CalcDmgFromMonsterAToMonsterB(uint8_t AMonLevel, uint16_t AMonAtk, uint16_t BMonDef, uint16_t AMonPsi, uint16_t BMonRes, uint8_t AMovePower1, uint8_t AMovePower2, uint8_t AMovePower3, uint8_t AMoveSplit, BOOL IsMonAPlayer)
{
    DWORD Random;
    uint16_t Random16;

    uint16_t PotentialDmg;
    uint16_t HighestRoll;
    uint16_t LowestRoll;
    float Power1Ratio;
    float Power2Ratio;
    float Power3Ratio;
    uint8_t Power1 = 0;
    uint8_t Power2 = 0;
    uint8_t Power3 = 0;

    uint16_t AMonAtkMod = 0;
    uint16_t AMonPsiMod = 0;
    uint16_t BMonDefMod = 0;
    uint16_t BMonResMod = 0;

    if (IsMonAPlayer == TRUE && AMoveSplit != SPLIT_STATUS)
    {
        for (uint8_t stattype = 0; stattype < 4; stattype++)
        {
            if (stattype == 0)     // atk
            {
                switch (gStatChangesCurrentPlayerMon[stattype])
                {
                    case 7:
                    {
                        AMonAtkMod = AMonAtk;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        AMonAtkMod = ((2.0f * AMonAtk) / ((7 - gStatChangesCurrentPlayerMon[stattype]) + 2));
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        AMonAtkMod = ((((gStatChangesCurrentPlayerMon[stattype] - 7) + 2) / 2.0f) * AMonAtk);
                        break;
                    }
                }
            }
            else if (stattype == 1)     // def
            {
                switch (gStatChangesCurrentOpponentMon[stattype])
                {
                    case 7:
                    {
                        BMonDefMod = BMonDef;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        BMonDefMod = ((2 / ((7 - gStatChangesCurrentPlayerMon[stattype]) + 2)) * BMonDef);
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        BMonDefMod = ((((gStatChangesCurrentPlayerMon[stattype] - 7) + 2) / 2) * BMonDef);
                        break;
                    }
                }
            }
            else if (stattype == 3)     // psi
            {
                switch (gStatChangesCurrentPlayerMon[stattype])
                {
                    case 7:
                    {
                        AMonPsiMod = AMonPsi;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        AMonPsiMod = ((2 / ((7 - gStatChangesCurrentPlayerMon[stattype]) + 2)) * AMonPsi);
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        AMonPsiMod = ((((gStatChangesCurrentPlayerMon[stattype] - 7) + 2) / 2) * AMonPsi);
                        break;
                    }
                }
            }
            else if (stattype == 4)     // res
            {
                switch (gStatChangesCurrentOpponentMon[stattype])
                {
                    case 7:
                    {
                        BMonResMod = BMonRes;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        BMonResMod = ((2 / ((7 - gStatChangesCurrentPlayerMon[stattype]) + 2)) * BMonRes);
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        BMonResMod = ((((gStatChangesCurrentPlayerMon[stattype] - 7) + 2) / 2) * BMonRes);
                        break;
                    }
                }
            }
        }
    }
    else if (IsMonAPlayer == FALSE && AMoveSplit != SPLIT_STATUS)               ////////////////////when AMonster is opponent
    {
        for (uint8_t stattype = 0; stattype < 4; stattype++)
        {
            if (stattype == 0)     // atk
            {
                switch (gStatChangesCurrentOpponentMon[stattype])
                {
                    case 7:
                    {
                        AMonAtkMod = AMonAtk;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        AMonAtkMod = ((2.0f * AMonAtk) / ((7 - gStatChangesCurrentOpponentMon[stattype]) + 2));
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        AMonAtkMod = ((((gStatChangesCurrentOpponentMon[stattype] - 7) + 2) / 2.0f) * AMonAtk);
                        break;
                    }
                }
            }
            else if (stattype == 1)     // def
            {
                switch (gStatChangesCurrentPlayerMon[stattype])
                {
                    case 7:
                    {
                        BMonDefMod = BMonDef;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        BMonDefMod = ((2 / ((7 - gStatChangesCurrentOpponentMon[stattype]) + 2)) * BMonDef);
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        BMonDefMod = ((((gStatChangesCurrentOpponentMon[stattype] - 7) + 2) / 2) * BMonDef);
                        break;
                    }
                }
            }
            else if (stattype == 3)     // psi
            {
                switch (gStatChangesCurrentOpponentMon[stattype])
                {
                    case 7:
                    {
                        AMonPsiMod = AMonPsi;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        AMonPsiMod = ((2 / ((7 - gStatChangesCurrentOpponentMon[stattype]) + 2)) * AMonPsi);
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        AMonPsiMod = ((((gStatChangesCurrentOpponentMon[stattype] - 7) + 2) / 2) * AMonPsi);
                        break;
                    }
                }
            }
            else if (stattype == 4)     // res
            {
                switch (gStatChangesCurrentPlayerMon[stattype])
                {
                    case 7:
                    {
                        BMonResMod = BMonRes;
                        break;
                    }
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    {
                        BMonResMod = ((2 / ((7 - gStatChangesCurrentOpponentMon[stattype]) + 2)) * BMonRes);
                        break;
                    }
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    {
                        BMonResMod = ((((gStatChangesCurrentOpponentMon[stattype] - 7) + 2) / 2) * BMonRes);
                        break;
                    }
                }
            }
        }
    }

    if (AMoveSplit == SPLIT_PHYS)
    {
        if (AMovePower1 > 0)
        {
            if (AMonAtkMod > BMonDefMod * 1.15)
            {
                Power1Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 85.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 1.15 && AMonAtkMod > BMonDefMod * 0.85)
            {
                Power1Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 100.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 0.85 && AMonAtkMod > BMonDefMod * 0.6)
            {
                Power1Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 110.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 0.6)
            {
                Power1Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 120.0f));
            }
            else
            {
                Power1Ratio = 0;
            }
            Power1 = ((((AMonLevel / 2) + 4) * AMovePower1 * Power1Ratio) / 64) + 4;
        }


        if (AMovePower2 > 0)
        {
            if (AMonAtkMod > BMonDefMod * 1.25)
            {
                Power2Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 95.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 1.25 && AMonAtkMod > BMonDefMod * 0.75)
            {
                Power2Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 90.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 0.75 && AMonAtkMod > BMonDefMod * 0.5)
            {
                Power2Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 100.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 0.6)
            {
                Power2Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 110.0f));
            }
            else
            {
                Power2Ratio = 0;
            }
            Power2 = ((((AMonLevel / 2) + 4) * AMovePower2 * Power2Ratio) / 64) + 4;
        }


        if (AMovePower3 > 0)
        {
            if (AMonAtkMod > BMonDefMod * 1.05)
            {
                Power3Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 100.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 1.05 && AMonAtkMod > BMonDefMod * 0.95)
            {
                Power3Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 95.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 0.95 && AMonAtkMod > BMonDefMod * 0.6)
            {
                Power3Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 85.0f));
            }
            else if (AMonAtkMod <= BMonDefMod * 0.6)
            {
                Power3Ratio = ((AMonAtkMod * 100.0f) / (BMonDefMod * 70.0f));
            }
            else
            {
                Power3Ratio = 0;
            }
            Power3 = ((((AMonLevel / 2) + 4) * AMovePower3 * Power3Ratio) / 64) + 4;
        }
    }
    else if (AMoveSplit == SPLIT_PSI)
    {
        if (AMovePower1 > 0)
        {
            if (AMonPsiMod > BMonResMod * 1.15)
            {
                Power1Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 85.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 1.15 && AMonPsiMod > BMonResMod * 0.85)
            {
                Power1Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 100.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 0.85 && AMonPsiMod > BMonResMod * 0.6)
            {
                Power1Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 110.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 0.6)
            {
                Power1Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 120.0f));
            }
            else
            {
                Power1Ratio = 0;
            }
            Power1 = ((((AMonLevel / 2) + 4) * AMovePower1 * Power1Ratio) / 64) + 4;
        }


        if (AMovePower2 > 0)
        {
            if (AMonPsiMod > BMonResMod * 1.25)
            {
                Power2Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 95.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 1.25 && AMonPsiMod > BMonResMod * 0.75)
            {
                Power2Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 90.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 0.75 && AMonPsiMod > BMonResMod * 0.5)
            {
                Power2Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 100.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 0.6)
            {
                Power2Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 110.0f));
            }
            else
            {
                Power2Ratio = 0;
            }
            Power2 = ((((AMonLevel / 2) + 4) * AMovePower2 * Power2Ratio) / 64) + 4;
        }


        if (AMovePower3 > 0)
        {
            if (AMonPsiMod > BMonResMod * 1.05)
            {
                Power3Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 100.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 1.05 && AMonPsiMod > BMonResMod * 0.95)
            {
                Power3Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 95.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 0.95 && AMonPsiMod > BMonResMod * 0.6)
            {
                Power3Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 85.0f));
            }
            else if (AMonPsiMod <= BMonResMod * 0.6)
            {
                Power3Ratio = ((AMonPsiMod * 100.0f) / (BMonResMod * 70.0f));
            }
            else
            {
                Power3Ratio = 0;
            }
            Power3 = ((((AMonLevel / 2) + 4) * AMovePower3 * Power3Ratio) / 64) + 4;
        }
    }

    HighestRoll = Power1 + Power2 + Power3;
    LowestRoll = HighestRoll * 0.85;

    rand_s((unsigned int*)&Random);
    Random16 = (uint16_t*)Random;

    PotentialDmg = (Random16 % (HighestRoll - LowestRoll + 1)) + LowestRoll;

    gWasLastMoveCriticalHit = FALSE;
    if (Random16 % 16 == 0 && AMoveSplit != SPLIT_STATUS && gLastMoveElementalBonus != ELEMENT_IMMUNE)
    {
        gWasLastMoveCriticalHit = TRUE;
        PotentialDmg *= 1.5;
    }
    return (PotentialDmg);
}

void ModifyMonsterHealthValueGetKO(uint16_t damageToMonster, BOOL isPlayerSideMonster, BOOL IsPlayerMoveFirst)
{
    
    if ((gOpponentParty[gCurrentOpponentPartyMember].Health != 0) && (gPlayerParty[gCurrentPartyMember].Health != 0))
    {
        if (isPlayerSideMonster == TRUE)
        {
            for (uint16_t i = damageToMonster; i > 0; i--)
            {
                gPlayerParty[gCurrentPartyMember].Health--;
                if (gPlayerParty[gCurrentPartyMember].Health == 0)
                {
                    gWasMonsterKOed = TRUE;
                    goto FinishedDealingDamage;
                }
            }
        }
        else
        {
            for (uint16_t i = damageToMonster; i > 0; i--)
            {
                gOpponentParty[gCurrentOpponentPartyMember].Health--;
                if (gOpponentParty[gCurrentOpponentPartyMember].Health == 0)
                {
                    gWasMonsterKOed = TRUE;
                    goto FinishedDealingDamage;
                }
            }
        }
    }
    else if ((gOpponentParty[gCurrentOpponentPartyMember].Health != 0) || (gPlayerParty[gCurrentPartyMember].Health != 0))
    {
        gWasMonsterKOed = TRUE;
    }

FinishedDealingDamage:

    switch (gCurrentBattleState)
    {
        case BATTLESTATE_FIRSTMOVE_CALC:
        {
            if (gWasLastMoveCriticalHit == TRUE || gLastMoveElementalBonus != ELEMENT_NEUTRAL || (IsPlayerMoveFirst && gStatsChangedFromPlayerMove) || ( !IsPlayerMoveFirst && gStatsChangedFromOpponentMove))
            {
                gCurrentBattleState = BATTLESTATE_FIRSTMOVE_POSTTEXT;
            }
            else
            {
                if (gWasMonsterKOed == TRUE)
                {
                    gCurrentBattleState = BATTLESTATE_KO;
                }
                else
                {
                    gCurrentBattleState = BATTLESTATE_SECONDMOVE_TEXT;
                }
            }
            break;
        }
        case BATTLESTATE_SECONDMOVE_CALC:
        {
            if (gWasLastMoveCriticalHit == TRUE || gLastMoveElementalBonus != ELEMENT_NEUTRAL || (!IsPlayerMoveFirst && gStatsChangedFromPlayerMove) || (IsPlayerMoveFirst && gStatsChangedFromOpponentMove))
            {
                gCurrentBattleState = BATTLESTATE_SECONDMOVE_POSTTEXT;
            }
            else
            {
                if (gWasMonsterKOed == TRUE)
                {
                    gCurrentBattleState = BATTLESTATE_KO;
                }
                else
                {
                    gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
                }
            }
            break;
        }
    }

    return(FALSE);
}

uint16_t GetElementaBonusDamage(uint16_t damageBeforeElement, BOOL isPlayerMonsterMoveTarget)
{
    uint16_t DamageAfterElement;

    uint8_t DefendingElement_1;
    uint8_t DefendingElement_2;

    uint8_t AttackingElement;

    if (isPlayerMonsterMoveTarget == TRUE)
    {
        AttackingElement = gBattleMoves[gSelectedOpponentMove].element;

        DefendingElement_1 = gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].element1;
        DefendingElement_2 = gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].element2;
        if (DefendingElement_1 == DefendingElement_2)
        {
            DefendingElement_2 = ELEMENT_NULL;
        }
    }
    else
    {
        AttackingElement = gBattleMoves[gSelectedPlayerMove].element;

        DefendingElement_1 = gBaseStats[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Index].element1;
        DefendingElement_2 = gBaseStats[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Index].element2;
        if (DefendingElement_1 == DefendingElement_2)
        {
            DefendingElement_2 = ELEMENT_NULL;
        }
    }

    switch (AttackingElement)
    {
        case ELEMENT_NONE:
        {
            if (DefendingElement_1 == ELEMENT_EARTH || DefendingElement_1 == ELEMENT_METAL || DefendingElement_1 == ELEMENT_DEATH)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_SOUL)
            {
                DamageAfterElement = 0;
                gLastMoveElementalBonus = ELEMENT_IMMUNE;
                return(DamageAfterElement);
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_EARTH:
        {
            if (DefendingElement_1 == ELEMENT_FIRE || DefendingElement_1 == ELEMENT_ELECTRIC || DefendingElement_1 == ELEMENT_METAL)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_LIFE)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_AIR)
            {
                DamageAfterElement = 0;
                gLastMoveElementalBonus = ELEMENT_IMMUNE;
                return(DamageAfterElement);
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_AIR:
        {
            if (DefendingElement_1 == ELEMENT_FIRE || DefendingElement_1 == ELEMENT_LIFE)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_ELECTRIC || DefendingElement_1 == ELEMENT_METAL)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_FIRE:
        {
            if (DefendingElement_1 == ELEMENT_NONE || DefendingElement_1 == ELEMENT_AIR || DefendingElement_1 == ELEMENT_METAL || DefendingElement_1 == ELEMENT_LIFE || DefendingElement_1 == ELEMENT_DEATH)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_EARTH || DefendingElement_1 == ELEMENT_FIRE || DefendingElement_1 == ELEMENT_WATER)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_WATER:
        {
            if (DefendingElement_1 == ELEMENT_EARTH || DefendingElement_1 == ELEMENT_FIRE || DefendingElement_1 == ELEMENT_DEATH)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_WATER || DefendingElement_1 == ELEMENT_LIFE)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_ELECTRIC:
        {
            if (DefendingElement_1 == ELEMENT_AIR || DefendingElement_1 == ELEMENT_WATER || DefendingElement_1 == ELEMENT_METAL)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_DEATH)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_EARTH)
            {
                DamageAfterElement = 0;
                gLastMoveElementalBonus = ELEMENT_IMMUNE;
                return(DamageAfterElement);
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_METAL:
        {
            if (DefendingElement_1 == ELEMENT_DEATH)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_EARTH || DefendingElement_1 == ELEMENT_WATER || DefendingElement_1 == ELEMENT_ELECTRIC || DefendingElement_1 == ELEMENT_METAL)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_SOUL:
        {
            if (DefendingElement_1 == ELEMENT_DEATH || DefendingElement_1 == ELEMENT_SOUL)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_AIR || DefendingElement_1 == ELEMENT_LIFE)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_LIFE:
        {
            if (DefendingElement_1 == ELEMENT_EARTH || DefendingElement_1 == ELEMENT_WATER || DefendingElement_1 == ELEMENT_SOUL)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_AIR || DefendingElement_1 == ELEMENT_FIRE || DefendingElement_1 == ELEMENT_METAL || DefendingElement_1 == ELEMENT_LIFE)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
        case ELEMENT_DEATH:
        {
            if (DefendingElement_1 == ELEMENT_NONE || DefendingElement_1 == ELEMENT_EARTH || DefendingElement_1 == ELEMENT_AIR || DefendingElement_1 == ELEMENT_LIFE)
            {
                DamageAfterElement = damageBeforeElement * 2;
                gLastMoveElementalBonus = ELEMENT_BONUS;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_WATER || DefendingElement_1 == ELEMENT_DEATH)
            {
                DamageAfterElement = damageBeforeElement / 2;
                gLastMoveElementalBonus = ELEMENT_RESIST;
                break;
            }
            else if (DefendingElement_1 == ELEMENT_METAL || DefendingElement_1 == ELEMENT_SOUL)
            {
                DamageAfterElement = 0;
                gLastMoveElementalBonus = ELEMENT_IMMUNE;
                return(DamageAfterElement);
            }
            else
            {
                DamageAfterElement = damageBeforeElement;
                break;
            }
        }
    }

    //////found multipliers for first defensive element now repeat process

    switch (AttackingElement)
    {
        case ELEMENT_NONE:
        {
            if (DefendingElement_2 == ELEMENT_EARTH || DefendingElement_2 == ELEMENT_METAL || DefendingElement_2 == ELEMENT_DEATH)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_SOUL)
            {
                DamageAfterElement = 0;
                gLastMoveElementalBonus = ELEMENT_IMMUNE;
                return(DamageAfterElement);
            }
            else
            {
                break;
            }
        }
        case ELEMENT_EARTH:
        {
            if (DefendingElement_2 == ELEMENT_FIRE || DefendingElement_2 == ELEMENT_ELECTRIC || DefendingElement_2 == ELEMENT_METAL)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_LIFE)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_AIR)
            {
                DamageAfterElement = 0;
                gLastMoveElementalBonus = ELEMENT_IMMUNE;
                return(DamageAfterElement);
            }
            else
            {
                break;
            }
        }
        case ELEMENT_AIR:
        {
            if (DefendingElement_2 == ELEMENT_FIRE || DefendingElement_2 == ELEMENT_LIFE)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_ELECTRIC || DefendingElement_2 == ELEMENT_METAL)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else
            {
                break;
            }
        }
        case ELEMENT_FIRE:
        {
            if (DefendingElement_2 == ELEMENT_NONE || DefendingElement_2 == ELEMENT_AIR || DefendingElement_2 == ELEMENT_METAL || DefendingElement_2 == ELEMENT_LIFE || DefendingElement_2 == ELEMENT_DEATH)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_EARTH || DefendingElement_2 == ELEMENT_FIRE || DefendingElement_2 == ELEMENT_WATER)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else
            {
                break;
            }
        }
        case ELEMENT_WATER:
        {
            if (DefendingElement_2 == ELEMENT_EARTH || DefendingElement_2 == ELEMENT_FIRE || DefendingElement_2 == ELEMENT_DEATH)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_WATER || DefendingElement_2 == ELEMENT_LIFE)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else
            {
                break;
            }
        }
        case ELEMENT_ELECTRIC:
        {
            if (DefendingElement_2 == ELEMENT_AIR || DefendingElement_2 == ELEMENT_WATER || DefendingElement_2 == ELEMENT_METAL)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_DEATH)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_EARTH)
            {
                DamageAfterElement = 0;
                return(DamageAfterElement);
            }
            else
            {
                break;
            }
        }
        case ELEMENT_METAL:
        {
            if (DefendingElement_2 == ELEMENT_DEATH)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_EARTH || DefendingElement_2 == ELEMENT_WATER || DefendingElement_2 == ELEMENT_ELECTRIC || DefendingElement_2 == ELEMENT_METAL)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else
            {
                break;
            }
        }
        case ELEMENT_SOUL:
        {
            if (DefendingElement_2 == ELEMENT_DEATH || DefendingElement_2 == ELEMENT_SOUL)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_AIR || DefendingElement_2 == ELEMENT_LIFE)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else
            {
                break;
            }
        }
        case ELEMENT_LIFE:
        {
            if (DefendingElement_2 == ELEMENT_EARTH || DefendingElement_2 == ELEMENT_WATER || DefendingElement_2 == ELEMENT_SOUL)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_AIR || DefendingElement_2 == ELEMENT_FIRE || DefendingElement_2 == ELEMENT_METAL || DefendingElement_2 == ELEMENT_LIFE)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else
            {
                break;
            }
        }
        case ELEMENT_DEATH:
        {
            if (DefendingElement_2 == ELEMENT_NONE || DefendingElement_2 == ELEMENT_EARTH || DefendingElement_2 == ELEMENT_AIR || DefendingElement_2 == ELEMENT_LIFE)
            {
                DamageAfterElement *= 2;
                if (gLastMoveElementalBonus == ELEMENT_RESIST)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_BONUS;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_WATER || DefendingElement_2 == ELEMENT_DEATH)
            {
                DamageAfterElement /= 2;
                if (gLastMoveElementalBonus == ELEMENT_BONUS)
                {
                    gLastMoveElementalBonus = ELEMENT_NEUTRAL;
                }
                else
                {
                    gLastMoveElementalBonus = ELEMENT_RESIST;
                }
                break;
            }
            else if (DefendingElement_2 == ELEMENT_METAL || DefendingElement_2 == ELEMENT_SOUL)
            {
                DamageAfterElement = 0;
                gLastMoveElementalBonus = ELEMENT_IMMUNE;
                return(DamageAfterElement);
            }
            else
            {
                break;
            }
        }
    }

    ////////////////////calculate bonus when user and move have same element

    if (isPlayerMonsterMoveTarget == TRUE)
    {
        if (AttackingElement == gBaseStats[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Index].element1 || AttackingElement == gBaseStats[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Index].element2)
        {
            DamageAfterElement *= 1.5;
        }

        //////////////////calculate bonuses from elemental boosting equipable items
        if (AttackingElement == gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.HeldItem - 1)        //first 10 equip items (1...10) are all element boosters and align with the values for elements in order
        {
            DamageAfterElement *= 1.25;
        }
        else if (AttackingElement == gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.HeldItem - 11)
        {
            DamageAfterElement *= 1.1;
        }
    }
    else
    {
        if (AttackingElement == gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].element1 || AttackingElement == gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].element2)
        {
            DamageAfterElement *= 1.5;
        }

        //////////////////calculate bonuses from elemental boosting equipable items
        if (AttackingElement == gPlayerParty[gCurrentPartyMember].DriveMonster.HeldItem - 1)        //first 10 equip items (1...10) are all element boosters and align with the values for elements in order
        {
            DamageAfterElement *= 1.25;
        }
        else if (AttackingElement == gPlayerParty[gCurrentPartyMember].DriveMonster.HeldItem - 11)
        {
            DamageAfterElement *= 1.1;
        }
    }

    return(DamageAfterElement);
}

void GenerateOpponentMove(uint8_t Opponent)
{
    if (Opponent == NULL)       ////TODO: make own function so opponent can always generate a move
    {
        gSelectedOpponentMoveSlot = CalculateOpponentMoveChoice(FLAG_NPCAI_RANDOM);
        gSelectedOpponentMove = gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[gSelectedOpponentMoveSlot];
    }
    else
    {
        gSelectedOpponentMoveSlot = CalculateOpponentMoveChoice(gCharacterSprite[Opponent].BattleAiFlag);
        gSelectedOpponentMove = gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[gSelectedOpponentMoveSlot];
    }
}
