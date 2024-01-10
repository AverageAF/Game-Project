
#include "Main.h"

#include "BattleScreen.h"

#include "MonsterData.h"





///////// load starting monster

uint8_t gPartyMemberToSwitchIn = 0;
uint8_t gCurrentPartyMember = 0;
uint8_t gCurrentOpponentPartyMember = 0;
uint8_t gSelectedPlayerMove = 0;

////////Initial starting choices at the start of battle

MENUITEM gMI_BattleScreen_FightButton = { "Fight!", (GAME_RES_WIDTH / 2) - (6 * 7 / 2) + 2, 200, TRUE, MenuItem_BattleScreen_FightButton };

MENUITEM gMI_BattleScreen_EscapeButton = { "Escape", (GAME_RES_WIDTH / 2) - (6 * 7 / 2) + 2, 226, TRUE, MenuItem_BattleScreen_EscapeButton };

MENUITEM gMI_BattleScreen_SwitchButton = { "Switch", 64 + 12, 210, TRUE, MenuItem_BattleScreen_SwitchButton };

MENUITEM gMI_BattleScreen_ItemsButton = { "Items", 256 + 14, 210, TRUE, MenuItem_BattleScreen_ItemsButton };

MENUITEM* gMI_BattleScreen_Items[] = { &gMI_BattleScreen_FightButton, &gMI_BattleScreen_EscapeButton, &gMI_BattleScreen_SwitchButton, &gMI_BattleScreen_ItemsButton };

MENU gMenu_BattleScreen = { "Battle Menu", 0 , _countof(gMI_BattleScreen_Items), gMI_BattleScreen_Items };

////////Menu choices for switching monsters in battle

