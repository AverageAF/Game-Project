
#include "flags.h"


void SetGameFlag(uint8_t flag)
{
    gGameFlags[flag] = TRUE;
}

void ClearGameFlag(uint8_t flag)
{
    gGameFlags[flag] = FALSE;
}

void ClearTempGameFlags(void)
{
    for (uint8_t tempflag = FLAG_TEMP_1; tempflag < TEMP_FLAGS_END; tempflag++)
    {
        gGameFlags[tempflag] = FALSE;
    }
}

//void InitTempGameFlagsFromGameArea(void)

void ClearAllGameFlags(void)
{
    for (uint8_t flags = START_OF_FLAGS; flags < END_OF_FLAGS + 1; flags++) //clear all flags
    {
        gGameFlags[flags] = FALSE;
    }
}