
#include "Main.h"

#include "BattleScreen.h"

#include "MonsterData.h"





///////// load starting monster

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

//MENUITEM gMI_SwitchScreen_PartySlot0 = { &gPlayer.Party[0].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64, FALSE, MenuItem_SwitchScreen_PartySlot0 };
//MENUITEM gMI_SwitchScreen_PartySlot1 = { &gPlayer.Party[1].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 16, FALSE, MenuItem_SwitchScreen_PartySlot1 };
//MENUITEM gMI_SwitchScreen_PartySlot2 = { &gPlayer.Party[2].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 32, FALSE, MenuItem_SwitchScreen_PartySlot2 };
//MENUITEM gMI_SwitchScreen_PartySlot3 = { &gPlayer.Party[3].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 48, FALSE, MenuItem_SwitchScreen_PartySlot3 };
//MENUITEM gMI_SwitchScreen_PartySlot4 = { &gPlayer.Party[4].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 64, FALSE, MenuItem_SwitchScreen_PartySlot4 };
//MENUITEM gMI_SwitchScreen_PartySlot5 = { &gPlayer.Party[5].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 80, FALSE, MenuItem_SwitchScreen_PartySlot5 };

//MENUITEM gMI_SwitchScreen_BackButton = { "Back", (GAME_RES_WIDTH / 2) - (6 * 4 / 2), 64 + 96, TRUE, MenuItem_SwitchScreen_BackButton };

//MENUITEM* gMI_SwitchScreen_Items[] = { &gMI_SwitchScreen_PartySlot0, &gMI_SwitchScreen_PartySlot1, &gMI_SwitchScreen_PartySlot2, &gMI_SwitchScreen_PartySlot3, &gMI_SwitchScreen_PartySlot4, &gMI_SwitchScreen_PartySlot5 };

//MENU gMenu_SwitchScreen = { "Switch Menu", 0, _countof(gMI_SwitchScreen_Items), gMI_SwitchScreen_Items };

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

