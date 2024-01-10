#include "Main.h"
#include "SaveGameSave.h"
#include "OverWorld.h"
#include "InventoryItems.h"

MENUITEM gMI_SaveGameSave_Back = { "Back",(GAME_RES_WIDTH / 2) - (6 * 5 / 2) - 150, (GAME_RES_HEIGHT - 14), TRUE, MenuItem_SaveGameSave_Back };

MENUITEM gMI_SaveGameSave_SaveSlot1 = { "Slot1",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (96), TRUE, MenuItem_SaveGameSave_Slot1 };

MENUITEM gMI_SaveGameSave_SaveSlot2 = { "Slot2",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (128), TRUE, MenuItem_SaveGameSave_Slot2 };

MENUITEM gMI_SaveGameSave_SaveSlot3 = { "Slot3",(GAME_RES_WIDTH / 2) - (6 * 6 / 2), (160), TRUE, MenuItem_SaveGameSave_Slot3 };

MENUITEM* gMI_SaveGameSaveItems[] = { &gMI_SaveGameSave_Back, &gMI_SaveGameSave_SaveSlot1, &gMI_SaveGameSave_SaveSlot2, &gMI_SaveGameSave_SaveSlot3 };

MENU gMenu_SaveGameSave = { "Save Save Slot", 1, _countof(gMI_SaveGameSaveItems), gMI_SaveGameSaveItems };