MENUITEM gMI_SwitchScreen_PartySlot0 = { &gPlayerParty[0].DriveMonster.nickname, 70, 187, FALSE, MenuItem_SwitchScreen_PartySlot0 };
MENUITEM gMI_SwitchScreen_PartySlot1 = { &gPlayerParty[1].DriveMonster.nickname, 200, 187, FALSE, MenuItem_SwitchScreen_PartySlot1 };
MENUITEM gMI_SwitchScreen_PartySlot2 = { &gPlayerParty[2].DriveMonster.nickname, 70, 207, FALSE, MenuItem_SwitchScreen_PartySlot2 };
MENUITEM gMI_SwitchScreen_PartySlot3 = { &gPlayerParty[3].DriveMonster.nickname, 200, 207, FALSE, MenuItem_SwitchScreen_PartySlot3 };
MENUITEM gMI_SwitchScreen_PartySlot4 = { &gPlayerParty[4].DriveMonster.nickname, 70, 230, FALSE, MenuItem_SwitchScreen_PartySlot4 };
MENUITEM gMI_SwitchScreen_PartySlot5 = { &gPlayerParty[5].DriveMonster.nickname, 200, 230, FALSE, MenuItem_SwitchScreen_PartySlot5 };

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
    static uint8_t OpponentMove = 0;
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
            BattleTextLineCount = 0;
            //CopyMonsterToOpponentParty(0, GenerateScriptedMonsterForWildEncounter(3, 5, 0));
            CopyMonsterToOpponentParty(0, GenerateRandMonsterForWildEncounter(5, 0));
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
    }

    //////TODO: Add battle intro and battle music/////////////
    //if (LocalFrameCounter == 0)
    //{
    //    StopGameMusic();
    //    /*PlayGameMusic(&MusicBattleInto01, FALSE, TRUE);
    //    PlayGameMusic(&MusicBattle01, TRUE, FALSE);*/       ////queue full loop behind intro
    //}

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
            if (Opponent == NULL)
            {
                OpponentMove = CalculateOpponentMoveChoice(FLAG_NPCAI_RANDOM);
            }
            else
            {
                OpponentMove = CalculateOpponentMoveChoice(gCharacterSprite[Opponent].BattleAiFlag);
            }

            IsPlayerMovingFirst = CalculateSpeedPriorityIfPlayerMovesFirst(gPlayerParty[gCurrentPartyMember].Speed, gOpponentParty[gCurrentOpponentPartyMember].Speed);

            gCurrentBattleState = BATTLESTATE_FIRSTMOVE_TEXT;
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
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname, &gBattleMoveNames[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]]);
                BattleTextLineCount++;
            }
            /*
            sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "It dealt extra element damage!");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[4], sizeof(gBattleTextLine[4]), "It was resisted...");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[5], sizeof(gBattleTextLine[5]), "Devastating damage!");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[6], sizeof(gBattleTextLine[6]), "Extra devastating element damage!");
            BattleTextLineCount++;*/


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
            if (IsPlayerMovingFirst == TRUE)
            {
                DamageToOpponent = 
                    CalcPotentialDamageToPlayerMonster (
                    gPlayerParty[gCurrentPartyMember].Level,
                    gPlayerParty[gCurrentPartyMember].Attack,
                    gOpponentParty[gCurrentOpponentPartyMember].Defense,
                    gPlayerParty[gCurrentPartyMember].Psi,
                    gOpponentParty[gCurrentOpponentPartyMember].Resolve,
                    gBattleMoves[gSelectedPlayerMove].power1,
                    gBattleMoves[gSelectedPlayerMove].power2,
                    gBattleMoves[gSelectedPlayerMove].power3,
                    gBattleMoves[gSelectedPlayerMove].split
                );

                MonsterHasKOed = ModifyMonsterHealthValueGetKO(DamageToOpponent, FALSE);
            }
            else
            {
                DamageToPlayer = 
                    CalcPotentialDamageToPlayerMonster (
                    gOpponentParty[gCurrentOpponentPartyMember].Level,
                    gOpponentParty[gCurrentOpponentPartyMember].Attack,
                    gPlayerParty[gCurrentPartyMember].Defense,
                    gOpponentParty[gCurrentOpponentPartyMember].Psi,
                    gPlayerParty[gCurrentPartyMember].Resolve,
                    gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].power1,
                    gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].power2,
                    gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].power3,
                    gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].split
                );

                MonsterHasKOed = ModifyMonsterHealthValueGetKO(DamageToPlayer, TRUE);
            }

            if (MonsterHasKOed == TRUE)
            {
                gCurrentBattleState = BATTLESTATE_KO;
            }

            break;
        }
        case BATTLESTATE_SWITCHING_TEXT:
        {
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
            
            /*
            sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "It dealt extra element damage!");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[4], sizeof(gBattleTextLine[4]), "It was resisted...");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[5], sizeof(gBattleTextLine[5]), "Devastating damage!");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[6], sizeof(gBattleTextLine[6]), "Extra devastating element damage!");
            BattleTextLineCount++;*/


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
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s used %s!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname, &gBattleMoveNames[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]]);
                BattleTextLineCount++;
            }
            /*
            sprintf_s((char*)gBattleTextLine[3], sizeof(gBattleTextLine[3]), "It dealt extra element damage!");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[4], sizeof(gBattleTextLine[4]), "It was resisted...");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[5], sizeof(gBattleTextLine[5]), "Devastating damage!");
            BattleTextLineCount++;
            sprintf_s((char*)gBattleTextLine[6], sizeof(gBattleTextLine[6]), "Extra devastating element damage!");
            BattleTextLineCount++;*/



            TextHasFinished = BlitBattleStateTextBox_Text(BATTLESTATE_SECONDMOVE_WAIT, BattleTextLineCount, LocalFrameCounter);

            if (TextHasFinished == TRUE)
            {
                goto WaitOpening;
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
            if (IsPlayerMovingFirst == FALSE)
            {
                //player is dealing dmg to opponent on second move bc FALSE
                DamageToOpponent =
                    CalcPotentialDamageToPlayerMonster(
                        gPlayerParty[gCurrentPartyMember].Level,
                        gPlayerParty[gCurrentPartyMember].Attack,
                        gOpponentParty[gCurrentOpponentPartyMember].Defense,
                        gPlayerParty[gCurrentPartyMember].Psi,
                        gOpponentParty[gCurrentOpponentPartyMember].Resolve,
                        gBattleMoves[gSelectedPlayerMove].power1,
                        gBattleMoves[gSelectedPlayerMove].power2,
                        gBattleMoves[gSelectedPlayerMove].power3,
                        gBattleMoves[gSelectedPlayerMove].split
                    );

                MonsterHasKOed = ModifyMonsterHealthValueGetKO(DamageToOpponent, FALSE);
            }
            else
            {
                DamageToPlayer =
                    CalcPotentialDamageToPlayerMonster(
                        gOpponentParty[gCurrentOpponentPartyMember].Level,
                        gOpponentParty[gCurrentOpponentPartyMember].Attack,
                        gPlayerParty[gCurrentPartyMember].Defense,
                        gOpponentParty[gCurrentOpponentPartyMember].Psi,
                        gPlayerParty[gCurrentPartyMember].Resolve,
                        gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].power1,
                        gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].power2,
                        gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].power3,
                        gBattleMoves[gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.Moves[OpponentMove]].split
                    );

                MonsterHasKOed = ModifyMonsterHealthValueGetKO(DamageToPlayer, TRUE);
            }

            if (MonsterHasKOed == TRUE)
            {
                gCurrentBattleState = BATTLESTATE_KO;
            }

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

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_BattleScreen_Items[gMenu_BattleScreen.SelectedItem]->x - 6, gMI_BattleScreen_Items[gMenu_BattleScreen.SelectedItem]->y);
}

