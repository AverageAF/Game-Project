#include "Main.h"

#include "OverWorld.h"

#include "MonsterData.h"

#include "Inventory.h"
#include "InventoryItems.h"


///////////////////////////////////////////////////////////
                //buy sell back buttons
///////////////////////////////////////////////////////////

MENUITEM gBuyButton = { "BUY", 90, 104, TRUE, 1 };

MENUITEM gSellButton = { "SELL", 90, 124, TRUE, 2 };

MENUITEM gBackButton = { "BACK", 90, 144, TRUE, 3 };

MENUITEM* gBuySellBackItems[] = { &gBuyButton, &gSellButton, &gBackButton };

MENU gButtonMenu = { "Selected button", 0, _countof(gBuySellBackItems), gBuySellBackItems };

///////////////////////////////////////////////////////////

BOOL gFade = FALSE;
BOOL gPostDialogueMenu = FALSE;
BOOL gScriptActive = FALSE;

//TOREMOVE: make the script a hold multiple rows of arrays to store information like the sprite to take the action
//uint8_t gScriptSprite = 3;
//SCENE_SCRIPT gMovementScriptArray[MAX_MOVEMENT_SCRIPTS] = { START_OF_SCRIPT, WALK_LEFT, WALK_LEFT, WALK_LEFT, WALK_LEFT, FACE_UP, DELAY_64, WALK_LEFT, WALK_LEFT, WALK_LEFT, DIALOGUE_TRIGGER, WALK_RIGHT, WALK_RIGHT, WALK_RIGHT, WALK_RIGHT, WALK_RIGHT, WALK_RIGHT, END_OF_SCRIPT };

SCENE_SCRIPT gSceneScriptArray[MAX_MOVEMENT_SCRIPTS] = { {.Script = START_OF_SCRIPT, .Actor = 0}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = FACE_UP, .Actor = 3}, {.Script = DELAY_64, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = WALK_LEFT, .Actor = 3}, {.Script = DIALOGUE_TRIGGER, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_DOWN, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = WALK_RIGHT, .Actor = 3}, {.Script = FACE_DOWN, .Actor = 3}, {.Script = END_OF_SCRIPT, .Actor = 3}};
//

//used to check the current queued script
uint8_t gCurrentScript = 0;

void DrawOverworldScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen = 1;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    static int16_t BrightnessAdjustment = -255;

    if ((gFade == TRUE) || (gGamePerformanceData.TotalFramesRendered > (LastFrameSeen + 1)))
    {
        gFade = FALSE;
        LocalFrameCounter = 0;
        BrightnessAdjustment = -255;
        gInputEnabled = FALSE;
        gOverWorldControls = TRUE;
    }

    if (gPlayerParty[0].DriveMonster.hasIndex == TRUE && gPlayerParty[0].Health == 0 && gPlayerParty[1].Health == 0 && gPlayerParty[2].Health == 0 && gPlayerParty[3].Health == 0 && gPlayerParty[4].Health == 0 && gPlayerParty[5].Health == 0)
    {
        TeleportPlayerBlackOut();
    }

    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &TextColor, &BrightnessAdjustment);

    if (LocalFrameCounter == 30)
    {
        if (MusicIsPlaying() == FALSE)
        {
            PlayGameMusic(gCurrentArea.Music, TRUE, TRUE);
        }
    }

    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);


    if (gGamePaused == FALSE)
    {
        TriggerNPCMovement(LocalFrameCounter, MOVEMENT_SPIN);
    }

    ModifyCharVisibility();

    CharSpriteDrawHandler(BrightnessAdjustment);

    Blit32BppBitmapToBuffer(&gPlayer.Sprite[gPlayer.CurrentSuit][gPlayer.SpriteIndex + gPlayer.Direction], gPlayer.ScreenPos.x, gPlayer.ScreenPos.y, BrightnessAdjustment);


    //TODO:
    InitiateDialogueAndCutscene(LocalFrameCounter);
    //if (InitiateDialogueAndCutscene(LocalFrameCounter) == TRUE)
    //{
    //
    //  ....
    // 
    //}


    if (gGamePerformanceData.DisplayDebugInfo)
    {
        DisplayDebugTiles();
    }

    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}


void PPI_Overworld(void)
{
    if (gOverWorldControls == TRUE)
    {
        if (gGameInput.EscapeKeyPressed && gGameInput.EscapeKeyAlreadyPressed)
        {
            gPreviousGameState = gCurrentGameState;
            gCurrentGameState = GAMESTATE_TITLESCREEN;
            PlayGameSound(&gSoundMenuChoose);
            PauseGameMusic();
        }

        if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
        {
            gPreviousGameState = gCurrentGameState;
            gCurrentGameState = GAMESTATE_INVENTORYSCREEN;
        }

        double SprintEncounter = 1;
        double ItemEncounter = 1;
        if (gPlayer.SprintingShoes == TRUE && !gGameInput.CtrlKeyPressed)
        {
            gPlayer.Sprinting = TRUE;
            SprintEncounter = 1.5;
        }
        else if (gPlayer.SprintingShoes == FALSE && !gGameInput.CtrlKeyPressed)
        {
            gPlayer.Sprinting = FALSE;
            SprintEncounter = 1;
        }
        else if (gPlayer.SprintingShoes == TRUE && gGameInput.CtrlKeyPressed)
        {
            gPlayer.Sprinting = FALSE;
            SprintEncounter = 1;
        }
        else if (gPlayer.SprintingShoes == FALSE && gGameInput.CtrlKeyPressed)
        {
            gPlayer.Sprinting = TRUE;
            SprintEncounter = 1.5;
        }

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            TurnNPCToPlayerStartDialogue();

            if (InteractWithTrigger() == TRUE)
            {
                TriggerInteractionHandler();
            }
        }

        if (!gPlayer.MovementRemaining)
        {
            HandlePlayerCollision();
        }
        else if ((gGamePerformanceData.TotalFramesRendered % 3 >= 1) || (gPlayer.Sprinting == TRUE))    ////slows movement down by ~1/3 of framerate unless sprinting       ( gGamePerformanceData.TotalFramesRendered % 2 == 0) would make walking 50% sprint speed, but I feel like 66% is more fun for the player
        {
            gPlayer.MovementRemaining--;

            HandleCameraPlayerMovementAndCharVisibility();

            for (uint16_t EncounterArea = 1; EncounterArea < NUM_ENCOUNTER_AREAS; EncounterArea++)
            {
                if ((gPlayer.WorldPos.x >= gEncounterAreas[EncounterArea].Area.left && gPlayer.WorldPos.x <= gEncounterAreas[EncounterArea].Area.right) && (gPlayer.WorldPos.y >= gEncounterAreas[EncounterArea].Area.top && gPlayer.WorldPos.y <= gEncounterAreas[EncounterArea].Area.bottom))
                {
                    gCurrentEncounterArea = gEncounterAreas[EncounterArea];
                    gPlayer.RandomEncounterPercent = gCurrentEncounterArea.EncounterRate * SprintEncounter * ItemEncounter;
                    break;
                }
            }

            switch (gPlayer.MovementRemaining)
            {
                case 15:
                {
                    gPlayer.HasMovedSincePort = TRUE;
                    gPlayer.SpriteIndex = 1;
                    break;
                }
                case 11:
                {
                    gPlayer.SpriteIndex = 1;
                    break;
                }
                case 7:
                {
                    gPlayer.SpriteIndex = 2;
                    break;
                }
                case 3:
                {
                    gPlayer.SpriteIndex = 2;
                    break;
                }
                case 0:
                {
                    gPlayer.SpriteIndex = 0;
                    
                    ///TODO function for all "landing on a tile" conditions

                    HandleTileFunctions();

                    break;
                }
            }
        }
    }
    else if (gOverWorldControls == FALSE)   //movement and camerapan ect..
    {
        if (gDialogueControls == TRUE)  //typewriter effect has finished and arrow in bottom corner has appeared
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
            {
                HandleNPCEvent();

                if (gScriptActive)
                {
                    if (gCharacterSprite[gSceneScriptArray[gCurrentScript].Actor].DialoguesBeforeLoop <= gCharacterSprite[gSceneScriptArray[gCurrentScript].Actor].DialogueFlag)
                    {
                        gCharacterSprite[gSceneScriptArray[gCurrentScript].Actor].DialogueFlag = gCharacterSprite[gSceneScriptArray[gCurrentScript].Actor].DialogueLoopReturn;
                        gCurrentScript++;
                        gDialogueControls = FALSE;
                    }
                    else
                    {
                        gCharacterSprite[gSceneScriptArray[gCurrentScript].Actor].DialogueFlag++;
                        gDialogueControls = FALSE;
                    }
                }

                gFinishedDialogueTextAnimation = FALSE;
            }
            else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
            {
                uint8_t StoreButtons = PPI_BuySellBackBox();
            }
            else if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
            {
                uint8_t StoreButtons = PPI_BuySellBackBox();
            }

        }
        else        //no dialogue controls, no overworld controls
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && !gScriptActive)         //allows for skipping typewriter effect to instant dialogue blit
            {
                gFinishedDialogueTextAnimation = TRUE;
            }
            
            if (gScriptActive)
            {
                BOOL ScriptActive = ApplyMovementScriptSprite(gSceneScriptArray, gPlayer.WorldPos.x + 128, gPlayer.WorldPos.y, FALSE);

                if (!ScriptActive)
                {
                    //turn off cutscene
                    gScriptActive = FALSE;
                    ExitDialogue();
                }
            }
        }
    }
}                                                   


