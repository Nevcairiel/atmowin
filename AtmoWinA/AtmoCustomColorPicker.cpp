/*
 * AtmoCustomColorPicker.cpp: basic dialog for color selection - used only for inheritance to CAtmoColorPicker and CAtmoWhiteSetup
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#include "StdAfx.h"
#include "atmocustomcolorpicker.h"

#include "StdAfx.h"
#include "resource.h"
#include "windowsx.h"
#include "AtmoConfig.h"
#include "AtmoConnection.h"
#include "AtmoTools.h"
#include "commctrl.h"

CAtmoCustomColorPicker::CAtmoCustomColorPicker(HINSTANCE hInst, HWND parent, WORD dlgRessourceID, CAtmoDynData *pAtmoDynData, int red,int green,int blue):
                  CBasicDialog(hInst, dlgRessourceID, parent)
{
    this->m_pAtmoDynData = pAtmoDynData;
    outputColor(red,green,blue);
    m_iRed   = red;
    m_iGreen = green;
    m_iBlue  = blue;
}

CAtmoCustomColorPicker::~CAtmoCustomColorPicker(void)
{
}

void CAtmoCustomColorPicker::UpdateColorControls(ATMO_BOOL sliders,ATMO_BOOL edits) {
     // edits und sliders in sync!
     HWND hwndCtrl;
     char buffer[10];
     if(edits == ATMO_TRUE) {
        hwndCtrl = this->getDlgItem(IDC_EDT_RED);
        sprintf(buffer,"%d",this->m_iRed);
        Edit_SetText(hwndCtrl,buffer);

        hwndCtrl = this->getDlgItem(IDC_EDT_GREEN);
        sprintf(buffer,"%d",this->m_iGreen);
        Edit_SetText(hwndCtrl,buffer);

        hwndCtrl = this->getDlgItem(IDC_EDT_BLUE);
        sprintf(buffer,"%d",this->m_iBlue);
        Edit_SetText(hwndCtrl,buffer);
     }

     if(sliders == ATMO_TRUE) {
        hwndCtrl = this->getDlgItem(IDC_SL_RED);
        SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iRed);

        hwndCtrl = this->getDlgItem(IDC_SL_GREEN);
        SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iGreen);

        hwndCtrl = this->getDlgItem(IDC_SL_BLUE);
        SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iBlue);
     }
}

ATMO_BOOL CAtmoCustomColorPicker::InitDialog(WPARAM wParam) {
     HWND hwndCtrl;
     hwndCtrl = this->getDlgItem(IDC_EDT_RED);
     Edit_LimitText(hwndCtrl,3);
     hwndCtrl = this->getDlgItem(IDC_EDT_GREEN);
     Edit_LimitText(hwndCtrl,3);
     hwndCtrl = this->getDlgItem(IDC_EDT_BLUE);
     Edit_LimitText(hwndCtrl,3);

     hwndCtrl = this->getDlgItem(IDC_SL_RED);
     SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
     SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
     SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iRed);
     SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

     hwndCtrl = this->getDlgItem(IDC_SL_GREEN);
     SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
     SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
     SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iGreen);
     SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

     hwndCtrl = this->getDlgItem(IDC_SL_BLUE);
     SendMessage(hwndCtrl, TBM_SETRANGEMIN, 0, 0);
     SendMessage(hwndCtrl, TBM_SETRANGEMAX, 0, 255);
     SendMessage(hwndCtrl, TBM_SETPOS, 1, this->m_iBlue);
     SendMessage(hwndCtrl, TBM_SETTICFREQ, 8, 0);

     UpdateColorControls(ATMO_TRUE, ATMO_TRUE);
     return ATMO_TRUE;
}

ATMO_BOOL CAtmoCustomColorPicker::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {
    switch(wmId) {
        case IDOK: {
           EndDialog(this->m_hDialog, wmId);
           break;
        }

        case IDCANCEL: {
           EndDialog(this->m_hDialog, wmId);
           break;
        }

        case IDC_EDT_RED: {
             if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=0) && (value<=255)) {
                      this->m_iRed = value;
                      UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
                      outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
             }
             break;
        }

        case IDC_EDT_GREEN: {
             if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=0) && (value<=255)) {
                      this->m_iGreen = value;
                      UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
                      outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
             }
             break;
        }

        case IDC_EDT_BLUE: {
             if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=0) && (value<=255)) {
                      this->m_iBlue = value;
                      UpdateColorControls(ATMO_TRUE, ATMO_FALSE);
                      outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
             }
             break;
        }

      default:
          return ATMO_FALSE;
    }
 return ATMO_TRUE;
}

void CAtmoCustomColorPicker::HandleHorzScroll(int code,int position,HWND scrollBarHandle) {
    // WM_HSCROLL handler!
    //  WM_HSCROLL

    int dlgItemId = GetDlgCtrlID(scrollBarHandle);
    switch(dlgItemId) {
           case IDC_SL_RED:
                this->m_iRed   = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                break;

           case IDC_SL_GREEN:
                this->m_iGreen = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                break;

           case IDC_SL_BLUE:
                this->m_iBlue  = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                break;
           default:
               return;

    }
   UpdateColorControls(ATMO_FALSE, ATMO_TRUE);
   outputColor(this->m_iRed, this->m_iGreen, this->m_iBlue);
}


void CAtmoCustomColorPicker::outputColor(int red,int green,int blue) {
}


