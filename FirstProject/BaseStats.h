#pragma once

#include "SimpleConstants.h"
#include "Main.h"

#define MONSTER_GENDERLESS 255
#define MONSTER_FEMALE 254
#define PERCENT_FEMALE(percent) min(254, ((percent * 255) / 100))

const struct BaseStats gBaseStats[] =			//////setting up basestats for all monsters in the game systematically by array of species
{
	[MONSTER_NULL] = {0},
	[MONSTER_WOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_NONE,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_EARTHWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_EARTH,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_AIRWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_AIR,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_FIREWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_FIRE,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_WATERWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_WATER,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_ELECTRICWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_ELECTRIC,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_METALWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_METAL,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_SOULWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_SOUL,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_LIFEWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_LIFE,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
	[MONSTER_DEATHWOLF] =
	{
		.baseHP = 45,
		.baseAttack = 45,
		.baseDefense = 45,
		.baseSpeed = 45,
		.basePsi = 45,
		.baseResolve = 45,
		.element1 = ELEMENT_DEATH,
		.element2 = ELEMENT_NULL,
		.catchrate = 190,
		.expYield = 64,
		.trainingYieldAttack = 1,
		.genderRatio = PERCENT_FEMALE(22.5),
		.eggHatch = 20,
		.friendship = 70,
		.growthRate = GROWTH_RATE_NORMAL,
		.eggGroup1 = 1,
		.eggGroup2 = 1,
		.abilities = {ABILITY_RUFF_HOUSE, ABILITY_NONE, ABILITY_RUFF_HOUSE}
	},
};