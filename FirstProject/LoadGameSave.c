#include "Main.h"
#include "LoadGameSave.h"
#include "OverWorld.h"
#include "MonsterData.h"

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

    // read the file contents into a string 
    char buffer[4096];
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
        //return 1;
    }

    // access the JSON data 
    for (uint8_t letter = 0; letter < MAX_NAME_LENGTH - 1; letter++)
    {
        char* nameletter = malloc(6);
        snprintf(nameletter, 6, "Name%d", letter);
        cJSON* playerletter = cJSON_GetObjectItemCaseSensitive(json, nameletter);       //searching for object "nameletter" like Name0 or Name1
        if (cJSON_IsNumber(playerletter) && (playerletter->valueint != NULL))
        {
            gPlayer.Name[letter] = playerletter->valueint;
        }
    }

    cJSON* PartyCount = cJSON_GetObjectItemCaseSensitive(json, "PartyCount");
    if (cJSON_IsNumber(PartyCount) && (PartyCount->valueint != NULL))
    {
        gPlayerPartyCount = PartyCount->valueint;
    }

    for (uint8_t monster = 0; monster < gPlayerPartyCount; monster++)
    {
        //struct Monster* LoadingMonster = { 0 };
        //ZeroMonsterData(LoadingMonster);
        //ZeroDriveMonsterData(&LoadingMonster->DriveMonster);
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
            switch (loadedinfo->valueint)   ////BUGFIX: probably will break because I havent incorperated GAMEAREA.index into MonsterData.c
            {
                case 0:
                    SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MET_LOCATION, gHome01Area.Name);
                    break;
                case 1:
                    SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MET_LOCATION, gOverworldArea.Name);
                    break;
                case 2:
                    SetDriveMonsterData(&LoadingDMonster, MONSTER_DATA_MET_LOCATION, gDungeon01Area.Name);
                    break;
            }
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
        if (cJSON_IsNumber(loadedinfo) && (loadedinfo->valueint != NULL))
        {
            gPlayerParty[monster].Health = loadedinfo->valueint;
        }
    }

    // delete the JSON object 
    cJSON_Delete(json);
    //return 0;
    PlayGameSound(&gSoundMenuChoose);
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