void TeleportHandler(void)
{
    BOOL PortalFound = FALSE;

    for (uint16_t Counter = 0; Counter < _countof(gPortCoords); Counter++)
    {
        if (gPlayer.WorldPos.x == gPortCoords[Counter].WorldPos.x && gPlayer.WorldPos.y == gPortCoords[Counter].WorldPos.y)
        {
            StopGameMusic();
            PortalFound = TRUE;
            gPlayer.HasMovedSincePort = FALSE;
            gFade = TRUE;
            gPlayer.WorldPos = gPortCoords[gPortCoords[Counter].PortDestIndex].WorldPos;
            gCamera = gPortCoords[Counter].CameraDest;
            //gPlayer.WorldPos.x += gPortCoords[Counter].WorldPosAdd.x;
            //gPlayer.WorldPos.y += gPortCoords[Counter].WorldPosAdd.y;
            //gCamera.x += gPortCoords[Counter].CameraPosAdd.x;
            //gCamera.y += gPortCoords[Counter].CameraPosAdd.y;
            gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
            gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
            gCurrentArea = gPortCoords[Counter].AreaDest;

            LoadUnloadSpritesVIAGameArea();
            LoadUnloadTriggerVIAGameArea();

            break;
        }
    }
    if (PortalFound == FALSE)
    {
        ASSERT(FALSE, "Player is standing on a portal with no portal handler!")
    }
}


void RandomMonsterEncounter(_In_ GAMESTATE* PreviousGameState, _Inout_ GAMESTATE* CurrentGameState)
{
    PreviousGameState = CurrentGameState;
    *CurrentGameState = GAMESTATE_BATTLE_MONSTER;
}

void TrainerEncounter(_In_ GAMESTATE* PreviousGameState, _Inout_ GAMESTATE* CurrentGameState)
{
    PreviousGameState = CurrentGameState;
    *CurrentGameState = GAMESTATE_BATTLE_TRAINER;
}

//TODO: better movement please!
void TriggerNPCMovement(_In_ uint64_t Counter)
{
    for (uint8_t Index = 0; Index <= NUM_CHAR_SPRITES; Index++)
    {
        if (gCharacterSprite[Index].Loaded == TRUE)
        {
            switch (gCharacterSprite[Index].Movement)
            {
                case (MOVEMENT_STILL):
                case (MOVEMENT_ITEMPICKUP):
                {
                    break;
                }
                case (MOVEMENT_SPIN):
                {
                    if ((Counter % 30 == 0) && (gCharacterSprite[Index].Visible == TRUE))
                    {
                        if (gCharacterSprite[Index].Direction >= 9)
                        {
                            gCharacterSprite[Index].Direction = 0;
                        }
                        else
                        {
                            gCharacterSprite[Index].Direction += 3;
                        }
                    }
                    break;
                }
                case (MOVEMENT_LOOK_AROUND):
                {
                    if ((Counter % 90 == 0) && (gCharacterSprite[Index].Visible == TRUE))
                    {
                        DWORD Random = 0;

                        rand_s((unsigned int*)&Random);
                        if (Random % 2 == 0)
                        {
                            Random = (Random % 4) * 3;

                            gCharacterSprite[Index].Direction = Random;
                        }
                    }
                    break;
                }
                case (MOVEMENT_WALK_UP_DOWN):
                {
                    if (gCharacterSprite[Index].WorldPos.y == gCharacterSprite[Index].ResetWorldPos.y)
                    {
                        gCharacterSprite[Index].Direction = UP;
                    }
                    else if (gCharacterSprite[Index].WorldPos.y == gCharacterSprite[Index].ResetWorldPos.y - (gCharacterSprite[Index].MovementRange.y * 16))
                    {
                        gCharacterSprite[Index].Direction = DOWN;
                    }

                    if (gCharacterSprite[Index].Direction == UP && gCharacterSprite[Index].MovementRemaining != 0)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                        {
                            gCharacterSprite[Index].Direction = DOWN;
                        }
                        else if (Counter % 2 == 0)
                        {
                            gCharacterSprite[Index].WorldPos.y--;
                            gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                            gCharacterSprite[Index].MovementRemaining--;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == UP && !gCharacterSprite[Index].MovementRemaining)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y + 15) || (gCharacterSprite[Index].WorldPos.y < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                        {
                            //if (gCharacterSprite[Index].WorldPos.x == gCharacterSprite[Index].ResetWorldPos.x - (gCharacterSprite[Index].MovementRange.x * 16))
                            {
                                gCharacterSprite[Index].Direction = DOWN;
                            }
                        }
                        else
                        {
                            gCharacterSprite[Index].MovementRemaining = 16;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == DOWN && gCharacterSprite[Index].MovementRemaining != 0)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y) || (gCharacterSprite[Index].WorldPos.y + 16 < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                        {
                            //if (gCharacterSprite[Index].ScreenPos.x == gCharacterSprite[Index].ResetScreenPos.x)
                            {
                                gCharacterSprite[Index].Direction = UP;
                            }
                        }
                        else if (Counter % 2 == 0)
                        {
                            gCharacterSprite[Index].WorldPos.y++;
                            gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                            gCharacterSprite[Index].MovementRemaining--;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == DOWN && !gCharacterSprite[Index].MovementRemaining)
                    {
                        //////      (not     (below the bottom of player 16x16)               or        (above the full player 16x16) )                     and (not            (to the right of 16x16)                            or                   (left of 16x16) )
                        if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y + 15) || (gCharacterSprite[Index].WorldPos.y < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                        {
                            gCharacterSprite[Index].Direction = UP;
                        }
                        else
                        {
                            gCharacterSprite[Index].MovementRemaining = 16;
                        }
                    }


                    if (!(((gCharacterSprite[Index].ScreenPos.x >= 0) && (gCharacterSprite[Index].ScreenPos.x < GAME_RES_WIDTH - 16)) && ((gCharacterSprite[Index].ScreenPos.y >= 0) && (gCharacterSprite[Index].ScreenPos.y < GAME_RES_HEIGHT - 16))))
                    {
                        gCharacterSprite[Index].Visible = FALSE;
                    }
                    else
                    {
                        gCharacterSprite[Index].Visible = TRUE;
                    }
                    break;
                }
                case (MOVEMENT_WALK_LEFT_RIGHT):
                {
                    if (gCharacterSprite[Index].WorldPos.x == gCharacterSprite[Index].ResetWorldPos.x)
                    {
                        gCharacterSprite[Index].Direction = LEFT;
                    }
                    else if (gCharacterSprite[Index].WorldPos.x == gCharacterSprite[Index].ResetWorldPos.x - (gCharacterSprite[Index].MovementRange.x * 16))
                    {
                        gCharacterSprite[Index].Direction = RIGHT;
                    }

                    if (gCharacterSprite[Index].Direction == LEFT && gCharacterSprite[Index].MovementRemaining != 0)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                        {
                            gCharacterSprite[Index].Direction = RIGHT;
                        }
                        else if (Counter % 2 == 0)
                        {
                            gCharacterSprite[Index].WorldPos.x--;
                            gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                            gCharacterSprite[Index].MovementRemaining--;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == LEFT && !gCharacterSprite[Index].MovementRemaining)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x + 15) || (gCharacterSprite[Index].WorldPos.x < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                        {
                            //if (gCharacterSprite[Index].WorldPos.x == gCharacterSprite[Index].ResetWorldPos.x - (gCharacterSprite[Index].MovementRange.x * 16))
                            {
                                gCharacterSprite[Index].Direction = RIGHT;
                            }
                        }
                        else
                        {
                            gCharacterSprite[Index].MovementRemaining = 16;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == RIGHT && gCharacterSprite[Index].MovementRemaining != 0)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x) || (gCharacterSprite[Index].WorldPos.x + 16 < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                        {
                            //if (gCharacterSprite[Index].ScreenPos.x == gCharacterSprite[Index].ResetScreenPos.x)
                            {
                                gCharacterSprite[Index].Direction = LEFT;
                            }
                        }
                        else if (Counter % 2 == 0)
                        {
                            gCharacterSprite[Index].WorldPos.x++;
                            gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                            gCharacterSprite[Index].MovementRemaining--;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == RIGHT && !gCharacterSprite[Index].MovementRemaining)
                    {
                        //////      (not     (to the right of full player 16x16)               or        (to the left of full player 16x16) )              and (not        (below of full player 16x16)                            or          (bottom of sprite 16x16 is above top of player 16x16) )
                        if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x + 15) || (gCharacterSprite[Index].WorldPos.x < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                        {
                            gCharacterSprite[Index].Direction = LEFT;
                        }
                        else
                        {
                            gCharacterSprite[Index].MovementRemaining = 16;
                        }
                    }


                    if (!(((gCharacterSprite[Index].ScreenPos.x >= 0) && (gCharacterSprite[Index].ScreenPos.x < GAME_RES_WIDTH - 16)) && ((gCharacterSprite[Index].ScreenPos.y >= 0) && (gCharacterSprite[Index].ScreenPos.y < GAME_RES_HEIGHT - 16))))
                    {
                        gCharacterSprite[Index].Visible = FALSE;
                    }
                    else
                    {
                        gCharacterSprite[Index].Visible = TRUE;
                    }
                    break;
                }
                case (MOVEMENT_SPARKLE):
                {

                    ////TODO: MOVEMENT_SPARKLE02, MOVEMENT_SPARKLE_03, etc for more animations

                    gCharacterSprite[Index] = CharSpriteSparkleAnim(gCharacterSprite[Index], Counter, gSparkle01);

                    break;
                }
                default:
                {
                    ASSERT(FALSE, "INGAMESPRITE has an unknown movement type!")
                }
            }
        }
    }
}