void DrawMoveButtons(void)
{
    //slot 1
    DrawWindow(64, 185, 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);

    //slot 2
    DrawWindow(64, 187 + 12, 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);
                       
    //slot 3
    DrawWindow(64, 189 + 24, 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);
                 
    //slot 4
    DrawWindow(64, 191 + 36, 240, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_ROUNDED);
                            
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

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_MoveScreen_Items[gMenu_MoveScreen.SelectedItem]->x - 6, gMI_MoveScreen_Items[gMenu_MoveScreen.SelectedItem]->y);
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

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gMI_SwitchScreen_Items[gMenu_SwitchScreen.SelectedItem]->x - 9, gMI_SwitchScreen_Items[gMenu_SwitchScreen.SelectedItem]->y);
}

//////// initial choice menu

void MenuItem_BattleScreen_FightButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_CHOOSE_MOVE;
}

void MenuItem_BattleScreen_EscapeButton(void)
{
    for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
    {
        if ((gCharacterSprite[Index].InteractedWith == TRUE) && (gCharacterSprite[Index].Event == EVENT_FLAG_BATLLE))
        {
            gCharacterSprite[Index].InteractedWith = FALSE;
            gCharacterSprite[Index].Event = EVENT_FLAG_NONE;
            gCharacterSprite[Index].SightRange = 0;
            break;
        }
    }

    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_OVERWORLD;
    gInputEnabled = FALSE;
    StopGameMusic();
}

void MenuItem_BattleScreen_SwitchButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_CHOOSE_MONSTER;
}

void MenuItem_BattleScreen_ItemsButton(void)
{

}

/////// switching monsters screen

void MenuItem_SwitchScreen_PartySlot0(void)
{
    if (gCurrentPartyMember == 0 || gPlayerParty[0].Health == 0)
    {
        //trying to swap to monster already in battle
    }
    else
    {
        gPartyMemberToSwitchIn = 0;
        gCurrentBattleState = BATTLESTATE_SWITCHING_TEXT;
    }
}

void MenuItem_SwitchScreen_PartySlot1(void)
{
    if (gCurrentPartyMember == 1 || gPlayerParty[1].Health == 0)
    {
        //trying to swap to monster already in battle
    }
    else
    {
        gPartyMemberToSwitchIn = 1;
        gCurrentBattleState = BATTLESTATE_SWITCHING_TEXT;
    }
}

void MenuItem_SwitchScreen_PartySlot2(void)
{
    if (gCurrentPartyMember == 2 || gPlayerParty[2].Health == 0)
    {
        //trying to swap to monster already in battle
    }
    else
    {
        gPartyMemberToSwitchIn = 2;
        gCurrentBattleState = BATTLESTATE_SWITCHING_TEXT;
    }
}

void MenuItem_SwitchScreen_PartySlot3(void)
{
    if (gCurrentPartyMember == 3 || gPlayerParty[3].Health == 0)
    {
        //trying to swap to monster already in battle
    }
    else
    {
        gPartyMemberToSwitchIn = 3;
        gCurrentBattleState = BATTLESTATE_SWITCHING_TEXT;
    }
}

