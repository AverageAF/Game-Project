#include "Main.h"
#include "flags.h"

#include "OverWorld.h"
#include "Dialogue.h"
#include "MonsterData.h"

#include "Inventory.h"
#include "InventoryItems.h"
#include "NPCData.h"


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


//TODO: make more of these
SCENE_SCRIPT gSceneScriptArray[MAX_MOVEMENT_SCRIPTS] = { 0 };

//Currently activated cutscene
uint8_t gCurrentScene = 0;
//

//used to check the current queued script in scenescriptarray[]
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
        gScriptActive = FALSE;
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
        TriggerNPCMovement(LocalFrameCounter);
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
                    if (CheckIfLastDialogueNPC(gSceneScriptArray[gCurrentScript].Actor))
                    {
                        gCurrentScript++;
                    }
                    GoToNextDialogueNPC(gSceneScriptArray[gCurrentScript].Actor);
                    gDialogueControls = FALSE;
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
            //only active during scripts when current script is DIALOGUE_TRIGGER
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed && (!gScriptActive || gSceneScriptArray[gCurrentScript].Script == DIALOGUE_TRIGGER))         //allows for skipping typewriter effect to instant dialogue blit
            {
                gFinishedDialogueTextAnimation = TRUE;
            }
            
            if (gScriptActive)
            {
                //TOFIX!
                //xstart and ystart are temp, they only work for the professor script
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


//TODO: better movement please! this is full of if statements!!!
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

    CreateMonster(&monster, index, level, USE_RANDOM_GENETICS, FALSE, 0, 0);
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

    BlitStringToBuffer("�", &g6x7Font, &COLOR_BLACK, gBuySellBackItems[gButtonMenu.SelectedItem]->x - 6, gBuySellBackItems[gButtonMenu.SelectedItem]->y);

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

        if (gameareaindex == gCharacterSprite[sprite].GameAreaIndex && gCharacterSprite[sprite].Exists == TRUE && !CheckGameFlag(gCharacterSprite[sprite].GameFlag))
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

        if (gameareaindex == gTriggerTiles[trigger].GameAreaIndex && gTriggerTiles[trigger].Exists == TRUE && !CheckGameFlag(gTriggerTiles[trigger].GameFlag))
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

//used for items on the ground the player can pickup
void GiveItemAndRemoveSprite(uint8_t spriteIndex)
{
    switch (GetEventTypeFromSpriteIndex(spriteIndex))
    {
        case EVENT_EQUIPITEM_REPEAT:
        case EVENT_EQUIPITEM_ONCE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have an event without a dissappearing sprite inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_EQUIPITEM_NOSPRITE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerEquipItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        case EVENT_USEITEM_REPEAT:
        case EVENT_USEITEM_ONCE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have an event without a dissappearing sprite inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_USEITEM_NOSPRITE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerUseItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        case EVENT_VALUEITEM_REPEAT:
        case EVENT_VALUEITEM_ONCE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have an event without a dissappearing sprite inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_VALUEITEM_NOSPRITE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerValueItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        case EVENT_ADVENTUREITEM_REPEAT:
        case EVENT_ADVENTUREITEM_ONCE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have an event without a dissappearing sprite inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_ADVENTUREITEM_NOSPRITE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerAdventureItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        default:
        {
            LogMessageA(LL_ERROR, "[%s] ERROR! CharacterSprite[%d] had an unusable value as an event flag! Event = %d !", __FUNCTION__, spriteIndex, GetEventTypeFromSpriteIndex(spriteIndex));
            break;
        }
    }
    RemoveCharSprite(spriteIndex);
}

void GiveItemChangeNPCEvent(uint8_t spriteIndex)
{
    switch (GetEventTypeFromSpriteIndex(spriteIndex))
    {
        case EVENT_EQUIPITEM_REPEAT:
        case EVENT_EQUIPITEM_NOSPRITE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have the wrong kind of event inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_EQUIPITEM_ONCE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerEquipItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        case EVENT_USEITEM_REPEAT:
        case EVENT_USEITEM_NOSPRITE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have the wrong kind of event inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_USEITEM_ONCE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerUseItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        case EVENT_VALUEITEM_REPEAT:
        case EVENT_VALUEITEM_NOSPRITE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have the wrong kind of event inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_VALUEITEM_ONCE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerValueItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        case EVENT_ADVENTUREITEM_REPEAT:
        case EVENT_ADVENTUREITEM_NOSPRITE:
            LogMessageA(LL_WARNING, "[%s] Warning! CharacterSprite[%d] was found to have the wrong kind of event inside of this function!", __FUNCTION__, spriteIndex);
        case EVENT_ADVENTUREITEM_ONCE:
        {
            for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
            {
                GivePlayerAdventureItem(gNPCEventTable[spriteIndex].EventItemsIndex[i], gNPCEventTable[spriteIndex].EventItemsCount[i]);
            }
            break;
        }
        default:
        {
            LogMessageA(LL_ERROR, "[%s] ERROR! CharacterSprite[%d] had an unusable value as an event flag! Event = %d !", __FUNCTION__, spriteIndex, GetEventTypeFromSpriteIndex(spriteIndex));
            break;
        }
    }
    gNPCEventTable[spriteIndex].Event = EVENT_TALK;
    gCharacterSprite[spriteIndex].InteractedWith = FALSE;
}

void RemoveCharSprite(uint8_t Index)
{
    gNPCEventTable[Index].Event = EVENT_NULL;
    gCharacterSprite[Index].InteractedWith = FALSE;
    gCharacterSprite[Index].Exists = FALSE;
    gCharacterSprite[Index].Loaded = FALSE;
    gCharacterSprite[Index].Visible = FALSE;
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
        //Reset sprites that are offscreen by four tiles
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
    EVENT_TYPE WorkingEvent;
    uint8_t WorkingSprite;

    for (uint8_t Sprite = 0; Sprite <= NUM_CHAR_SPRITES; Sprite++)
    {
        if (gCharacterSprite[Sprite].InteractedWith == TRUE)
        {
            //checks if required flag is true or if no req flag was set and verifies that there is a flag to be set at all
            if ((CheckGameFlag(gCharacterSprite[Sprite].CheckFlag) | gCharacterSprite[Sprite].CheckFlag == FLAG_NULL) && gCharacterSprite[Sprite].SetFlag != FLAG_NULL)
            {
                SetGameFlag(gCharacterSprite[Sprite].SetFlag);
                gCharacterSprite[Sprite].SetFlag = FLAG_NULL;
            }
            WorkingEvent = gNPCEventTable[Sprite].Event;
            WorkingSprite = Sprite;
            result = TRUE;
            break;
        }
    }

    if (result)
    {
        switch (WorkingEvent)
        {
            case EVENT_STORE:
            {
                if (gPostDialogueMenu == TRUE)
                {
                    DrawBuySellBackBox();
                    break;
                }
            }
            default:
            {
                if (gNPCEventTable[WorkingSprite].Event != EVENT_NULL)
                {
                    DrawDialogueBox(GetCurrentDialogueFromNPC(WorkingSprite), LocalFrameCounter, NULL);
                }
                else
                {
                    LogMessageA(LL_ERROR, "[%s] gCharacterSprite[WorkingSprite].Event was EVENT_NULL in function CharSpriteInteractionHandler in OverWorld.c! WorkingSprite was %d!", __FUNCTION__, WorkingSprite);
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

//TODO: this function needs a lot of work!
// 
// 
//TODO:new list of movement scripts
//TOFIX TOTEST: need to make an xstart and ystart for every new npc controlled by the script!
//TODO DONE use table gSceneStartingPosition to set up
//TODO DONE if gSceneStartingPosition has a x,y of 0,0 then use the existing npc coordinates from gCharacterSprite, just as QOL
// 
//TODO:make this function accept a value that then forces the player to move while controls are disabled
// currently it would need to create a giant if statement that wou ld swap gCharacter sprite movement with gPlayer movement
//  ||
//  \/
 
 
//returns FALSE when last script has finished executing and function has reset for the next array of scripts to be passed into it
BOOL ApplyMovementScriptSprite(_In_ SCENE_SCRIPT scriptarray[], _In_ uint16_t xstart, _In_ uint16_t ystart, _In_ BOOL removeSpriteAfterScript)
{
    ///this function will be called every frame, yet I want it to queue up actions to be done at 30 frame intervals but updated & displayed every frame.
    BOOL returnval = TRUE;
    static uint16_t localframecounter;
    static uint8_t actorsthisscene[MAX_NPCS_PER_SCRIPT];

    uint8_t CurrentActorID = 0;
    MOVEMENT_SCRIPT WorkingScript = MOVE_NULL;

    if (scriptarray[gCurrentScript].Actor == 0xFF && scriptarray[gCurrentScript].Script == START_OF_SCRIPT && gCurrentScript == 0)
    {
        uint8_t ActorID = 0;

        //Initialize cutscene
        for (uint8_t npcinit = 0; npcinit < MAX_NPCS_PER_SCRIPT; npcinit++)
        {
            ActorID = gSceneStartingPosition[gCurrentScene][npcinit].Actor;
            gCharacterSprite[ActorID].Exists = TRUE;
            gCharacterSprite[ActorID].Loaded = TRUE;
            //option to leave npc invisible instead of immediately loading it as visible in scene

            if (gSceneStartingPosition[gCurrentScene][npcinit].Pos.x != 0 && gSceneStartingPosition[gCurrentScene][npcinit].Pos.y != 0)     //ignore all this if their coords are 0,0 and instead just use the coords the charsprite already has
            {
                gCharacterSprite[ActorID].WorldPos.x = gSceneStartingPosition[gCurrentScene][npcinit].Pos.x;
                gCharacterSprite[ActorID].WorldPos.y = gSceneStartingPosition[gCurrentScene][npcinit].Pos.y;
            }

        }
        localframecounter = 1;
        gCurrentScript++;
    }
    else if (scriptarray[gCurrentScript].Actor == 0xFF && scriptarray[gCurrentScript].Script == END_OF_SCRIPT)
    {
        //end cutscene
        gCurrentScript = 0;
        localframecounter = 0;
        returnval = FALSE;
    }
    else
    {
        CurrentActorID = scriptarray[gCurrentScript].Actor;
        WorkingScript = scriptarray[gCurrentScript].Script;

        switch (CurrentActorID)
        {
            case 0xFF:  //player
            {
                
                switch (WorkingScript)
                {
                    case MOVE_NULL:
                    {
                        if (localframecounter % 64 == 0)
                        {
                            gCurrentScript++;
                            //exactly identical to delay_64 except we log as a warning
                            LogMessageA(LL_WARNING, "[%s] Warning, WorkingScript in function ApplyMovementScriptSprite() was MOVE_NULL!", __FUNCTION__);
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
                            gPlayer.Direction = DOWN;
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
                            gPlayer.Direction = LEFT;
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
                            gPlayer.Direction = UP;
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
                            gPlayer.Direction = RIGHT;
                        }
                        break;
                    }
                    case WALK_DOWN:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = DOWN;

                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.y++;
                            gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_LEFT:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = LEFT;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.x--;
                            gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_UP:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = UP;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.y--;
                            gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_RIGHT:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = RIGHT;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.x++;
                            gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_DOWN:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = UP;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.y++;
                            gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_LEFT:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = RIGHT;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.x--;
                            gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_UP:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = DOWN;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.y--;
                            gPlayer.ScreenPos.y = gPlayer.WorldPos.y - gCamera.y;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_RIGHT:
                    {
                        if (!gPlayer.MovementRemaining)
                        {
                            gPlayer.MovementRemaining = 16;
                            gPlayer.Direction = LEFT;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            switch (gPlayer.MovementRemaining)
                            {
                                case 15:
                                {
                                    gPlayer.SpriteIndex = 2;
                                    break;
                                }
                                case 7:
                                {
                                    gPlayer.SpriteIndex = 1;
                                    break;
                                }
                            }
                            gPlayer.WorldPos.x++;
                            gPlayer.ScreenPos.x = gPlayer.WorldPos.x - gCamera.x;
                            gPlayer.MovementRemaining--;
                            if (!gPlayer.MovementRemaining)
                            {
                                gPlayer.SpriteIndex = 0;
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
                    case START_OF_SCRIPT:
                    case END_OF_SCRIPT:
                    {
                        break;
                    }
                    default:
                    {
                        LogMessageA(LL_ERROR, "[%s] Error, Working script was unknown value in ApplyMovementScript! Value of %d!", __FUNCTION__, WorkingScript);
                        break;
                    }
                }

                break;
            }
            default:    //NPC
            {
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
                            gCharacterSprite[CurrentActorID].Direction = DOWN;
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
                            gCharacterSprite[CurrentActorID].Direction = LEFT;
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
                            gCharacterSprite[CurrentActorID].Direction = UP;
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
                            gCharacterSprite[CurrentActorID].Direction = RIGHT;
                        }
                        break;
                    }
                    case WALK_DOWN:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = DOWN;
                            gCharacterSprite[CurrentActorID].WorldPos.y++;
                            gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                            {
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_LEFT:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = LEFT;
                            gCharacterSprite[CurrentActorID].WorldPos.x--;
                            gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                            {
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_UP:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = UP;
                            gCharacterSprite[CurrentActorID].WorldPos.y--;
                            gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                            {
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_RIGHT:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = RIGHT;
                            gCharacterSprite[CurrentActorID].WorldPos.x++;
                            gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                            {
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_DOWN:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = UP;
                            gCharacterSprite[CurrentActorID].WorldPos.y++;
                            gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                            {
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_LEFT:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = RIGHT;
                            gCharacterSprite[CurrentActorID].WorldPos.x--;
                            gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                            {
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_UP:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = DOWN;
                            gCharacterSprite[CurrentActorID].WorldPos.y--;
                            gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                            {
                                gCurrentScript++;
                            }
                        }
                        break;
                    }
                    case WALK_BW_RIGHT:
                    {
                        if (!gCharacterSprite[CurrentActorID].MovementRemaining)
                        {
                            gCharacterSprite[CurrentActorID].MovementRemaining = 16;
                        }
                        else if (localframecounter % 2 == 0)
                        {
                            gCharacterSprite[CurrentActorID].Direction = LEFT;
                            gCharacterSprite[CurrentActorID].WorldPos.x++;
                            gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
                            gCharacterSprite[CurrentActorID].MovementRemaining--;
                            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
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
                    case START_OF_SCRIPT:
                    case END_OF_SCRIPT:
                    {
                        break;
                    }
                    default:
                    {
                        LogMessageA(LL_ERROR, "[%s] Error, Working script was unknown value in ApplyMovementScript! Value of %d!", __FUNCTION__, WorkingScript);
                        break;
                    }
                }

                break;
            }
        }
    }

    localframecounter++;
    return(returnval);







    /////THIS SUCKS!!! TOREMOVE
//
//
//
//newactor:
//
//    CurrentActorID = scriptarray[gCurrentScript].Actor;
//
//    //test for if CurrentActorID is the player
//    if (CurrentActorID == 0xFF)
//    {
//        
//    }
//    else
//    {
//
//    }
//
//
//    //if current actor is applying movement first time this scene then store it
//    for (uint8_t actororder = 0; actororder < MAX_NPCS_PER_SCRIPT; actororder++)
//    {
//        if (actorsthisscene[actororder] == 0)
//        {
//            if (scriptarray[gCurrentScript].Script == START_OF_SCRIPT)   //catches the exception where start_of_script uses the player's identifier
//            {
//                break;
//            }
//
//            //store
//            actorsthisscene[actororder] = CurrentActorID;
//
//            if (gSceneStartingPosition[gCurrentScene][actororder].Actor != CurrentActorID)
//            {
//                //we somehow are out of sync between gSceneStartingPosition and gSceneScriptArray(which reads from gSceneScriptTable)
//                LogMessageA(LL_ERROR, "[%s] ERROR! gSceneStartingPostition and gSceneScriptArray are out of sync! %d vs %d !", __FUNCTION__, gSceneStartingPosition[gCurrentScene][actororder].Actor, CurrentActorID);
//            }
//            else
//            {
//                if (gSceneStartingPosition[gCurrentScene][actororder].Pos.x == 0)
//                {
//                    //leave gCharacterSprite with its normal x coordinates
//                }
//                else
//                {
//                    gCharacterSprite[CurrentActorID].WorldPos.x = gSceneStartingPosition[gCurrentScene][actororder].Pos.x;
//                }
//
//                if (gSceneStartingPosition[gCurrentScene][actororder].Pos.y == 0)
//                {
//                    //leave gCharacterSprite with its normal y coordinates
//                }
//                else
//                {
//                    gCharacterSprite[CurrentActorID].WorldPos.y = gSceneStartingPosition[gCurrentScene][actororder].Pos.y;
//                }
//
//                gCharacterSprite[CurrentActorID].Exists = TRUE;
//                gCharacterSprite[CurrentActorID].Loaded = TRUE;
//                gCharacterSprite[CurrentActorID].Visible = TRUE;
//            }
//
//            break;
//        }
//        else if (actorsthisscene[actororder] == CurrentActorID)
//        {
//            break;
//        }
//    }
//
//    if (scriptarray[gCurrentScript].Script == START_OF_SCRIPT)
//    {
//        gCurrentScript++;
//        localframecounter = 1;
//
//        //restarts function after being initialized
//        goto newactor;
//    }
//
//    MOVEMENT_SCRIPT WorkingScript = scriptarray[gCurrentScript].Script;
//
//    switch (WorkingScript)
//    {
//        case MOVE_NULL:
//        {
//            if (localframecounter % 64 == 0)
//            {
//                gCurrentScript++;
//                //exactly identical to delay_64 except we log as a warning
//                LogMessageA(LL_WARNING, "[%s] Warning, WorkingScript in function ApplyMovementSprite() was MOVE_NULL!", __FUNCTION__);
//            }
//            break;
//        }
//        case FACE_DOWN:
//        {
//            if (localframecounter % 32 == 0)
//            {
//                gCurrentScript++;
//            }
//            else if (localframecounter % 16 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = DOWN;
//            }
//            break;
//        }
//        case FACE_LEFT:
//        {
//            if (localframecounter % 32 == 0)
//            {
//                gCurrentScript++;
//            }
//            else if (localframecounter % 16 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = LEFT;
//            }
//            break;
//        }
//        case FACE_UP:
//        {
//            if (localframecounter % 32 == 0)
//            {
//                gCurrentScript++;
//            }
//            else if (localframecounter % 16 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = UP;
//            }
//            break;
//        }
//        case FACE_RIGHT:
//        {
//            if (localframecounter % 32 == 0)
//            {
//                gCurrentScript++;
//            }
//            else if (localframecounter % 16 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = RIGHT;
//            }
//            break;
//        }
//        case WALK_DOWN:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = DOWN;
//                gCharacterSprite[CurrentActorID].WorldPos.y++;
//                gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case WALK_LEFT:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = LEFT;
//                gCharacterSprite[CurrentActorID].WorldPos.x--;
//                gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case WALK_UP:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = UP;
//                gCharacterSprite[CurrentActorID].WorldPos.y--;
//                gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case WALK_RIGHT:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = RIGHT;
//                gCharacterSprite[CurrentActorID].WorldPos.x++;
//                gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case WALK_BW_DOWN:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = UP;
//                gCharacterSprite[CurrentActorID].WorldPos.y++;
//                gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case WALK_BW_LEFT:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = RIGHT;
//                gCharacterSprite[CurrentActorID].WorldPos.x--;
//                gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case WALK_BW_UP:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = DOWN;
//                gCharacterSprite[CurrentActorID].WorldPos.y--;
//                gCharacterSprite[CurrentActorID].ScreenPos.y = gCharacterSprite[CurrentActorID].WorldPos.y - gCamera.y;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case WALK_BW_RIGHT:
//        {
//            if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//            {
//                gCharacterSprite[CurrentActorID].MovementRemaining = 16;
//            }
//            else if (localframecounter % 2 == 0)
//            {
//                gCharacterSprite[CurrentActorID].Direction = LEFT;
//                gCharacterSprite[CurrentActorID].WorldPos.x++;
//                gCharacterSprite[CurrentActorID].ScreenPos.x = gCharacterSprite[CurrentActorID].WorldPos.x - gCamera.x;
//                gCharacterSprite[CurrentActorID].MovementRemaining--;
//                if (!gCharacterSprite[CurrentActorID].MovementRemaining)
//                {
//                    gCurrentScript++;
//                }
//            }
//            break;
//        }
//        case DELAY_16:
//        {
//            if (localframecounter % 16 == 0)
//            {
//                gCurrentScript++;
//            }
//            break;
//        }
//        case DELAY_32:
//        {
//            if (localframecounter % 32 == 0)
//            {
//                gCurrentScript++;
//            }
//            break;
//        }
//        case DELAY_48:
//        {
//            if (localframecounter % 48 == 0)
//            {
//                gCurrentScript++;
//            }
//            break;
//        }
//        case DELAY_64:
//        {
//            if (localframecounter % 64 == 0)
//            {
//                gCurrentScript++;
//            }
//            break;
//        }
//        case DIALOGUE_TRIGGER:
//        {
//            //delay increasing gCurrentScript until player input
//            break;
//        }
//        case START_OF_SCRIPT:
//        case END_OF_SCRIPT:
//        {
//            break;
//        }
//        default: 
//        {
//            LogMessageA(LL_ERROR, "[%s] Error, Working script was unknown value in ApplyMovementScript! Value of %d!", __FUNCTION__, WorkingScript);
//            break;
//        }
//    }
//
//    if (scriptarray[gCurrentScript].Script == END_OF_SCRIPT)
//    {
//        if (removeSpriteAfterScript)
//        {
//            //TODO: this only removes the last sprite, not all of them
//            gCharacterSprite[CurrentActorID].Exists = FALSE;
//            gCharacterSprite[CurrentActorID].Loaded = FALSE;
//            gCharacterSprite[CurrentActorID].Visible = FALSE;
//        }
//        gCurrentScript = 0;
//        localframecounter = 0;
//        //reset how i keep track of the actors this scene
//        for (uint8_t i = 0; i < MAX_NPCS_PER_SCRIPT; i++)
//        {
//            actorsthisscene[i] = 0;
//        }
//        return(FALSE);
//    }
//
//    localframecounter++;
//    return(TRUE);
}

BOOL CheckNPCHasLOSWithPlayer(uint8_t spriteId)
{
    BOOL result = FALSE;
    if (gNPCEventTable[spriteId].Event == EVENT_BATTLE)
    {
        switch (gCharacterSprite[spriteId].Direction)
        {
            case DOWN:
            {
                if (((gCharacterSprite[spriteId].WorldPos.y + (gCharacterSprite[spriteId].SightRange * 16) >= gPlayer.WorldPos.y) && gCharacterSprite[spriteId].WorldPos.y <= gPlayer.WorldPos.y) && (gCharacterSprite[spriteId].WorldPos.x == gPlayer.WorldPos.x) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gNPCEventTable[spriteId].Event == EVENT_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
                {
                    gPlayer.Direction = UP;
                    result = TRUE;
                }
                break;
            }
            case LEFT:
            {
                if (((gCharacterSprite[spriteId].WorldPos.x - (gCharacterSprite[spriteId].SightRange * 16) <= gPlayer.WorldPos.x) && gCharacterSprite[spriteId].WorldPos.x >= gPlayer.WorldPos.x) && (gCharacterSprite[spriteId].WorldPos.y == gPlayer.WorldPos.y) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gNPCEventTable[spriteId].Event == EVENT_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
                {
                    gPlayer.Direction = RIGHT;
                    result = TRUE;
                }
                break;
            }
            case RIGHT:
            {
                if (((gCharacterSprite[spriteId].WorldPos.x + (gCharacterSprite[spriteId].SightRange * 16) >= gPlayer.WorldPos.x) && gCharacterSprite[spriteId].WorldPos.x <= gPlayer.WorldPos.x) && (gCharacterSprite[spriteId].WorldPos.y == gPlayer.WorldPos.y) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gNPCEventTable[spriteId].Event == EVENT_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
                {
                    gPlayer.Direction = LEFT;
                    result = TRUE;
                }
                break;
            }
            case UP:
            {
                if (((gCharacterSprite[spriteId].WorldPos.y - (gCharacterSprite[spriteId].SightRange * 16) <= gPlayer.WorldPos.y) && gCharacterSprite[spriteId].WorldPos.y >= gPlayer.WorldPos.y) && (gCharacterSprite[spriteId].WorldPos.x == gPlayer.WorldPos.x) && !(gCharacterSprite[spriteId].Movement == MOVEMENT_SPARKLE || gCharacterSprite[spriteId].Movement == MOVEMENT_ITEMPICKUP || gNPCEventTable[spriteId].Event == EVENT_NULL) && gCharacterSprite[spriteId].Loaded == TRUE)
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

        //not using this return value???
        CharSpriteInteractionHandler(counter);

        if (gScriptActive && gSceneScriptArray[gCurrentScript].Script == DIALOGUE_TRIGGER)
        {
            //TODO: make better way of knowing what scene is what and what actor is what, this is prone to mistakes

            DrawDialogueBox(GetCurrentDialogueFromNPC(gSceneScriptArray[gCurrentScript].Actor), counter, NULL);
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

            if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) + 1][gPlayer.WorldPos.x / 16] == gPassableTiles[Counter])    //for preventing the player from walking to a passable tile above the cliff tile
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
    for (uint8_t telepads = 0; telepads < NUM_TELEPORT_TILES; telepads++)   //Teleport tiles
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

    for (uint8_t index = 0; index < NUM_CHAR_SPRITES; index++)      ////NPC sees Player
    {
        BOOL PlayerInLOS = CheckNPCHasLOSWithPlayer(index);

        if (PlayerInLOS)
        {
            gCharacterSprite[index].InteractedWith = TRUE;
            EnterDialogue();
            break;
        }
    }

    for (uint8_t triggers = 0; triggers < NUM_TRIGGERS; triggers++)     //Trigger tiles
    {

        BOOL TriggerActivated = IsPlayerOnActiveTrigger(triggers);

        if (TriggerActivated)
        {
            gTriggerTiles[triggers].InteractedWith = TRUE;
            EnterDialogue();
            TriggerInteractionHandler();
            break;
        }

    }

    for (uint8_t EncounterTile = 0; EncounterTile < _countof(gEncounterTiles); EncounterTile++)     ///Wild encounter tile
    {
        if ((gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16)] == gEncounterTiles[EncounterTile]) && (gPlayer.StepsTaken - gPlayer.StepsSinceLastEncounter > BATTLE_ENCOUNTER_GRACE_PERIOD))
        {
            DWORD Random = 0;

            rand_s((unsigned int*)&Random);

            Random = Random % 1000;

            if (Random > (1000 - gPlayer.RandomEncounterPercent))
            {
                gPlayer.StepsSinceLastEncounter = gPlayer.StepsTaken;
                GoToDestGamestate(GAMESTATE_BATTLE_MONSTER);
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
            ///TODO: GetEventFlagReq();     /// this will check if the sprite has an alternate event/dialogue locked behind a gGameFlag[]

            switch (GetEventTypeFromSpriteIndex(Index))
            {
                case EVENT_TALK:
                {
                    if (CheckIfLastDialogueNPC(Index))
                    {
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gGamePaused = FALSE;
                        gOverWorldControls = TRUE;
                        ///TODO: GetEventFlagToSet();
                    }
                    gDialogueControls = FALSE;
                    GoToNextDialogueNPC(Index);
                    break;
                }
                case EVENT_BATTLE:
                {
                    if (CheckIfLastDialogueNPC(Index))
                    {
                        GoToDestGamestate(GAMESTATE_BATTLE_TRAINER);
                        //gCharacterSprite[Index].InteractedWith = FALSE;           //turn off after battle has been won instead of here, so that we can reset battle if player loses
                        gGamePaused = FALSE;
                        gOverWorldControls = TRUE;
                        ///TODO: GetEventFlagToSet();
                    }
                    gDialogueControls = FALSE;
                    GoToNextDialogueNPC(Index);
                    break;
                }
                case EVENT_HEAL:
                {
                    if (CheckIfLastDialogueNPC(Index))
                    {
                        HealPlayerParty();
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gGamePaused = FALSE;
                        gOverWorldControls = TRUE;
                        ///TODO: GetEventFlagToSet();       ///the only time healing should set a flag is to unlock the city/map as a fast travel destination
                    }
                    gDialogueControls = FALSE;
                    GoToNextDialogueNPC(Index);
                    break;
                }
                case EVENT_EQUIPITEM_ONCE:
                case EVENT_USEITEM_ONCE:
                case EVENT_VALUEITEM_ONCE:
                case EVENT_ADVENTUREITEM_ONCE:
                {
                    if (CheckIfLastDialogueNPC(Index))
                    {
                        GiveItemChangeNPCEvent(Index);
                        gGamePaused = FALSE;
                        gOverWorldControls = TRUE;
                        ///TODO: GetEventFlagToSet();
                    }
                    gDialogueControls = FALSE;
                    GoToNextDialogueNPC(Index);
                    break;
                }
                case EVENT_EQUIPITEM_REPEAT:
                case EVENT_USEITEM_REPEAT:
                case EVENT_VALUEITEM_REPEAT:
                case EVENT_ADVENTUREITEM_REPEAT:
                {
                    //TODO: add .Cooldown to NPC_EVENT_DATA??? so that events can be repeatable on a cooldown
                }
                case EVENT_MONSTER:
                {
                    if (CheckIfLastDialogueNPC(Index))
                    {
                        if (gNPCEventTable[Index].EventMonsterIndex == 0 || gNPCEventTable[Index].EventMonsterLevel == 0)           //give player first index at level 1 if sprite is set up incorrectly
                        {
                            gNPCEventTable[Index].EventMonsterIndex = 1;
                            gNPCEventTable[Index].EventMonsterLevel = 1;
                            LogMessageA(LL_WARNING, "[%s] gNPCEventTable[%d] tried to give a monster to player with either 0 index or 0 level!", __FUNCTION__, Index);
                        }
                        ScriptGiveMonster(gNPCEventTable[Index].EventMonsterIndex, gNPCEventTable[Index].EventMonsterLevel, gNPCEventTable[Index].EventMonsterItem);
                        gNPCEventTable[Index].Event = EVENT_TALK;
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gGamePaused = FALSE;
                        gOverWorldControls = TRUE;
                        ///TODO: GetEventFlagToSet();
                    }
                    gDialogueControls = FALSE;
                    GoToNextDialogueNPC(Index);
                    break;
                }
                case EVENT_MOVEMENT:
                {
                    //TODO:... start a script???
                }
                case EVENT_STORE:
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
                                GoToDestGamestate(GAMESTATE_STORE);

                                //gNPCEventTable[Index].DialogueFlag = gNPCEventTable[Index].DialogueLoopReturn;
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
                                GoToDestGamestate(GAMESTATE_INVENTORYSCREEN);
                                gPreviousPockets = gCurrentPockets;
                                gCurrentPockets = POCKETSTATE_VALUABLE;
                                gSellingItems = TRUE;       //sell mode

                                //gNPCEventTable[Index].DialogueFlag = gNPCEventTable[Index].DialogueLoopReturn;
                                gCharacterSprite[Index].InteractedWith = FALSE;
                                gGamePaused = FALSE;
                                gDialogueControls = FALSE;
                                gOverWorldControls = TRUE;
                                gPostDialogueMenu = FALSE;
                                break;
                            case 3:
                                //back
                                ////reset dialogueflag and beforeloop to give a goodbye message when exiting

                                //gNPCEventTable[Index].DialogueFlag = gNPCEventTable[Index].DialogueLoopReturn;
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
                        GoToNextDialogueNPC(Index);
                    }
                    else if (gPostDialogueMenu == FALSE)
                    {
                        gPostDialogueMenu = CheckIfLastDialogueNPC(Index);
                        GoToNextDialogueNPC(Index);
                    }
                    break;
                }
                case EVENT_EQUIPITEM_NOSPRITE:
                case EVENT_USEITEM_NOSPRITE:
                case EVENT_VALUEITEM_NOSPRITE:
                case EVENT_ADVENTUREITEM_NOSPRITE:
                {
                    if (CheckIfLastDialogueNPC(Index))
                    {
                        GiveItemAndRemoveSprite(Index);
                        gGamePaused = FALSE;
                        gOverWorldControls = TRUE;
                    }
                    gDialogueControls = FALSE;
                    GoToNextDialogueNPC(Index);
                    break;
                }
                case EVENT_DRIVE_STORAGE:
                {
                    if (CheckIfLastDialogueNPC(Index))
                    {
                        GoToDestGamestate(GAMESTATE_DRIVE_STORAGE);
                        gCharacterSprite[Index].Direction = gCharacterSprite[Index].ResetDirection;
                        gCharacterSprite[Index].InteractedWith = FALSE;
                        gGamePaused = FALSE;
                        gOverWorldControls = TRUE;
                    }
                    gDialogueControls = FALSE;
                    GoToNextDialogueNPC(Index);
                    break;
                }
                case EVENT_NULL:
                default:
                {
                    LogMessageA(LL_ERROR, "[%s] ERROR! EVENT_FLAG associated with gCharacterSprite[%d] was EVENT_FLAG_NULL or unrecognized! EVENT_FLAG = %d !", __FUNCTION__, Index, GetEventTypeFromSpriteIndex(Index));
                    break;
                }
            }
        }
    }

    //for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
    //{
    //    if (gCharacterSprite[Index].InteractedWith == TRUE)
    //    {
    //        switch (gCharacterSprite[Index].Event)
    //        {
    //            case EVENT_FLAG_BATTLE:
    //            {
    //                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
    //                {
    //                    TrainerEncounter(&gPreviousGameState, &gCurrentGameState);
    //                    gGamePaused = FALSE;
    //                    gDialogueControls = FALSE;
    //                    gOverWorldControls = TRUE;
    //                }
    //                else
    //                {
    //                    gCharacterSprite[Index].DialogueFlag++;
    //                    gDialogueControls = FALSE;
    //                }
    //                break;
    //            }
    //            case EVENT_FLAG_MONSTER:
    //            {
    //                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
    //                {
    //                    if (gCharacterSprite[Index].EventMonsterIndex == 0 || gCharacterSprite[Index].EventMonsterLevel == 0)           //give player first index at level 1 if sprite is set up incorrectly
    //                    {
    //                        gCharacterSprite[Index].EventMonsterIndex = 1;
    //                        gCharacterSprite[Index].EventMonsterLevel = 1;
    //                        LogMessageA(LL_WARNING, "[%s] gCharacterSprite[%d] tried to give a monster to player with either 0 index or 0 level!", __FUNCTION__, Index);
    //                    }
    //                    ScriptGiveMonster(gCharacterSprite[Index].EventMonsterIndex, gCharacterSprite[Index].EventMonsterLevel, gCharacterSprite[Index].EventMonsterItem);
    //                    gCharacterSprite[Index].Event = EVENT_FLAG_TALK;
    //                    gCharacterSprite[Index].InteractedWith = FALSE;
    //                    gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
    //                    gGamePaused = FALSE;
    //                    gDialogueControls = FALSE;
    //                    gOverWorldControls = TRUE;
    //                }
    //                else
    //                {
    //                    gCharacterSprite[Index].DialogueFlag++;
    //                    gDialogueControls = FALSE;
    //                }
    //                break;
    //            }
    //            case EVENT_FLAG_HEAL:
    //            {
    //                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)        //for when dialogues are longer than one box
    //                {
    //                    HealPlayerParty();  //call heal
    //                    gCharacterSprite[Index].InteractedWith = FALSE;
    //                    gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
    //                    gGamePaused = FALSE;
    //                    gDialogueControls = FALSE;
    //                    gOverWorldControls = TRUE;
    //                }
    //                else
    //                {
    //                    gCharacterSprite[Index].DialogueFlag++;
    //                    gDialogueControls = FALSE;
    //                }
    //                break;
    //            }
    //            case EVENT_FLAG_EQUIPITEM_ONCE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_EQUIP);
    //                break;
    //            }
    //            case EVENT_FLAG_USEITEM_ONCE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_USE);
    //                break;
    //            }
    //            case EVENT_FLAG_VALUEITEM_ONCE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_VALUE);
    //                break;
    //            }
    //            case EVENT_FLAG_ADVENTUREITEM_ONCE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharSpriteEvent(gCharacterSprite[Index], ITEMTYPE_ADVENTURE);
    //                break;
    //            }
    //            case EVENT_FLAG_EQUIPITEM_NOSPRITE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_EQUIP);
    //                break;
    //            }
    //            case EVENT_FLAG_USEITEM_NOSPRITE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_USE);
    //                break;
    //            }
    //            case EVENT_FLAG_VALUEITEM_NOSPRITE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_VALUE);
    //                break;
    //            }
    //            case EVENT_FLAG_ADVENTUREITEM_NOSPRITE:
    //            {
    //                gCharacterSprite[Index] = GivePlayerItemFromCharAndRemoveSprite(gCharacterSprite[Index], ITEMTYPE_ADVENTURE);
    //                break;
    //            }
    //            case EVENT_FLAG_STORE:
    //            {
    //                if (gPostDialogueMenu == TRUE)
    //                {
    //                    ///////say a dialogue then ask if player wants to buy/sell/exit 
    //                    uint8_t StoreButtons = PPI_BuySellBackBox();

    //                    switch (StoreButtons)
    //                    {
    //                        case 1:
    //                            //buy
    //                            ////create a new store.c file? and send us to that gamestate when buying items?? 
    //                            gPreviousGameState = gCurrentGameState;
    //                            gCurrentGameState = GAMESTATE_STORE;
    //                            gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
    //                            gCharacterSprite[Index].InteractedWith = FALSE;
    //                            gGamePaused = FALSE;
    //                            gDialogueControls = FALSE;
    //                            gOverWorldControls = TRUE;
    //                            gPostDialogueMenu = FALSE;
    //                            gStoreSpriteIndex = Index;
    //                            break;
    //                        case 2:
    //                            //sell
    //                            ////go to gamestatepockets->valuablepockets and check a variable to put us in "sell" mode
    //                            gPreviousGameState = gCurrentGameState;
    //                            gCurrentGameState = GAMESTATE_INVENTORYSCREEN;
    //                            gPreviousPockets = gCurrentPockets;
    //                            gCurrentPockets = POCKETSTATE_VALUABLE;
    //                            gSellingItems = TRUE;       //sell mode
    //                            gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
    //                            gCharacterSprite[Index].InteractedWith = FALSE;
    //                            gGamePaused = FALSE;
    //                            gDialogueControls = FALSE;
    //                            gOverWorldControls = TRUE;
    //                            gPostDialogueMenu = FALSE;
    //                            break;
    //                        case 3:
    //                            //back
    //                            ////reset dialogueflag and beforeloop to give a goodbye message when exiting
    //                            gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
    //                            gCharacterSprite[Index].InteractedWith = FALSE;
    //                            gGamePaused = FALSE;
    //                            gDialogueControls = FALSE;
    //                            gOverWorldControls = TRUE;
    //                            gPostDialogueMenu = FALSE;
    //                            break;
    //                        case 0:
    //                        default:
    //                        {
    //                            break;
    //                        }
    //                    }
    //                }
    //                else if (gPostDialogueMenu == FALSE)
    //                {
    //                    if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)     //not <= for stores extra dialogue while buy/sell/back
    //                    {
    //                        gPostDialogueMenu = TRUE;
    //                        gCharacterSprite[Index].DialogueFlag++;
    //                    }
    //                    else
    //                    {
    //                        gCharacterSprite[Index].DialogueFlag++;
    //                        gDialogueControls = FALSE;
    //                    }
    //                }
    //                break;
    //            }
    //            case EVENT_FLAG_DRIVE_STORAGE:
    //            {
    //                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)     //not <= for extra dialogue while booting storage
    //                {

    //                    gPreviousGameState = gCurrentGameState;
    //                    gCurrentGameState = GAMESTATE_DRIVE_STORAGE;
    //                    gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
    //                    gCharacterSprite[Index].Direction = gCharacterSprite[Index].ResetDirection;
    //                    gCharacterSprite[Index].InteractedWith = FALSE;
    //                    gGamePaused = FALSE;
    //                    gDialogueControls = FALSE;
    //                    gOverWorldControls = TRUE;
    //                    gPostDialogueMenu = FALSE;
    //                }
    //                else
    //                {
    //                    gCharacterSprite[Index].DialogueFlag++;
    //                    gDialogueControls = FALSE;
    //                }
    //                break;
    //            }
    //            ////Just a normal dialogue
    //            default:
    //            {
    //                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
    //                {
    //                    gCharacterSprite[Index].InteractedWith = FALSE;
    //                    gGamePaused = FALSE;
    //                    gDialogueControls = FALSE;
    //                    gOverWorldControls = TRUE;
    //                }
    //                else
    //                {
    //                    gCharacterSprite[Index].DialogueFlag++;
    //                    gDialogueControls = FALSE;
    //                }
    //                break;
    //            }
    //        }
    //    }
    //    break;      //break out of for loop searching for more sprites interacted with, obviously if we just interacted with one there shouldn't be more
    //}

}

BOOL IsPlayerOnActiveTrigger(uint8_t index)
{
    BOOL result = FALSE;
    if (gTriggerTiles[index].Loaded && gTriggerTiles[index].Exists && (gTriggerTiles[index].WorldPos.x == gPlayer.WorldPos.x) && (gTriggerTiles[index].WorldPos.y == gPlayer.WorldPos.y) && !CheckGameFlag(gTriggerTiles[index].GameFlag))
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
        if (gTriggerTiles[tile].InteractedWith == TRUE)
        {
            //TODO: move this so it happens at the end of a scene or interaction instead of at the beginning////////////////////
            //set flag upon stepping on a non null trigger
            if (gTriggerTiles[tile].SetFlag != FLAG_NULL)
            {
                SetGameFlag(gTriggerTiles[tile].SetFlag);
                gTriggerTiles[tile].SetFlag = FLAG_NULL;
            }
            //////////////////////////////////////////////

            workingtile = gTriggerTiles[tile];
            tileindex = tile;
            result = TRUE;
            break;
        }
    }

    if (result)
    {
        switch (workingtile.Type)
        {
            case TRIGGER_TYPE_INFINITE:
            case TRIGGER_TYPE_ONCE:
            {
                if (workingtile.Type == TRIGGER_TYPE_ONCE)
                {
                     gTriggerTiles[tileindex].Type = TRIGGER_TYPE_BLANK;
                     gTriggerTiles[tileindex].Exists = FALSE;
                }
            }

            ///TODO: case TRIGGER_TYPE_COOLDOWN: ....etc

            gTriggerTiles[tileindex].InteractedWith = FALSE;

            PopulateSceneScriptArray(tileindex);

            gScriptActive = TRUE;
            break;

            case TRIGGER_TYPE_BLANK:
            {
                result = FALSE;
                break;
            }

            default: 
            {
                //assuming NULL or BLANK flags were accidentially triggered
                LogMessageA(LL_ERROR, "[%s] ERROR! TRIGGER_TYPE_NULL or unrecognized value for workingtile.type in OverWorld.c! Value = %d", __FUNCTION__, workingtile.Type);
                if (workingtile.Exists == TRUE && gTriggerTiles[tileindex].Exists == TRUE)
                {
                    gTriggerTiles[tileindex].Exists = FALSE;
                    LogMessageA(LL_ERROR, "[%s] Success! workingtile.type was found to be enabled while NULL or using an unrecognized value, successfully disabled tile with .exists = FALSE! Index = %d!", __FUNCTION__, tileindex);
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

void PopulateSceneScriptArray(uint8_t currentscene)
{
    ClearSceneScriptArray();

    gCurrentScene = currentscene;

    for (uint8_t i = 0; i < MAX_MOVEMENT_SCRIPTS; i++)
    {
        gSceneScriptArray[i].Actor = gSceneScriptTable[currentscene][i].Actor;
        gSceneScriptArray[i].Script = gSceneScriptTable[currentscene][i].Script;
        if (gSceneScriptTable[currentscene][i].Script == END_OF_SCRIPT)
        {
            break;
        }
    }
}

void ClearSceneScriptArray(void)
{
    for (uint8_t i = 0; i < MAX_MOVEMENT_SCRIPTS; i++)
    {
        switch (i)
        {
            case 0:
            {
                gSceneScriptArray[i].Script = START_OF_SCRIPT;
                gSceneScriptArray[i].Actor = 0xFF;
                break;
            }
            case 1:
            {
                gSceneScriptArray[i].Script = END_OF_SCRIPT;
                gSceneScriptArray[i].Actor = 0xFF;
                break;
            }
            default:
            {
                gSceneScriptArray[i].Script = NULL_SCRIPT;
                gSceneScriptArray[i].Actor = 0;
                break;
            }
        }
    }
}