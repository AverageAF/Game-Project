#include "Main.h"

#include "OverWorld.h"

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


    /////////////////seems to work for one sprite TODO test with multiple sprites
    TriggerNPCMovement(LocalFrameCounter, MOVEMENT_SPIN);

    for (uint8_t Index = 0; Index <= MAX_SPRITE_LOAD; Index++)
    {
        if (gCharacterSprite[Index].Visible == TRUE)
        {
            Blit32BppBitmapToBuffer(&gCharacterSprite[Index].SpriteS[gCharacterSprite[Index].SpriteIndexS + gCharacterSprite[Index].DirectionS], gCharacterSprite[Index].ScreenPosS.x, gCharacterSprite[Index].ScreenPosS.y, BrightnessAdjustment);
        }
    }


    Blit32BppBitmapToBuffer(&gPlayer.Sprite[gPlayer.CurrentSuit][gPlayer.SpriteIndex + gPlayer.Direction], gPlayer.ScreenPos.x, gPlayer.ScreenPos.y, BrightnessAdjustment);


    DrawDialogueBox("Hello!\nHow are you today?\nI'm enjoying myself here, next\nto the lake!", NULL, NULL); 


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
    if (gDialogueControls == FALSE)
    {
        if (gGameInput.EscapeKeyPressed && gGameInput.EscapeKeyAlreadyPressed)
        {
            gPreviousGameState = gCurrentGameState;
            gCurrentGameState = GAMESTATE_TITLESCREEN;
            PlayGameSound(&gSoundMenuChoose);

            PauseGameMusic();
        }

        //ASSERT(gCamera.x <= gCurrentArea.right - GAME_RES_WIDTH, "Camera went out of bounds!");

        //ASSERT(gCamera.y <= gCurrentArea.bottom - GAME_RES_HEIGHT, "Camera went out of bounds!");


        /////////////////////////////////////////////TODO Interacting with a sprite pauses the game, removes player movement and enables dialogue controls,
        /////////////////////////////////////////////     faces the interacted sprite towards the player, and finnally opens a dialogue box and waits for player input.
        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
            {
                if (gCharacterSprite[Index].Visible == TRUE)
                {
                    switch (gPlayer.Direction)          ////////decending directional ordering (up = 9, right = 6, left = 3, down = 0)
                    {
                        case (UP):
                        {
                            if (gCharacterSprite[Index].WorldPosS.y == gPlayer.WorldPos.y - 16)
                            {
                                PlayGameSound(&gSoundMenuChoose);
                                EnterDialogue();
                                gCharacterSprite[Index].DirectionS = DOWN_S;
                            }
                            break;
                        }
                        case (RIGHT):
                        {
                            if (gCharacterSprite[Index].WorldPosS.x == gPlayer.WorldPos.x + 16)
                            {
                                PlayGameSound(&gSoundMenuChoose);
                                EnterDialogue();
                                gCharacterSprite[Index].DirectionS = LEFT_S;
                            }
                            break;
                        }
                        case (LEFT):
                        {
                            if (gCharacterSprite[Index].WorldPosS.x == gPlayer.WorldPos.x - 16)
                            {
                                PlayGameSound(&gSoundMenuChoose);
                                EnterDialogue();
                                gCharacterSprite[Index].DirectionS = RIGHT_S;
                            }
                            break;
                        }
                        case (DOWN):
                        {
                            if (gCharacterSprite[Index].WorldPosS.y == gPlayer.WorldPos.y + 16)
                            {
                                PlayGameSound(&gSoundMenuChoose);
                                EnterDialogue();
                                gCharacterSprite[Index].DirectionS = UP_S;
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
                for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                {
                    if (gCharacterSprite[Index].Visible == TRUE)
                    {
                        if ((gPlayer.WorldPos.x == gCharacterSprite[Index].WorldPosS.x) && (gPlayer.WorldPos.y + 16 == gCharacterSprite[Index].WorldPosS.y))
                        {
                            NoSpriteCollision = FALSE;
                            break;
                        }
                        else
                        {
                            NoSpriteCollision = TRUE;
                            break;
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
                gPlayer.Direction = DOWN;
            }
            else if (gGameInput.ALeftKeyPressed)
            {
                for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
                {
                    if (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) - 1] == gPassableTiles[Counter])
                    {
                        CanMoveToDesiredTile = TRUE;

                        break;
                    }
                }
                for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                {
                    if (gCharacterSprite[Index].Visible == TRUE)
                    {
                        if ((gPlayer.WorldPos.x - 16 == gCharacterSprite[Index].WorldPosS.x) && (gPlayer.WorldPos.y == gCharacterSprite[Index].WorldPosS.y))
                        {
                            NoSpriteCollision = FALSE;
                            break;
                        }
                        else
                        {
                            NoSpriteCollision = TRUE;
                            break;
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
                gPlayer.Direction = LEFT;
            }
            else if (gGameInput.DRightKeyPressed)
            {
                for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
                {
                    if (gOverWorld01.TileMap.Map[gPlayer.WorldPos.y / 16][(gPlayer.WorldPos.x / 16) + 1] == gPassableTiles[Counter])
                    {
                        CanMoveToDesiredTile = TRUE;

                        break;
                    }
                }
                for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                {
                    if (gCharacterSprite[Index].Visible == TRUE)
                    {
                        if ((gPlayer.WorldPos.x + 16 == gCharacterSprite[Index].WorldPosS.x) && (gPlayer.WorldPos.y == gCharacterSprite[Index].WorldPosS.y))
                        {
                            NoSpriteCollision = FALSE;
                            break;
                        }
                        else
                        {
                            NoSpriteCollision = TRUE;
                            break;
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
                gPlayer.Direction = RIGHT;
            }
            else if (gGameInput.WUpKeyPressed)
            {
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
                    for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                    {
                        if (gCharacterSprite[Index].Visible == TRUE)
                        {
                            if ((gPlayer.WorldPos.x == gCharacterSprite[Index].WorldPosS.x) && (gPlayer.WorldPos.y - 16 == gCharacterSprite[Index].WorldPosS.y))
                            {
                                NoSpriteCollision = FALSE;
                                break;
                            }
                            else
                            {
                                NoSpriteCollision = TRUE;
                                break;
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
                gPlayer.Direction = UP;
            }
        }
        else
        {
            gPlayer.MovementRemaining--;

            if (gPlayer.Direction == DOWN)
            {
                if (gPlayer.ScreenPos.y < GAME_RES_HEIGHT - 64)
                {
                    gPlayer.ScreenPos.y++;
                }
                else
                {
                    if (gCamera.y < gCurrentArea.Area.bottom - GAME_RES_HEIGHT)
                    {
                        for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE)
                            {
                                if (gCharacterSprite[Index].Visible == TRUE)
                                {
                                    if (gCharacterSprite[Index].ScreenPosS.y <= 0 || gCharacterSprite[Index].ScreenPosS.y >= GAME_RES_HEIGHT - 16)
                                    {
                                        gCharacterSprite[Index].Visible = FALSE;
                                    }
                                }
                                gCharacterSprite[Index].ScreenPosS.y--;

                                if (gCharacterSprite[Index].Visible == FALSE)
                                {
                                    if ((gCharacterSprite[Index].ScreenPosS.x >= 0 && gCharacterSprite[Index].ScreenPosS.x <= GAME_RES_WIDTH - 16) && (gCharacterSprite[Index].ScreenPosS.y >= 0 && gCharacterSprite[Index].ScreenPosS.y <= GAME_RES_HEIGHT - 16))
                                    {
                                        gCharacterSprite[Index].Visible = TRUE;
                                    }
                                }
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
                if (gPlayer.ScreenPos.x > 64)
                {

                    gPlayer.ScreenPos.x--;
                }
                else
                {
                    if (gCamera.x > gCurrentArea.Area.left)
                    {
                        for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE)
                            {
                                if (gCharacterSprite[Index].Visible == TRUE)
                                {
                                    if (gCharacterSprite[Index].ScreenPosS.x <= 0 || gCharacterSprite[Index].ScreenPosS.x >= GAME_RES_WIDTH - 16)
                                    {
                                        gCharacterSprite[Index].Visible = FALSE;
                                    }
                                }
                                gCharacterSprite[Index].ScreenPosS.x++;

                                if (gCharacterSprite[Index].Visible == FALSE)
                                {
                                    if ((gCharacterSprite[Index].ScreenPosS.x >= 0 && gCharacterSprite[Index].ScreenPosS.x <= GAME_RES_WIDTH - 16) && (gCharacterSprite[Index].ScreenPosS.y >= 0 && gCharacterSprite[Index].ScreenPosS.y <= GAME_RES_HEIGHT - 16))
                                    {
                                        gCharacterSprite[Index].Visible = TRUE;
                                    }
                                }
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
                if (gPlayer.ScreenPos.x < GAME_RES_WIDTH - 64)
                {
                    gPlayer.ScreenPos.x++;
                }
                else
                {
                    if (gCamera.x < gCurrentArea.Area.right - GAME_RES_WIDTH)
                    {
                        for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE)
                            {
                                if (gCharacterSprite[Index].Visible == TRUE)
                                {
                                    if (gCharacterSprite[Index].ScreenPosS.x <= 0 || gCharacterSprite[Index].ScreenPosS.x >= GAME_RES_WIDTH - 16)
                                    {
                                        gCharacterSprite[Index].Visible = FALSE;
                                    }
                                }
                                gCharacterSprite[Index].ScreenPosS.x--;

                                if (gCharacterSprite[Index].Visible == FALSE)
                                {
                                    if ((gCharacterSprite[Index].ScreenPosS.x >= 0 && gCharacterSprite[Index].ScreenPosS.x <= GAME_RES_WIDTH - 16) && (gCharacterSprite[Index].ScreenPosS.y >= 0 && gCharacterSprite[Index].ScreenPosS.y <= GAME_RES_HEIGHT - 16))
                                    {
                                        gCharacterSprite[Index].Visible = TRUE;
                                    }
                                }
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
                if (gPlayer.ScreenPos.y > 64)
                {
                    gPlayer.ScreenPos.y--;
                }
                else
                {
                    if (gCamera.y > gCurrentArea.Area.top)
                    {
                        for (uint8_t Index = 0; Index < MAX_SPRITE_LOAD; Index++)
                        {
                            if (gCharacterSprite[Index].Exists == TRUE)
                            {
                                if (gCharacterSprite[Index].Visible == TRUE)
                                {
                                    if (gCharacterSprite[Index].ScreenPosS.y <= 0 || gCharacterSprite[Index].ScreenPosS.y >= GAME_RES_HEIGHT - 16)
                                    {
                                        gCharacterSprite[Index].Visible = FALSE;
                                    }
                                }
                                gCharacterSprite[Index].ScreenPosS.y++;

                                if (gCharacterSprite[Index].Visible == FALSE)
                                {
                                    if ((gCharacterSprite[Index].ScreenPosS.x >= 0 && gCharacterSprite[Index].ScreenPosS.x <= GAME_RES_WIDTH - 16) && (gCharacterSprite[Index].ScreenPosS.y >= 0 && gCharacterSprite[Index].ScreenPosS.y <= GAME_RES_HEIGHT - 16))
                                    {
                                        gCharacterSprite[Index].Visible = TRUE;
                                    }
                                }
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
                case 12:
                {
                    gPlayer.SpriteIndex = 1;
                    break;
                }
                case 8:
                {
                    gPlayer.SpriteIndex = 2;
                    break;
                }
                case 4:
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
                        if (gPlayer.StepsTaken - gPlayer.StepsSinceLastEncounter > BATTLE_ENCOUNTER_GRACE_PERIOD)
                        {
                            DWORD Random = 0;

                            rand_s((unsigned int*)&Random);

                            Random = Random % 1000;

                            if (Random > (1000 - gPlayer.RandomEncounterPercent))
                            {
                                gPlayer.StepsSinceLastEncounter = gPlayer.StepsTaken;
                                RandomMonsterEncounter(&gPreviousGameState, &gCurrentGameState);
                            }
                        }
                    }

                    break;
                }
                default:
                {

                }
            }
        }
    }
    else
    {
        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            gGamePaused = FALSE;
            gDialogueControls = FALSE;
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
            gPlayer.WorldPos.x = gPortCoords[Counter].WorldDest.x;
            gPlayer.WorldPos.y = gPortCoords[Counter].WorldDest.y;
            gPlayer.ScreenPos.x = gPortCoords[Counter].ScreenPos.x;
            gPlayer.ScreenPos.y = gPortCoords[Counter].ScreenPos.y;
            gCamera.x = gPortCoords[Counter].CameraPos.x;
            gCamera.y = gPortCoords[Counter].CameraPos.y;
            gCurrentArea = gPortCoords[Counter].AreaDest;
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
    *CurrentGameState = GAMESTATE_BATTLE;
}


void TriggerNPCMovement(_In_ uint64_t Counter, _In_ MOVEMENTTYPE MovementFlag)
{
    if (gGamePaused == FALSE)
    {
        for (uint8_t Index = 0; Index <= MAX_SPRITE_LOAD; Index++)
        {
            if (Counter % 30 == 0)
            {
                if (gCharacterSprite[Index].Visible == TRUE)
                {
                    switch (gCharacterSprite[Index].Movement)
                    {
                        case (MOVEMENT_SPIN):
                        {
                            if (gCharacterSprite[Index].DirectionS > 2)
                            {
                                gCharacterSprite[Index].DirectionS = 0;
                            }
                            else
                            {
                                gCharacterSprite[Index].DirectionS++;
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
    }
}