
#include "Main.h"

#include "BattleScreen.h"


///////// load starting monster

UNIQUEMONSTER gCurrentPartyMember = { 0 };      //TODO find a way to make a variable for "in" monster and not just indexing

////////Initial starting choices at the start of battle

MENUITEM gMI_BattleScreen_FightButton = { "Fight!", (GAME_RES_WIDTH / 2) - (6 * 7 / 2) + 2, 200, TRUE, MenuItem_BattleScreen_FightButton };

MENUITEM gMI_BattleScreen_EscapeButton = { "Escape", (GAME_RES_WIDTH / 2) - (6 * 7 / 2) + 2, 226, TRUE, MenuItem_BattleScreen_EscapeButton };

MENUITEM gMI_BattleScreen_SwitchButton = { "Switch", 64 + 12, 210, TRUE, MenuItem_BattleScreen_SwitchButton };

MENUITEM gMI_BattleScreen_ItemsButton = { "Items", 256 + 14, 210, TRUE, MenuItem_BattleScreen_ItemsButton };

MENUITEM* gMI_BattleScreen_Items[] = { &gMI_BattleScreen_FightButton, &gMI_BattleScreen_EscapeButton, &gMI_BattleScreen_SwitchButton, &gMI_BattleScreen_ItemsButton };

MENU gMenu_BattleScreen = { "Battle Menu", 0 , _countof(gMI_BattleScreen_Items), gMI_BattleScreen_Items };

////////Menu choices for switching monsters in battle

MENUITEM gMI_SwitchScreen_PartySlot0 = { &gPlayer.Party[0].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64, FALSE, MenuItem_SwitchScreen_PartySlot0 };
MENUITEM gMI_SwitchScreen_PartySlot1 = { &gPlayer.Party[1].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 16, FALSE, MenuItem_SwitchScreen_PartySlot1 };
MENUITEM gMI_SwitchScreen_PartySlot2 = { &gPlayer.Party[2].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 32, FALSE, MenuItem_SwitchScreen_PartySlot2 };
MENUITEM gMI_SwitchScreen_PartySlot3 = { &gPlayer.Party[3].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 48, FALSE, MenuItem_SwitchScreen_PartySlot3 };
MENUITEM gMI_SwitchScreen_PartySlot4 = { &gPlayer.Party[4].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 64, FALSE, MenuItem_SwitchScreen_PartySlot4 };
MENUITEM gMI_SwitchScreen_PartySlot5 = { &gPlayer.Party[5].MonsterBaseInfo.Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MONSTER_NAME_LENGTH) / 2), 64 + 80, FALSE, MenuItem_SwitchScreen_PartySlot5 };

MENUITEM gMI_SwitchScreen_BackButton = { "Back", (GAME_RES_WIDTH / 2) - (6 * 4 / 2), 64 + 96, TRUE, MenuItem_SwitchScreen_BackButton };

MENUITEM* gMI_SwitchScreen_Items[] = { &gMI_SwitchScreen_PartySlot0, &gMI_SwitchScreen_PartySlot1, &gMI_SwitchScreen_PartySlot2, &gMI_SwitchScreen_PartySlot3, &gMI_SwitchScreen_PartySlot4, &gMI_SwitchScreen_PartySlot5 };

MENU gMenu_SwitchScreen = { "Switch Menu", 0, _countof(gMI_SwitchScreen_Items), gMI_SwitchScreen_Items };

//////////Menu choices for selecting moves

MENUITEM gMI_MoveScreen_MoveSlot0 = { &gCurrentPartyMember.LearnedMoveSlot[0].Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MOVE_NAME_LENGTH) / 2), 187, TRUE, MenuItem_MoveScreen_MoveSlot0};

MENUITEM gMI_MoveScreen_MoveSlot1 = { &gCurrentPartyMember.LearnedMoveSlot[1].Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MOVE_NAME_LENGTH) / 2), 201, TRUE, MenuItem_MoveScreen_MoveSlot1 };

MENUITEM gMI_MoveScreen_MoveSlot2 = { &gCurrentPartyMember.LearnedMoveSlot[2].Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MOVE_NAME_LENGTH) / 2), 215, TRUE, MenuItem_MoveScreen_MoveSlot2 };