uint8_t ScriptGiveMonster(uint8_t index, uint8_t level, uint16_t item)
{
    int sentToPC;
    uint8_t heldItem[2];
    struct Monster monster;
    //uint16_t targetIndex;         TODO:later when monsters have extra forms

    CreateMonster(&monster, index, level, USE_RANDOM_GENETICS, FALSE, 0, 0, 0);
    heldItem[0] = item;
    heldItem[1] = item >> 8;
    SetMonsterData(&monster, MONSTER_DATA_HELDITEM, heldItem);

    sentToPC = GiveMonsterToPlayer(&monster);
    return(sentToPC);
}

void HealPlayerParty(void)
{
    uint8_t i, j;
    uint8_t arg[4];

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        uint16_t maxHP = GetMonsterData(&gPlayerParty[i], MONSTER_DATA_MAX_HEALTH, NULL);
        arg[0] = maxHP;
        arg[1] = maxHP >> 8;
        SetMonsterData(&gPlayerParty[i], MONSTER_DATA_HEALTH, arg);

        arg[0] = 0;
        arg[1] = 0;
        arg[2] = 0;
        arg[3] = 0;
        SetMonsterData(&gPlayerParty[i], MONSTER_DATA_STATUS, arg);
    }
}

void TeleportPlayerBlackOut(void)
{
    StopGameMusic();

    gCurrentArea = gHomeArea;
    LoadUnloadSpritesVIAGameArea();
    LoadUnloadTriggerVIAGameArea();

    gPlayer.ScreenPos.x = 192; 
    gPlayer.ScreenPos.y = 96;  
    gPlayer.WorldPos.x = 4048; 
    gPlayer.WorldPos.y = 352;  
    gCamera.x = 3856;          
    gCamera.y = 256;           
    gPlayer.Direction = DOWN;

    HealPlayerParty();
}

void GivePlayerEquipItem(uint16_t itemIndex, uint16_t amount)
{
    gEquipableItems[itemIndex].Count += amount;
    if (gEquipableItems[itemIndex].Count > 0)
    {
        gEquipableItems[itemIndex].HasItem = TRUE;
    }
}

void GivePlayerUseItem(uint16_t itemIndex, uint16_t amount)
{
    gUseableItems[itemIndex].Count += amount;
    if (gUseableItems[itemIndex].Count > 0)
    {
        gUseableItems[itemIndex].HasItem = TRUE;
    }
}

void GivePlayerValueItem(uint16_t itemIndex, uint16_t amount)
{
    gValuableItems[itemIndex].Count += amount;
    if (gValuableItems[itemIndex].Count > 0)
    {
        gValuableItems[itemIndex].HasItem = TRUE;
    }
}

void GivePlayerAdventureItem(uint16_t itemIndex, uint16_t amount)
{
    gAdventureItems[itemIndex].Count += amount;
    if (gAdventureItems[itemIndex].Count > 0)
    {
        gAdventureItems[itemIndex].HasItem = TRUE;
    }
}

void ModifyCharVisibility(void)
{
    for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
    {
        if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
        {
            if (gCharacterSprite[Index].Visible == TRUE)
            {
                if (gCharacterSprite[Index].ScreenPos.y <= 0 || gCharacterSprite[Index].ScreenPos.y >= GAME_RES_HEIGHT - 16 || gCharacterSprite[Index].ScreenPos.x <= 0 || gCharacterSprite[Index].ScreenPos.x >= GAME_RES_WIDTH - 16)
                {
                    gCharacterSprite[Index].Visible = FALSE;
                }
            }
            if (gCharacterSprite[Index].Visible == FALSE)
            {
                if ((gCharacterSprite[Index].ScreenPos.x >= 0 && gCharacterSprite[Index].ScreenPos.x <= GAME_RES_WIDTH - 16) && (gCharacterSprite[Index].ScreenPos.y >= 0 && gCharacterSprite[Index].ScreenPos.y <= GAME_RES_HEIGHT - 16))
                {
                    gCharacterSprite[Index].Visible = TRUE;
                }
            }
        }
    }
}


