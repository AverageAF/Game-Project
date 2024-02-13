
#include "InventoryItems.h"

#include "SimpleConstants.h"

InventoryItem gEquipableItems[NUM_EQUIP_ITEMS] = {
    [INV_EQUIP_ITEM_0] =
    {
        .ValueCurrency = 0,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "ERROR NULL ITEM",
        .Effect = ITEM_EQUIP_EFFECT_NULL,
        .Name = "NULL",
        .Sprite = 0,
        .Favorite = FALSE,
    },



    [INV_EQUIP_ITEM_1] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of NONE \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "None booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_2] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of EARTH \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Earth booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_3] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of AIR \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Air booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_4] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of FIRE \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Fire booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_5] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of WATER \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Water booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_6] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of ELECTRIC \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Electric booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_7] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of METAL \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Metal booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_8] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of SOUL \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Soul booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_9] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of LIFE \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Life booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_10] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Boosts the damage of DEATH \nelement moves while equipped \n \n +25% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Death booster",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_11] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof NONE element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Smooth stone",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_12] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof EARTH element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Dry clay",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_13] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof AIR element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Light feather",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_14] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof FIRE element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Wax candle",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_15] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof WATER element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Teardrop pearl",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_16] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof ELECTRIC element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Spent sparkplug",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_17] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof METAL element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Abrasive whetstone",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_18] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof SOUL element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Torn tarot",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_19] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof LIFE element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Blooming sunflower",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_20] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Slightly boosts the damage \nof DEATH element moves \nwhile equipped \n \n +10% damage",
        .Effect = ITEM_EQUIP_EFFECT_ELEMENT_BOOST,
        .Name = "Razor blade",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_21] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 21,
        .Name = "EQUIPITEM 21",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_22] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 22,
        .Name = "EQUIPITEM 22",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_23] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 23,
        .Name = "EQUIPITEM 23",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_EQUIP_ITEM_24] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 24,
        .Name = "EQUIPITEM 24",
        .Sprite = 0,
        .Favorite = FALSE,
    },

};




InventoryItem gUseableItems[NUM_USABLE_ITEMS] = {
    [INV_USABLE_ITEM_0] =
    {
        .ValueCurrency = 100,
        .HasItem = FALSE,
        .Count = 1,
        .Description = "A common medicine to heal \nmonsters, heals 20HP",
        .Effect = ITEM_USE_EFFECT_HEAL_MONSTER,
        .Name = "HP Useable [MKI]",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_1] =
    {
        .ValueCurrency = 800,
        .HasItem = FALSE,
        .Count = 1,
        .Description = "Monster medicine, heals 50HP",
        .Effect = ITEM_USE_EFFECT_HEAL_MONSTER,
        .Name = "HP Useable [MKII]",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_2] =
    {
        .ValueCurrency = 1600,
        .HasItem = FALSE,
        .Count = 1,
        .Description = "Rare medicine, heals 100HP",
        .Effect = ITEM_USE_EFFECT_HEAL_MONSTER,
        .Name = "HP Useable [MKIII]",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_3] =
    {
        .ValueCurrency = 2700,
        .HasItem = FALSE,
        .Count = 1,
        .Description = "Strong medicine, heals 250HP",
        .Effect = ITEM_USE_EFFECT_HEAL_MONSTER,
        .Name = "HP Useable [MKIV]",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_4] =
    {
        .ValueCurrency = 4200,
        .HasItem = FALSE,
        .Count = 1,
        .Description = "Potent monster medicine, \ncompletely heals to MAX HP",
        .Effect = ITEM_USE_EFFECT_HEAL_MONSTER,
        .Name = "HP Useable [MKV]",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_5] =
    {
        .ValueCurrency = 100,
        .HasItem = FALSE,
        .Count = 5,
        .Description = "A simple capture device \nfor wild monsters",
        .Effect = ITEM_USE_EFFECT_CAPTURE,
        .Name = "Capture Device A",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_6] =
    {
        .ValueCurrency = 1000,
        .HasItem = FALSE,
        .Count = 2,
        .Description = "Greater capture device",
        .Effect = ITEM_USE_EFFECT_CAPTURE,
        .Name = "Capture Device B",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_7] =
    {
        .ValueCurrency = 1600,
        .HasItem = TRUE,
        .Count = 100,
        .Description = "Grand capture device",
        .Effect = ITEM_USE_EFFECT_CAPTURE,
        .Name = "Capture Device C",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_8] =
    {
        .ValueCurrency = 50,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Gives +100 Experience",
        .Effect = ITEM_USE_EFFECT_EXP_MONSTER,
        .Name = "Tiny Exp shard",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_9] =
    {
        .ValueCurrency = 125,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Gives +250 Experience",
        .Effect = ITEM_USE_EFFECT_EXP_MONSTER,
        .Name = "Small Exp shard",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_10] =
    {
        .ValueCurrency = 250,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Gives +500 Experience",
        .Effect = ITEM_USE_EFFECT_EXP_MONSTER,
        .Name = "Medium Exp shard",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_11] =
    {
        .ValueCurrency = 500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Gives +1000 Experience",
        .Effect = ITEM_USE_EFFECT_EXP_MONSTER,
        .Name = "Greater Exp shard",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_12] =
    {
        .ValueCurrency = 1500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Gives +3000 Experience",
        .Effect = ITEM_USE_EFFECT_EXP_MONSTER,
        .Name = "Large Exp shard",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_13] =
    {
        .ValueCurrency = 5500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Gives +10000 Experience",
        .Effect = ITEM_USE_EFFECT_EXP_MONSTER,
        .Name = "Massive Exp shard",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_14] =
    {
        .ValueCurrency = 7200,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Gives enough Exp to Level Up",
        .Effect = ITEM_USE_EFFECT_EXP_MONSTER,
        .Name = "Experience Crystal",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_15] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Topaz",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_16] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Opal",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_17] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Cinnabar",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_18] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Sapphire",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_19] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Magnetite",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_20] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Titanite",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_21] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Amethyst",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_22] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Jadeite",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_23] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A beautiful gemstone, \nbeloved by certain monsters",
        .Effect = ITEM_USE_EFFECT_UPGRADE,
        .Name = "Onyx",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_24] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Just barely brings any \nmonster back from being \nK.O.'ed",
        .Effect = ITEM_USE_EFFECT_REVIVE_MONSTER,
        .Name = "Weak Revive",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_25] =
    {
        .ValueCurrency = 2500,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Brings any monster back \nfrom being K.O.'ed with \nhalf HP",
        .Effect = ITEM_USE_EFFECT_REVIVE_MONSTER,
        .Name = "Monster Revive",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_USABLE_ITEM_26] =
    {
        .ValueCurrency = 5000,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Brings any monster back \nfrom being K.O.'ed with \nfull HP",
        .Effect = ITEM_USE_EFFECT_REVIVE_MONSTER,
        .Name = "Full Revive",
        .Sprite = 0,
        .Favorite = FALSE,
    },

};


