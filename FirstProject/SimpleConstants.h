#pragma once


///////////////////////////////MISC////////////////////////////////////

//exp growth rates

#define GROWTH_RATE_VERY_FAST 0
#define GROWTH_RATE_FAST 1
#define GROWTH_RATE_NORMAL 2
#define GROWTH_RATE_SLOW 3
#define GROWTH_RATE_VERY_SLOW 4
#define GROWTH_RATE_LEGENDARY 5

//upgrade types

#define UPGRADE_TYPE_NULL 0
#define UPGRADE_TYPE_MAX 1
#define UPGRADE_TYPE_USEITEM 2
#define UPGRADE_TYPE_EQUIPITEM 3
#define UPGRADE_TYPE_LVLUP 4
#define UPGRADE_TYPE_LVLUPLOCATION 5
#define UPGRADE_TYPE_LVLUPPARTY 6
#define UPGRADE_TYPE_LVLUPFRIEND 7

///////////////////////////////MISC////////////////////////////////////

//////////////////////////////ITEMS////////////////////////////////////

#define ITEMTYPE_EQUIP 0
#define ITEMTYPE_USE 1
#define ITEMTYPE_VALUE 2
#define ITEMTYPE_ADVENTURE 3

#define INV_EQUIP_ITEM_0 0
#define INV_EQUIP_ITEM_1 1
#define INV_EQUIP_ITEM_2 2
#define INV_EQUIP_ITEM_3 3
#define INV_EQUIP_ITEM_4 4
#define INV_EQUIP_ITEM_5 5
#define INV_EQUIP_ITEM_6 6
#define INV_EQUIP_ITEM_7 7
#define INV_EQUIP_ITEM_8 8
#define INV_EQUIP_ITEM_9 9
#define INV_EQUIP_ITEM_10 10
#define INV_EQUIP_ITEM_11 11
#define INV_EQUIP_ITEM_12 12
#define INV_EQUIP_ITEM_13 13
#define INV_EQUIP_ITEM_14 14
#define INV_EQUIP_ITEM_15 15
#define INV_EQUIP_ITEM_16 16
#define INV_EQUIP_ITEM_17 17
#define INV_EQUIP_ITEM_18 18
#define INV_EQUIP_ITEM_19 19
#define INV_EQUIP_ITEM_20 20
#define INV_EQUIP_ITEM_21 21
#define INV_EQUIP_ITEM_22 22
#define INV_EQUIP_ITEM_23 23
#define INV_EQUIP_ITEM_24 24

#define NUM_EQUIP_ITEMS 25

#define ITEM_EQUIP_EFFECT_NULL 0
#define ITEM_EQUIP_EFFECT_NONE 1
#define ITEM_EQUIP_EFFECT_ELEMENT_BOOST 2
#define ITEM_EQUIP_EFFECT_HEAL_BELOW_ONETHIRD 3
#define ITEM_EQUIP_EFFECT_HEAL_BELOW_ONEEIGHTH 4
#define ITEM_EQUIP_EFFECT_HEAL_IMMEDIATELY 5
#define ITEM_EQUIP_EFFECT_THORNS 6
#define ITEM_EQUIP_EFFECT_ENDURE_KO 7
#define ITEM_EQUIP_EFFECT_EJECT 8
#define ITEM_EQUIP_EFFECT_STATCHANGE 9
#define ITEM_EQUIP_EFFECT_LOCKEDIN_MOVE 10

#define INV_USABLE_ITEM_0 0
#define INV_USABLE_ITEM_1 1
#define INV_USABLE_ITEM_2 2
#define INV_USABLE_ITEM_3 3
#define INV_USABLE_ITEM_4 4
#define INV_USABLE_ITEM_5 5
#define INV_USABLE_ITEM_6 6
#define INV_USABLE_ITEM_7 7
#define INV_USABLE_ITEM_8 8
#define INV_USABLE_ITEM_9 9
#define INV_USABLE_ITEM_10 10
#define INV_USABLE_ITEM_11 11
#define INV_USABLE_ITEM_12 12
#define INV_USABLE_ITEM_13 13
#define INV_USABLE_ITEM_14 14
#define INV_USABLE_ITEM_15 15
#define INV_USABLE_ITEM_16 16
#define INV_USABLE_ITEM_17 17
#define INV_USABLE_ITEM_18 18
#define INV_USABLE_ITEM_19 19
#define INV_USABLE_ITEM_20 20
#define INV_USABLE_ITEM_21 21
#define INV_USABLE_ITEM_22 22
#define INV_USABLE_ITEM_23 23
#define INV_USABLE_ITEM_24 24
#define INV_USABLE_ITEM_25 25
#define INV_USABLE_ITEM_26 26