void MenuItem_SwitchScreen_PartySlot4(void)
{
    if (gCurrentPartyMember == 4 || gPlayerParty[4].Health == 0)
    {
        //trying to swap to monster already in battle
    }
    else
    {
        gPartyMemberToSwitchIn = 4;
        gCurrentBattleState = BATTLESTATE_SWITCHING_TEXT;
    }
}

void MenuItem_SwitchScreen_PartySlot5(void)
{
    if (gCurrentPartyMember == 5 || gPlayerParty[5].Health == 0)
    {
        //trying to swap to monster already in battle
    }
    else
    {
        gPartyMemberToSwitchIn = 5;
        gCurrentBattleState = BATTLESTATE_SWITCHING_TEXT;
    }
}

void MenuItem_SwitchScreen_BackButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
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

uint16_t CalcPotentialDamageToPlayerMonster(uint8_t oppLevel, uint16_t oppMonAtk, uint16_t playerMonDef, uint16_t oppMonPsi, uint16_t playerMonRes, uint8_t movePower1, uint8_t movePower2, uint8_t movePower3, uint8_t split)
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

    if (split == SPLIT_PHYS)
    {
        if (movePower1 > 0)
        {
            if (oppMonAtk > playerMonDef * 1.15)
            {
                Power1Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 85.0f));
            }
            else if (oppMonAtk <= playerMonDef * 1.15 && oppMonAtk > playerMonDef * 0.85)
            {
                Power1Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 100.0f));
            }
            else if (oppMonAtk <= playerMonDef * 0.85 && oppMonAtk > playerMonDef * 0.6)
            {
                Power1Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 110.0f));
            }
            else if (oppMonAtk <= playerMonDef * 0.6)
            {
                Power1Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 120.0f));
            }
            else
            {
                Power1Ratio = 0;
            }
            Power1 = ((((oppLevel / 2) + 4) * movePower1 * Power1Ratio) / 64) + 4;
        }


        if (movePower2 > 0)
        {
            if (oppMonAtk > playerMonDef * 1.25)
            {
                Power2Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 95.0f));
            }
            else if (oppMonAtk <= playerMonDef * 1.25 && oppMonAtk > playerMonDef * 0.75)
            {
                Power2Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 90.0f));
            }
            else if (oppMonAtk <= playerMonDef * 0.75 && oppMonAtk > playerMonDef * 0.5)
            {
                Power2Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 100.0f));
            }
            else if (oppMonAtk <= playerMonDef * 0.6)
            {
                Power2Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 110.0f));
            }
            else
            {
                Power2Ratio = 0;
            }
            Power2 = ((((oppLevel / 2) + 4) * movePower2 * Power2Ratio) / 64) + 4;
        }


        if (movePower3 > 0)
        {
            if (oppMonAtk > playerMonDef * 1.05)
            {
                Power3Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 100.0f));
            }
            else if (oppMonAtk <= playerMonDef * 1.05 && oppMonAtk > playerMonDef * 0.95)
            {
                Power3Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 95.0f));
            }
            else if (oppMonAtk <= playerMonDef * 0.95 && oppMonAtk > playerMonDef * 0.6)
            {
                Power3Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 85.0f));
            }
            else if (oppMonAtk <= playerMonDef * 0.6)
            {
                Power3Ratio = ((oppMonAtk * 100.0f) / (playerMonDef * 70.0f));
            }
            else
            {
                Power3Ratio = 0;
            }
            Power3 = ((((oppLevel / 2) + 4) * movePower3 * Power3Ratio) / 64) + 4;
        }
    }
    else if (split == SPLIT_PSI)
    {
        if (movePower1 > 0)
        {
            if (oppMonPsi > playerMonRes * 1.15)
            {
                Power1Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 85.0f));
            }
            else if (oppMonPsi <= playerMonRes * 1.15 && oppMonPsi > playerMonRes * 0.85)
            {
                Power1Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 100.0f));
            }
            else if (oppMonPsi <= playerMonRes * 0.85 && oppMonPsi > playerMonRes * 0.6)
            {
                Power1Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 110.0f));
            }
            else if (oppMonPsi <= playerMonRes * 0.6)
            {
                Power1Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 120.0f));
            }
            else
            {
                Power1Ratio = 0;
            }
            Power1 = ((((oppLevel / 2) + 4) * movePower1 * Power1Ratio) / 64) + 4;
        }


        if (movePower2 > 0)
        {
            if (oppMonPsi > playerMonRes * 1.25)
            {
                Power2Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 95.0f));
            }
            else if (oppMonPsi <= playerMonRes * 1.25 && oppMonPsi > playerMonRes * 0.75)
            {
                Power2Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 90.0f));
            }
            else if (oppMonPsi <= playerMonRes * 0.75 && oppMonPsi > playerMonRes * 0.5)
            {
                Power2Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 100.0f));
            }
            else if (oppMonPsi <= playerMonRes * 0.6)
            {
                Power2Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 110.0f));
            }
            else
            {
                Power2Ratio = 0;
            }
            Power2 = ((((oppLevel / 2) + 4) * movePower2 * Power2Ratio) / 64) + 4;
        }


        if (movePower3 > 0)
        {
            if (oppMonPsi > playerMonRes * 1.05)
            {
                Power3Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 100.0f));
            }
            else if (oppMonPsi <= playerMonRes * 1.05 && oppMonPsi > playerMonRes * 0.95)
            {
                Power3Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 95.0f));
            }
            else if (oppMonPsi <= playerMonRes * 0.95 && oppMonPsi > playerMonRes * 0.6)
            {
                Power3Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 85.0f));
            }
            else if (oppMonPsi <= playerMonRes * 0.6)
            {
                Power3Ratio = ((oppMonPsi * 100.0f) / (playerMonRes * 70.0f));
            }
            else
            {
                Power3Ratio = 0;
            }
            Power3 = ((((oppLevel / 2) + 4) * movePower3 * Power3Ratio) / 64) + 4;
        }
    }

    HighestRoll = Power1 + Power2 + Power3;
    LowestRoll = HighestRoll * 0.85;

    rand_s((unsigned int*)&Random);
    Random16 = (uint16_t*)Random;

    PotentialDmg = (Random16 % (HighestRoll - LowestRoll + 1)) + LowestRoll;

    if (Random16 % 16 == 0)     //TODO:BATTLE_TEXT_FLAG for crits, element effectiveness, ect..
    {
        PotentialDmg += (PotentialDmg * 1.5);
    }
    return (PotentialDmg);
}