void DrawBuySellBackBox(void)
{
    DrawWindow(80, 100, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    DrawWindow(80, 120, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    DrawWindow(80, 140, 48, 16, &COLOR_BLACK, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

    BlitStringToBuffer(gBuyButton.Name, &g6x7Font, &COLOR_BLACK, gBuyButton.x, gBuyButton.y);

    BlitStringToBuffer(gSellButton.Name, &g6x7Font, &COLOR_BLACK, gSellButton.x, gSellButton.y);

    BlitStringToBuffer(gBackButton.Name, &g6x7Font, &COLOR_BLACK, gBackButton.x, gBackButton.y);

    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, gBuySellBackItems[gButtonMenu.SelectedItem]->x - 6, gBuySellBackItems[gButtonMenu.SelectedItem]->y);

}

uint8_t PPI_BuySellBackBox(void)
{
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gButtonMenu.SelectedItem == 2)
        {
            PlayGameSound(&gSoundMenuChoose);
            return(3);
        }
        else
        {
            gButtonMenu.SelectedItem = 2;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    else if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gButtonMenu.SelectedItem > 0)
        {
            gButtonMenu.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gButtonMenu.SelectedItem = 2;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    else if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        if (gButtonMenu.SelectedItem < 2)
        {
            gButtonMenu.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gButtonMenu.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    else if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        PlayGameSound(&gSoundMenuChoose);
        return(gBuySellBackItems[gButtonMenu.SelectedItem]->Action);
    }
    else
    {
        return(0);
    }
}

//TODO: returns FALSE when there is an un recognized number of gTransitionsPerMap
BOOL MapTransition(void)
{
    BOOL result = TRUE;
    uint8_t TransitionAreaIndex = gCurrentArea.Index;
    uint8_t TransitionsPerMap = gTransitionsPerMap[TransitionAreaIndex];

    switch (TransitionsPerMap)
    {
        case 0:
        {
            goto End;
        }
        case 1:
        case 2:
        case 3:
        case 4:
        {
            for (uint8_t TransPerMapTemp = TransitionsPerMap; TransPerMapTemp > 0; TransPerMapTemp--)
            {
                if ((gPlayer.WorldPos.x >= gTransitionAreas[TransitionAreaIndex][TransPerMapTemp].TilesArea.left && gPlayer.WorldPos.x < gTransitionAreas[TransitionAreaIndex][TransPerMapTemp].TilesArea.right) && (gPlayer.WorldPos.y >= gTransitionAreas[TransitionAreaIndex][TransPerMapTemp].TilesArea.top && gPlayer.WorldPos.y < gTransitionAreas[TransitionAreaIndex][TransPerMapTemp].TilesArea.bottom))
                {
                    gCamera.x += gTransitionAreas[TransitionAreaIndex][TransPerMapTemp].CameraPosAdd.x;
                    gCamera.y += gTransitionAreas[TransitionAreaIndex][TransPerMapTemp].CameraPosAdd.y;

                    gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
                    gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;

                    for (uint16_t Index = 0; Index < NUM_GAME_AREAS; Index++)
                    {
                        if (gGameAreas[Index].Index == gTransitionAreas[TransitionAreaIndex][TransPerMapTemp].TransAreaDestIndex)
                        {
                            gCurrentArea = gGameAreas[Index];
                            LoadUnloadSpritesVIAGameArea();
                            LoadUnloadTriggerVIAGameArea();
                            break;
                        }
                    }

                    goto End;
                }
            }
            break;
        }
        default:
        {
            //ASSERT(FALSE, "[%s] Error! GameArea has an unexpected number of transitions! (%d)", __FUNCTION__, TransitionsPerMap);


            result = FALSE;
            //return false to allow this function to be logged
        }
    }
End:
    return(result);
}

////Function must be called immediately after gCurrentArea is updated in any way (teleport, transition, KO, etc...)
void LoadUnloadSpritesVIAGameArea(void)
{
    const uint16_t gameareaindex = gCurrentArea.Index;

    for (uint16_t sprite = 0; sprite < NUM_CHAR_SPRITES; sprite++)
    {
        gCharacterSprite[sprite].MovementRemaining = 0;
        gCharacterSprite[sprite].WorldPos = gCharacterSprite[sprite].ResetOriginWorldPos;
        gCharacterSprite[sprite].ResetWorldPos = gCharacterSprite[sprite].ResetOriginWorldPos;
        gCharacterSprite[sprite].ScreenPos.x = gCharacterSprite[sprite].WorldPos.x - gCamera.x;
        gCharacterSprite[sprite].ScreenPos.y = gCharacterSprite[sprite].WorldPos.y - gCamera.y;
        gCharacterSprite[sprite].Direction = gCharacterSprite[sprite].ResetDirection;
        gCharacterSprite[sprite].Loaded = FALSE;
        gCharacterSprite[sprite].Visible = FALSE;

        if (gameareaindex == gCharacterSprite[sprite].GameAreaIndex && gCharacterSprite[sprite].Exists == TRUE)
        {
            gCharacterSprite[sprite].Loaded = TRUE;
            if ((gCharacterSprite[sprite].ScreenPos.x >= 0) && (gCharacterSprite[sprite].ScreenPos.x <= GAME_RES_WIDTH - 16) && (gCharacterSprite[sprite].ScreenPos.y >= 0) && (gCharacterSprite[sprite].ScreenPos.y <= GAME_RES_HEIGHT - 16))
            {
                gCharacterSprite[sprite].Visible = TRUE;
            }
        }
    }
}

//must be called whenever gCurrentArea is updated
void LoadUnloadTriggerVIAGameArea(void)
{
    const uint16_t gameareaindex = gCurrentArea.Index;

    for (uint16_t trigger = 0; trigger < NUM_TRIGGERS; trigger++)
    {
        gTriggerTiles[trigger].Loaded = FALSE;

        if (gameareaindex == gTriggerTiles[trigger].GameAreaIndex && gTriggerTiles[trigger].Exists == TRUE)
        {
            gTriggerTiles[trigger].Loaded = TRUE;
        }
    }
}

INGAMESPRITE CharSpriteSparkleAnim( INGAMESPRITE _Inout_ charactersprite, uint16_t _In_ counter, GAMEBITMAP _In_ sparkletype[])
{

    if ((counter % 300 == 0) && (charactersprite.Visible == TRUE))       ////every 5 sec roll 33% chance to start sparkle anim.
    {
        DWORD Random = 0;

        rand_s((unsigned int*)&Random);
        if (Random % 3 == 0)                ////if success then trigger start of animation
        {
            charactersprite.AnimationFrame = 1;
        }
    }
    else if ((counter % 5 == 0) && (charactersprite.Visible == TRUE) && (charactersprite.AnimationFrame > 0))
    {
        switch (charactersprite.AnimationFrame)
        {
            case 1:
            {
                charactersprite.Sprite[FACING_DOWN_0] = sparkletype[0];
                charactersprite.AnimationFrame++;
                break;
            }
            case 2:
            {
                charactersprite.Sprite[FACING_DOWN_0] = sparkletype[1];
                charactersprite.AnimationFrame++;
                break;
            }
            case 3:
            {
                charactersprite.Sprite[FACING_DOWN_0] = sparkletype[2];
                charactersprite.AnimationFrame++;
                break;
            }
            case 4:
            {
                charactersprite.Sprite[FACING_DOWN_0] = sparkletype[3];
                charactersprite.AnimationFrame++;
                break;
            }
            case 5:
            {
                charactersprite.Sprite[FACING_DOWN_0] = sparkletype[2];
                charactersprite.AnimationFrame++;
                break;
            }
            case 6:
            {
                charactersprite.Sprite[FACING_DOWN_0] = sparkletype[1];
                charactersprite.AnimationFrame++;
                break;
            }
            case 7:
            {
                charactersprite.Sprite[FACING_DOWN_0] = sparkletype[0];
                charactersprite.AnimationFrame++;
                break;
            }
            case 8:
            {
                charactersprite.Sprite[FACING_DOWN_0] = gEmptySprite;
                charactersprite.AnimationFrame = 0;
            }
        }
    }
    return(charactersprite);
}

INGAMESPRITE GivePlayerItemFromCharSpriteEvent(INGAMESPRITE _Inout_ charactersprite, uint8_t _In_ itemtype)
{
    if (charactersprite.DialoguesBeforeLoop <= charactersprite.DialogueFlag)
    {
        for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
        {
            switch (itemtype)
            {
                case 0:
                    //equip
                    GivePlayerEquipItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                case 1:
                    //use
                    GivePlayerUseItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                case 2:
                    //value
                    GivePlayerValueItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                case 3:
                    //adventure
                    GivePlayerAdventureItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                default:
                {
                    ASSERT(itemtype <= 3, "ERROR, Unknown itemtype in GivePlayerItemFromCharSpriteEvent");
                }
            }
        }
        charactersprite.Event = EVENT_FLAG_TALK;
        charactersprite.InteractedWith = FALSE;
        gGamePaused = FALSE;
        gDialogueControls = FALSE;
        gOverWorldControls = TRUE;
    }
    else
    {
        charactersprite.DialogueFlag++;
        gDialogueControls = FALSE;
    }
    return(charactersprite);
}

//This will be used for item pickups that dissappear after aquiring them.
//// ITEMTYPE_EQUIP = 0, ITEMTYPE_USE = 1, ITEMTYPE_VALUE = 2, ITEMTYPE_ADVENTURE = 3
INGAMESPRITE GivePlayerItemFromCharAndRemoveSprite(INGAMESPRITE _Inout_ charactersprite, uint8_t _In_ itemtype)
{
    if (charactersprite.DialoguesBeforeLoop <= charactersprite.DialogueFlag)
    {
        for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
        {
            switch (itemtype)
            {
                case 0:
                    GivePlayerEquipItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                case 1:
                    GivePlayerUseItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                case 2:
                    GivePlayerValueItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                case 3:
                    GivePlayerAdventureItem(charactersprite.EventItemsIndex[i], charactersprite.EventItemsCount[i]);
                    break;
                default:
                {
                    ASSERT(itemtype <= 3, "ERROR, Unknown itemtype in GivePlayerItemFromCharSpriteEvent");
                }
            }
        }
        charactersprite.Event = EVENT_FLAG_NULL;
        charactersprite.InteractedWith = FALSE;
        charactersprite.Exists = FALSE;
        charactersprite.Loaded = FALSE;
        charactersprite.Visible = FALSE;
        gGamePaused = FALSE;
        gDialogueControls = FALSE;
        gOverWorldControls = TRUE;
    }
    else
    {
        charactersprite.DialogueFlag++;
        gDialogueControls = FALSE;
    }
    return(charactersprite);
}

void CharSpriteDrawHandler(uint16_t BrightnessAdjustment)
{
    for (uint8_t Index = 0; Index <= NUM_CHAR_SPRITES; Index++)
    {
        if (gCharacterSprite[Index].MovementRemaining >= 0 && gCharacterSprite[Index].Visible == TRUE)
        {
            gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;       //screen position is always true position minus camera position
            gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;

            switch (gCharacterSprite[Index].MovementRemaining)
            {
                case 16:
                case 0:
                {
                    gCharacterSprite[Index].SpriteIndex = 0;
                    break;
                }
                case 15:
                case 14:
                case 13:
                case 12:
                {
                    gCharacterSprite[Index].SpriteIndex = 1;
                    break;
                }
                case 11:
                case 10:
                case 9:
                case 8:
                {
                    gCharacterSprite[Index].SpriteIndex = 1;
                    break;
                }
                case 7:
                case 6:
                case 5:
                case 4:
                {
                    gCharacterSprite[Index].SpriteIndex = 2;
                    break;
                }
                case 3:
                case 2:
                case 1:
                {
                    gCharacterSprite[Index].SpriteIndex = 2;
                    break;
                }
            }
        }
        else if (gCharacterSprite[Index].MovementRemaining == 0 && gCharacterSprite[Index].Visible == TRUE)
        {
            gCharacterSprite[Index].SpriteIndex = 0;
        }

        if (gCharacterSprite[Index].Visible == TRUE)
        {
            Blit32BppBitmapToBuffer(&gCharacterSprite[Index].Sprite[gCharacterSprite[Index].SpriteIndex + gCharacterSprite[Index].Direction], gCharacterSprite[Index].ScreenPos.x, gCharacterSprite[Index].ScreenPos.y, BrightnessAdjustment);
        }
        //Reset sprites that are offscreen plust extra
        else if ((gCharacterSprite[Index].Visible == FALSE) && !(((gCharacterSprite[Index].ScreenPos.x >= -64) && (gCharacterSprite[Index].ScreenPos.x < GAME_RES_WIDTH + 48)) && ((gCharacterSprite[Index].ScreenPos.y >= -64) && (gCharacterSprite[Index].ScreenPos.y < GAME_RES_HEIGHT + 48))))        ///////reset any sprite off screen
        {
            gCharacterSprite[Index].WorldPos = gCharacterSprite[Index].ResetWorldPos;
            gCharacterSprite[Index].Direction = gCharacterSprite[Index].ResetDirection;
            gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;       //screen position is always true position minus camera position
            gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;

            //gCharacterSprite[Index].SightRange = gCharacterSprite[Index].ResetSightRange;             ////only for debugging (will reset with an item?)
        }
    }
}

//returns true when successful interaction
BOOL CharSpriteInteractionHandler(uint64_t LocalFrameCounter)
{
    BOOL result = FALSE;
    EVENT_FLAGS WorkingEvent;
    uint8_t WorkingSprite;

    for (uint8_t Sprite = 0; Sprite <= NUM_CHAR_SPRITES; Sprite++)
    {
        if (gCharacterSprite[Sprite].InteractedWith == TRUE)
        {
            WorkingEvent = gCharacterSprite[Sprite].Event;
            WorkingSprite = Sprite;
            result = TRUE;
            break;
        }
    }
    if (result)
    {
        switch (WorkingEvent)
        {
            case EVENT_FLAG_STORE:
            {
                if (gPostDialogueMenu == TRUE)
                {
                    DrawBuySellBackBox();
                    break;
                }
            }

            //TODO: make own function for TRIGGER_TILES and the scenes they trigger

            /*case EVENT_FLAG_TRIGGER_INFINITE:
            case EVENT_FLAG_TRIGGER_ONCE:
            {
                if (gCharacterSprite[WorkingSprite].Event == EVENT_FLAG_TRIGGER_ONCE)
                {
                    gCharacterSprite[WorkingSprite].Event = EVENT_FLAG_NULL;
                    gCharacterSprite[WorkingSprite].Exists = FALSE;
                }
                gCharacterSprite[WorkingSprite].InteractedWith = FALSE;
                gScriptActive = TRUE;
                break;
            }*/
            default:
            {
                if (gCharacterSprite[WorkingSprite].Event != EVENT_FLAG_NULL)
                {
                    DrawDialogueBox(gCharacterSprite[WorkingSprite].Dialogue[gCharacterSprite[WorkingSprite].DialogueFlag], LocalFrameCounter, NULL);
                }
                else
                {
                    LogMessageA(LL_ERROR, "[%s] gCharacterSprite[WorkingSprite].Event was EVENT_FLAG_NULL in function CharSpriteInteractionHandler in OverWorld.c! WorkingSprite was %d!", __FUNCTION__, WorkingSprite);
                }
            }
        }
    }
    return(result);
}

void DisplayDebugTiles(void)
{
    char Buffer[4] = { 0 };
    //debug tile centered on player
    _itoa_s(gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][gPlayer.WorldPos.x / 16], Buffer, sizeof(Buffer), 10);

    BlitStringToBuffer(Buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, gPlayer.ScreenPos.x + 5, gPlayer.ScreenPos.y + 4);
    //tile above player
    if (gPlayer.ScreenPos.y >= 16)
    {
        _itoa_s(gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) - 1][(gPlayer.WorldPos.x / 16)], Buffer, sizeof(Buffer), 10);

        BlitStringToBuffer(Buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, gPlayer.ScreenPos.x + 5, (gPlayer.ScreenPos.y - 16) + 4);
    }
    //to the right of player
    if (gPlayer.ScreenPos.x <= GAME_RES_WIDTH - 16)
    {
        _itoa_s(gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) + 1], Buffer, sizeof(Buffer), 10);

        BlitStringToBuffer(Buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (gPlayer.ScreenPos.x + 16) + 5, (gPlayer.ScreenPos.y) + 4);
    }
    //to the left of player
    if (gPlayer.ScreenPos.x >= 16)
    {
        _itoa_s(gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) - 1], Buffer, sizeof(Buffer), 10);

        BlitStringToBuffer(Buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (gPlayer.ScreenPos.x - 16) + 5, (gPlayer.ScreenPos.y) + 4);
    }
    //below the player
    if (gPlayer.ScreenPos.y < GAME_RES_HEIGHT - 16)
    {
        _itoa_s(gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) + 1][(gPlayer.WorldPos.x / 16)], Buffer, sizeof(Buffer), 10);

        BlitStringToBuffer(Buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (gPlayer.ScreenPos.x) + 5, (gPlayer.ScreenPos.y + 16) + 4);
    }
    BlitStringToBuffer("Press ~ to leave debug", &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, 2, GAME_RES_HEIGHT - 9);
}

