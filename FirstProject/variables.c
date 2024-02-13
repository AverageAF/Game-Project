
#include "variables.h"


void SetGameVar(uint8_t var, uint16_t value)
{
    gGameVariables[var] = value;
}

uint16_t GetGameVar(uint8_t var)
{
    return(gGameVariables[var]);
}

void ClearGameVar(uint8_t var)
{
    gGameVariables[var] = 0;
}

void ClearTempGameVars(void)
{
    for (uint8_t tempvar = VAR_TEMP_1; tempvar < TEMP_VARS_END; tempvar++)
    {
        gGameVariables[tempvar] = 0;
    }
}