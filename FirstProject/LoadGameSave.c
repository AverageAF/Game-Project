#include "Main.h"
#include "LoadGameSave.h"
#include "OverWorld.h"
#include "MonsterData.h"
#include "InventoryItems.h"
#include "MonsterStorage.h"

#include "flags.h"
#include "variables.h"

MENUITEM gMI_LoadGameSave_Back = { "Back",(GAME_RES_WIDTH / 2) - (6 * 5 / 2) - 150, (GAME_RES_HEIGHT - 14), TRUE, MenuItem_LoadGameSave_Back };

MENUITEM gMI_LoadGameSave_SaveSlot1 = { "Slot1",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (96), TRUE, MenuItem_LoadGameSave_Slot1 };

MENUITEM gMI_LoadGameSave_SaveSlot2 = { "Slot2",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (128), TRUE, MenuItem_LoadGameSave_Slot2 };

MENUITEM gMI_LoadGameSave_SaveSlot3 = { "Slot3",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (160), TRUE, MenuItem_LoadGameSave_Slot3 };

MENUITEM gMI_LoadGameSave_DeleteSaveSlot1 = { "[Delete 1]",(GAME_RES_WIDTH / 2) - (6 * 6 / 2) + 128, (96), TRUE, MenuItem_LoadGameSave_DeleteSlot1 };

MENUITEM gMI_LoadGameSave_DeleteSaveSlot2 = { "[Delete 2]",(GAME_RES_WIDTH / 2) - (6 * 6 / 2) + 128, (128), TRUE, MenuItem_LoadGameSave_DeleteSlot2 };

MENUITEM gMI_LoadGameSave_DeleteSaveSlot3 = { "[Delete 3]",(GAME_RES_WIDTH / 2) - (6 * 6 / 2) + 128, (160), TRUE, MenuItem_LoadGameSave_DeleteSlot3 };

MENUITEM* gMI_LoadGameSaveItems[] = { &gMI_LoadGameSave_Back, &gMI_LoadGameSave_SaveSlot1, &gMI_LoadGameSave_SaveSlot2, &gMI_LoadGameSave_SaveSlot3, &gMI_LoadGameSave_DeleteSaveSlot1, &gMI_LoadGameSave_DeleteSaveSlot2, &gMI_LoadGameSave_DeleteSaveSlot3 };

MENU gMenu_LoadGameSave = { "Load Save Slot", 1, _countof(gMI_LoadGameSaveItems), gMI_LoadGameSaveItems };