#define NUM_USABLE_ITEMS 27

#define ITEM_USE_EFFECT_NULL 0
#define ITEM_USE_EFFECT_HEAL_MONSTER 1
#define ITEM_USE_EFFECT_CAPTURE 2
#define ITEM_USE_EFFECT_EXP_MONSTER 3
#define ITEM_USE_EFFECT_HAPPINESS 4
#define ITEM_USE_EFFECT_TRAINING_INC 5
#define ITEM_USE_EFFECT_TRAINING_DEC 6
#define ITEM_USE_EFFECT_UPGRADE 7
#define ITEM_USE_EFFECT_TEACHMOVE_ONCE 8
#define ITEM_USE_EFFECT_TEACHMOVE_INFINITE 9
#define ITEM_USE_EFFECT_CURE_STATUS 10
#define ITEM_USE_EFFECT_NO_ENCOUNTER 11
#define ITEM_USE_EFFECT_INC_ENCOUNTER 12
#define ITEM_USE_EFFECT_REVIVE_MONSTER 13

#define INV_VALUABLE_ITEM_0 0
#define INV_VALUABLE_ITEM_1 1
#define INV_VALUABLE_ITEM_2 2
#define INV_VALUABLE_ITEM_3 3
#define INV_VALUABLE_ITEM_4 4
#define INV_VALUABLE_ITEM_5 5
#define INV_VALUABLE_ITEM_6 6
#define INV_VALUABLE_ITEM_7 7
#define INV_VALUABLE_ITEM_8 8
#define INV_VALUABLE_ITEM_9 9
#define INV_VALUABLE_ITEM_10 10
#define INV_VALUABLE_ITEM_11 11
#define INV_VALUABLE_ITEM_12 12
#define INV_VALUABLE_ITEM_13 13
#define INV_VALUABLE_ITEM_14 14
#define INV_VALUABLE_ITEM_15 15
#define INV_VALUABLE_ITEM_16 16
#define INV_VALUABLE_ITEM_17 17
#define INV_VALUABLE_ITEM_18 18
#define INV_VALUABLE_ITEM_19 19
#define INV_VALUABLE_ITEM_20 20
#define INV_VALUABLE_ITEM_21 21
#define INV_VALUABLE_ITEM_22 22
#define INV_VALUABLE_ITEM_23 23
#define INV_VALUABLE_ITEM_24 24

#define NUM_VALUABLE_ITEMS 25

#define INV_ADVENTURE_ITEM_0 0
#define INV_ADVENTURE_ITEM_1 1
#define INV_ADVENTURE_ITEM_2 2
#define INV_ADVENTURE_ITEM_3 3
#define INV_ADVENTURE_ITEM_4 4
#define INV_ADVENTURE_ITEM_5 5
#define INV_ADVENTURE_ITEM_6 6
#define INV_ADVENTURE_ITEM_7 7
#define INV_ADVENTURE_ITEM_8 8
#define INV_ADVENTURE_ITEM_9 9
#define INV_ADVENTURE_ITEM_10 10
#define INV_ADVENTURE_ITEM_11 11
#define INV_ADVENTURE_ITEM_12 12
#define INV_ADVENTURE_ITEM_13 13
#define INV_ADVENTURE_ITEM_14 14
#define INV_ADVENTURE_ITEM_15 15
#define INV_ADVENTURE_ITEM_16 16
#define INV_ADVENTURE_ITEM_17 17
#define INV_ADVENTURE_ITEM_18 18
#define INV_ADVENTURE_ITEM_19 19
#define INV_ADVENTURE_ITEM_20 20
#define INV_ADVENTURE_ITEM_21 21
#define INV_ADVENTURE_ITEM_22 22
#define INV_ADVENTURE_ITEM_23 23
#define INV_ADVENTURE_ITEM_24 24

#define NUM_ADVENTURE_ITEMS 25

//////////////////////////////ITEMS////////////////////////////////////