void DrawSaveGameSave(void)
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
        gMenu_SaveGameSave.SelectedItem = 1;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));
    //memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);


    ApplyFadeIn(LocalFrameCounter, COLOR_NES_WHITE, &White, &BrightnessAdjustment);

    ApplyFadeIn(LocalFrameCounter, (PIXEL32) { 0x00, 0x00, 0xFF, 0xFF }, & Red, & BrightnessAdjustment);

    BlitStringToBuffer(gMenu_SaveGameSave.Name, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(gMenu_SaveGameSave.Name) * 6 / 2), 60);

    for (uint8_t MenuItem = 0; MenuItem < gMenu_SaveGameSave.ItemCount; MenuItem++)
    {
        if (gMenu_SaveGameSave.Items[MenuItem]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMenu_SaveGameSave.Items[MenuItem]->Name, &g6x7Font, &White, gMenu_SaveGameSave.Items[MenuItem]->x, gMenu_SaveGameSave.Items[MenuItem]->y);
        }
    }
    BlitStringToBuffer("»", &g6x7Font, &White, gMenu_SaveGameSave.Items[gMenu_SaveGameSave.SelectedItem]->x - 6, gMenu_SaveGameSave.Items[gMenu_SaveGameSave.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void PPI_SaveGameSave(void)
{
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_SaveGameSave.SelectedItem != 0)
        {
            gMenu_SaveGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_SaveGameSave.Items[gMenu_SaveGameSave.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        gMenu_SaveGameSave.Items[gMenu_SaveGameSave.SelectedItem]->Action();
        PlayGameSound(&gSoundMenuChoose);
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_SaveGameSave.SelectedItem == 0)
        {
            gMenu_SaveGameSave.SelectedItem = 3;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_SaveGameSave.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        if (gMenu_SaveGameSave.SelectedItem == 3)
        {
            gMenu_SaveGameSave.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_SaveGameSave.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
}

void MenuItem_SaveGameSave_Back(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_TITLESCREEN;
}

void MenuItem_SaveGameSave_Slot1(void)
{
    // create a cJSON object 
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "Name0", gPlayer.Name[0]);
    cJSON_AddNumberToObject(json, "Name1", gPlayer.Name[1]);
    cJSON_AddNumberToObject(json, "Name2", gPlayer.Name[2]);
    cJSON_AddNumberToObject(json, "Name3", gPlayer.Name[3]);
    cJSON_AddNumberToObject(json, "Name4", gPlayer.Name[4]);
    cJSON_AddNumberToObject(json, "Name5", gPlayer.Name[5]);
    cJSON_AddNumberToObject(json, "Name6", gPlayer.Name[6]);
    cJSON_AddNumberToObject(json, "Name7", gPlayer.Name[7]);

    cJSON_AddNumberToObject(json, "WorldPosX", gPlayer.WorldPos.x);
    cJSON_AddNumberToObject(json, "WorldPosY", gPlayer.WorldPos.y);
    cJSON_AddNumberToObject(json, "ScreenPosX", gPlayer.ScreenPos.x);
    cJSON_AddNumberToObject(json, "ScreenPosY", gPlayer.ScreenPos.y);
    cJSON_AddNumberToObject(json, "CameraPosX", gCamera.x);
    cJSON_AddNumberToObject(json, "CameraPosY", gCamera.y);
    cJSON_AddNumberToObject(json, "CurrentAreaIndex", gCurrentArea.Index);
    cJSON_AddNumberToObject(json, "PartyCount", gPlayerPartyCount); 


    char* monsterinfo = malloc(16);
    for (uint8_t Monster = 0; Monster < gPlayerPartyCount; Monster++)
    {
        snprintf(monsterinfo, 16, "Nickname%d", Monster);
        cJSON_AddStringToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.nickname);
        snprintf(monsterinfo, 16, "MonsterIndex%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.Index);
        snprintf(monsterinfo, 16, "Experience%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.Experience);
        snprintf(monsterinfo, 16, "Friendship%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.Friendship);
        snprintf(monsterinfo, 16, "OriginalOwner%d", Monster);
        cJSON_AddStringToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.playerName);
        snprintf(monsterinfo, 16, "PlayerSeed%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.PlayerSeed);
        snprintf(monsterinfo, 16, "MonsterSeed%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.MonsterSeed);
        snprintf(monsterinfo, 16, "MetLevel%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.MetLevel);
        snprintf(monsterinfo, 16, "MetLocation%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.MetLocation);
        snprintf(monsterinfo, 16, "CurrentHealth%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].Health);

        snprintf(monsterinfo, 16, "HpGenes%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.HpGenetics);
        snprintf(monsterinfo, 16, "AtkGenes%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.AttackGenetics);
        snprintf(monsterinfo, 16, "DefGenes%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.DefenseGenetics);
        snprintf(monsterinfo, 16, "SpeGenes%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.SpeedGenetics);
        snprintf(monsterinfo, 16, "PsiGenes%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.PsiGenetics);
        snprintf(monsterinfo, 16, "ResGenes%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.ResolveGenetics);

        snprintf(monsterinfo, 16, "HpTrain%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.HpTraining);
        snprintf(monsterinfo, 16, "AtkTrain%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.AttackTraining);
        snprintf(monsterinfo, 16, "DefTrain%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.DefenseTraining);
        snprintf(monsterinfo, 16, "SpeTrain%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.SpeedTraining);
        snprintf(monsterinfo, 16, "PsiTrain%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.PsiTraining);
        snprintf(monsterinfo, 16, "ResTrain%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.ResolveTraining);

        snprintf(monsterinfo, 16, "SigMove%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.SignatureMove);
        snprintf(monsterinfo, 16, "AbilityNum%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.AbilityNumber);
        snprintf(monsterinfo, 16, "MoveFirst%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.Moves[0]);
        snprintf(monsterinfo, 16, "MoveSecond%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.Moves[1]);
        snprintf(monsterinfo, 16, "MoveThird%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.Moves[2]);
        snprintf(monsterinfo, 16, "MoveFourth%d", Monster);
        cJSON_AddNumberToObject(json, monsterinfo, gPlayerParty[Monster].DriveMonster.Moves[3]);
    }

    for (uint8_t sprite = 0; sprite < NUM_CHAR_SPRITES; sprite++)
    {
        char* SpriteInfo = malloc(16);
        if (gCharacterSprite[sprite].Exists == TRUE)
        {
            snprintf(SpriteInfo, 16, "ScreenPosX%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].ScreenPos.x);
            snprintf(SpriteInfo, 16, "ScreenPosY%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].ScreenPos.y);
            snprintf(SpriteInfo, 16, "WorldPosX%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].WorldPos.x);
            snprintf(SpriteInfo, 16, "WorldPosY%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].WorldPos.y);
            snprintf(SpriteInfo, 16, "RWorldPosX%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].ResetWorldPos.x);
            snprintf(SpriteInfo, 16, "RWorldPosY%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].ResetWorldPos.y);
            snprintf(SpriteInfo, 16, "Direction%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].Direction);
            snprintf(SpriteInfo, 16, "Event%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].Event);
            snprintf(SpriteInfo, 16, "Exists%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].Exists);
            snprintf(SpriteInfo, 16, "Loaded%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].Loaded);
            snprintf(SpriteInfo, 16, "SightRange%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].SightRange);
            snprintf(SpriteInfo, 16, "DialogueFlag%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].DialogueFlag);

        }
        else
        {
            snprintf(SpriteInfo, 16, "Event%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].Event);
            snprintf(SpriteInfo, 16, "Exists%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].Exists);
            snprintf(SpriteInfo, 16, "Loaded%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].Loaded);
            snprintf(SpriteInfo, 16, "SightRange%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].SightRange);
            snprintf(SpriteInfo, 16, "DialogueFlag%d", sprite);
            cJSON_AddNumberToObject(json, SpriteInfo, gCharacterSprite[sprite].DialogueFlag);
        }
    }

    char* item = malloc(20);
    for (uint8_t equip = 0; equip < NUM_EQUIP_ITEMS; equip++)
    {
        snprintf(item, 20, "Equip%d", equip);
        cJSON_AddNumberToObject(json, item, gEquipableItems[equip].Count);
    }
    for (uint8_t use = 0; use < NUM_USABLE_ITEMS; use++)
    {
        snprintf(item, 20, "Use%d", use);
        cJSON_AddNumberToObject(json, item, gUseableItems[use].Count);
    }
    for (uint8_t value = 0; value < NUM_VALUABLE_ITEMS; value++)
    {
        snprintf(item, 20, "Value%d", value);
        cJSON_AddNumberToObject(json, item, gValuableItems[value].Count);
    }
    for (uint8_t adventure = 0; adventure < NUM_ADVENTURE_ITEMS; adventure++)
    {
        snprintf(item, 20, "Adventure%d", adventure);
        cJSON_AddNumberToObject(json, item, gAdventureItems[adventure].Count);
    }

    char* flag = malloc(16);
    for (uint8_t flagindex = START_OF_FLAGS; flagindex < END_OF_FLAGS; flagindex++)
    {
        if (gGameFlags[flagindex] == TRUE)
        {
            snprintf(flag, 16, "Flag%d", flagindex);
            cJSON_AddNumberToObject(json, flag, gGameFlags[flagindex]);
        }
    }

    // convert the cJSON object to a JSON string 
    char* json_str = cJSON_Print(json);


    // write the JSON string to a file 
    FILE* fp;
#pragma warning(suppress: 4996)         //compiler doesnt like fopen, but most programers online seem to agree its fairly safe
    fp = fopen("saveslot1.json", "w");
#pragma warning(pop)
    if (fp == NULL) {
        ASSERT(FALSE, "Error: Unable to open the save file saveslot1.json for writing");
        //return 1;
    }
    printf("%s\n", json_str);
    fputs(json_str, fp);
    fclose;
    
    // free the JSON string and cJSON object 
    cJSON_free(json_str);
    cJSON_Delete(json);
    //return 0;
    PlayGameSound(&gSoundMenuChoose);
}


void MenuItem_SaveGameSave_Slot2(void)
{

}


void MenuItem_SaveGameSave_Slot3(void)
{

}