#pragma once

#include "Main.h"
#include "SimpleConstants.h"

#define LEVEL_UP_MOVE(lvl, moveLearned) {.move = moveLearned, .level = lvl}

/////////////////////////////////////////////////monsterlearnsets

static const struct LevelUpMove sNULLLevelUpLearnset[] = {
	LEVEL_UP_MOVE( 1, BATTLEMOVE_NULL),
	LEVEL_UP_END
};

static const struct LevelUpMove sWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_END
};

static const struct LevelUpMove sEarthWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_GROUND_POUND),
	LEVEL_UP_END
};

static const struct LevelUpMove sAirWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_BREEZE_ATTACK),
	LEVEL_UP_END
};

static const struct LevelUpMove sFireWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_FIRE_CHARGE),
	LEVEL_UP_END
};

static const struct LevelUpMove sWaterWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_SPLASH_ATTACK),
	LEVEL_UP_END
};

static const struct LevelUpMove sElectricWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_SHOCK_SPRINT),
	LEVEL_UP_END
};

static const struct LevelUpMove sMetalWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_IRON_SLAM),
	LEVEL_UP_END
};

static const struct LevelUpMove sSoulWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_SPIRIT_SMACK),
	LEVEL_UP_END
};

static const struct LevelUpMove sLifeWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_FLORAL_ATTACK),
	LEVEL_UP_END
};

static const struct LevelUpMove sDeathWolfLevelUpLearnset[] = {
	LEVEL_UP_MOVE(1, BATTLEMOVE_POUNCE),
	LEVEL_UP_MOVE(5, BATTLEMOVE_GROWL),
	LEVEL_UP_MOVE(7, BATTLEMOVE_BARK),
	LEVEL_UP_MOVE(10, BATTLEMOVE_SCRATCH),
	LEVEL_UP_MOVE(12, BATTLEMOVE_BITE),
	LEVEL_UP_MOVE(15, BATTLEMOVE_DEADLY_SWIPE),
	LEVEL_UP_END
};

////////////////////////////////////////////////pointers

const struct LevelUpMove* const gLevelUpMoves[NUM_MONSTERS] =
{
	[MONSTER_NULL] = sNULLLevelUpLearnset,
	[MONSTER_WOLF] = sWolfLevelUpLearnset,
	[MONSTER_EARTHWOLF] = sEarthWolfLevelUpLearnset,
	[MONSTER_AIRWOLF] = sAirWolfLevelUpLearnset,
	[MONSTER_FIREWOLF] = sFireWolfLevelUpLearnset,
	[MONSTER_WATERWOLF] = sWaterWolfLevelUpLearnset,
	[MONSTER_ELECTRICWOLF] = sElectricWolfLevelUpLearnset,
	[MONSTER_METALWOLF] = sMetalWolfLevelUpLearnset,
	[MONSTER_SOULWOLF] = sSoulWolfLevelUpLearnset,
	[MONSTER_LIFEWOLF] = sLifeWolfLevelUpLearnset,
	[MONSTER_DEATHWOLF] = sDeathWolfLevelUpLearnset,
};