/////////////////////////////ELEMENTS//////////////////////////////////
#define ELEMENT_NONE 0		//calcite
#define ELEMENT_EARTH 1		//topaz		amber
#define ELEMENT_AIR 2		//gypsum	opal	turquoise
#define ELEMENT_FIRE 3		//cinnabar	ruby
#define ELEMENT_WATER 4		//saphire	opal	pearl
#define ELEMENT_ELECTRIC 5	//quartz	magnetite
#define ELEMENT_METAL 6		//titanite	hematite
#define ELEMENT_SOUL 7		//amethyst
#define ELEMENT_LIFE 8		//emerald	jadeite
#define ELEMENT_DEATH 9		//onyx		jet	coal

#define ELEMENT_COUNT 10

#define ELEMENT_NULL 255
#define ELEMENT_STATUS 254

//elemental relationships

#define ELEMENT_NEUTRAL 0
#define ELEMENT_RESIST 1
#define ELEMENT_BONUS 2
#define ELEMENT_IMMUNE 3

#define ELEMENT_4X_BONUS 4
#define ELEMENT_4X_RESIST 5

/////////////////////////////ELEMENTS//////////////////////////////////

/////////////////////////////MONSTERS//////////////////////////////////

#define MONSTER_NULL 0
#define MONSTER_WOLF 1
#define MONSTER_EARTHWOLF 2
#define MONSTER_AIRWOLF 3
#define MONSTER_FIREWOLF 4
#define MONSTER_WATERWOLF 5
#define MONSTER_ELECTRICWOLF 6
#define MONSTER_METALWOLF 7
#define MONSTER_SOULWOLF 8
#define MONSTER_LIFEWOLF 9
#define MONSTER_DEATHWOLF 10

//TOADD: new monsters

#define MONSTER_NEWUT 11
#define MONSTER_NEWUND 12

#define MONSTER_CLAYNEE 13
#define MONSTER_MUDDEN 14

#define MONSTER_STRATLIA 15
#define MONSTER_SEDIMEAN 16

#define MONSTER_HAWKLIN 17
#define MONSTER_SQUAKEEN 18
#define MONSTER_HOWLDREGE 19

#define MONSTER_LILYTAD 20
#define MONSTER_LILYBUD 21
#define MONSTER_LILYBLOOM 22

#define MONSTER_PSIMATE 23

//starters

#define MONSTER_CRUSTLING 24
#define MONSTER_CHISELAE 25
#define MONSTER_TERRACHNID 26

#define MONSTER_HOOTRIG 27
#define MONSTER_HOWLHORN 28
#define MONSTER_NOCSTORM 29

#define MONSTER_KINDLEKIT 30
#define MONSTER_IGNITSUNE 31
#define MONSTER_INFERNOX 32

#define MONSTER_CAUDITI 33
#define MONSTER_MURKAMANDER 34
#define MONSTER_SALAMENACE 35

//starters

#define MONSTER_BRAMBLING 36
#define MONSTER_TOXITHORN 37

#define MONSTER_INFANTREE 38
#define MONSTER_GERMAKNIGHT 39
#define MONSTER_STALKKING 40

//

//#define NUM_MONSTERS (MONSTER_SALAMENACE + 1)
#define NUM_MONSTERS (MONSTER_DEATHWOLF + 1)

/////////////////////////////MONSTERS//////////////////////////////////

///////////////////////////////MOVES///////////////////////////////////

#define BATTLEMOVE_NULL 0
#define BATTLEMOVE_POUNCE 1
#define BATTLEMOVE_GROWL 2
#define BATTLEMOVE_BARK 3
#define BATTLEMOVE_SCRATCH 4
#define BATTLEMOVE_BITE 5

//Unique wolf moves
#define BATTLEMOVE_GROUND_POUND 6
#define BATTLEMOVE_BREEZE_ATTACK 7
#define BATTLEMOVE_FIRE_CHARGE 8
#define BATTLEMOVE_SPLASH_ATTACK 9
#define BATTLEMOVE_SHOCK_SPRINT 10
#define BATTLEMOVE_IRON_SLAM 11
#define BATTLEMOVE_SPIRIT_SMACK 12
#define BATTLEMOVE_FLORAL_ATTACK 13
#define BATTLEMOVE_DEADLY_SWIPE 14

//TOADD:

//#define BATTLEMOVE_BLOCK 15
//#define BATTLEMOVE_SWITCH_OUT 16
//#define BATTLEMOVE_STONE_THROW 17
//#define BATTLEMOVE_WATER_SPRAY 18
//#define BATTLEMOVE_ZAP 19
//#define BATTLEMOVE_RAPID_HIT 20
//#define BATTLEMOVE_SMOLDER 21
//#define BATTLEMOVE_HIGH_WINDS 22
//#define BATTLEMOVE_PETAL_SHOWER 23