void DrawLoadGameSave(void)
{
    static PIXEL32 White = { 0x00, 0x00, 0x00, 0x00 };
    static PIXEL32 Red = { 0x00, 0x00, 0x00, 0x00 };
    //static PIXEL32 LimeGreen = { 0x00, 0x00, 0x00, 0x00 };

    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static int16_t BrightnessAdjustment = -255;

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
        gInputEnabled = FALSE;
        gMenu_LoadGameSave.SelectedItem = 1;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));
    //memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);


    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &White, &BrightnessAdjustment);

    ApplyFadeIn(LocalFrameCounter, (PIXEL32){ 0x00, 0x00, 0xFF, 0xFF }, &Red, &BrightnessAdjustment);

    BlitStringToBuffer(gMenu_LoadGameSave.Name, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(gMenu_LoadGameSave.Name) * 6 / 2), 60);

    for (uint8_t MenuItem = 0; MenuItem < gMenu_LoadGameSave.ItemCount; MenuItem++)
    {
        if (gMenu_LoadGameSave.Items[MenuItem]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMenu_LoadGameSave.Items[MenuItem]->Name, &g6x7Font, &White, gMenu_LoadGameSave.Items[MenuItem]->x, gMenu_LoadGameSave.Items[MenuItem]->y);
        }
    }
    BlitStringToBuffer("»", &g6x7Font, &White, gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->x - 6, gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_LoadGameSave(void)
{
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_LoadGameSave.SelectedItem != 0)
        {
            gMenu_LoadGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
	{
		gMenu_LoadGameSave.Items[gMenu_LoadGameSave.SelectedItem]->Action();
		PlayGameSound(&gSoundMenuChoose);
	}
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_LoadGameSave.SelectedItem == 0)
        {
            gMenu_LoadGameSave.SelectedItem = 3;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_LoadGameSave.SelectedItem > 3)
        {
            gMenu_LoadGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_LoadGameSave.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        if (gMenu_LoadGameSave.SelectedItem >= 3)
        {
            gMenu_LoadGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_LoadGameSave.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if ((gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed) || (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed))
    {
        if (gMenu_LoadGameSave.SelectedItem == 1 || gMenu_LoadGameSave.SelectedItem == 2 || gMenu_LoadGameSave.SelectedItem == 3)
        {
            gMenu_LoadGameSave.SelectedItem += 3;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_LoadGameSave.SelectedItem == 4 || gMenu_LoadGameSave.SelectedItem == 5 || gMenu_LoadGameSave.SelectedItem == 6)
        {
            gMenu_LoadGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
}

void MenuItem_LoadGameSave_Back(void)
{
	gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_TITLESCREEN;
}

void MenuItem_LoadGameSave_Slot1(void)
{
    // open the file 
#pragma warning(suppress: 4996)         //compiler doesnt like fopen, but most programers online seem to agree its fairly safe
    FILE* fp = fopen("saveslot1.json", "r");
#pragma warning(pop)
    if (fp == NULL) {
        ASSERT(FALSE, "Unable to open save file saveslot1.json for reading");
        //return 1;
    }

    //read the file contents into a string
    char buffer[16384] = { 0 };                         //TODO: move data to heap instead of asking for large stack, need to spend time messing around with heapalloc() to learn more about it
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    // parse the JSON data 
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ASSERT(FALSE, "Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
    }

    // access the JSON data 
    for (uint8_t letter = 0; letter < MAX_NAME_LENGTH - 1; letter++)
    {
        char* nameletter = malloc(6);
        snprintf(nameletter, 6, "Name%d", letter);
        cJSON* playerletter = cJSON_GetObjectItemCaseSensitive(json, nameletter);       //searching for object "nameletter" like Name0 or Name1
        if (cJSON_IsNumber(playerletter) && (playerletter != NULL))
        {
            gPlayer.Name[letter] = playerletter->valueint;
        }
    }

    cJSON* PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "WorldPosX");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo->valueint != NULL))
    {
        gPlayer.WorldPos.x = PlayerInfo->valueint;
    }
    PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "WorldPosY");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo->valueint != NULL))
    {
        gPlayer.WorldPos.y = PlayerInfo->valueint;
    }
    PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "ScreenPosX");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo->valueint != NULL))
    {
        gPlayer.ScreenPos.x = PlayerInfo->valueint;
    }
    PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "ScreenPosY");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo->valueint != NULL))
    {
        gPlayer.ScreenPos.y = PlayerInfo->valueint;
    }
    PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "CameraPosX");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo != NULL))       //BUGFIX: something is preventing '0' to be an allowed value in this if statement, despite it being used all over this document, dropped '->valueint'
    {
        gCamera.x = PlayerInfo->valueint;
    }
    PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "CameraPosY");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo != NULL))
    {
        gCamera.y = PlayerInfo->valueint;
    }
    PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "CurrentAreaIndex");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo->valueint != NULL))
    {
        for (uint8_t gamearea = 0; gamearea < NUM_GAME_AREAS; gamearea++)
        {
            if (PlayerInfo->valueint == gamearea)
            {
                gCurrentArea = gGameAreas[gamearea];
            }
        }
    }
    PlayerInfo = cJSON_GetObjectItemCaseSensitive(json, "PartyCount");
    if (cJSON_IsNumber(PlayerInfo) && (PlayerInfo->valueint != NULL))
    {
        gPlayerPartyCount = PlayerInfo->valueint;
    }

    for (uint8_t monster = 0; monster < gPlayerPartyCount; monster++)
    {
        struct DriveMonster LoadingDMonster = { 0 };
        ZeroDriveMonsterData(&LoadingDMonster);
        char* monsterinfo = malloc(16);
        cJSON* loadedinfo;

        snprintf(monsterinfo, 16, "Nickname%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsString(loadedinfo) && (loadedinfo->valuestring != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_NICKNAME, loadedinfo->valuestring);
        }
        snprintf(monsterinfo, 16, "MonsterIndex%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_INDEX, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "Experience%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_EXPERIENCE, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "Friendship%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_FRIENDSHIP, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "OriginalOwner%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsString(loadedinfo) && (loadedinfo->valuestring != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_PLAYER_NAME, loadedinfo->valuestring);
        }
        snprintf(monsterinfo, 16, "PlayerSeed%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_PLAYER_SEED, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "MonsterSeed%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MONSTER_SEED, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "MetLevel%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MET_LEVEL, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "MetLocation%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MET_LOCATION, &gGameAreas[loadedinfo->valueint].Index);
        }
        snprintf(monsterinfo, 16, "HpGenes%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_HP_GENETICS, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "AtkGenes%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_ATTACK_GENETICS, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "DefGenes%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_DEFENSE_GENETICS, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "SpeGenes%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_SPEED_GENETICS, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "PsiGenes%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_PSI_GENETICS, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "ResGenes%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_RESOLVE_GENETICS, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "HpTrain%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_HP_TRAINING, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "AtkTrain%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_ATTACK_TRAINING, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "DefTrain%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_DEFENSE_TRAINING, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "SpeTrain%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_SPEED_TRAINING, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "PsiTrain%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_PSI_TRAINING, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "ResTrain%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_RESOLVE_TRAINING, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "SigMove%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_SIGNATURE_MOVE, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "AbilityNum%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_ABILITY_NUMBER, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "MoveFirst%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MOVE_1, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "MoveSecond%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MOVE_2, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "MoveThird%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MOVE_3, &loadedinfo->valueint);
        }
        snprintf(monsterinfo, 16, "MoveFourth%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MOVE_4, &loadedinfo->valueint);
        }

        gPlayerParty[monster].DriveMonster = LoadingDMonster;
        uint8_t Level = GetLevelFromDriveMonsterExp(&gPlayerParty[monster].DriveMonster);
        SetMonsterData(&gPlayerParty[monster], MONSTER_DATA_LEVEL, &Level);
        CalculateMonsterStats(&gPlayerParty[monster]);

        snprintf(monsterinfo, 16, "CurrentHealth%d", monster);
        loadedinfo = cJSON_GetObjectItemCaseSensitive(json, monsterinfo);
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo != NULL))
        {
            gPlayerParty[monster].Health = loadedinfo->valueint;
        }
    }

    char* InfoName = malloc(16);
    cJSON* SpriteInfo;
    for (uint8_t sprite = 0; sprite < NUM_CHAR_SPRITES; sprite++)
    {
        snprintf(InfoName, 16, "ScreenPosX%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].ScreenPos.x = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "ScreenPosY%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].ScreenPos.y = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "WorldPosX%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].WorldPos.x = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "WorldPosY%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].WorldPos.y = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "RWorldPosX%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].ResetWorldPos.x = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "RWorldPosY%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].ResetWorldPos.y = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "Direction%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].Direction = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "Event%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].Event = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "Exists%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo != NULL))
        {
            gCharacterSprite[sprite].Exists = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "Loaded%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].Loaded = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "SightRange%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].SightRange = SpriteInfo->valueint;
        }
        snprintf(InfoName, 16, "DialogueFlag%d", sprite);
        SpriteInfo = cJSON_GetObjectItemCaseSensitive(json, InfoName);
        if (cJSON_IsNumber(SpriteInfo) && (SpriteInfo->valueint != NULL))
        {
            gCharacterSprite[sprite].DialogueFlag = SpriteInfo->valueint;
        }
    }

    char* ItemIndex = malloc(20);
    cJSON* Item;
    for (uint8_t equip = 0; equip < NUM_EQUIP_ITEMS; equip++)
    {
        snprintf(ItemIndex, 20, "Equip%d", equip);
        Item = cJSON_GetObjectItemCaseSensitive(json, ItemIndex);
        if (cJSON_IsNumber(Item) && (Item->valueint != NULL))
        {
            gEquipableItems[equip].Count = Item->valueint;
            gEquipableItems[equip].HasItem = TRUE;
        }
        else
        {
            gEquipableItems[equip].Count = 0;
            gEquipableItems[equip].HasItem = FALSE;
        }
    }
    for (uint8_t use = 0; use < NUM_USABLE_ITEMS; use++)
    {
        snprintf(ItemIndex, 20, "Use%d", use);
        Item = cJSON_GetObjectItemCaseSensitive(json, ItemIndex);
        if (cJSON_IsNumber(Item) && (Item->valueint != NULL))
        {
            gUseableItems[use].Count = Item->valueint;
            gUseableItems[use].HasItem = TRUE;
        }
        else
        {
            gUseableItems[use].Count = 0;
            gUseableItems[use].HasItem = FALSE;
        }
    }
    for (uint8_t value = 0; value < NUM_VALUABLE_ITEMS; value++)
    {
        snprintf(ItemIndex, 20, "Value%d", value);
        Item = cJSON_GetObjectItemCaseSensitive(json, ItemIndex);
        if (cJSON_IsNumber(Item) && (Item->valueint != NULL))
        {
            gValuableItems[value].Count = Item->valueint;
            gValuableItems[value].HasItem = TRUE;
        }
        else
        {
            gValuableItems[value].Count = 0;
            gValuableItems[value].HasItem = FALSE;
        }
    }
    for (uint8_t adventure = 0; adventure < NUM_ADVENTURE_ITEMS; adventure++)
    {
        snprintf(ItemIndex, 20, "Adventure%d", adventure);
        Item = cJSON_GetObjectItemCaseSensitive(json, ItemIndex);
        if (cJSON_IsNumber(Item) && (Item->valueint != NULL))
        {
            gAdventureItems[adventure].Count = Item->valueint;
            gAdventureItems[adventure].HasItem = TRUE;
        }
        else
        {
            gAdventureItems[adventure].Count = 0;
            gAdventureItems[adventure].HasItem = FALSE;
        }
    }

    char* FlagIndex = malloc(16);
    cJSON* Flag;
    for (uint8_t flagindex = START_OF_FLAGS; flagindex < END_OF_FLAGS; flagindex++)
    {
        snprintf(FlagIndex, 16, "Flag%d", flagindex);
        Flag = cJSON_GetObjectItemCaseSensitive(json, FlagIndex);
        if (cJSON_IsNumber(Flag) && (Flag->valueint == TRUE))
        {
            gGameFlags[flagindex] = Flag->valueint;
        }
    }

    //delete the JSON object
    cJSON_Delete(json);

    PlayGameSound(&gSoundMenuChoose);

    LoadUnloadSpritesVIAGameArea();
}


void MenuItem_LoadGameSave_Slot2(void)
{

}


void MenuItem_LoadGameSave_Slot3(void)
{

}

void MenuItem_LoadGameSave_DeleteSlot1(void)
{

}

void MenuItem_LoadGameSave_DeleteSlot2(void)
{

}

void MenuItem_LoadGameSave_DeleteSlot3(void)
{

}

//in pokeemerald offset is the sum of player's trainer ID bytes
void SetSaveBlockPointers(uint16_t offset)          //TODO: if I use saveblocks for saving data, right now its just to set up empty monster storage boxes
{
    //struct SaveBlock1** save1_localVar = &gSaveBlockPtr;

    offset = (offset + Random16()) & (MOVE_BLOCK_RANGE - 4);

    //gSaveBlock2Ptr = (void*)(&gSaveBlock2) + offset;
    //*save1_localVar = (void*)(&gSaveBlock1) + offset;
    gMonsterStoragePtr = /*(void*)*/(&gMonsterStorageASLR) /*+ offset*/;

    //SetInventoryItemPointers();
}