#pragma once

#include "Main.h"
#include "SimpleConstants.h"
#include "OverWorld.h"

#include "MonsterData.h"
#include "MonsterStorage.h"
	
#include "flags.h"
#include "variables.h"


void DrawStorageMenuScreen(void);

void PPI_StorageMenu(void);

uint16_t CalcXBoxLength(uint16_t xTotal, uint16_t xMargin1, uint16_t xMargin2, uint16_t xGap);

uint16_t CalcYBoxLength(uint16_t yTotal, uint16_t yMargin, uint16_t yGap);

uint8_t ConvSelectedDriveSlotToSelectedPartySlot(void);

uint8_t ConvSelectedPartySlotToSelectedColumn(void);