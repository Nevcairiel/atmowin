#pragma once
#include <comdef.h>		
#include "AtmoWin_h.h"
#include "AtmoDefs.h"
#include "atmoregistry.h"

class CAtmoComRegistry
{
private:
    CAtmoComRegistry(void);
    ~CAtmoComRegistry(void);

public:
    static void SaveSettings(ATMO_BOOL force);
    static void DeleteSettings();
};