MENUITEM gMI_MoveScreen_MoveSlot3 = { &gCurrentPartyMember.LearnedMoveSlot[3].Name, (GAME_RES_WIDTH / 2) - (6 * (MAX_MOVE_NAME_LENGTH) / 2), 229, FALSE, MenuItem_MoveScreen_MoveSlot3 };

MENUITEM gMI_MoveScreen_MoveSlotSignature = { &gCurrentPartyMember.SignatureMove.Name, 276 + (6 * (MAX_MOVE_NAME_LENGTH) / 2), 206, FALSE, MenuItem_MoveScreen_SignatureMove };

MENUITEM gMI_MoveScreen_BackButton = { "Back", 52 + (6 * 4 / 2), 206, TRUE, MenuItem_MoveScreen_BackButton };

MENUITEM* gMI_MoveScreen_Items[] = { &gMI_MoveScreen_BackButton, &gMI_MoveScreen_MoveSlot0, &gMI_MoveScreen_MoveSlot1, &gMI_MoveScreen_MoveSlot2, &gMI_MoveScreen_MoveSlot3, &gMI_MoveScreen_MoveSlotSignature };

MENU gMenu_MoveScreen = { "Move Menu", 0, _countof(gMI_MoveScreen_Items), gMI_MoveScreen_Items };

//////////





void DrawBattleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    GAMEBITMAP* BattleScene = NULL;

    GAMEBITMAP* PlayerMonsterSprite = NULL;

    GAMEBITMAP* OpponentMonsterSprite = NULL;

    uint8_t Opponent = 0;

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
        gCurrentPartyMember = gPlayer.Party[0];
        gCurrentBattleState = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
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
        if (gPlayer.Party[0].MonsterBaseInfo.MonsterIndex == Counter)
        {
            PlayerMonsterSprite = &gBattleSpriteBack[Counter];
        }

        if (gCharacterSprite[Opponent].Party[0].MonsterBaseInfo.MonsterIndex == Counter)
        {
            //////////TODO obviously make front facing sprites
            OpponentMonsterSprite = &gBattleSpriteFront[Counter];
        }
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);

    DrawWindow(1, 1, 256, 128, &COLOR_NES_WHITE, NULL, &COLOR_NES_GRAY, WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_VERT_CENTERED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_SHADOWED);

    switch (gCurrentBattleState)
    {
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
            /////////TODO make battlescene floor higher for sprites to stand on
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
        Blit32BppBitmapToBuffer(BattleScene, (65), (57), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "BattleScene is NULL!");
    }

    if (PlayerMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(PlayerMonsterSprite, (65), (119), BrightnessAdjustment);
    }
    else
    {
        ASSERT(FALSE, "Battle monster is NULL!");
    }

    if (OpponentMonsterSprite != 0)
    {
        Blit32BppBitmapToBuffer(OpponentMonsterSprite, (255), (87), BrightnessAdjustment);
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
            if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
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
    DrawWindow(1, 190, 64, 28, &COLOR_BLACK, &COLOR_DARK_RED, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    DrawWindow(1, 222, 64, 16, &COLOR_BLACK, &COLOR_LIGHT_BLUE, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    DrawWindow(64, 190, 64, 48, &COLOR_BLACK, &COLOR_FORREST_GREEN, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    DrawWindow(256, 190, 64, 48, &COLOR_BLACK, &COLOR_NES_TAN, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

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
    DrawWindow(1, 185, 190, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_ROUNDED);

    DrawWindow(1, 187 + 12, 190, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_ROUNDED);
                            
    DrawWindow(1, 189 + 24, 190, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_ROUNDED);
                            
    DrawWindow(1, 191 + 36, 190, 11, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_ROUNDED);
                            
    DrawWindow(48, 186, 48, 51, &COLOR_BLACK, &COLOR_DARK_RED, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);
                            
    DrawWindow(288, 186, 48, 51, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_NES_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_ROUNDED);

    for (uint8_t Counter = 0; Counter < _countof(gMI_BattleScreen_Items); Counter++)
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

}