//returns FALSE when last script has finished executing and function has reset for the next array of scripts to be passed into it
BOOL ApplyMovementScriptSprite(_In_ SCENE_SCRIPT scriptarray[], _In_ uint16_t xstart, _In_ uint16_t ystart, _In_ BOOL removeSpriteAfterScript)
{

    ///this function will be called every frame, yet I want it to queue up actions to be done at 30 frame intervals but updated & displayed every frame.

    static uint16_t localframecounter = 0;

    uint8_t CurrentActor = scriptarray[gCurrentScript].Actor;

    //new list of movement scripts
    if (scriptarray[gCurrentScript].Script == START_OF_SCRIPT)
    {
        gCurrentScript++;
        CurrentActor = scriptarray[gCurrentScript].Actor;
        gCharacterSprite[CurrentActor].WorldPos.x = xstart;
        gCharacterSprite[CurrentActor].WorldPos.y = ystart;
        gCharacterSprite[CurrentActor].Exists = TRUE;
        gCharacterSprite[CurrentActor].Loaded = TRUE;
        gCharacterSprite[CurrentActor].Visible = TRUE;
    }

    MOVEMENT_SCRIPT WorkingScript = scriptarray[gCurrentScript].Script;

    switch (WorkingScript)
    {
        case MOVE_NULL:
        {
            if (localframecounter % 64 == 0)
            {
                gCurrentScript++;
                //exactly identical to delay_64 except we log as a warning
                LogMessageA(LL_WARNING, "[%s] Warning, WorkingScript in function ApplyMovementSprite() was MOVE_NULL!", __FUNCTION__);
            }
            break;
        }
        case FACE_DOWN:
        {
            if (localframecounter % 32 == 0)
            {
                gCurrentScript++;
            }
            else if (localframecounter % 16 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = DOWN;
            }
            break;
        }
        case FACE_LEFT:
        {
            if (localframecounter % 32 == 0)
            {
                gCurrentScript++;
            }
            else if (localframecounter % 16 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = LEFT;
            }
            break;
        }
        case FACE_UP:
        {
            if (localframecounter % 32 == 0)
            {
                gCurrentScript++;
            }
            else if (localframecounter % 16 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = UP;
            }
            break;
        }
        case FACE_RIGHT:
        {
            if (localframecounter % 32 == 0)
            {
                gCurrentScript++;
            }
            else if (localframecounter % 16 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = RIGHT;
            }
            break;
        }
        case WALK_DOWN:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = DOWN;
                gCharacterSprite[CurrentActor].WorldPos.y++;
                gCharacterSprite[CurrentActor].ScreenPos.y = gCharacterSprite[CurrentActor].WorldPos.y - gCamera.y;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case WALK_LEFT:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = LEFT;
                gCharacterSprite[CurrentActor].WorldPos.x--;
                gCharacterSprite[CurrentActor].ScreenPos.x = gCharacterSprite[CurrentActor].WorldPos.x - gCamera.x;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case WALK_UP:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = UP;
                gCharacterSprite[CurrentActor].WorldPos.y--;
                gCharacterSprite[CurrentActor].ScreenPos.y = gCharacterSprite[CurrentActor].WorldPos.y - gCamera.y;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case WALK_RIGHT:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = RIGHT;
                gCharacterSprite[CurrentActor].WorldPos.x++;
                gCharacterSprite[CurrentActor].ScreenPos.x = gCharacterSprite[CurrentActor].WorldPos.x - gCamera.x;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case WALK_BW_DOWN:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = UP;
                gCharacterSprite[CurrentActor].WorldPos.y++;
                gCharacterSprite[CurrentActor].ScreenPos.y = gCharacterSprite[CurrentActor].WorldPos.y - gCamera.y;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case WALK_BW_LEFT:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = RIGHT;
                gCharacterSprite[CurrentActor].WorldPos.x--;
                gCharacterSprite[CurrentActor].ScreenPos.x = gCharacterSprite[CurrentActor].WorldPos.x - gCamera.x;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case WALK_BW_UP:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = DOWN;
                gCharacterSprite[CurrentActor].WorldPos.y--;
                gCharacterSprite[CurrentActor].ScreenPos.y = gCharacterSprite[CurrentActor].WorldPos.y - gCamera.y;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case WALK_BW_RIGHT:
        {
            if (!gCharacterSprite[CurrentActor].MovementRemaining)
            {
                gCharacterSprite[CurrentActor].MovementRemaining = 16;
            }
            else if (localframecounter % 2 == 0)
            {
                gCharacterSprite[CurrentActor].Direction = LEFT;
                gCharacterSprite[CurrentActor].WorldPos.x++;
                gCharacterSprite[CurrentActor].ScreenPos.x = gCharacterSprite[CurrentActor].WorldPos.x - gCamera.x;
                gCharacterSprite[CurrentActor].MovementRemaining--;
                if (!gCharacterSprite[CurrentActor].MovementRemaining)
                {
                    gCurrentScript++;
                }
            }
            break;
        }
        case DELAY_16:
        {
            if (localframecounter % 16 == 0)
            {
                gCurrentScript++;
            }
            break;
        }
        case DELAY_32:
        {
            if (localframecounter % 32 == 0)
            {
                gCurrentScript++;
            }
            break;
        }
        case DELAY_48:
        {
            if (localframecounter % 48 == 0)
            {
                gCurrentScript++;
            }
            break;
        }
        case DELAY_64:
        {
            if (localframecounter % 64 == 0)
            {
                gCurrentScript++;
            }
            break;
        }
        case DIALOGUE_TRIGGER:
        {
            //delay increasing gCurrentScript until player input
            break;
        }
        default: 
        {
            LogMessageA(LL_ERROR, "[%s] Error, Working script was unknown value in ApplyMovementScript! Value of %d!", __FUNCTION__, WorkingScript);
            break;
        }
    }

    if (scriptarray[gCurrentScript].Script == END_OF_SCRIPT)
    {
        if (removeSpriteAfterScript)
        {
            gCharacterSprite[CurrentActor].Exists = FALSE;
            gCharacterSprite[CurrentActor].Loaded = FALSE;
            gCharacterSprite[CurrentActor].Visible = FALSE;
        }
        gCurrentScript = 0;
        localframecounter = 0;
        return(FALSE);
    }

    localframecounter++;
    return(TRUE);
}

