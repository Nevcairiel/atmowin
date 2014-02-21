#include "StdAfx.h"
#include "atmoclassicconfigdialog.h"
#include "resource.h"

CAtmoClassicConfigDialog::CAtmoClassicConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
  CBasicDialog(hInst, IDD_CFG_ATMO_DLG, parent)
{
   this->m_pConfig = pConfig;
}

CAtmoClassicConfigDialog::~CAtmoClassicConfigDialog(void)
{
}


ATMO_BOOL CAtmoClassicConfigDialog::InitDialog(WPARAM wParam) {

        m_hCbxComports = getDlgItem(IDC_COMPORT);
        InitDialog_ComPorts( m_hCbxComports );
        int com = m_pConfig->getComport() - 1;
        if(com < 0) com = 0;

        ComboBox_SetCurSel(m_hCbxComports, com);

        return ATMO_FALSE;
}


ATMO_BOOL CAtmoClassicConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {

    switch(wmId) {
        case IDOK: {
           int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));
           m_pConfig->setComport(comportSel + 1);

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
