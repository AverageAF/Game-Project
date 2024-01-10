
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