//

#define NUM_BATTLEMOVES (BATTLEMOVE_DEADLY_SWIPE + 1)

#define MONSTER_HAS_MAX_MOVES 0xFFFF
#define MONSTER_ALREADY_KNOWS_MOVE 0xFFFE
#define MONSTER_MOVE_NONE 0

#define EFFECT_NULL 0
#define EFFECT_HIT 1
#define EFFECT_MULTI_HIT 2
#define EFFECT_HIT_FLINCH 3
#define EFFECT_ATTACK_UP 4
#define EFFECT_ATTACK_DOWN 5
#define EFFECT_DEFENSE_UP 6
#define EFFECT_DEFENSE_DOWN 7
#define EFFECT_PSI_UP 8
#define EFFECT_PSI_DOWN 9
#define EFFECT_RESOLVE_UP 10
#define EFFECT_RESOLVE_DOWN 11
#define EFFECT_BLOCK 12
#define EFFECT_SWITCH 13
#define EFFECT_HEAL 14

#define FLAG_CONTACTMOVE 1
#define FLAG_BLOCKABLEMOVE 2
#define FLAG_STATUSMOVE 3
#define FLAG_PRIORITY_1 4
#define FLAG_PRIORITY_2 5
#define FLAG_PRIORITY_3 6
#define FLAG_PRIORITY_4 7
#define FLAG_PRIORITY_LAST 8

#define SPLIT_PHYS 0x0
#define SPLIT_PSI 0x1
#define SPLIT_STATUS 0x2

#define MOVE_TARGET_OPPONENT 0
#define MOVE_TARGET_SELF 1

///////////////////////////////MOVES///////////////////////////////////

////////////////////////SIGNATURE//MOVES///////////////////////////////

#define BATTLESIGMOVE_NULL 0
#define BATTLESIGMOVE_AGGRESIVE_ATTACK 1
#define BATTLESIGMOVE_AGGRESIVE_ELEMENT 2

#define NUM_BATTLESIGMOVES (3 - 1)

////////////////////////SIGNATURE//MOVES///////////////////////////////

///////////////////////////ABILITIES///////////////////////////////////

#define NORMAL_ABILITY_SLOTS 2
#define HIDDEN_ABILITY_SLOTS 1

#define ABILITY_NAME_LENGTH 16
#define NUM_ABILITY_SLOTS (NORMAL_ABILITY_SLOTS + HIDDEN_ABILITY_SLOTS)

///////

#define ABILITY_NONE 0
#define ABILITY_RUFF_HOUSE 1	//same gender +atk
#define ABILITY_KEEN_EYE 2		//+crits
#define ABILITY_WATCHFUL 3		//-crits on opponent???
#define ABILITY_SLIMY_SKIN 4	//clears status 50% chance per turn
#define ABILITY_SWIFT_SWIMMER 5	//faster speed in water
#define ABILITY_DRY_BODY 6		//half damage from water moves
#define ABILITY_HYDROPHILIC 7	//immune to water damage, heals 1/4 max hp
#define ABILITY_ROCK_SOLID 8	//contact moves increase defense
#define ABILITY_INTIMIDATE 9	//-atk on opponent
#define ABILITY_PHOTOSYNTHESIS 10 //Heal 1/8 max hp while sun is out
#define ABILITY_SOLAR_POWER 11	//+50% to life attacks while sun is out
#define ABILITY_FORESIGHT 12	//Reveals an opponents move at random
#define ABILITY_THORNS 13		//Recieving contact damage deals 1/12 max hp dmg back
#define ABILITY_BRAVERY 14		//+defense and +resolve when at under 1/3 max hp


#define NUM_ABILITIES (4 - 1)

///////////////////////////ABILITIES///////////////////////////////////

///////////////////////////SCRIPTS////////////////////////////////////

#define NULL_SCRIPT 0
#define INTRO_SCRIPT 1
#define HOME_SCRIPT 2
#define PROFESSOR_SCRIPT 3
#define PREVENT_PROFESSOR_SCRIPT 4
#define RIVAL_1_SCRIPT 5

#define TOTAL_SCENE_SCRIPTS (6)

#define MAX_NPCS_PER_SCRIPT 8

///////////////////////////SCRIPTS////////////////////////////////////

