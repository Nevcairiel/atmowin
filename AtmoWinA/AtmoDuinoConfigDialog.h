#pragma once
#include "basicdialog.h"
#include "AtmoConfig.h"

class CAtmoDuinoConfigDialog :
    public CBasicDialog
{
private:
    CAtmoConfig *m_pConfig;

private: // handles...
    HWND m_hCbxComports;

protected:
    virtual ATMO_BOOL InitDialog(WPARAM wParam);
    virtual ATMO_BOOL ExecuteCommand(HWND hControl,int wmId, int wmEvent);

public:
    CAtmoDuinoConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig);
    ~CAtmoDuinoConfigDialog(void);
};


