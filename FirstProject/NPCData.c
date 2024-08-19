#include "Main.h"

#include "NPCData.h"
#include "Dialogue.h"
#include "Inventory.h"
#include "BattleScreen.h"
#include "MonsterData.h"
//#include "ExperienceTables.h"
//#include "BaseStats.h"


INGAMESPRITE gInitSprite[NUM_CHAR_SPRITES] = {

	[NPC_NULL] =
	{
		.Name = "NULL!",

		.WorldPos.x = 0,
		.WorldPos.y = 0,
		.ResetWorldPos.x = 0,
		.ResetWorldPos.y = 0,
		.ResetOriginWorldPos.x = 0,
		.ResetOriginWorldPos.y = 0,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite[0] =	//set up a function here
		
		.Movement = MOVEMENT_STILL,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = FALSE,

		.GameAreaIndex = 0,

		.SightRange = 0,
		.ResetSightRange = 0,

		.MovementRange = 0,

	},
	[NPC_BED_01] =
	{
		.Name = "Your Bed",

		.WorldPos.x = 4032,
		.WorldPos.y = 656,
		.ResetWorldPos.x = 4032,
		.ResetWorldPos.y = 656,
		.ResetOriginWorldPos.x = 4032,
		.ResetOriginWorldPos.y = 656,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_STILL,

		.Visible = TRUE,
		.Loaded = TRUE,
		.Exists = TRUE,

		.GameAreaIndex = 9,

		.SightRange = 0,
		.ResetSightRange = 0,

		.MovementRange = 0,

	},
	[NPC_PROF] =
	{
		.Name = "PROFESSOR",

		.WorldPos.x = 4064,
		.WorldPos.y = 96,
		.ResetWorldPos.x = 4064,
		.ResetWorldPos.y = 96,
		.ResetOriginWorldPos.x = 4064,
		.ResetOriginWorldPos.y = 96,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_STILL,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = TRUE,

		.GameAreaIndex = 8,

		.SightRange = 0,
		.ResetSightRange = 0,

		.MovementRange = 1,

	},
	[NPC_HEAL_01] =
	{
		.Name = "Nurse",

		.WorldPos.x = 528,
		.WorldPos.y = 4624,
		.ResetWorldPos.x = 528,
		.ResetWorldPos.y = 4624,
		.ResetOriginWorldPos.x = 528,
		.ResetOriginWorldPos.y = 4624,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_STILL,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = TRUE,

		.GameAreaIndex = 2,

		.SightRange = 0,
		.ResetSightRange = 0,

		.MovementRange = 0,

	},
	[NPC_JIMMY] =
	{
		.Name = "Jimmy",

		.WorldPos.x = 896,
		.WorldPos.y = 4464,
		.ResetWorldPos.x = 896,
		.ResetWorldPos.y = 4464,
		.ResetOriginWorldPos.x = 896,
		.ResetOriginWorldPos.y = 4464,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_STILL,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = TRUE,

		.GameAreaIndex = 3,

		.SightRange = 3,
		.ResetSightRange = 3,

		.MovementRange = 0,

	},
	[NPC_JERRY] =
	{
		.Name = "Jerry",

		.WorldPos.x = 1136,
		.WorldPos.y = 4496,
		.ResetWorldPos.x = 1136,
		.ResetWorldPos.y = 4496,
		.ResetOriginWorldPos.x = 1136,
		.ResetOriginWorldPos.y = 4496,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_STILL,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = TRUE,

		.GameAreaIndex = 3,

		.SightRange = 3,
		.ResetSightRange = 3,

		.MovementRange = 0,

	},
	[NPC_STORAGE_01] =
	{
		.Name = "DSS",

		.WorldPos.x = 4112,
		.WorldPos.y = 64,
		.ResetWorldPos.x = 4112,
		.ResetWorldPos.y = 64,
		.ResetOriginWorldPos.x = 4112,
		.ResetOriginWorldPos.y = 64,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_STILL,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = TRUE,

		.GameAreaIndex = 8,

		.SightRange = 0,
		.ResetSightRange = 0,

		.MovementRange = 0,

	},
	[NPC_ITEM_01] =
	{
		.Name = "Item01",

		.WorldPos.x = 608,
		.WorldPos.y = 4464,
		.ResetWorldPos.x = 608,
		.ResetWorldPos.y = 4464,
		.ResetOriginWorldPos.x = 608,
		.ResetOriginWorldPos.y = 4464,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_ITEMPICKUP,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = TRUE,

		.GameAreaIndex = 2,

		.SightRange = 0,
		.ResetSightRange = 0,

		.MovementRange = 0,

	},
	[NPC_HIDDENITEM_01] =
	{
		.Name = "Sparkle01",

		.WorldPos.x = 416,
		.WorldPos.y = 4608,
		.ResetWorldPos.x = 416,
		.ResetWorldPos.y = 4608,
		.ResetOriginWorldPos.x = 416,
		.ResetOriginWorldPos.y = 4608,

		.Direction = DOWN,
		.ResetDirection = DOWN,
		.ResetOriginDirection = DOWN,

		//.Sprite

		.Movement = MOVEMENT_SPARKLE,

		.Visible = FALSE,
		.Loaded = FALSE,
		.Exists = TRUE,

		.GameAreaIndex = 2,

		.SightRange = 0,
		.ResetSightRange = 0,

		.MovementRange = 0,

	},

};

