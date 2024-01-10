#include "Main.h"

#include "OverWorld.h"

#include "MonsterData.h"

#include "Inventory.h"
#include "InventoryItems.h"

BOOL gFade = FALSE;

void DrawOverworldScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

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
            //PlayGameMusic(&gMusicOverWorld01);
            PlayGameMusic(gCurrentArea.Music, TRUE, TRUE);
        }
    }


    BlitBackgroundToBuffer(&gOverWorld01.GameBitmap, BrightnessAdjustment);


    if (gGamePaused == FALSE)
    {
        TriggerNPCMovement(LocalFrameCounter, MOVEMENT_SPIN);
    }

    ModifyCharVisibility();

    for (uint8_t Index = 0; Index <= NUM_CHAR_SPRITES; Index++)
    {
        if (gCharacterSprite[Index].MovementRemaining >= 0 && gCharacterSprite[Index].Visible == TRUE)
        {
            switch (gCharacterSprite[Index].MovementRemaining)
            {
                case 16:
                {
                    gCharacterSprite[Index].SpriteIndex = 0;
                    break;
                }
                case 15:
                {
                    gCharacterSprite[Index].SpriteIndex = 1;
                    break;
                }
                case 11:
                {
                    gCharacterSprite[Index].SpriteIndex = 1;
                    break;
                }
                case 7:
                {
                    gCharacterSprite[Index].SpriteIndex = 2;
                    break;
                }
                case 3:
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
        else if ((gCharacterSprite[Index].Visible == FALSE) && !(((gCharacterSprite[Index].ResetScreenPos.x >= -64) && (gCharacterSprite[Index].ResetScreenPos.x < GAME_RES_WIDTH + 48)) && ((gCharacterSprite[Index].ResetScreenPos.y >= -64) && (gCharacterSprite[Index].ResetScreenPos.y < GAME_RES_HEIGHT + 48))))        ///////reset any sprite off screen
        {
            gCharacterSprite[Index].ScreenPos = gCharacterSprite[Index].ResetScreenPos;
            gCharacterSprite[Index].WorldPos = gCharacterSprite[Index].ResetWorldPos;
            gCharacterSprite[Index].Direction = gCharacterSprite[Index].ResetDirection;
            //gCharacterSprite[Index].SightRange = gCharacterSprite[Index].ResetSightRange;             ////only for debugging (will reset with an item?)
        }
    }

    Blit32BppBitmapToBuffer(&gPlayer.Sprite[gPlayer.CurrentSuit][gPlayer.SpriteIndex + gPlayer.Direction], gPlayer.ScreenPos.x, gPlayer.ScreenPos.y, BrightnessAdjustment);

    if (gOverWorldControls == FALSE)
    {
        for (uint8_t Sprite = 0; Sprite <= NUM_CHAR_SPRITES; Sprite++)
        {
            if (gCharacterSprite[Sprite].InteractedWith == TRUE) 
            {
                DrawDialogueBox(gCharacterSprite[Sprite].Dialogue[gCharacterSprite[Sprite].DialogueFlag], LocalFrameCounter, NULL);
                goto FoundSprite;
            }
        }
    }
FoundSprite:



    if (gGamePerformanceData.DisplayDebugInfo)
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

        //ASSERT(gCamera.x <= gCurrentArea.right - GAME_RES_WIDTH, "Camera went out of bounds!");

        //ASSERT(gCamera.y <= gCurrentArea.bottom - GAME_RES_HEIGHT, "Camera went out of bounds!");

        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
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
                                gCharacterSprite[Index].ScreenPos.x = gPlayer.ScreenPos.x + 16;
                                gCharacterSprite[Index].ScreenPos.y = gPlayer.ScreenPos.y;
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
                                gCharacterSprite[Index].ScreenPos.y = gPlayer.ScreenPos.y - 16;
                                gCharacterSprite[Index].ScreenPos.x = gPlayer.ScreenPos.x;
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
                                gCharacterSprite[Index].ScreenPos.x = gPlayer.ScreenPos.x - 16;
                                gCharacterSprite[Index].ScreenPos.y = gPlayer.ScreenPos.y;
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
                                gCharacterSprite[Index].ScreenPos.y = gPlayer.ScreenPos.y + 16;
                                gCharacterSprite[Index].ScreenPos.x = gPlayer.ScreenPos.x;
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

        if (!gPlayer.MovementRemaining)
        {
            BOOL CanMoveToDesiredTile = FALSE;
            BOOL NoSpriteCollision = FALSE;
            if (gGameInput.SDownKeyPressed)
            {
                gPlayer.Direction = DOWN;
                for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
                {
                    if (((gPlayer.WorldPos.y / 16) + 1 > gOverWorld01.TileMap.Height - 1))      ////not perfect but prevents crashing when walking to bottom of world
                    {
                        break;
                    }

                    if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) + 1][gPlayer.WorldPos.x / 16] == gPassableTiles[Counter])
                    {
                        CanMoveToDesiredTile = TRUE;

                        break;
                    }
                }
                for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                {
                    if (gCharacterSprite[Index].Visible == TRUE)
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
                for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
                {
                    if (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) - 1] == gPassableTiles[Counter])
                    {
                        CanMoveToDesiredTile = TRUE;

                        break;
                    }
                }
                for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                {
                    if (gCharacterSprite[Index].Visible == TRUE)
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
                for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
                {
                    if (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) + 1] == gPassableTiles[Counter])
                    {
                        CanMoveToDesiredTile = TRUE;

                        break;
                    }
                }
                for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                {
                    //////this big line of code means: if any pixel of 16x16 sprite is in the way of moving
                    if (gCharacterSprite[Index].Visible == TRUE)
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
                    for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
                    {
                        if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16) - 1][gPlayer.WorldPos.x / 16] == gPassableTiles[Counter])
                        {
                            CanMoveToDesiredTile = TRUE;

                            break;
                        }
                    }
                    for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                    {
                        if (gCharacterSprite[Index].Visible == TRUE)
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
        else
        {
            gPlayer.MovementRemaining--;


            if (gPlayer.Direction == DOWN)
            {
                if (gPlayer.ScreenPos.y < GAME_RES_HEIGHT - 128)   ////keeps player centered
                {
                    gPlayer.ScreenPos.y++;
                }
                else
                {
                    if (gCamera.y < gCurrentArea.Area.bottom - GAME_RES_HEIGHT)
                    {
                        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                            {
                                gCharacterSprite[Index].ScreenPos.y--;
                                gCharacterSprite[Index].ResetScreenPos.y--;
                                ModifyCharVisibility();
                            }
                        }
                        gCamera.y++;
                    }
                    else
                    {
                        gPlayer.ScreenPos.y++;
                    }
                }
                gPlayer.WorldPos.y++;
            }
            else if (gPlayer.Direction == LEFT)
            {
                if (gPlayer.ScreenPos.x > 192)   ////keeps player centered
                {

                    gPlayer.ScreenPos.x--;
                }
                else
                {
                    if (gCamera.x > gCurrentArea.Area.left)
                    {
                        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                            {
                                gCharacterSprite[Index].ScreenPos.x++;
                                gCharacterSprite[Index].ResetScreenPos.x++;
                                ModifyCharVisibility();
                            }
                        }
                        gCamera.x--;
                    }
                    else
                    {
                        gPlayer.ScreenPos.x--;
                    }
                }
                gPlayer.WorldPos.x--;
            }
            else if (gPlayer.Direction == RIGHT)
            {
                if (gPlayer.ScreenPos.x < GAME_RES_WIDTH - 192)   ////keeps player centered
                {
                    gPlayer.ScreenPos.x++;
                }
                else
                {
                    if (gCamera.x < gCurrentArea.Area.right - GAME_RES_WIDTH)
                    {
                        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                            {
                                gCharacterSprite[Index].ScreenPos.x--;
                                gCharacterSprite[Index].ResetScreenPos.x--;
                                ModifyCharVisibility();
                            }
                        }
                        gCamera.x++;
                    }
                    else
                    {
                        gPlayer.ScreenPos.x++;
                    }
                }
                gPlayer.WorldPos.x++;
            }
            else if (gPlayer.Direction == UP)
            {
                if (gPlayer.ScreenPos.y > 128)   ////keeps player centered
                {
                    gPlayer.ScreenPos.y--;
                }
                else
                {
                    if (gCamera.y > gCurrentArea.Area.top)
                    {
                        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE && gCharacterSprite[Index].Loaded == TRUE)
                            {
                                gCharacterSprite[Index].ScreenPos.y++;
                                gCharacterSprite[Index].ResetScreenPos.y++;
                                ModifyCharVisibility();
                            }
                        }
                        gCamera.y--;
                    }
                    else
                    {
                        gPlayer.ScreenPos.y--;
                    }
                }
                gPlayer.WorldPos.y--;
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
                    // is the player on a portal
                    if (gOverWorld01.TileMap.Map[(gPlayer.WorldPos.y / 16)][(gPlayer.WorldPos.x / 16)] == TILE_TELEPORT01)
                    {
                        if (gPlayer.HasMovedSincePort == TRUE)
                        {
                            TeleportHandler();
                        }
                    }
                    else
                    {
                        for (uint8_t Index = 0; Index < NUM_CHAR_SPRITES; Index++)
                        {
                            switch (gCharacterSprite[Index].Direction)
                            {
                                case DOWN:
                                {
                                    if (((gCharacterSprite[Index].WorldPos.y + (gCharacterSprite[Index].SightRange * 16) >= gPlayer.WorldPos.y) && gCharacterSprite[Index].WorldPos.y <= gPlayer.WorldPos.y) && (gCharacterSprite[Index].WorldPos.x == gPlayer.WorldPos.x))
                                    {
                                        gPlayer.Direction = UP;
                                        gCharacterSprite[Index].InteractedWith = TRUE;
                                        EnterDialogue();
                                    }
                                    break;
                                }
                                case LEFT:
                                {
                                    if (((gCharacterSprite[Index].WorldPos.x - (gCharacterSprite[Index].SightRange * 16) <= gPlayer.WorldPos.x) && gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x) && (gCharacterSprite[Index].WorldPos.y == gPlayer.WorldPos.y))
                                    {
                                        gPlayer.Direction = RIGHT;
                                        gCharacterSprite[Index].InteractedWith = TRUE;
                                        EnterDialogue();
                                    }
                                    break;
                                }
                                case RIGHT:
                                {
                                    if (((gCharacterSprite[Index].WorldPos.x + (gCharacterSprite[Index].SightRange * 16) >= gPlayer.WorldPos.x) && gCharacterSprite[Index].WorldPos.x <= gPlayer.WorldPos.x) && (gCharacterSprite[Index].WorldPos.y == gPlayer.WorldPos.y))
                                    {
                                        gPlayer.Direction = LEFT;
                                        gCharacterSprite[Index].InteractedWith = TRUE;
                                        EnterDialogue();
                                    }
                                    break;
                                }
                                case UP:
                                {
                                    if (((gCharacterSprite[Index].WorldPos.y - (gCharacterSprite[Index].SightRange * 16) <= gPlayer.WorldPos.y) && gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y) && (gCharacterSprite[Index].WorldPos.x == gPlayer.WorldPos.x))
                                    {
                                        gPlayer.Direction = DOWN;
                                        gCharacterSprite[Index].InteractedWith = TRUE;
                                        EnterDialogue();
                                    }
                                    break;
                                }
                                default:
                                {
                                    ASSERT(FALSE, "Sprite is facing an unknown direction while looking for player!");
                                }
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

                                    for (uint8_t EncounterArea = 0; EncounterArea < NUM_ENCOUNTER_AREAS; EncounterArea++)       //TODO: lots of if nested if statements maybe work on that
                                    {
                                        if ((gPlayer.WorldPos.x >= gEncounterAreas[EncounterArea].Area.left) && (gPlayer.WorldPos.x <= gEncounterAreas[EncounterArea].Area.right) && (gPlayer.WorldPos.y >= gEncounterAreas[EncounterArea].Area.top) && (gPlayer.WorldPos.y <= gEncounterAreas[EncounterArea].Area.bottom))
                                        {
                                            gCurrentEncounterArea = gEncounterAreas[EncounterArea];
                                            gPlayer.StepsSinceLastEncounter = gPlayer.StepsTaken;
                                            RandomMonsterEncounter(&gPreviousGameState, &gCurrentGameState);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                default:
                {
                    //ASSERT(FALSE, "Player landed on an uneven pixel!");                       ////seems to still happen at 0???
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
                                    TrainerEncounter(&gPreviousGameState, &gCurrentGameState);            //TODO: Make trainer battle seperate from wild encounter
                                }
                                else
                                {
                                    gCharacterSprite[Index].DialogueFlag++;
                                    gDialogueControls = FALSE;
                                    goto SkipExitingDialogue;
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
                                    gCharacterSprite[Index].Event = EVENT_FLAG_NONE;
                                    gCharacterSprite[Index].InteractedWith = FALSE;
                                    gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                                }
                                else
                                {
                                    gCharacterSprite[Index].DialogueFlag++;
                                    gDialogueControls = FALSE;
                                    goto SkipExitingDialogue;
                                }
                                break;
                            }
                            case EVENT_FLAG_HEAL:
                            {
                                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)        //for when dialogues are longer than one box
                                {
                                    HealPlayerParty();
                                    gCharacterSprite[Index].InteractedWith = FALSE;
                                    gCharacterSprite[Index].DialogueFlag = gCharacterSprite[Index].DialogueLoopReturn;
                                }
                                else
                                {
                                    gCharacterSprite[Index].DialogueFlag++;
                                    gDialogueControls = FALSE;
                                    goto SkipExitingDialogue;
                                }
                                break;
                            }
                            case EVENT_FLAG_ITEM_ONCE:
                            {
                                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
                                {
                                    for (uint8_t i = 0; i < MAX_ITEMS_GIVE; i++)
                                    {
                                        GivePlayerItem(gCharacterSprite[Index].EventItemsIndex[i], gCharacterSprite[Index].EventItemsCount[i]);
                                    }
                                    gCharacterSprite[Index].Event = EVENT_FLAG_NONE;
                                    gCharacterSprite[Index].InteractedWith = FALSE;
                                }
                                else
                                {
                                    gCharacterSprite[Index].DialogueFlag++;
                                    gDialogueControls = FALSE;
                                    goto SkipExitingDialogue;
                                }
                                break;
                            }
                            default:
                            {
                                if (gCharacterSprite[Index].DialoguesBeforeLoop <= gCharacterSprite[Index].DialogueFlag)
                                {
                                    gCharacterSprite[Index].InteractedWith = FALSE;
                                }
                                else
                                {
                                    gCharacterSprite[Index].DialogueFlag++;
                                    gDialogueControls = FALSE;
                                    goto SkipExitingDialogue;
                                }
                                break;
                            }
                            break;      //break out of for loop searching for more sprites interacted with, obviously if we just interacted with one there shouldn't be more
                        }
                    }
                }
                gGamePaused = FALSE;
                gDialogueControls = FALSE;
                gOverWorldControls = TRUE;

            SkipExitingDialogue:        //used to skip the above BOOL globals to keep us in dialogue mode

                gFinishedDialogueTextAnimation = FALSE;
            }
        }
        else        //no dialogue or overworld controls
        {
            if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)         //allows for skipping typewriter effect to instant dialogue blit
            {
                gFinishedDialogueTextAnimation = TRUE;
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
            gPlayer.WorldPos.x += gPortCoords[Counter].WorldPosAdd.x;
            gPlayer.WorldPos.y += gPortCoords[Counter].WorldPosAdd.y;
            gPlayer.ScreenPos.x += gPortCoords[Counter].ScreenPosAdd.x;
            gPlayer.ScreenPos.y += gPortCoords[Counter].ScreenPosAdd.y;
            gCamera.x += gPortCoords[Counter].CameraPosAdd.x;
            gCamera.y += gPortCoords[Counter].CameraPosAdd.y;
            gCurrentArea = gPortCoords[Counter].AreaDest;
            for (uint8_t Sprite = 0; Sprite < NUM_CHAR_SPRITES; Sprite++)
            {
                if ((gPortCoords[Counter].SpritesToLoad[Sprite] == TRUE) && (gCharacterSprite[Sprite].Loaded == FALSE))             /////////TODO make so game unloads sprites and reloads them via teleport
                {
                    gCharacterSprite[Sprite].Loaded = TRUE;
                    if ((gCharacterSprite[Sprite].ScreenPos.x >= 0) && (gCharacterSprite[Sprite].ScreenPos.x <= GAME_RES_WIDTH - 16) && (gCharacterSprite[Sprite].ScreenPos.y >= 0) && (gCharacterSprite[Sprite].ScreenPos.y <= GAME_RES_HEIGHT - 16))
                    {
                        gCharacterSprite[Sprite].Visible = TRUE;
                    }
                }
                if ((gPortCoords[Counter].SpritesToUnload[Sprite] == TRUE) && (gCharacterSprite[Sprite].Loaded == TRUE))           //////TODO FIX gCharacterSprite[1].Exists = 40???? should be boolean something is overwriting it
                {
                    gCharacterSprite[Sprite].Loaded = FALSE;
                    gCharacterSprite[Sprite].Visible = FALSE;
                }
            }
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


void TriggerNPCMovement(_In_ uint64_t Counter)
{
    for (uint8_t Index = 0; Index <= NUM_CHAR_SPRITES; Index++)
    {
        if (gCharacterSprite[Index].Loaded == TRUE)
        {
            switch (gCharacterSprite[Index].Movement)
            {
                case (MOVEMENT_STILL):
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
                    if (gCharacterSprite[Index].Direction == UP)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                        {
                            gCharacterSprite[Index].Direction = DOWN;
                        }
                        else
                        {
                            gCharacterSprite[Index].ScreenPos.y--;
                            gCharacterSprite[Index].WorldPos.y--;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == DOWN)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.y > gPlayer.WorldPos.y) || (gCharacterSprite[Index].WorldPos.y + 16 < gPlayer.WorldPos.y))) && (!((gCharacterSprite[Index].WorldPos.x >= gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x + 16 <= gPlayer.WorldPos.x))))
                        {
                            gCharacterSprite[Index].Direction = UP;
                        }
                        else
                        {
                            gCharacterSprite[Index].ScreenPos.y++;
                            gCharacterSprite[Index].WorldPos.y++;
                        }
                    }

                    if (gCharacterSprite[Index].ScreenPos.y == gCharacterSprite[Index].ResetScreenPos.y)
                    {
                        gCharacterSprite[Index].Direction = UP;
                    }
                    else if (gCharacterSprite[Index].ScreenPos.y == gCharacterSprite[Index].ResetScreenPos.y - (gCharacterSprite[Index].MovementRange.y * 16))
                    {
                        gCharacterSprite[Index].Direction = DOWN;
                    }

                    if (!(((gCharacterSprite[Index].ScreenPos.y >= 0) && (gCharacterSprite[Index].ScreenPos.y < GAME_RES_HEIGHT - 16)) && ((gCharacterSprite[Index].ScreenPos.x >= 0) && (gCharacterSprite[Index].ScreenPos.x < GAME_RES_WIDTH - 16))))
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
                    if (gCharacterSprite[Index].Direction == LEFT && gCharacterSprite[Index].MovementRemaining != 0)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                        {
                            gCharacterSprite[Index].Direction = RIGHT;
                        }
                        else
                        {
                            gCharacterSprite[Index].ScreenPos.x--;
                            gCharacterSprite[Index].WorldPos.x--;
                            gCharacterSprite[Index].MovementRemaining--;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == LEFT && !gCharacterSprite[Index].MovementRemaining)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                        {
                            if (gCharacterSprite[Index].ScreenPos.x == gCharacterSprite[Index].ResetScreenPos.x - (gCharacterSprite[Index].MovementRange.x * 16))
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
                        else
                        {
                            gCharacterSprite[Index].ScreenPos.x++;
                            gCharacterSprite[Index].WorldPos.x++;
                            gCharacterSprite[Index].MovementRemaining--;
                        }
                    }
                    else if (gCharacterSprite[Index].Direction == RIGHT && !gCharacterSprite[Index].MovementRemaining)
                    {
                        //////this big line says: if any pixel of 16x16 player is slightly in the way (or ontop of by one pixel)
                        if ((!((gCharacterSprite[Index].WorldPos.x > gPlayer.WorldPos.x + 16) || (gCharacterSprite[Index].WorldPos.x < gPlayer.WorldPos.x))) && (!((gCharacterSprite[Index].WorldPos.y >= gPlayer.WorldPos.y + 16) || (gCharacterSprite[Index].WorldPos.y + 16 <= gPlayer.WorldPos.y))))
                        {
                            gCharacterSprite[Index].Direction = LEFT;
                        }
                        else
                        {
                            gCharacterSprite[Index].MovementRemaining = 16;
                        }
                    }

                    if (gCharacterSprite[Index].ScreenPos.x == gCharacterSprite[Index].ResetScreenPos.x)
                    {
                        gCharacterSprite[Index].Direction = LEFT;
                    }
                    else if (gCharacterSprite[Index].ScreenPos.x == gCharacterSprite[Index].ResetScreenPos.x - (gCharacterSprite[Index].MovementRange.x * 16))
                    {
                        gCharacterSprite[Index].Direction = RIGHT;
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

    for (uint8_t i = 0; i < NUM_CHAR_SPRITES; i++)  //TOUSE: steal this to make a sprite resetting function???
    {
        gCharacterSprite[i].ScreenPos = gCharacterSprite[i].ResetOriginScreenPos;
        gCharacterSprite[i].WorldPos = gCharacterSprite[i].ResetOriginWorldPos;
        gCharacterSprite[i].ResetScreenPos = gCharacterSprite[i].ResetOriginScreenPos;
        gCharacterSprite[i].ResetWorldPos = gCharacterSprite[i].ResetOriginWorldPos;
        gCharacterSprite[i].Direction = gCharacterSprite[i].ResetDirection;
        gCharacterSprite[i].Loaded = FALSE;
        gCharacterSprite[i].Visible = FALSE;
    }

    gCurrentArea = gHome01Area;

    gPlayer.ScreenPos.x = 192; 
    gPlayer.ScreenPos.y = 96;  
    gPlayer.WorldPos.x = 4048; 
    gPlayer.WorldPos.y = 352;  
    gCamera.x = 3856;          
    gCamera.y = 256;           
    gPlayer.Direction = DOWN;

    HealPlayerParty();
}

void GivePlayerItem(uint16_t itemIndex, uint16_t amount)
{
    gEquipableItems[itemIndex].Count += amount;
    if (gEquipableItems[itemIndex].Count > 0)
    {
        gEquipableItems[itemIndex].HasItem = TRUE;
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