BOOL gSkipToNextText;



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
    static int32_t CalculatedExpReward = 0;
    static uint8_t OpponentMove = 0;
    static BOOL IsPlayerMovingFirst = FALSE;
    static BOOL MonsterHasKOed = FALSE;

    GAMEBITMAP* BattleScene = NULL;

    GAMEBITMAP* PlayerMonsterSprite = NULL;

    GAMEBITMAP* OpponentMonsterSprite = NULL;

    uint8_t Opponent = NULL;



    char BattleTextLineScratch[40] = { 0 };

    for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
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
            PlayerMonsterSprite = &gBattleSpriteBack[Counter];
        }
        if (gOpponentParty[0].DriveMonster.Index == Counter)
        {
            OpponentMonsterSprite = &gBattleSpriteFront[Counter];
        }
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawWindow(64, 48, 256, 128, &COLOR_NES_WHITE, NULL, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);

    switch (gCurrentBattleState)
    {
        case BATTLESTATE_OPENING_TEXT:
        {
            if (gRegistryParams.TextSpeed == 4 || gFinishedBattleTextAnimation == TRUE)
            {
                gPreviousBattleState = gCurrentBattleState;
                gCurrentBattleState = BATTLESTATE_OPENING_WAIT;
                gSkipToNextText = FALSE;
            }
            else
            {
                DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

                if ((LocalFrameCounter % (gRegistryParams.TextSpeed + 1) == 0) && (gFinishedBattleTextAnimation == FALSE))
                {
                    if (BattleTextLineCharactersToShow <= strlen(gBattleTextLine[BattleTextRowsToShow + 1]))
                    {
                        BattleTextLineCharactersToShow++;
                        BattleTextLineCharactersWritten++;
                    }
                    else if ((BattleTextLineCharactersToShow > strlen(gBattleTextLine[BattleTextRowsToShow + 1])) && (BattleTextRowsToShow + 1 <= (BattleTextLineCount)))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextRowsToShow++;
                    }
                    else if (BattleTextRowsToShow + 1 > (BattleTextLineCount))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextLineCharactersWritten = 0;
                        BattleTextRowsToShow = 0;
                        gCurrentBattleState = BATTLESTATE_OPENING_WAIT;
                        gFinishedBattleTextAnimation = TRUE;

                        goto WaitOpening;
                    }
                }

                if (!gFinishedBattleTextAnimation)
                {
                    switch (BattleTextRowsToShow)
                    {
                        case 0:
                        {
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[1]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
                            break;
                        }
                        case 1:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[2]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            break;
                        }
                        case 2:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[3]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            break;
                        }
                        case 3:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[4]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            break;
                        }
                        case 4:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[5]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            break;
                        }
                        case 5:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[6]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            break;
                        }
                        case 6:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[7]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
                            break;
                        }
                    }
                }
                
            }

            break;
        }
        case BATTLESTATE_OPENING_WAIT:
        {
    WaitOpening:

            DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);


            BlitStringToBuffer(gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
            if (BattleTextLineCount > 1)
            {
                BlitStringToBuffer(gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
            }
            if (BattleTextLineCount > 2)
            {
                BlitStringToBuffer(gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
            }
            if (BattleTextLineCount > 3)
            {
                BlitStringToBuffer(gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
            }
            if (BattleTextLineCount > 4)
            {
                BlitStringToBuffer(gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
            }
            if (BattleTextLineCount > 5)
            {
                BlitStringToBuffer(gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
            }
            if (BattleTextLineCount > 6)
            {
                BlitStringToBuffer(gBattleTextLine[7], &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
            }
            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, 312, 228);
            gFinishedBattleTextAnimation = TRUE;

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


            if (gRegistryParams.TextSpeed == 4 || gFinishedBattleTextAnimation == TRUE)
            {
                gPreviousBattleState = gCurrentBattleState;
                gCurrentBattleState = BATTLESTATE_FIRSTMOVE_WAIT;
                gSkipToNextText = FALSE;
            }
            else
            {
                DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

                if ((LocalFrameCounter % (gRegistryParams.TextSpeed + 1) == 0) && (gFinishedBattleTextAnimation == FALSE))
                {
                    if (BattleTextLineCharactersToShow <= strlen(gBattleTextLine[BattleTextRowsToShow + 1]))
                    {
                        BattleTextLineCharactersToShow++;
                        BattleTextLineCharactersWritten++;
                    }
                    else if ((BattleTextLineCharactersToShow > strlen(gBattleTextLine[BattleTextRowsToShow + 1])) && (BattleTextRowsToShow + 1 <= (BattleTextLineCount)))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextRowsToShow++;
                    }
                    else if (BattleTextRowsToShow + 1 > (BattleTextLineCount))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextLineCharactersWritten = 0;
                        BattleTextRowsToShow = 0;
                        gCurrentBattleState = BATTLESTATE_FIRSTMOVE_WAIT;
                        gFinishedBattleTextAnimation = TRUE;

                        goto WaitFirstMove;
                    }
                }

                if (!gFinishedBattleTextAnimation)
                {
                    switch (BattleTextRowsToShow)
                    {
                        case 0:
                        {
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[1]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
                            break;
                        }
                        case 1:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[2]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            break;
                        }
                        case 2:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[3]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            break;
                        }
                        case 3:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[4]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            break;
                        }
                        case 4:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[5]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            break;
                        }
                        case 5:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[6]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            break;
                        }
                        case 6:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[7]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
                            break;
                        }
                    }
                }

            }

            break;
        }
        case BATTLESTATE_FIRSTMOVE_WAIT:
        {
        WaitFirstMove:

            DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);


            BlitStringToBuffer(gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
            if (BattleTextLineCount > 1)
            {
                BlitStringToBuffer(gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
            }
            if (BattleTextLineCount > 2)
            {
                BlitStringToBuffer(gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
            }
            if (BattleTextLineCount > 3)
            {
                BlitStringToBuffer(gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
            }
            if (BattleTextLineCount > 4)
            {
                BlitStringToBuffer(gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
            }
            if (BattleTextLineCount > 5)
            {
                BlitStringToBuffer(gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
            }
            if (BattleTextLineCount > 6)
            {
                BlitStringToBuffer(gBattleTextLine[7], &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
            }
            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, 312, 228);
            gFinishedBattleTextAnimation = TRUE;

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


            if (gRegistryParams.TextSpeed == 4 || gFinishedBattleTextAnimation == TRUE)
            {
                gPreviousBattleState = gCurrentBattleState;
                gCurrentBattleState = BATTLESTATE_SECONDMOVE_WAIT;
                gSkipToNextText = FALSE;
            }
            else
            {
                DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

                if ((LocalFrameCounter % (gRegistryParams.TextSpeed + 1) == 0) && (gFinishedBattleTextAnimation == FALSE))
                {
                    if (BattleTextLineCharactersToShow <= strlen(gBattleTextLine[BattleTextRowsToShow + 1]))
                    {
                        BattleTextLineCharactersToShow++;
                        BattleTextLineCharactersWritten++;
                    }
                    else if ((BattleTextLineCharactersToShow > strlen(gBattleTextLine[BattleTextRowsToShow + 1])) && (BattleTextRowsToShow + 1 <= (BattleTextLineCount)))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextRowsToShow++;
                    }
                    else if (BattleTextRowsToShow + 1 > (BattleTextLineCount))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextLineCharactersWritten = 0;
                        BattleTextRowsToShow = 0;
                        gCurrentBattleState = BATTLESTATE_SECONDMOVE_WAIT;
                        gFinishedBattleTextAnimation = TRUE;

                        goto WaitSecondMove;
                    }
                }

                if (!gFinishedBattleTextAnimation)
                {
                    switch (BattleTextRowsToShow)
                    {
                        case 0:
                        {
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[1]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
                            break;
                        }
                        case 1:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[2]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            break;
                        }
                        case 2:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[3]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            break;
                        }
                        case 3:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[4]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            break;
                        }
                        case 4:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[5]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            break;
                        }
                        case 5:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[6]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            break;
                        }
                        case 6:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[7]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
                            break;
                        }
                    }
                }

            }

            break;
        }
        case BATTLESTATE_SECONDMOVE_WAIT:
        {
        WaitSecondMove:

            DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);


            BlitStringToBuffer(gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
            if (BattleTextLineCount > 1)
            {
                BlitStringToBuffer(gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
            }
            if (BattleTextLineCount > 2)
            {
                BlitStringToBuffer(gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
            }
            if (BattleTextLineCount > 3)
            {
                BlitStringToBuffer(gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
            }
            if (BattleTextLineCount > 4)
            {
                BlitStringToBuffer(gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
            }
            if (BattleTextLineCount > 5)
            {
                BlitStringToBuffer(gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
            }
            if (BattleTextLineCount > 6)
            {
                BlitStringToBuffer(gBattleTextLine[7], &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
            }
            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, 312, 228);
            gFinishedBattleTextAnimation = TRUE;

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
            BOOL IsPlayerMonsterKOed = FALSE;

            if (gPlayerParty[gCurrentPartyMember].Health == 0)
            {
                IsPlayerMonsterKOed = TRUE;
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
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s was KO'ed!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname);
                BattleTextLineCount++;
                sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s blacked out!", &gPlayer.Name);
                BattleTextLineCount++;
            }
            else
            {
                sprintf_s((char*)gBattleTextLine[1], sizeof(gBattleTextLine[1]), "%s was KO'ed!", &gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname);
                BattleTextLineCount++;
                sprintf_s((char*)gBattleTextLine[2], sizeof(gBattleTextLine[2]), "%s gained %d Exp!", &gPlayerParty[gCurrentPartyMember].DriveMonster.nickname, CalculatedExpReward);
                BattleTextLineCount++;
            }

            if (gRegistryParams.TextSpeed == 4 || gFinishedBattleTextAnimation == TRUE)
            {
                gPreviousBattleState = gCurrentBattleState;
                gCurrentBattleState = BATTLESTATE_KO_WAIT;
                gSkipToNextText = FALSE;
            }
            else
            {
                DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

                if ((LocalFrameCounter % (gRegistryParams.TextSpeed + 1) == 0) && (gFinishedBattleTextAnimation == FALSE))
                {
                    if (BattleTextLineCharactersToShow <= strlen(gBattleTextLine[BattleTextRowsToShow + 1]))
                    {
                        BattleTextLineCharactersToShow++;
                        BattleTextLineCharactersWritten++;
                    }
                    else if ((BattleTextLineCharactersToShow > strlen(gBattleTextLine[BattleTextRowsToShow + 1])) && (BattleTextRowsToShow + 1 <= (BattleTextLineCount)))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextRowsToShow++;
                    }
                    else if (BattleTextRowsToShow + 1 > (BattleTextLineCount))
                    {
                        BattleTextLineCharactersToShow = 0;
                        BattleTextLineCharactersWritten = 0;
                        BattleTextRowsToShow = 0;
                        gCurrentBattleState = BATTLESTATE_KO_WAIT;
                        gFinishedBattleTextAnimation = TRUE;

                        goto WaitKO;
                    }
                }

                if (!gFinishedBattleTextAnimation)
                {
                    switch (BattleTextRowsToShow)
                    {
                        case 0:
                        {
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[1]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
                            break;
                        }
                        case 1:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[2]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            break;
                        }
                        case 2:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[3]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            break;
                        }
                        case 3:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[4]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            break;
                        }
                        case 4:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[5]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            break;
                        }
                        case 5:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[6]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            break;
                        }
                        case 6:
                        {
                            BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                            BlitStringToBuffer((char*)gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                            snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[7]);
                            BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
                            break;
                        }
                    }
                }

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


                    MonsterTryLearningNewMove(&gPlayerParty[gCurrentPartyMember], TRUE);        ///////////TODO:FIX not working doesnt give moves when leveling
                }
            }

            DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);


            BlitStringToBuffer(gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
            if (BattleTextLineCount > 1)
            {
                BlitStringToBuffer(gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
            }
            if (BattleTextLineCount > 2)
            {
                BlitStringToBuffer(gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
            }
            if (BattleTextLineCount > 3)
            {
                BlitStringToBuffer(gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
            }
            if (BattleTextLineCount > 4)
            {
                BlitStringToBuffer(gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
            }
            if (BattleTextLineCount > 5)
            {
                BlitStringToBuffer(gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
            }
            if (BattleTextLineCount > 6)
            {
                BlitStringToBuffer(gBattleTextLine[7], &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
            }
            BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, 312, 228);
            gFinishedBattleTextAnimation = TRUE;

            break;
        }
        case BATTLESTATE_RUN_FIGHT:
        {
            DrawBattleButtons();
            break;
        }
        case BATTLESTATE_SWITCH_FIGHT:
        {

        }
        case BATTLESTATE_CHOOSE_MOVE:
        {
            DrawMoveButtons();
            break;
        }
        case BATTLESTATE_CHOOSE_MONSTER:
        {

        }
        case BATTLESTATE_CHOOSE_ITEM:
        {

        }
    }


    switch (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][gPlayer.WorldPos.x / 16])
    {
        case TILE_GRASS_01:
        {
            BattleScene = &gBattleScreen_Grass01;
            break;
        }
        case TILE_TALLGRASS_01:
        {
            BattleScene = &gBattleScreen_Grass01;
            break;
        }
        case TILE_STONEBRICKS_FLOOR_01:
        {
            BattleScene = &gBattleScreen_StoneBricks01;
            break;
        }
        default:
        {
            ASSERT(FALSE, "Player encountered a monster on an unknown tile!")
        }
    }

    if (BattleScene != 0)
    {
        Blit32BppBitmapToBuffer(BattleScene, (65), (49), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "BattleScene is NULL!");
    }

    if (PlayerMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(PlayerMonsterSprite, (65), (111), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Player battle monster is NULL!");
    }

    if (OpponentMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(OpponentMonsterSprite, (255), (79), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Opponent battle monster is NULL!");
    }

    /////////////////////////////////////////TEMP HP BARS//////////////////////////////////////////////////////

    /*uint16_t HpBarSize = snprintf(NULL, 0, "%d", gPlayerParty[gCurrentPartyMember].Health);
    char* HpBarString = malloc(HpBarSize + 1);
    snprintf(HpBarString, HpBarSize + 1, "%d", gPlayerParty[gCurrentPartyMember].Health);
    BlitStringToBuffer(HpBarString, &g6x7Font, &COLOR_DARK_RED, 65, 111);

    HpBarSize = snprintf(NULL, 0, "%d", gOpponentParty[gCurrentOpponentPartyMember].Health);
    HpBarString = malloc(HpBarSize + 1);
    snprintf(HpBarString, HpBarSize + 1, "%d", gOpponentParty[gCurrentOpponentPartyMember].Health);
    BlitStringToBuffer(HpBarString, &g6x7Font, &COLOR_DARK_RED, 255, 79);*/

    uint16_t HpPercent = 100 - ((gPlayerParty[gCurrentPartyMember].Health * 100) / (gPlayerParty[gCurrentPartyMember].MaxHealth));

    uint16_t ExpPercent = 100 - (((gPlayerParty[gCurrentPartyMember].DriveMonster.Experience - gExperienceTables[gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].growthRate][gPlayerParty[gCurrentPartyMember].Level]) * 100) / (gExperienceTables[gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].growthRate][gPlayerParty[gCurrentPartyMember].Level + 1] - gExperienceTables[gBaseStats[gPlayerParty[gCurrentPartyMember].DriveMonster.Index].growthRate][gPlayerParty[gCurrentPartyMember].Level]));

    DrawMonsterHpBar(65 + 1, 111 - 8, HpPercent, ExpPercent, gPlayerParty[gCurrentPartyMember].Level, gPlayerParty[gCurrentPartyMember].DriveMonster.nickname, TRUE);

    HpPercent = 100 - ((gOpponentParty[gCurrentOpponentPartyMember].Health * 100) / (gOpponentParty[gCurrentOpponentPartyMember].MaxHealth));

    DrawMonsterHpBar(255 - 46, 79 - 8, HpPercent, 0, gOpponentParty[gCurrentOpponentPartyMember].Level, gOpponentParty[gCurrentOpponentPartyMember].DriveMonster.nickname, FALSE);

    //////////////////////////////////////////////TODO///REMOVE////////////////////////////////////////////////

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
                gFinishedBattleTextAnimation = FALSE;
                MenuItem_BattleScreen_EscapeButton();
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

        }
        case BATTLESTATE_CHOOSE_ITEM:
        {

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

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gMI_MoveScreen_Items[gMenu_MoveScreen.SelectedItem]->x - 6, gMI_MoveScreen_Items[gMenu_MoveScreen.SelectedItem]->y);
}

void DrawMonsterHpBar(uint16_t x, uint16_t y, uint8_t percentHp100, uint8_t percentExp100, uint8_t monsterLevel, char* monsterNickname, BOOL showExpBar)
{
    DrawWindow(x - 3, y - 10, 100 + 16, 2 + 13, &COLOR_BLACK, &COLOR_DARK_WHITE, NULL, WINDOW_FLAG_ROUNDED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    int32_t StartingScreenPixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * y) + x;

    for (int Row = 0; Row < 2; Row++)
    {

        for (int Pixel = 0; Pixel < 100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row);
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_DARK_GREEN, sizeof(PIXEL32));
        }

        for (int Pixel = 0; Pixel < 100 - percentHp100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row);
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_LIGHT_GREEN, sizeof(PIXEL32));
        }
    }
    if (showExpBar)
    {
        for (int Pixel = 0; Pixel < 100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * 3) + 8;
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_DARK_BLUE, sizeof(PIXEL32));
        }

        for (int Pixel = 0; Pixel < 100 - percentExp100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * 3) + 8;
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_NEON_BLUE, sizeof(PIXEL32));
        }
    }

    uint16_t LevelSize = snprintf(NULL, 0, "%d", monsterLevel);
    char* LevelString = malloc(LevelSize + 6);
    snprintf(LevelString, LevelSize + 6, "Lvl: %d", monsterLevel);
    BlitStringToBuffer(LevelString, &g6x7Font, &COLOR_DARK_RED, x - 1, y - 8);

    BlitStringToBuffer(monsterNickname, &g6x7Font, &COLOR_DARK_RED, x + 11 + 30, y - 8);
}

//////// initial choice menu

void MenuItem_BattleScreen_FightButton(void)
{
    gPreviousBattleState = gCurrentBattleState;
    gCurrentBattleState = BATTLESTATE_CHOOSE_MOVE;
}

void MenuItem_BattleScreen_EscapeButton(void)
{
    for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
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

}

void MenuItem_BattleScreen_ItemsButton(void)
{

}

/////// switching monsters screen

void MenuItem_SwitchScreen_PartySlot0(void)
{

}

void MenuItem_SwitchScreen_PartySlot1(void)
{

}

void MenuItem_SwitchScreen_PartySlot2(void)
{

}

void MenuItem_SwitchScreen_PartySlot3(void)
{

}

void MenuItem_SwitchScreen_PartySlot4(void)
{

}

void MenuItem_SwitchScreen_PartySlot5(void)
{

}

void MenuItem_SwitchScreen_BackButton(void)
{

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
    //end player turn start calculating
}

void MenuItem_MoveScreen_MoveSlot3(void)
{
    //end player turn start calculating
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
        PotentialDmg =+  (PotentialDmg * 1.5);
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


