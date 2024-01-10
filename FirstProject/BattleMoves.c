
#include "Main.h"

static const uint8_t sNullDescription[] = "-----------";
static const uint8_t sPounceDescription[] = "Pounce Desc";
static const uint8_t sGrowlDescription[] = "Growl Desc";
static const uint8_t sBarkDescription[] = "Bark Desc";
static const uint8_t sScratchDescription[] = "Scratch Desc";
static const uint8_t sBiteDescription[] = "Bite Desc";
static const uint8_t sGroundPoundDescription[] = "Ground Pound Desc";
static const uint8_t sBreezeAttackDescription[] = "Breeze Attack Desc";
static const uint8_t sFireChargeDescription[] = "Fire Charge Desc";
static const uint8_t sSplashAttackDescription[] = "Splash Attack Desc";
static const uint8_t sShockSprintDescription[] = "Shock Sprint Desc";
static const uint8_t sIronSlamDescription[] = "Iron Slam Desc";
static const uint8_t sSpiritSmackDescription[] = "Spirit Smack Desc";
static const uint8_t sFloralAttackDescription[] = "Floral Attack Desc";
static const uint8_t sDeadlySwipeDescription[] = "Deadly Swipe Desc";

const uint8_t* const gMoveDescriptionPointers[NUM_BATTLEMOVES] =
{
	[BATTLEMOVE_NULL] = sNullDescription,
	[BATTLEMOVE_POUNCE] = sPounceDescription,
	[BATTLEMOVE_GROWL] = sGrowlDescription,
	[BATTLEMOVE_BARK] = sBarkDescription,
	[BATTLEMOVE_SCRATCH] = sScratchDescription,
	[BATTLEMOVE_BITE] = sBiteDescription,
	[BATTLEMOVE_GROUND_POUND] = sGroundPoundDescription,
	[BATTLEMOVE_BREEZE_ATTACK] = sBreezeAttackDescription,
	[BATTLEMOVE_FIRE_CHARGE] = sFireChargeDescription,
	[BATTLEMOVE_SPLASH_ATTACK] = sSplashAttackDescription,
	[BATTLEMOVE_SHOCK_SPRINT] = sShockSprintDescription,
	[BATTLEMOVE_IRON_SLAM] = sIronSlamDescription,
	[BATTLEMOVE_SPIRIT_SMACK] = sSpiritSmackDescription,
	[BATTLEMOVE_FLORAL_ATTACK] = sFloralAttackDescription,
	[BATTLEMOVE_DEADLY_SWIPE] = sDeadlySwipeDescription,
};

const uint8_t gBattleMoveNames[NUM_BATTLEMOVES][MAX_MOVE_NAME_LENGTH + 1] =
{
	[BATTLEMOVE_NULL] = "------",
	[BATTLEMOVE_POUNCE] = "Pounce",
	[BATTLEMOVE_GROWL] = "Growl",
	[BATTLEMOVE_BARK] = "Bark",
	[BATTLEMOVE_SCRATCH] = "Scratch",
	[BATTLEMOVE_BITE] = "Bite",
	[BATTLEMOVE_GROUND_POUND] = "Ground Pound",
	[BATTLEMOVE_BREEZE_ATTACK] = "Breeze Attack",
	[BATTLEMOVE_FIRE_CHARGE] = "Fire Charge",
	[BATTLEMOVE_SPLASH_ATTACK] = "Splash Attack",
	[BATTLEMOVE_SHOCK_SPRINT] = "Shock Sprint",
	[BATTLEMOVE_IRON_SLAM] = "Iron Slam",
	[BATTLEMOVE_SPIRIT_SMACK] = "Spirit Smack",
	[BATTLEMOVE_FLORAL_ATTACK] = "Floral Attack",
	[BATTLEMOVE_DEADLY_SWIPE] = "Deadly Swipe",
};

const struct BattleMove gBattleMoves[NUM_BATTLEMOVES] =
{
	[BATTLEMOVE_NULL] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 0,
		.power3 = 0,

		.element = ELEMENT_NULL,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = 0,
		.split = SPLIT_STATUS,
	},


	[BATTLEMOVE_POUNCE] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 30,
		.power3 = 0,

		.element = ELEMENT_NONE,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_GROWL] =
	{
		.effect = EFFECT_ATTACK_DOWN,

		.power1 = 0,
		.power2 = 0,
		.power3 = 0,

		.element = ELEMENT_STATUS,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_STATUSMOVE,
		.split = SPLIT_STATUS,
	},


	[BATTLEMOVE_BARK] =
	{
		.effect = EFFECT_ATTACK_UP,

		.power1 = 0,
		.power2 = 0,
		.power3 = 0,

		.element = ELEMENT_STATUS,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_SELF,
		.speedPriority = 0,
		.flags = FLAG_STATUSMOVE,
		.split = SPLIT_STATUS,
	},


	[BATTLEMOVE_SCRATCH] =
	{
		.effect = EFFECT_HIT,

		.power1 = 40,
		.power2 = 0,
		.power3 = 0,

		.element = ELEMENT_NONE,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_BITE] =
	{
		.effect = EFFECT_HIT_FLINCH,

		.power1 = 0,
		.power2 = 0,
		.power3 = 50,

		.element = ELEMENT_NONE,
		.secondEffectChance = 10,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_GROUND_POUND] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_EARTH,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},
	

	[BATTLEMOVE_BREEZE_ATTACK] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_AIR,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_FIRE_CHARGE] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_FIRE,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_SPLASH_ATTACK] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_WATER,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_SHOCK_SPRINT] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_ELECTRIC,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_IRON_SLAM] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_METAL,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_SPIRIT_SMACK] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_SOUL,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_FLORAL_ATTACK] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_LIFE,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	},


	[BATTLEMOVE_DEADLY_SWIPE] =
	{
		.effect = EFFECT_HIT,

		.power1 = 0,
		.power2 = 60,
		.power3 = 0,

		.element = ELEMENT_DEATH,
		.secondEffectChance = 0,
		.target = MOVE_TARGET_OPPONENT,
		.speedPriority = 0,
		.flags = FLAG_CONTACTMOVE | FLAG_BLOCKABLEMOVE,
		.split = SPLIT_PHYS,
	}
};