InventoryItem gValuableItems[NUM_VALUABLE_ITEMS] = {
    [INV_VALUABLE_ITEM_0] =
    {
        .ValueCurrency = 0,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "ERROR NULL",
        .Effect = 0,
        .Name = "NULL",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_1] =
    {
        .ValueCurrency = 250,
        .HasItem = TRUE,
        .Count = 10,
        .Description = "-----",
        .Effect = 0,
        .Name = "GOLD 1",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_2] =
    {
        .ValueCurrency = 500,
        .HasItem = TRUE,
        .Count = 10,
        .Description = "-----",
        .Effect = 0,
        .Name = "GOLD 2",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_3] =
    {
        .ValueCurrency = 1000,
        .HasItem = TRUE,
        .Count = 10,
        .Description = "-----",
        .Effect = 0,
        .Name = "GOLD 3",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_4] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 4",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_5] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 5",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_6] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 6",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_7] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 7",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_8] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 8",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_9] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 9",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_10] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 10",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_11] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 11",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_12] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 12",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_13] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 13",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_14] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 14",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_15] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 15",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_16] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 16",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_17] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 17",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_18] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 18",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_19] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 19",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_20] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 20",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_21] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 21",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_22] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 22",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_23] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 23",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_VALUABLE_ITEM_24] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "VALUABLEITEM 24",
        .Sprite = 0,
        .Favorite = FALSE,
    },

};


InventoryItem gAdventureItems[NUM_ADVENTURE_ITEMS] = {
    [INV_ADVENTURE_ITEM_0] =
    {
        .ValueCurrency = 0,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "ERROR: NULL ITEM",
        .Effect = 0,
        .Name = "NULL",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_1] =
    {
        .ValueCurrency = 0,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Hold CTRL to run, or \nUSE to toggle running",
        .Effect = 1,
        .Name = "Sprinting Shoes",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_2] =
    {
        .ValueCurrency = 0,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "A map of the local region",
        .Effect = 1,
        .Name = "Region Map",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_3] =
    {
        .ValueCurrency = 0,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "Your personal collection of \ndata on all monsters that you \ndiscover",
        .Effect = 1,
        .Name = "Monster Journal",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_4] =
    {
        .ValueCurrency = 0,
        .HasItem = TRUE,
        .Count = 1,
        .Description = "This device will call an AIR \nelement monster to fly you to \nany discovered town or city",
        .Effect = 1,
        .Name = "Flying Device",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_5] =
    {
        .ValueCurrency = 0,
        .HasItem = FALSE,
        .Count = 1,
        .Description = "This glorious pendant displays \nyour victories against the \nElemental Challengers",
        .Effect = 1,
        .Name = "Triumphant Pendant",
        .Sprite = 0,                                    ////assign this sprite to a variable, allow the pendant sprite to change with every victory
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_6] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 6",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_7] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 7",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_8] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 8",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_9] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 9",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_10] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 10",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_11] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 11",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_12] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 12",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_13] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 13",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_14] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 14",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_15] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 15",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_16] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 16",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_17] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 17",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_18] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 18",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_19] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 19",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_20] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 20",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_21] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 21",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_22] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 22",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_23] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 23",
        .Sprite = 0,
        .Favorite = FALSE,
    },


    [INV_ADVENTURE_ITEM_24] =
    {
        .ValueCurrency = 1,
        .HasItem = FALSE,
        .Count = 0,
        .Description = "-----",
        .Effect = 0,
        .Name = "ADVENTUREITEM 24",
        .Sprite = 0,
        .Favorite = FALSE,
    },

};