BOOL CheckNPCHasLOSWithPlayer(uint8_t spriteId)
{
    BOOL result = FALSE;
    //for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
    {
        switch (gCharacterSprite[spriteId].Direction)
        {
            case DOWN:
            {
                if (((gCharacterSprite[spriteId].WorldPos.y + (gCharacterSprite[spriteId].SightRange * 16) >= gPlayer.WorldPos.y) && gCharacterSprite[spriteId].WorldPos.y <= gPlayer.WorldPos.y) && (gCharacterSprite[spriteId].WorldPos.x == gPlayer.WorldPos.x) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gCharacterSprite[spriteId].Event == EVENT_FLAG_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
                {
                    gPlayer.Direction = UP;
                    result = TRUE;
                }
                break;
            }
            case LEFT:
            {
                if (((gCharacterSprite[spriteId].WorldPos.x - (gCharacterSprite[spriteId].SightRange * 16) <= gPlayer.WorldPos.x) && gCharacterSprite[spriteId].WorldPos.x >= gPlayer.WorldPos.x) && (gCharacterSprite[spriteId].WorldPos.y == gPlayer.WorldPos.y) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gCharacterSprite[spriteId].Event == EVENT_FLAG_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
                {
                    gPlayer.Direction = RIGHT;
                    result = TRUE;
                }
                break;
            }
            case RIGHT:
            {
                if (((gCharacterSprite[spriteId].WorldPos.x + (gCharacterSprite[spriteId].SightRange * 16) >= gPlayer.WorldPos.x) && gCharacterSprite[spriteId].WorldPos.x <= gPlayer.WorldPos.x) && (gCharacterSprite[spriteId].WorldPos.y == gPlayer.WorldPos.y) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gCharacterSprite[spriteId].Event == EVENT_FLAG_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
                {
                    gPlayer.Direction = LEFT;
                    result = TRUE;
                }
                break;
            }
            case UP:
            {
                if (((gCharacterSprite[spriteId].WorldPos.y - (gCharacterSprite[spriteId].SightRange * 16) <= gPlayer.WorldPos.y) && gCharacterSprite[spriteId].WorldPos.y >= gPlayer.WorldPos.y) && (gCharacterSprite[spriteId].WorldPos.x == gPlayer.WorldPos.x) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gCharacterSprite[spriteId].Event == EVENT_FLAG_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
                {
                    gPlayer.Direction = DOWN;
                    result = TRUE;
                }
                break;
            }
            default:
            {
                ASSERT(FALSE, "Sprite is facing an unknown direction while looking for player!");
            }
        }
    }
    return (result);
}

void InitiateDialogueAndCutscene(uint64_t counter)
{
    if (gOverWorldControls == FALSE && gGamePaused == TRUE)
    {
        CharSpriteInteractionHandler(counter);

        if (gScriptActive && gSceneScriptArray[gCurrentScript].Script == DIALOGUE_TRIGGER)
        {
            //TODO: make better way of knowing what scene is what and what actor is what, this is prone to mistakes

            DrawDialogueBox(gCharacterSprite[gSceneScriptArray[gCurrentScript].Actor].Dialogue[gCharacterSprite[gSceneScriptArray[gCurrentScript].Actor].DialogueFlag], counter, NULL);
        }
    }
}

void TurnNPCToPlayerStartDialogue(void)
{
    for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
    {
        if (gCharacterSprite[Index].Visible == TRUE)
        {
            switch (gPlayer.Direction)          ////////decending directional ordering (right = 9, up = 6, left = 3, down = 0) and then break to make sure switch doesnt select two options
            {
                case (RIGHT):
                {
                    if ((gCharacterSprite[Index].WorldPos.x == gPlayer.WorldPos.x + 16) && (gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y - 4) && (gCharacterSprite[Index].WorldPos.y <= gPlayer.WorldPos.y + 4))
                    {
                        gCharacterSprite[Index].WorldPos.x = gPlayer.WorldPos.x + 16;
                        gCharacterSprite[Index].WorldPos.y = gPlayer.WorldPos.y;
                        gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                        gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                        gCharacterSprite[Index].InteractedWith = TRUE;
                        gCharacterSprite[Index].Direction = LEFT;
                        EnterDialogue();
                    }
                    break;
                }
                case (UP):
                {
                    if ((gCharacterSprite[Index].WorldPos.y == gPlayer.WorldPos.y - 16) && (gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x - 4) && (gCharacterSprite[Index].WorldPos.x <= gPlayer.WorldPos.x + 4))
                    {
                        gCharacterSprite[Index].WorldPos.y = gPlayer.WorldPos.y - 16;
                        gCharacterSprite[Index].WorldPos.x = gPlayer.WorldPos.x;
                        gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                        gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                        gCharacterSprite[Index].InteractedWith = TRUE;
                        gCharacterSprite[Index].Direction = DOWN;
                        EnterDialogue();
                    }
                    break;
                }
                case (LEFT):
                {
                    if ((gCharacterSprite[Index].WorldPos.x == gPlayer.WorldPos.x - 16) && (gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y - 4) && (gCharacterSprite[Index].WorldPos.y <= gPlayer.WorldPos.y + 4))
                    {
                        gCharacterSprite[Index].WorldPos.x = gPlayer.WorldPos.x - 16;
                        gCharacterSprite[Index].WorldPos.y = gPlayer.WorldPos.y;
                        gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                        gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                        gCharacterSprite[Index].InteractedWith = TRUE;
                        gCharacterSprite[Index].Direction = RIGHT;
                        EnterDialogue();
                    }
                    break;
                }
                case (DOWN):
                {
                    if ((gCharacterSprite[Index].WorldPos.y == gPlayer.WorldPos.y + 16) && (gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x - 4) && (gCharacterSprite[Index].WorldPos.x <= gPlayer.WorldPos.x + 4))
                    {
                        gCharacterSprite[Index].WorldPos.y = gPlayer.WorldPos.y + 16;
                        gCharacterSprite[Index].WorldPos.x = gPlayer.WorldPos.x;
                        gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                        gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                        gCharacterSprite[Index].InteractedWith = TRUE;
                        gCharacterSprite[Index].Direction = UP;
                        EnterDialogue();
                    }
                    break;
                }
                default:
                {
                    ASSERT(FALSE, "Player is facing an unknown direction!")
                }
            }
        }
    }
}

void HandlePlayerCollision(void)
{
    BOOL CanMoveToDesiredTile = FALSE;
    BOOL NoSpriteCollision = FALSE;
    if (gGameInput.SDownKeyPressed)
    {
        gPlayer.Direction = DOWN;
        //map collision
        for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
        {
            if (((gPlayer.WorldPos.y / 16) + 1 > gOverWorld01.TileMap.Height - 1))      ////not perfect but prevents crashing when walking to bottom of world
            {
                break;
            }

            if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) + 1][gPlayer.WorldPos.x / 16] == gPassableTiles[Counter])
            {
                for (uint8_t Cliff = 0; Cliff < _countof(gCliffEdgeTiles); Cliff++)
                {
                    if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) + 1][gPlayer.WorldPos.x / 16] == gCliffEdgeTiles[Cliff])
                    {
                        CanMoveToDesiredTile = FALSE;
                        break;
                    }
                    else
                    {
                        CanMoveToDesiredTile = TRUE;
                    }
                }
                break;
            }
        }
        //sprite collision
        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
        {
            if (gCharacterSprite[Index].Visible == TRUE && (gCharacterSprite[Index].Movement != MOVEMENT_SPARKLE))
            {
                //////this big line of code means: if any pixel of 16x16 sprite is in the way of moving
                if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                {
                    NoSpriteCollision = FALSE;
                    break;
                }
                else
                {
                    NoSpriteCollision = TRUE;
                }
            }
            else
            {
                NoSpriteCollision = TRUE;
            }
        }
        //set movementremaining
        if (CanMoveToDesiredTile == TRUE && NoSpriteCollision == TRUE)
        {
            if (gPlayer.ScreenPos.y < GAME_RES_HEIGHT - 16)
            {
                gPlayer.Direction = DOWN;
                gPlayer.MovementRemaining = 16;
                gPlayer.StepsTaken++;
            }
        }
    }
    else if (gGameInput.ALeftKeyPressed)
    {
        gPlayer.Direction = LEFT;
        //map collision
        for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
        {
            if (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) - 1] == gPassableTiles[Counter])
            {
                CanMoveToDesiredTile = TRUE;

                break;
            }
        }
        //sprite collision
        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
        {
            if (gCharacterSprite[Index].Visible == TRUE && (gCharacterSprite[Index].Movement != MOVEMENT_SPARKLE))
            {
                //////this big line of code means: if any pixel of 16x16 sprite is in the way of moving
                if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x) || (gCharacterSprite[Index].WorldPos.x + 16 < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                {
                    NoSpriteCollision = FALSE;
                    break;
                }
                else
                {
                    NoSpriteCollision = TRUE;
                }
            }
            else
            {
                NoSpriteCollision = TRUE;
            }
        }
        //set movementremaining
        if (CanMoveToDesiredTile == TRUE && NoSpriteCollision == TRUE)
        {
            if (gPlayer.ScreenPos.x > 0)
            {
                gPlayer.Direction = LEFT;
                gPlayer.MovementRemaining = 16;
                gPlayer.StepsTaken++;
            }
        }
    }
    else if (gGameInput.DRightKeyPressed)
    {
        gPlayer.Direction = RIGHT;
        //map collision
        for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
        {
            if (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) + 1] == gPassableTiles[Counter])
            {
                CanMoveToDesiredTile = TRUE;

                break;
            }
        }
        //sprite collision
        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
        {
            //////this big line of code means: if any pixel of 16x16 sprite is in the way of moving
            if (gCharacterSprite[Index].Visible == TRUE && (gCharacterSprite[Index].Movement != MOVEMENT_SPARKLE))
            {
                if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                {
                    NoSpriteCollision = FALSE;
                    break;
                }
                else
                {
                    NoSpriteCollision = TRUE;
                }
            }
            else
            {
                NoSpriteCollision = TRUE;
            }
        }
        //set movementremaining
        if (CanMoveToDesiredTile == TRUE && NoSpriteCollision == TRUE)
        {
            if (gPlayer.ScreenPos.x < GAME_RES_WIDTH - 16)
            {
                gPlayer.Direction = RIGHT;
                gPlayer.MovementRemaining = 16;
                gPlayer.StepsTaken++;
            }
        }
    }
    else if (gGameInput.WUpKeyPressed)
    {
        gPlayer.Direction = UP;
        if (gPlayer.ScreenPos.y > 0)
        {
            //map collision
            for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
            {
                if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) - 1][gPlayer.WorldPos.x / 16] == gPassableTiles[Counter])
                {
                    for (uint8_t Cliff = 0; Cliff < _countof(gCliffEdgeTiles); Cliff++)
                    {
                        if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16)][gPlayer.WorldPos.x / 16] == gCliffEdgeTiles[Cliff])
                        {
                            CanMoveToDesiredTile = FALSE;
                            break;
                        }
                        else
                        {
                            CanMoveToDesiredTile = TRUE;
                        }
                    }

                    break;
                }
            }
            //sprite collision
            for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
            {
                if (gCharacterSprite[Index].Visible == TRUE && (gCharacterSprite[Index].Movement != MOVEMENT_SPARKLE))
                {
                    //////this big line of code means: if any pixel of 16x16 sprite is in the way of moving
                    if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y) || (gCharacterSprite[Index].WorldPos.y + 16 < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                    {
                        NoSpriteCollision = FALSE;
                        break;
                    }
                    else
                    {
                        NoSpriteCollision = TRUE;
                    }
                }
                else
                {
                    NoSpriteCollision = TRUE;
                }
            }
            //set movementremaining
            if (CanMoveToDesiredTile == TRUE && NoSpriteCollision == TRUE)
            {
                if (gPlayer.ScreenPos.y > 0)
                {
                    gPlayer.Direction = UP;
                    gPlayer.MovementRemaining = 16;
                    gPlayer.StepsTaken++;
                }
            }
        }
    }
}

