
#include "Main.h"

#include "BattleScreen.h"


///////// load starting monster

struct Monster gCurrentPartyMember = { 0 };      //TODO find a way to make a variable for "in" monster and not just indexing

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

MENUITEM gMI_MoveScreen_MoveSlot0 = { "Slot1", 72, 187, TRUE, MenuItem_MoveScreen_MoveSlot0 };

MENUITEM gMI_MoveScreen_MoveSlot1 = { "Slot2", 72, 201, TRUE, MenuItem_MoveScreen_MoveSlot1};

MENUITEM gMI_MoveScreen_MoveSlot2 = { "Slot3", 72, 215, TRUE, MenuItem_MoveScreen_MoveSlot2};

MENUITEM gMI_MoveScreen_MoveSlot3 = { "Slot4", 72, 229, TRUE, MenuItem_MoveScreen_MoveSlot3};

MENUITEM gMI_MoveScreen_MoveSlotSignature = { "Signature", 316 , 206, TRUE, MenuItem_MoveScreen_SignatureMove};

MENUITEM gMI_MoveScreen_BackButton = { "Back", 1 + (6 * 4 / 2), 206, TRUE, MenuItem_MoveScreen_BackButton };

MENUITEM* gMI_MoveScreen_Items[] = { &gMI_MoveScreen_BackButton, &gMI_MoveScreen_MoveSlot0, &gMI_MoveScreen_MoveSlot1, &gMI_MoveScreen_MoveSlot2, &gMI_MoveScreen_MoveSlot3, &gMI_MoveScreen_MoveSlotSignature };

MENU gMenu_MoveScreen = { "Move Menu", 0, _countof(gMI_MoveScreen_Items), gMI_MoveScreen_Items };

//////////

char gBattleTextLine1[40];      //first line of dialogue in combat text

BOOL gSkipToNextText;



void DrawBattleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    static uint16_t BattleTextLine1CharactersToShow = 0;

    GAMEBITMAP* BattleScene = NULL;

    GAMEBITMAP* PlayerMonsterSprite = NULL;

    GAMEBITMAP* OpponentMonsterSprite = NULL;

    uint8_t Opponent = NULL;

    char BattleTextLine1Scratch[40];

    for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
    {
        if ((gCharacterSprite[Index].InteractedWith == TRUE) && (gCharacterSprite[Index].WantsToBattle == TRUE))
        {
            Opponent = Index;
            break;
        }
    }

    if ((gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        LocalFrameCounter = 0;
        gCurrentPartyMember = gPlayerParty[0];
        gCurrentBattleState = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
        BattleTextLine1CharactersToShow = 0;
        if (Opponent == NULL)
        {
            sprintf_s(gBattleTextLine1, sizeof(gBattleTextLine1), "%s encountered a wild NULL!", gPlayer.Name);
        }
        else
        {
            sprintf_s(gBattleTextLine1, sizeof(gBattleTextLine1), "%s was challenged by %s", gPlayer.Name, gCharacterSprite[Opponent].Name);
        }
    }


    //////TODO: Add battle intro and battle music/////////////
    //if (LocalFrameCounter == 0)
    //{
    //    StopGameMusic();
    //    /*PlayGameMusic(&MusicBattleInto01, FALSE, TRUE);
    //    PlayGameMusic(&MusicBattle01, TRUE, FALSE);*/       ////queue full loop behind intro
    //}

    for (uint8_t Counter = 0; Counter < TOTAL_MONSTERS; Counter++)
    {
        if (gPlayerParty[0].PcMonster.secure.substructs[0].type0.Index == Counter)
        {
            PlayerMonsterSprite = &gBattleSpriteBack[Counter];
        }

        //if (Opponent != NULL)         //temp removed so game doesnt assert on wild encounters
        {
            if (gOpponentParty[0].PcMonster.secure.substructs[0].type0.Index == Counter)
            {
                OpponentMonsterSprite = &gBattleSpriteFront[Counter];
            }
        }
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawWindow(64, 48, 256, 128, &COLOR_NES_WHITE, NULL, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);

    switch (gCurrentBattleState)
    {
        case BATTLESTATE_OPENING_TEXT:          //TODO: impliment longer strings with token finder
        {
            if (gRegistryParams.TextSpeed == 4)
            {
                gPreviousBattleState = gCurrentBattleState;
                gCurrentBattleState = BATTLESTATE_WAIT_INPUT1;
                gSkipToNextText = FALSE;
            }
            else
            {
                DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

                if (LocalFrameCounter % (gRegistryParams.TextSpeed + 1) == 0)
                {
                    if (BattleTextLine1CharactersToShow <= strlen(gBattleTextLine1))
                    {
                        BattleTextLine1CharactersToShow++;
                    }
                    else if (BattleTextLine1CharactersToShow > strlen(gBattleTextLine1))
                    {
                        gPreviousBattleState = gCurrentBattleState;
                        gCurrentBattleState = BATTLESTATE_WAIT_INPUT1;
                    }
                }

                snprintf(BattleTextLine1Scratch, BattleTextLine1CharactersToShow, "%s", gBattleTextLine1);

                BlitStringToBuffer(BattleTextLine1Scratch, &g6x7Font, &COLOR_BLACK, 66, 182);
            }

            break;
        }
        case BATTLESTATE_WAIT_INPUT1:       //TODO: impliment longer strings with a token finder
        {
            DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

            BlitStringToBuffer(gBattleTextLine1, &g6x7Font, &COLOR_BLACK, 66, 182);

            BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, 312, 228);

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
        ASSERT(FALSE, "Battle monster is NULL!");
    }

    if (OpponentMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(OpponentMonsterSprite, (255), (79), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Battle monster is NULL!");
    }

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
                gPreviousBattleState = gCurrentBattleState;
                gCurrentBattleState = BATTLESTATE_WAIT_INPUT1;
            }
            break;
        }
        case BATTLESTATE_WAIT_INPUT1:
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                gPreviousBattleState = gCurrentBattleState;
                gCurrentBattleState = BATTLESTATE_RUN_FIGHT;
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
        if ((gCharacterSprite[Index].InteractedWith == TRUE) && (gCharacterSprite[Index].WantsToBattle == TRUE))
        {
            gCharacterSprite[Index].InteractedWith = FALSE;
            gCharacterSprite[Index].WantsToBattle = FALSE;
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
    //end player turn start calculating
}

void MenuItem_MoveScreen_MoveSlot1(void)
{
    //end player turn start calculating
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