NPC_EVENT_DATA gNPCEventTable[NUM_CHAR_SPRITES] =
{
	[NPC_NULL] = {
		.Event = EVENT_FLAG_NULL,
		.EventItemsCount = 0,
		.EventItemsIndex = 0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
	[NPC_BED_01] = {
		.Event = EVENT_FLAG_HEAL,
		.EventItemsCount = 0,
		.EventItemsIndex = 0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
	[NPC_PROF] = {
		.Event = EVENT_FLAG_MONSTER,
		.EventItemsCount = 0,
		.EventItemsIndex = 0,
		.EventMonsterIndex = MONSTER_WOLF,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 5,
	},
	[NPC_HEAL_01] = {
		.Event = EVENT_FLAG_HEAL,
		.EventItemsCount = 0,
		.EventItemsIndex = 0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
	[NPC_JIMMY] = {
		.Event = EVENT_FLAG_BATTLE,
		.EventItemsCount = 0,
		.EventItemsIndex = 0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
	[NPC_JERRY] = {
		.Event = EVENT_FLAG_BATTLE,
		.EventItemsCount = 0,
		.EventItemsIndex = 0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
	[NPC_STORAGE_01] = {
		.Event = EVENT_FLAG_DRIVE_STORAGE,
		.EventItemsCount = 0,
		.EventItemsIndex = 0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
	[NPC_ITEM_01] = {
		.Event = EVENT_FLAG_USEITEM_NOSPRITE,
		.EventItemsCount[0] = 5,
		.EventItemsIndex[0] = INV_USABLE_ITEM_0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
	[NPC_HIDDENITEM_01] = {
		.Event = EVENT_FLAG_USEITEM_NOSPRITE,
		.EventItemsCount[0] = 5,
		.EventItemsIndex[0] = INV_USABLE_ITEM_0,
		.EventMonsterIndex = 0,
		.EventMonsterItem = 0,
		.EventMonsterLevel = 0,
	},
};

NPC_BATTLE_DATA gNPCBattleTable[NUM_CHAR_SPRITES] =
{
	[NPC_NULL] = {
		.BaseReward = 0,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	},
	[NPC_BED_01] = {
		.BaseReward = 0,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	},
	[NPC_PROF] = {
		.BaseReward = 0,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	},
	[NPC_HEAL_01] = {
		.BaseReward = 0,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	},
	[NPC_JIMMY] = {
		.BaseReward = 100,
		.BattleAiFlag = FLAG_NPCAI_SWITCHDEFENSIVE,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_LIFEWOLF,
			.Level = 5,
			.DriveMonster.AbilityNumber = ABILITY_RUFF_HOUSE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_GROWL,
			.DriveMonster.Moves[1] = BATTLEMOVE_POUNCE,
			.DriveMonster.Moves[2] = BATTLEMOVE_BARK,
			.DriveMonster.Moves[3] = BATTLEMOVE_FLORAL_ATTACK,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_EARTHWOLF,
			.Level = 6,
			.DriveMonster.AbilityNumber = ABILITY_RUFF_HOUSE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_GROWL,
			.DriveMonster.Moves[1] = BATTLEMOVE_POUNCE,
			.DriveMonster.Moves[2] = BATTLEMOVE_BARK,
			.DriveMonster.Moves[3] = BATTLEMOVE_GROUND_POUND,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_DEATHWOLF,
			.Level = 6,
			.DriveMonster.AbilityNumber = ABILITY_RUFF_HOUSE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_GROWL,
			.DriveMonster.Moves[1] = BATTLEMOVE_POUNCE,
			.DriveMonster.Moves[2] = BATTLEMOVE_BARK,
			.DriveMonster.Moves[3] = BATTLEMOVE_DEADLY_SWIPE,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	},
	[NPC_JERRY] = {
		.BaseReward = 50,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_WOLF,
			.Level = 5,
			.DriveMonster.AbilityNumber = ABILITY_RUFF_HOUSE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_GROWL,
			.DriveMonster.Moves[1] = BATTLEMOVE_POUNCE,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_FIREWOLF,
			.Level = 7,
			.DriveMonster.AbilityNumber = ABILITY_RUFF_HOUSE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_GROWL,
			.DriveMonster.Moves[1] = BATTLEMOVE_POUNCE,
			.DriveMonster.Moves[2] = BATTLEMOVE_BARK,
			.DriveMonster.Moves[3] = BATTLEMOVE_FIRE_CHARGE,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	},
	[NPC_STORAGE_01] = {
		.BaseReward = 0,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	},
	[NPC_ITEM_01] = {
		.BaseReward = 0,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
},
	[NPC_HIDDENITEM_01] = {
		.BaseReward = 0,
		.BattleAiFlag = FLAG_NPCAI_RANDOM,
		.Party[0] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[1] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[2] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[3] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[4] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
		.Party[5] =
		{
			.DriveMonster.Index = MONSTER_NULL,
			.Level = 1,
			.DriveMonster.AbilityNumber = ABILITY_NONE,
			.DriveMonster.HeldItem = 0,

			.DriveMonster.Moves[0] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[1] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[2] = BATTLEMOVE_NULL,
			.DriveMonster.Moves[3] = BATTLEMOVE_NULL,

			.DriveMonster.HpGenetics = 0,
			.DriveMonster.AttackGenetics = 0,
			.DriveMonster.DefenseGenetics = 0,
			.DriveMonster.PsiGenetics = 0,
			.DriveMonster.ResolveGenetics = 0,
			.DriveMonster.SpeedGenetics = 0,
		},
	}
};


EVENT_FLAGS GetEventFlagFromSpriteIndex(uint8_t SpriteIndex)
{
	return (gNPCEventTable[SpriteIndex].Event);
}

