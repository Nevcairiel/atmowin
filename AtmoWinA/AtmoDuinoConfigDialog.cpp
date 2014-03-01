#include "StdAfx.h"
#include "AtmoDuinoConfigDialog.h"
#include "resource.h"

CAtmoDuinoConfigDialog::CAtmoDuinoConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) :
CBasicDialog(hInst, IDD_CFG_ATMODUINO_DLG, parent)
{
    this->m_pConfig = pConfig;
}

CAtmoDuinoConfigDialog::~CAtmoDuinoConfigDialog(void)
{
}


ATMO_BOOL CAtmoDuinoConfigDialog::InitDialog(WPARAM wParam) {

    m_hCbxComports = getDlgItem(IDC_COMPORT);
    InitDialog_ComPorts(m_hCbxComports);
    int com = m_pConfig->getComport() - 1;
    if (com < 0) com = 0;

    ComboBox_SetCurSel(m_hCbxComports, com);

    char buf[10];
    HWND control;

    control = getDlgItem(IDC_ATMOCL_LED_AMOUNT);
    Edit_LimitText(control, 3);
    sprintf(buf, "%d", m_pConfig->getAtmoClLeds());
    Edit_SetText(control, buf);

    return ATMO_FALSE;
}


ATMO_BOOL CAtmoDuinoConfigDialog::ExecuteCommand(HWND hControl, int wmId, int wmEvent) {

    switch (wmId) {
    case IDOK: {
        int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));
        m_pConfig->setComport(comportSel + 1);

        HWND control = getDlgItem(IDC_ATMOCL_LED_AMOUNT);
        char buf[10];
        Edit_GetText(control, buf, sizeof(buf));
        m_pConfig->setAtmoClLeds(atoi(buf));

        EndDialog(this->m_hDialog, wmId);
        break;
    }

    case IDCANCEL: {
        // m_pBackupConfig --> wieder herstellen...
        EndDialog(this->m_hDialog, wmId);
        break;
    }

    default:
        return ATMO_FALSE;

    }

    return ATMO_TRUE;
}