void HandleCameraPlayerMovementAndCharVisibility(void)
{
    if (gPlayer.Direction == DOWN)
    {
        gPlayer.WorldPos.y++;
        if (gPlayer.ScreenPos.y < GAME_RES_HEIGHT - 128)   ////keeps player centered
        {
            gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
        }
        else
        {
            if (gCamera.y < gCurrentArea.Area.bottom - GAME_RES_HEIGHT)
            {
                gCamera.y++;
                gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;

                //checkfor sprites that are now offscreen, we dont want to draw offscreen
                for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                {
                    if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                    {
                        gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                        ModifyCharVisibility();
                    }
                }
            }
            else
            {
                gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
            }
        }
    }
    else if (gPlayer.Direction == LEFT)
    {
        gPlayer.WorldPos.x--;
        if (gPlayer.ScreenPos.x > 192)   ////keeps player centered
        {
            gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
        }
        else
        {
            if (gCamera.x > gCurrentArea.Area.left)
            {
                gCamera.x--;
                gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;

                //checkfor sprites that are now offscreen, we dont want to draw offscreen
                for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                {
                    if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                    {
                        gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                        ModifyCharVisibility();
                    }
                }
            }
            else
            {
                gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
            }
        }
    }
    else if (gPlayer.Direction == RIGHT)
    {
        gPlayer.WorldPos.x++;
        if (gPlayer.ScreenPos.x < GAME_RES_WIDTH - 192)   ////keeps player centered
        {
            gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
        }
        else
        {
            if (gCamera.x < gCurrentArea.Area.right - GAME_RES_WIDTH)
            {
                gCamera.x++;
                gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;

                //checkfor sprites that are now offscreen, we dont want to draw offscreen
                for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                {
                    if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                    {
                        gCharacterSprite[Index].ScreenPos.x = gCharacterSprite[Index].WorldPos.x - gCamera.x;
                        ModifyCharVisibility();
                    }
                }
            }
            else
            {
                gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
            }
        }
    }
    else if (gPlayer.Direction == UP)
    {
        gPlayer.WorldPos.y--;
        if (gPlayer.ScreenPos.y > 128)   ////keeps player centered
        {
            gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
        }
        else
        {
            if (gCamera.y > gCurrentArea.Area.top)
            {
                gCamera.y--;
                gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;

                //checkfor sprites that are now offscreen, we dont want to draw offscreen
                for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                {
                    if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                    {
                        gCharacterSprite[Index].ScreenPos.y = gCharacterSprite[Index].WorldPos.y - gCamera.y;
                        ModifyCharVisibility();
                    }
                }
            }
            else
            {
                gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
            }
        }
    }
}

void HandleTileFunctions(void)
{
    for (uint8_t telepads = 0; telepads < NUM_TELEPORT_TILES; telepads++)
    {
        if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16)][(gPlayer.WorldPos.x / 16)] == gTeleportTiles[telepads])
        {
            if (gPlayer.HasMovedSincePort == TRUE)
            {
                TeleportHandler();
                break;
            }
        }
    }

    for (uint8_t index = 0; index < NUM_CHAR_SPRITES; index++)
    {
        BOOL PlayerInLOS = CheckNPCHasLOSWithPlayer(index);

        if (PlayerInLOS)
        {
            gCharacterSprite[index].InteractedWith = TRUE;
            EnterDialogue();
            break;
        }
    }

    for (uint8_t triggers = 0; triggers < NUM_TRIGGERS; triggers++)
    {

        BOOL TriggerActivated = IsPlayerOnTrigger(triggers);

        if (TriggerActivated)
        {
            gTriggerTiles[triggers].InteractedWith = TRUE;
            EnterDialogue();
            TriggerInteractionHandler();
            break;
        }

    }

    for (uint8_t EncounterTile = 0; EncounterTile < _countof(gEncounterTiles); EncounterTile++)
    {
        if ((gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16)] == gEncounterTiles[EncounterTile]) && (gPlayer.StepsTaken - gPlayer.StepsSinceLastEncounter > BATTLE_ENCOUNTER_GRACE_PERIOD))
        {
            DWORD Random = 0;

            rand_s((unsigned int*)&Random);

            Random = Random % 1000;

            if (Random > (1000 - gPlayer.RandomEncounterPercent))
            {
                gPlayer.StepsSinceLastEncounter = gPlayer.StepsTaken;
                RandomMonsterEncounter(&gPreviousGameState, &gCurrentGameState);
            }
            break;
        }
    }

    if (MapTransition() == FALSE)
    {
        LogMessageA(LL_ERROR, "[%s] Function MapTransition in OverWorld.c returned FALSE!", __FUNCTION__);
    }
}