uint16_t CalcPotentialDamageToOpponentMonster(uint8_t playerLevel, uint16_t playerMonAtk, uint16_t oppMonDef, uint16_t playerMonPsi, uint16_t oppMonRes, uint8_t movePower1, uint8_t movePower2, uint8_t movePower3, uint8_t split)
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

    if (split == SPLIT_PHYS)
    {
        if (movePower1 > 0)
        {
            if (playerMonAtk > oppMonDef * 1.15)
            {
                Power1Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 85.0f));
            }
            else if (playerMonAtk <= oppMonDef * 1.15 && playerMonAtk > oppMonDef * 0.85)
            {
                Power1Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 100.0f));
            }
            else if (playerMonAtk <= oppMonDef * 0.85 && playerMonAtk > oppMonDef * 0.6)
            {
                Power1Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 110.0f));
            }
            else if (playerMonAtk <= oppMonDef * 0.6)
            {
                Power1Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 120.0f));
            }
            else
            {
                Power1Ratio = 0;
            }
            Power1 = ((((playerLevel / 2) + 4) * movePower1 * Power1Ratio) / 64) + 4;
        }


        if (movePower2 > 0)
        {
            if (playerMonAtk > oppMonDef * 1.25)
            {
                Power2Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 95.0f));
            }
            else if (playerMonAtk <= oppMonDef * 1.25 && playerMonAtk > oppMonDef * 0.75)
            {
                Power2Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 90.0f));
            }
            else if (playerMonAtk <= oppMonDef * 0.75 && playerMonAtk > oppMonDef * 0.5)
            {
                Power2Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 100.0f));
            }
            else if (playerMonAtk <= oppMonDef * 0.6)
            {
                Power2Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 110.0f));
            }
            else
            {
                Power2Ratio = 0;
            }
            Power2 = ((((playerLevel / 2) + 4) * movePower2 * Power2Ratio) / 64) + 4;
        }


        if (movePower3 > 0)
        {
            if (playerMonAtk > oppMonDef * 1.05)
            {
                Power3Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 100.0f));
            }
            else if (playerMonAtk <= oppMonDef * 1.05 && playerMonAtk > oppMonDef * 0.95)
            {
                Power3Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 95.0f));
            }
            else if (playerMonAtk <= oppMonDef * 0.95 && playerMonAtk > oppMonDef * 0.6)
            {
                Power3Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 85.0f));
            }
            else if (playerMonAtk <= oppMonDef * 0.6)
            {
                Power3Ratio = ((playerMonAtk * 100.0f) / (oppMonDef * 70.0f));
            }
            else
            {
                Power3Ratio = 0;
            }
            Power3 = ((((playerLevel / 2) + 4) * movePower3 * Power3Ratio) / 64) + 4;
        }
    }
    else if (split == SPLIT_PSI)
    {
        if (movePower1 > 0)
        {
            if (playerMonPsi > oppMonRes * 1.15)
            {
                Power1Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 85.0f));
            }
            else if (playerMonPsi <= oppMonRes * 1.15 && playerMonPsi > oppMonRes * 0.85)
            {
                Power1Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 100.0f));
            }
            else if (playerMonPsi <= oppMonRes * 0.85 && playerMonPsi > oppMonRes * 0.6)
            {
                Power1Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 110.0f));
            }
            else if (playerMonPsi <= oppMonRes * 0.6)
            {
                Power1Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 120.0f));
            }
            else
            {
                Power1Ratio = 0;
            }
            Power1 = ((((playerLevel / 2) + 4) * movePower1 * Power1Ratio) / 64) + 4;
        }


        if (movePower2 > 0)
        {
            if (playerMonPsi > oppMonRes * 1.25)
            {
                Power2Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 95.0f));
            }
            else if (playerMonPsi <= oppMonRes * 1.25 && playerMonPsi > oppMonRes * 0.75)
            {
                Power2Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 90.0f));
            }
            else if (playerMonPsi <= oppMonRes * 0.75 && playerMonPsi > oppMonRes * 0.5)
            {
                Power2Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 100.0f));
            }
            else if (playerMonPsi <= oppMonRes * 0.6)
            {
                Power2Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 110.0f));
            }
            else
            {
                Power2Ratio = 0;
            }
            Power2 = ((((playerLevel / 2) + 4) * movePower2 * Power2Ratio) / 64) + 4;
        }


        if (movePower3 > 0)
        {
            if (playerMonPsi > oppMonRes * 1.05)
            {
                Power3Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 100.0f));
            }
            else if (playerMonPsi <= oppMonRes * 1.05 && playerMonPsi > oppMonRes * 0.95)
            {
                Power3Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 95.0f));
            }
            else if (playerMonPsi <= oppMonRes * 0.95 && playerMonPsi > oppMonRes * 0.6)
            {
                Power3Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 85.0f));
            }
            else if (playerMonPsi <= oppMonRes * 0.6)
            {
                Power3Ratio = ((playerMonPsi * 100.0f) / (oppMonRes * 70.0f));
            }
            else
            {
                Power3Ratio = 0;
            }
            Power3 = ((((playerLevel / 2) + 4) * movePower3 * Power3Ratio) / 64) + 4;
        }
    }

    HighestRoll = Power1 + Power2 + Power3;
    LowestRoll = HighestRoll * 0.85;

    rand_s((unsigned int*)&Random);
    Random16 = (uint16_t*)Random;

    PotentialDmg = (Random16 % (HighestRoll - LowestRoll + 1)) + LowestRoll;

    if (Random16 % 16 == 0)     //TODO:BATTLE_TEXT_FLAG for crits, element effectiveness, ect..
    {
        PotentialDmg = +(PotentialDmg * 1.5);
    }
    return (PotentialDmg);
}

BOOL ModifyMonsterHealthValueGetKO(uint16_t damageToMonster, BOOL isPlayerSideMonster)
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
                    return(TRUE);
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
                    return(TRUE);
                }
            }
        }
    }
    else if ((gOpponentParty[gCurrentOpponentPartyMember].Health != 0) || (gPlayerParty[gCurrentPartyMember].Health != 0))
    {
        return(TRUE);
    }

    switch (gCurrentBattleState)
    {
        case BATTLESTATE_FIRSTMOVE_CALC:
        {
            gCurrentBattleState = BATTLESTATE_SECONDMOVE_TEXT;
            break;
        }
        case BATTLESTATE_SECONDMOVE_CALC:
        {
            gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
            break;
        }
    }

    return(FALSE);
}


