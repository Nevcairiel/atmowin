/*
 * AtmoWhiteSetup.cpp: Dialog for doing the white calibration of the LED Stripes in the software
 * works with all versions of the hardware but consumes some CPU ressources
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#include "StdAfx.h"

#include "resource.h"
#include "atmowhitesetup.h"
#include "atmocustomcolorpicker.h"
#include "atmotools.h"


CAtmoWhiteSetup::CAtmoWhiteSetup(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int red,int green,int blue,ATMO_BOOL useSoftware)
                : CAtmoCustomColorPicker(hInst, parent, IDD_WHITEADJ, pAtmoDynData, red, green, blue)
{
    CAtmoConfig *config = pAtmoDynData->getAtmoConfig();

    this->m_UseSoftware = useSoftware;

    m_GammaCorrect = config->getSoftware_gamma_mode();
    m_Global_Gamma = config->getSoftware_gamma_global();
    m_Gamma_Red    = config->getSoftware_gamma_red();
    m_Gamma_Green  = config->getSoftware_gamma_green();
    m_Gamma_Blue   = config->getSoftware_gamma_blue();
}

CAtmoWhiteSetup::~CAtmoWhiteSetup(void)
{
}

ATMO_BOOL CAtmoWhiteSetup::InitDialog(WPARAM wParam) {
     CAtmoCustomColorPicker::InitDialog(wParam);

     CAtmoConfig *config = m_pAtmoDynData->getAtmoConfig();

     HWND hwndCtrl;

     hwndCtrl = this->getDlgItem(IDC_ENABLE);
     if(this->m_UseSoftware)
        Button_SetCheck(hwndCtrl, BST_CHECKED);
     else
        Button_SetCheck(hwndCtrl, BST_UNCHECKED);

     // config->getSoftware_gamma_global();
     // config->getSoftware_gamma_red();
     // config->getSoftware_gamma_green();
     // config->getSoftware_gamma_blue();
     // AtmoGammaCorrect mode = config->getSoftware_gamma_mode();

     // order of items must match enum AtmoGammaCorrect!!
     hwndCtrl = getDlgItem(IDC_SW_GAMMA_MODE);
     ComboBox_AddString(hwndCtrl, "deaktiviert");
     ComboBox_AddString(hwndCtrl, "je Farbanteil");
     ComboBox_AddString(hwndCtrl, "Global");
     ComboBox_SetCurSel(hwndCtrl, int(m_GammaCorrect) );
 
     hwndCtrl = getDlgItem(IDC_EDT_GLOBALGAMMA);
     Edit_LimitText(hwndCtrl,3);
     hwndCtrl = getDlgItem(IDC_EDT_GAMMA_RED);
     Edit_LimitText(hwndCtrl,3);
     hwndCtrl = getDlgItem(IDC_EDT_GAMMA_GREEN);
     Edit_LimitText(hwndCtrl,3);
     hwndCtrl = getDlgItem(IDC_EDT_GAMMA_BLUE);
     Edit_LimitText(hwndCtrl,3);
     
     SetupSliderControl(IDC_SL_GLOBALGAMMA, 5, 35, m_Global_Gamma, 1);
     SetEditDouble(IDC_EDT_GLOBALGAMMA, (double)m_Global_Gamma / 10.0);

     // 1.1 - bis 3.5
     SetupSliderControl(IDC_SL_GAMMA_RED,5,35,m_Gamma_Red, 1);
     SetEditDouble(IDC_EDT_GAMMA_RED, (double)m_Gamma_Red / 10.0);

     SetupSliderControl(IDC_SL_GAMMA_GREEN,5,35,m_Gamma_Green,1);
     SetEditDouble(IDC_EDT_GAMMA_GREEN, (double)m_Gamma_Green / 10.0);

     SetupSliderControl(IDC_SL_GAMMA_BLUE2,5,35, m_Gamma_Blue , 1);
     SetEditDouble(IDC_EDT_GAMMA_BLUE, (double)m_Gamma_Blue / 10.0);


     SetWindowText(this->m_hDialog,"Weissabgleich"); //?
     return ATMO_TRUE;
}

void CAtmoWhiteSetup::HandleHorzScroll(int code,int position,HWND scrollBarHandle) {
    int dlgItemId = GetDlgCtrlID(scrollBarHandle);

    int pos   = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);

    switch(dlgItemId) {
          case IDC_SL_GLOBALGAMMA:
               m_Global_Gamma = pos;
               SetEditDouble(IDC_EDT_GLOBALGAMMA, (double)m_Global_Gamma / 10.0);
               break;

          case IDC_SL_GAMMA_RED:
               m_Gamma_Red = pos;
               SetEditDouble(IDC_EDT_GAMMA_RED, (double)m_Gamma_Red / 10.0);
               break;

          case IDC_SL_GAMMA_GREEN:
               m_Gamma_Green = pos;
               SetEditDouble(IDC_EDT_GAMMA_GREEN, (double)m_Gamma_Green / 10.0);
               break;

          case IDC_SL_GAMMA_BLUE2:
               m_Gamma_Blue = pos;
               SetEditDouble(IDC_EDT_GAMMA_BLUE, (double)m_Gamma_Blue / 10.0);
               break;

           default:
               CAtmoCustomColorPicker::HandleHorzScroll(code, position, scrollBarHandle);
               return;
    }
}


ATMO_BOOL CAtmoWhiteSetup::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {
    switch(wmId) {
        case IDC_ENABLE: {
            if( Button_GetCheck(hControl) == BST_CHECKED) {
                m_UseSoftware = ATMO_TRUE;
            } else {
                m_UseSoftware = ATMO_FALSE;
            }
            break;
        }

        case IDC_SW_GAMMA_MODE: {
             if(wmEvent == CBN_SELCHANGE) {
                int i = ComboBox_GetCurSel(hControl);
                if(i>=0) {
                   m_GammaCorrect = (AtmoGammaCorrect)i;
                }
             }
             break;
        }

        case IDC_EDT_GLOBALGAMMA: {
             if(wmEvent == EN_CHANGE) {
                double value;
                if(this->GetEditDouble(hControl,value) && (value>=0.5) && (value<=3.5)) {
                    m_Global_Gamma = int(value * 10);
                    SendMessage(getDlgItem(IDC_SL_GLOBALGAMMA), TBM_SETPOS, 1, m_Global_Gamma);
                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }

        case IDC_EDT_GAMMA_RED: {
             if(wmEvent == EN_CHANGE) {
                double value; // 1.1 3.5
                if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f)) {
                    int iValue = (int)(value*10.0);
                    m_Gamma_Red = iValue;
                    SendMessage(getDlgItem(IDC_SL_GAMMA_RED), TBM_SETPOS, 1, iValue);
                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }

        case IDC_EDT_GAMMA_GREEN: {
             if(wmEvent == EN_CHANGE) {
                double value; // 1.1 3.5
                if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f)) {
                    int iValue = (int)(value*10);
                    m_Gamma_Green = iValue;
                    SendMessage(getDlgItem(IDC_SL_GAMMA_GREEN), TBM_SETPOS, 1, iValue);
                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }

        case IDC_EDT_GAMMA_BLUE: {
             if(wmEvent == EN_CHANGE) {
                double value; // 1.1 3.5
                if(this->GetEditDouble(hControl,value) && (value>=1.1f) && (value<=3.5f)) {
                    int iValue = (int)(value*10);
                    m_Gamma_Blue = iValue;
                    SendMessage(getDlgItem(IDC_SL_GAMMA_BLUE2), TBM_SETPOS, 1, iValue);
                } else {
                    MessageBeep(MB_ICONEXCLAMATION);
                }
             }
             break;
        }


        default:
            return CAtmoCustomColorPicker::ExecuteCommand(hControl, wmId, wmEvent);
    }

   return ATMO_TRUE;
}

void CAtmoWhiteSetup::outputColor(int red,int green,int blue) {
     // red, green, blue -- are no color value -- thats are norming value for red,green and blue;!
     CAtmoConnection *pAtmoConnection = this->m_pAtmoDynData->getAtmoConnection();
     if((pAtmoConnection==NULL) || (pAtmoConnection->isOpen()==ATMO_FALSE)) return;

         // hardware weissableich ausser kraft setzen!
         // mittels Dummypaket? (alte Hardware wird dieses Paket stillschweigend ignorieren hoffe ich)
     pColorPacket packet;
     AllocColorPacket(packet, m_pAtmoDynData->getAtmoConfig()->getZoneCount());
     for(int i=0; i < packet->numColors; i++) {
         packet->zone[i].r = (255 * red) / 255;
         packet->zone[i].g = (255 * green) / 255;
         packet->zone[i].b = (255 * blue) / 255;
     }

     pAtmoConnection->SendData(packet);

     delete (char *)packet;
}

ATMO_BOOL CAtmoWhiteSetup::Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int &red, int &green, int &blue, ATMO_BOOL &useSoftware) {
       // so kann ColorPicker sein aktuellen Farbwert ausgeben - ohne etwas zu stören...
       EffectMode oldEffectMode = CAtmoTools::SwitchEffect(pAtmoDynData, emDisabled);

       CAtmoWhiteSetup *pAtmoWhiteSetup = new CAtmoWhiteSetup(hInst, parent, pAtmoDynData, red, green, blue, useSoftware);

       ATMO_BOOL result = (pAtmoWhiteSetup->ShowModal() == IDOK);
       if(result == ATMO_TRUE) {
           red   = pAtmoWhiteSetup->m_iRed;
           green = pAtmoWhiteSetup->m_iGreen;
           blue  = pAtmoWhiteSetup->m_iBlue;
           useSoftware = pAtmoWhiteSetup->m_UseSoftware;

           CAtmoConfig *pConfig = pAtmoDynData->getAtmoConfig();
           pConfig->setSoftware_gamma_global( pAtmoWhiteSetup->m_Global_Gamma );
           pConfig->setSoftware_gamma_red( pAtmoWhiteSetup->m_Gamma_Red );
           pConfig->setSoftware_gamma_green( pAtmoWhiteSetup->m_Gamma_Green );
           pConfig->setSoftware_gamma_blue( pAtmoWhiteSetup->m_Gamma_Blue );
           pConfig->setSoftware_gamma_mode( pAtmoWhiteSetup->m_GammaCorrect );
       }
       delete pAtmoWhiteSetup;

       CAtmoTools::SwitchEffect(pAtmoDynData, oldEffectMode);

       return result;
}