void HandleNPCEvent(void)
{
    for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
    {
        if (gCharacterSprite[Index].InteractedWith == TRUE)
        {
            switch (gCharacterSprite[Index].Event)
            {
                case EVENT_FLAG_BATLLE:
                {
                    if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
                    {
                        TrainerEncounter(&gPreviousGameState, &gCurrentGameState);
                        gGamePaused = FALSE;
                        gDialogueControls = FALSE;
                        gOverWorldControls = TRUE;
                    }
                    else
                    {
                        gCharacterSprite[Index].DialogueFlag++;
                        gDialogueControls = FALSE;
                    }
                    break;
                }
                case EVENT_FLAG_MONSTER:
                {
                    if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
                    {
                        if (gCharacterSprite[Index].EventMonsterIndex == 0 || gCharacterSprite[Index].EventMonsterLevel == 0)           //give player first index at level 1 if sprite is set up incorrectly
                        {
                            gCharacterSprite[Index].EventMonsterIndex = 1;
                            gCharacterSprite[Index].EventMonsterLevel = 1;
                            LogMessageA(LL_WARNING, "[%s] gCharacterSprite[%d] tried to give a monster to player with either 0 index or 0 level!", __FUNCTION__, Index);
                        }
                        ScriptGiveMonster(gCharacterSprite[Index].EventMonsterIndex, gCharacterSprite[Index].EventMonsterLevel, gCharacterSprite[Index].EventMonsterItem);
                        gCharacterSprite[Index].Event = EVENT_FLAG_TALK;
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                        gGamePaused = FALSE;
                        gDialogueControls = FALSE;
                        gOverWorldControls = TRUE;
                    }
                    else
                    {
                        gCharacterSprite[Index].DialogueFlag++;
                        gDialogueControls = FALSE;
                    }
                    break;
                }
                case EVENT_FLAG_HEAL:
                {
                    if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)        //for when dialogues are longer than one box
                    {
                        HealPlayerParty();  //call heal
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                        gGamePaused = FALSE;
                        gDialogueControls = FALSE;
                        gOverWorldControls = TRUE;
                    }
                    else
                    {
                        gCharacterSprite[Index].DialogueFlag++;
                        gDialogueControls = FALSE;
                    }
                    break;
                }
                case EVENT_FLAG_EQUIPITEM_ONCE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_EQUIP);
                    break;
                }
                case EVENT_FLAG_USEITEM_ONCE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_USE);
                    break;
                }
                case EVENT_FLAG_VALUEITEM_ONCE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_VALUE);
                    break;
                }
                case EVENT_FLAG_ADVENTUREITEM_ONCE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_ADVENTURE);
                    break;
                }
                case EVENT_FLAG_EQUIPITEM_NOSPRITE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_EQUIP);
                    break;
                }
                case EVENT_FLAG_USEITEM_NOSPRITE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_USE);
                    break;
                }
                case EVENT_FLAG_VALUEITEM_NOSPRITE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_VALUE);
                    break;
                }
                case EVENT_FLAG_ADVENTUREITEM_NOSPRITE:
                {
                    gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_ADVENTURE);
                    break;
                }
                case EVENT_FLAG_STORE:
                {
                    if (gPostDialogueMenu == TRUE)
                    {
                        ///////say a dialogue then ask if player wants to buy/sell/exit 
                        uint8_t StoreButtons = PPI_BuySellBackBox();

                        switch (StoreButtons)
                        {
                            case 1:
                                //buy
                                ////create a new store.c file? and send us to that gamestate when buying items?? 
                                gPreviousGameState = gCurrentGameState;
                                gCurrentGameState = GAMESTATE_STORE;
                                gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                                gCharacterSprite[Index].InteractedWith = FALSE;
                                gGamePaused = FALSE;
                                gDialogueControls = FALSE;
                                gOverWorldControls = TRUE;
                                gPostDialogueMenu = FALSE;
                                gStoreSpriteIndex = Index;
                                break;
                            case 2:
                                //sell
                                ////go to gamestatepockets->valuablepockets and check a variable to put us in "sell" mode
                                gPreviousGameState = gCurrentGameState;
                                gCurrentGameState = GAMESTATE_INVENTORYSCREEN;
                                gPreviousPockets = gCurrentPockets;
                                gCurrentPockets = POCKETSTATE_VALUABLE;
                                gSellingItems = TRUE;       //sell mode
                                gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                                gCharacterSprite[Index].InteractedWith = FALSE;
                                gGamePaused = FALSE;
                                gDialogueControls = FALSE;
                                gOverWorldControls = TRUE;
                                gPostDialogueMenu = FALSE;
                                break;
                            case 3:
                                //back
                                ////reset dialogueflag and beforeloop to give a goodbye message when exiting
                                gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                                gCharacterSprite[Index].InteractedWith = FALSE;
                                gGamePaused = FALSE;
                                gDialogueControls = FALSE;
                                gOverWorldControls = TRUE;
                                gPostDialogueMenu = FALSE;
                                break;
                            case 0:
                            default:
                            {
                                break;
                            }
                        }
                    }
                    else if (gPostDialogueMenu == FALSE)
                    {
                        if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)     //not <= for stores extra dialogue while buy/sell/back
                        {
                            gPostDialogueMenu = TRUE;
                            gCharacterSprite[Index].DialogueFlag++;
                        }
                        else
                        {
                            gCharacterSprite[Index].DialogueFlag++;
                            gDialogueControls = FALSE;
                        }
                    }
                    break;
                }
                case EVENT_FLAG_DRIVE_STORAGE:
                {
                    if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)     //not <= for extra dialogue while booting storage
                    {

                        gPreviousGameState = gCurrentGameState;
                        gCurrentGameState = GAMESTATE_DRIVE_STORAGE;
                        gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                        gCharacterSprite[Index].Direction = gCharacterSprite[Index].ResetDirection;
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gGamePaused = FALSE;
                        gDialogueControls = FALSE;
                        gOverWorldControls = TRUE;
                        gPostDialogueMenu = FALSE;
                    }
                    else
                    {
                        gCharacterSprite[Index].DialogueFlag++;
                        gDialogueControls = FALSE;
                    }
                    break;
                }
                ////Just a normal dialogue
                default:
                {
                    if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
                    {
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gGamePaused = FALSE;
                        gDialogueControls = FALSE;
                        gOverWorldControls = TRUE;
                    }
                    else
                    {
                        gCharacterSprite[Index].DialogueFlag++;
                        gDialogueControls = FALSE;
                    }
                    break;
                }
                break;      //break out of for loop searching for more sprites interacted with, obviously if we just interacted with one there shouldn't be more
            }
        }
    }
}

BOOL IsPlayerOnTrigger(uint8_t index)
{
    BOOL result = FALSE;
    if (gTriggerTiles[index].Loaded && gTriggerTiles[index].Exists && (gTriggerTiles[index].WorldPos.x == gPlayer.WorldPos.x) && (gTriggerTiles[index].WorldPos.y == gPlayer.WorldPos.y))
    {
        result = TRUE;
    }
    return(result);
}

//returns true when successful trigger interaction
BOOL TriggerInteractionHandler(void)
{
    BOOL result = FALSE;
    TRIGGER_TILE workingtile;
    uint8_t tileindex;
    
    for (uint8_t tile = 0; tile < NUM_TRIGGERS; tile++)
    {
        if (gTriggerTiles[tile].InteractedWith = TRUE)
        {
            workingtile = gTriggerTiles[tile];
            tileindex = tile;
            result = TRUE;
            break;
        }
    }

    if (result)
    {
        switch (workingtile.Flag)
        {
            case TRIGGER_FLAG_INFINITE:
            case TRIGGER_FLAG_ONCE:
            {
                if (workingtile.Flag == TRIGGER_FLAG_ONCE)
                {
                     gTriggerTiles[tileindex].Flag = TRIGGER_FLAG_BLANK;
                     gTriggerTiles[tileindex].Exists = FALSE;
                }
            }

            ///TODO: case TRIGGER_FLAG_COOLDOWN: ....etc

            gTriggerTiles[tileindex].InteractedWith = FALSE;
            gScriptActive = TRUE;
            break;
            case TRIGGER_FLAG_BLANK:
            {
                break;
            }

            default: 
            {
                //assuming NULL or BLANK flags were accidentially triggered
                LogMessageA(LL_ERROR, "[%s] ERROR! TRIGGER_FLAG_NULL or unrecognized value for workingtile.flag in OverWorld.c! Value = %d", __FUNCTION__, workingtile.Flag);
                if (workingtile.Exists == TRUE && gTriggerTiles[tileindex].Exists == TRUE)
                {
                    gTriggerTiles[tileindex].Exists = FALSE;
                    LogMessageA(LL_ERROR, "[%s] Success! workingtile.flag was found to be enabled while NULL or using an unrecognized value, successfully disabled tile with .exists = FALSE!", __FUNCTION__);
                }
                
                gTriggerTiles[tileindex].InteractedWith = FALSE;
                gScriptActive = FALSE;  ///TOUSE: could enable for debugging purposes?!
            }
        }
    }
    return(result);
}

BOOL InteractWithTrigger(void)
{
    BOOL result = FALSE;
    for (uint8_t Index = 0; Index < NUM_TRIGGERS; Index++)
    {
        if (gTriggerTiles[Index].Exists == TRUE && gTriggerTiles[Index].Loaded == TRUE && gTriggerTiles[Index].Interactive == TRUE)
        {
            switch (gPlayer.Direction)
            {
                case (RIGHT):
                {
                    if ((gTriggerTiles[Index].WorldPos.x == gPlayer.WorldPos.x + 16) && (gTriggerTiles[Index].WorldPos.y >= gPlayer.WorldPos.y - 4) && (gTriggerTiles[Index].WorldPos.y <= gPlayer.WorldPos.y + 4))
                    {
                        gTriggerTiles[Index].InteractedWith = TRUE;
                        result = TRUE;
                        EnterDialogue();
                    }
                    break;
                }
                case (UP):
                {
                    if ((gTriggerTiles[Index].WorldPos.y == gPlayer.WorldPos.y - 16) && (gTriggerTiles[Index].WorldPos.x >= gPlayer.WorldPos.x - 4) && (gTriggerTiles[Index].WorldPos.x <= gPlayer.WorldPos.x + 4))
                    {
                        gTriggerTiles[Index].InteractedWith = TRUE;
                        result = TRUE;
                        EnterDialogue();
                    }
                    break;
                }
                case (LEFT):
                {
                    if ((gTriggerTiles[Index].WorldPos.x == gPlayer.WorldPos.x - 16) && (gTriggerTiles[Index].WorldPos.y >= gPlayer.WorldPos.y - 4) && (gTriggerTiles[Index].WorldPos.y <= gPlayer.WorldPos.y + 4))
                    {
                        gTriggerTiles[Index].InteractedWith = TRUE;
                        result = TRUE;
                        EnterDialogue();
                    }
                    break;
                }
                case (DOWN):
                {
                    if ((gTriggerTiles[Index].WorldPos.y == gPlayer.WorldPos.y + 16) && (gTriggerTiles[Index].WorldPos.x >= gPlayer.WorldPos.x - 4) && (gTriggerTiles[Index].WorldPos.x <= gPlayer.WorldPos.x + 4))
                    {
                        gTriggerTiles[Index].InteractedWith = TRUE;
                        result = TRUE;
                        EnterDialogue();
                    }
                    break;
                }
                default:
                {
                    ASSERT(FALSE, "Player is facing an unknown direction!")
                }
            }
        }
    }
    return(result);
}