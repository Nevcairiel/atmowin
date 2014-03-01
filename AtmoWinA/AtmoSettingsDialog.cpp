/*
 * AtmoSettingsDialog.cpp: Setup Dialog for Atmo Win
 *
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#include "StdAfx.h"
#include "windowsx.h"

#include "atmosettingsdialog.h"
#include "AtmoConfig.h"
#include "AtmoThread.h"
#include "AtmoDisplays.h"

#include "AtmoConnection.h"

#include "AtmoTools.h"
#include "AtmoColorPicker.h"
#include "AtmoWhiteSetup.h"
#include "AtmoWhiteAdjHW.h"
#include "AtmoEditChannelAssignment.h"
#include "AtmoGradients.h"


#include "Resource.h"

CAtmoSettingsDialog::CAtmoSettingsDialog(HINSTANCE hInst, HWND parent, CAtmoDynData *pDynData) : CBasicDialog(hInst,IDD_SETUP,parent) {
    this->m_pDynData = pDynData;
    m_pBackupConfig = new CAtmoConfig();
    m_pBackupConfig->Assign( m_pDynData->getAtmoConfig() );
    m_SaveHardWhiteSetup = ATMO_FALSE;
}

CAtmoSettingsDialog::~CAtmoSettingsDialog(void) {
    delete m_pBackupConfig;
}

void CAtmoSettingsDialog::LoadDisplayList() {
     CAtmoDisplays *pAtmoDisplays = this->m_pDynData->getAtmoDisplays();
     HWND hwndCtrl;
     hwndCtrl = getDlgItem(IDC_DISPLAYS);

     ComboBox_ResetContent(hwndCtrl);

     for(int i=0;i<pAtmoDisplays->getCount();i++) {
         TAtmoDisplayInfo nfo = pAtmoDisplays->getDisplayInfo(i);
         ComboBox_AddString(hwndCtrl, nfo.infoText);
     }
     ComboBox_SetCurSel(hwndCtrl, m_pDynData->getAtmoConfig()->getLiveView_DisplayNr());
}

ATMO_BOOL CAtmoSettingsDialog::UpdateLiveViewValues(ATMO_BOOL showPreview) {
     HWND hwndCtrl;
     CAtmoDisplays *pAtmoDisplays = this->m_pDynData->getAtmoDisplays();
     CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();

     pAtmoDisplays->ReloadList(); // auf nummer sicher!

     hwndCtrl = getDlgItem(IDC_DISPLAYS);
     int i = ComboBox_GetCurSel(hwndCtrl);
     if(i>=pAtmoDisplays->getCount()) {
         pAtmoConfig->setLiveView_DisplayNr(0);
         LoadDisplayList();
         MessageBox(this->m_hDialog,"Ausgewähltes Display nicht mehr in der Liste ;-)","Fehler",MB_ICONERROR | MB_OK);
         return ATMO_FALSE;
     }
     pAtmoConfig->setLiveView_DisplayNr(i);


     // wird durch ComboBox Change Event schon gesetzt...
     //hwndCtrl = getDlgItem(IDC_FILTERMODE);
     //i = ComboBox_GetCurSel(hwndCtrl);
     //pAtmoConfig->setLiveViewFilterMode((AtmoFilterMode)i);


     // wird durch ClickEvent schon geändert...
     //hwndCtrl = getDlgItem(IDC_WIDESCREEN);
     //if(Button_GetCheck(hwndCtrl) == BST_CHECKED)
     //    pAtmoConfig->setLiveView_WidescreenMode(1);
     // else
     //    pAtmoConfig->setLiveView_WidescreenMode(0);


     if(showPreview == ATMO_TRUE) {
        CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
        if((pAtmoConnection!=NULL) && pAtmoConnection->isOpen()) {
            CAtmoTools::SwitchEffect(m_pDynData, emLivePicture);
         }
     }
     return ATMO_TRUE;
}


ATMO_BOOL CAtmoSettingsDialog::UpdateColorChangeValues(ATMO_BOOL showPreview) 
{
    HWND hEditDelay = this->getDlgItem(IDC_ED_COLORCHANGE_DELAY);
    HWND hEditSteps = this->getDlgItem(IDC_ED_COLORCHANGE_STEPS);
    char buffer[256];
    Edit_GetText(hEditDelay,buffer,255);
    int iDelay = atoi(buffer);
    if(iDelay < 10) {
       MessageBox(this->m_hDialog,"Delay should be more than 10ms for ColorChanger","Error",MB_OK | MB_ICONERROR);
       return ATMO_FALSE;
    }

    Edit_GetText(hEditSteps,buffer,255);
    atoi(buffer);
    int iSteps = atoi(buffer);
    if(iSteps < 1) {
        MessageBox(this->m_hDialog,"Steps should be more than zero for ColorChanger","Error",MB_OK | MB_ICONERROR);
        return ATMO_FALSE;
    }

    CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
    pAtmoConfig->setColorChanger_iDelay(iDelay);
    pAtmoConfig->setColorChanger_iSteps(iSteps);

    if(showPreview == ATMO_TRUE) {
        CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
        if ((m_pDynData->getEffectThread()==NULL) || (pAtmoConfig->getEffectMode()!=emColorChange))  {
            if((pAtmoConnection!=NULL) && pAtmoConnection->isOpen()) {
                CAtmoTools::SwitchEffect(m_pDynData, emColorChange);
            }
        }
    }

    return ATMO_TRUE;
}

ATMO_BOOL CAtmoSettingsDialog::UpdateLrColorChangeValues(ATMO_BOOL showPreview) 
{
    HWND hEditDelay = this->getDlgItem(IDC_ED_LRCOLORCHANGE_DELAY);
    HWND hEditSteps = this->getDlgItem(IDC_ED_LRCOLORCHANGE_STEPS);

    char buffer[256];
    Edit_GetText(hEditDelay,buffer,255);
    int iDelay = atoi(buffer);
    if(iDelay < 10) {
        MessageBox(this->m_hDialog,"Delay should be more than 10ms for LR ColorChanger","Error",MB_OK | MB_ICONERROR);
        return ATMO_FALSE;
    }

    Edit_GetText(hEditSteps,buffer,255);
    atoi(buffer);
    int iSteps = atoi(buffer);
    if(iSteps < 1) {
        MessageBox(this->m_hDialog,"Steps should be more than zero for LR ColorChanger","Error",MB_OK | MB_ICONERROR);
        return ATMO_FALSE;
    }

    CAtmoConfig *pAtmoConfig = m_pDynData->getAtmoConfig();
    pAtmoConfig->setLrColorChanger_iDelay(iDelay);
    pAtmoConfig->setLrColorChanger_iSteps(iSteps);

    if(showPreview == ATMO_TRUE) {
        CAtmoConnection *pAtmoConnection = m_pDynData->getAtmoConnection();
        if ((m_pDynData->getEffectThread()==NULL) || (pAtmoConfig->getEffectMode()!=emLrColorChange))  {
            if((pAtmoConnection!=NULL) && pAtmoConnection->isOpen()) {
                CAtmoTools::SwitchEffect(m_pDynData, emLrColorChange);
            }
        }
    }

    return ATMO_TRUE;
}

ATMO_BOOL CAtmoSettingsDialog::InitDialog(WPARAM wParam) 
{
        CAtmoConfig *config = m_pDynData->getAtmoConfig();
        char buffer[256],tmp[280];
        HWND hwndCtrl;

        GetWindowText(this->m_hDialog, buffer, sizeof(buffer));
        sprintf(tmp , "%s (%d x %d Edition)", buffer, CAP_WIDTH, CAP_HEIGHT );
        SetWindowText(this->m_hDialog, tmp);

        m_hCbxDevicetypes = getDlgItem(IDC_DEVICETYPE);
        for(int i=0; i<ATMO_DEVICE_COUNT; i++)
            ComboBox_AddString(m_hCbxDevicetypes, AtmoDeviceTypes[i]);
        if(m_pDynData->getAtmoConnection())
           ComboBox_SetCurSel(m_hCbxDevicetypes, (int)config->getConnectionType());
        else
           ComboBox_SetCurSel(m_hCbxDevicetypes, (int)-1);

        m_hCbxEffects = getDlgItem(IDC_EFFECTS);
        ComboBox_AddString(m_hCbxEffects, "Off");
        ComboBox_AddString(m_hCbxEffects, "Static colors");
        ComboBox_AddString(m_hCbxEffects, "Live");
        ComboBox_AddString(m_hCbxEffects, "Color changer");
        ComboBox_AddString(m_hCbxEffects, "Color changer L/R");
        // Achtung - dazu muss die Reihenfolge der Enum Deklaration in AtmoConfig.h mit obiger Liste Synchron sein*g*
        ComboBox_SetCurSel(m_hCbxEffects, (int)config->getEffectMode());


        hwndCtrl = this->getDlgItem(IDC_ED_COLORCHANGE_DELAY);
        sprintf(buffer,"%d",config->getColorChanger_iDelay());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,5);

        hwndCtrl = this->getDlgItem(IDC_ED_COLORCHANGE_STEPS);
        sprintf(buffer,"%d",config->getColorChanger_iSteps());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,4);

        hwndCtrl = this->getDlgItem(IDC_ED_LRCOLORCHANGE_DELAY);
        sprintf(buffer,"%d",config->getLrColorChanger_iDelay());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,5);

        hwndCtrl = this->getDlgItem(IDC_ED_LRCOLORCHANGE_STEPS);
        sprintf(buffer,"%d",config->getLrColorChanger_iSteps());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,4);

        hwndCtrl = this->getDlgItem(IDC_GDI_CAPURE_RATE);
        sprintf(buffer,"%d",config->getLiveView_GDI_FrameRate());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,2);

        hwndCtrl = this->getDlgItem(IDC_GDI_INTERLACE);
        sprintf(buffer,"%d",config->getLiveView_RowsPerFrame() );
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,2);


        hwndCtrl = this->getDlgItem(IDC_ED_ZONETOPC);
        sprintf(buffer,"%d",config->getZonesTopCount());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,2);

        hwndCtrl = this->getDlgItem(IDC_ED_ZONEBOTTOMC);
        sprintf(buffer,"%d",config->getZonesBottomCount());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,2);

        hwndCtrl = this->getDlgItem(IDC_ED_ZONELRC);
        sprintf(buffer,"%d",config->getZonesLRCount());
        Edit_SetText(hwndCtrl, buffer);
        Edit_LimitText(hwndCtrl,2);

        hwndCtrl = this->getDlgItem(IDC_CBX_ZONESUM);
        Button_SetCheck(hwndCtrl, config->getZoneSummary());

        hwndCtrl = this->getDlgItem(IDC_CHK_USESHUTDOWNCOLOR);
        Button_SetCheck(hwndCtrl, config->isSetShutdownColor());

        LoadDisplayList();


/*
     enum AtmoFilterMode { --> AtmoDefs.h!!
     afmNoFilter,
     afmCombined,
     afmPercent
  };
*/
        hwndCtrl = getDlgItem(IDC_FILTERMODE);
        ComboBox_AddString(hwndCtrl, "No Filter");
        ComboBox_AddString(hwndCtrl, "Combined");
        ComboBox_AddString(hwndCtrl, "Percent");
        ComboBox_SetCurSel(hwndCtrl, (int)config->getLiveViewFilterMode());

        hwndCtrl = getDlgItem(IDC_WIDESCREEN);
        Button_SetCheck(hwndCtrl, config->getLiveView_WidescreenMode());

        SetupSliderControl(IDC_EdgeWeighting,1,30,config->getLiveView_EdgeWeighting(),5);
        SendMessage(getDlgItem(IDC_EdgeWeighting),TBM_SETPAGESIZE,0,2);
        sprintf(buffer,"Edge weighting [%d]",config->getLiveView_EdgeWeighting());
        SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);

        SetupSliderControl(IDC_BrightCorrect,50,300,config->getLiveView_BrightCorrect(),15);
        SendMessage(getDlgItem(IDC_BrightCorrect),TBM_SETPAGESIZE,0,10);
        sprintf(buffer,"Brightness [%d%%]",config->getLiveView_BrightCorrect());
        SetStaticText(IDC_TXT_BRIGHTCORRECT, buffer);

        SetupSliderControl(IDC_DarknessLimit,0,10,config->getLiveView_DarknessLimit(),1);
        SendMessage(getDlgItem(IDC_DarknessLimit),TBM_SETPAGESIZE,0,1);
        sprintf(buffer,"Darkness Limit [%d]",config->getLiveView_DarknessLimit());
        SetStaticText(IDC_TXT_DARKNESSLIMIT, buffer);

        SetupSliderControl(IDC_HueWinSize,0,5,config->getLiveView_HueWinSize(),1);
        SendMessage(getDlgItem(IDC_HueWinSize),TBM_SETPAGESIZE,0,1);
        sprintf(buffer,"Hue windowing [%d]",config->getLiveView_HueWinSize());
        SetStaticText(IDC_TXT_HUEWINSIZE, buffer);

        SetupSliderControl(IDC_SatWinSize,0,5,config->getLiveView_SatWinSize(),1);
        SendMessage(getDlgItem(IDC_SatWinSize),TBM_SETPAGESIZE,0,1);
        sprintf(buffer,"Sat windowing [%d]",config->getLiveView_SatWinSize());
        SetStaticText(IDC_TXT_SATWINSIZE, buffer);

        SetupSliderControl(IDC_Filter_MeanLength,300,5000,config->getLiveViewFilter_MeanLength(),0);
        SendMessage(getDlgItem(IDC_Filter_MeanLength),TBM_SETPAGESIZE,0,100);
        sprintf(buffer,"Filter length [%d ms]",config->getLiveViewFilter_MeanLength());
        SetStaticText(IDC_TXT_Filter_MeanLength, buffer);

        SetupSliderControl(IDC_Filter_MeanThreshold,1,100,config->getLiveViewFilter_MeanThreshold(),0);
        SendMessage(getDlgItem(IDC_Filter_MeanThreshold),TBM_SETPAGESIZE,0,5);
        sprintf(buffer,"Filter threshold [%d%%]",config->getLiveViewFilter_MeanThreshold());
        SetStaticText(IDC_TXT_Filter_MeanThreshold, buffer);

        SetupSliderControl(IDC_Filter_PercentNew,1,100,config->getLiveViewFilter_PercentNew(),0);
        SendMessage(getDlgItem(IDC_Filter_PercentNew),TBM_SETPAGESIZE,0,5);
        sprintf(buffer,"Filter smoothness [%d%%]",config->getLiveViewFilter_PercentNew());
        SetStaticText(IDC_TXT_Filter_PercentNew, buffer);

        return ATMO_FALSE;
}

void CAtmoSettingsDialog::HandleHorzScroll(int code,int position,HWND scrollBarHandle) {
     // slider handling...
    char buffer[100];
    int dlgItemId = GetDlgCtrlID(scrollBarHandle);
    switch(dlgItemId) {
           case IDC_EdgeWeighting: {
                int EdgeWeighting = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Edge weighting [%d]",EdgeWeighting);
                SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_EdgeWeighting(EdgeWeighting);
                break;
           }

           case IDC_BrightCorrect: {
                int BrightCorrect = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Brightness [%d%%]",BrightCorrect);
                SetStaticText(IDC_TXT_BRIGHTCORRECT, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_BrightCorrect(BrightCorrect);
                break;
           }
           case IDC_DarknessLimit: {
                int DarknessLimit = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Darkness Limit [%d]",DarknessLimit);
                SetStaticText(IDC_TXT_DARKNESSLIMIT, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_DarknessLimit(DarknessLimit);
                break;
           }

           case IDC_HueWinSize: {
                int HueWinSize = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Hue windowing [%d]",HueWinSize);
                SetStaticText(IDC_TXT_HUEWINSIZE, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_HueWinSize(HueWinSize);
                break;
           }

           case IDC_SatWinSize: {
                int SatWinSize = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Sat windowing [%d]",SatWinSize);
                SetStaticText(IDC_TXT_SATWINSIZE, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveView_SatWinSize(SatWinSize);
                break;
           }

           case IDC_Filter_MeanLength: {
                int Filter_MeanLength = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Filter length [%d ms]",Filter_MeanLength);
                SetStaticText(IDC_TXT_Filter_MeanLength, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveViewFilter_MeanLength(Filter_MeanLength);
                break;
           }

           case IDC_Filter_MeanThreshold: {
                int Filter_MeanThreshold = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Filter threshold [%d%%]",Filter_MeanThreshold);
                SetStaticText(IDC_TXT_Filter_MeanThreshold, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveViewFilter_MeanThreshold(Filter_MeanThreshold);
                break;
           }

           case IDC_Filter_PercentNew: {
                int Filter_PercentNew = (int)SendMessage(scrollBarHandle, TBM_GETPOS, 0, 0);
                sprintf(buffer,"Filter smoothness [%d%%]",Filter_PercentNew);
                SetStaticText(IDC_TXT_Filter_PercentNew, buffer);
                this->m_pDynData->getAtmoConfig()->setLiveViewFilter_PercentNew(Filter_PercentNew);
                break;
           }

           default:
               return;

    }
}
void CAtmoSettingsDialog::HandleVertScroll(int code,int position,HWND scrollBarHandle) 
{
}

void CAtmoSettingsDialog::UpdateDeviceConnection(AtmoConnectionType conType)
{
    EffectMode oldEffect = CAtmoTools::SwitchEffect(m_pDynData, emDisabled);

    CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
    pAtmoConfig->setConnectionType(conType);

    if(CAtmoTools::RecreateConnection(m_pDynData) == ATMO_TRUE) {

       CAtmoTools::SwitchEffect(m_pDynData, oldEffect);

    } else {
        CAtmoConnection *connection = m_pDynData->getAtmoConnection();
        if(connection) {
           if(connection->ShowConfigDialog( m_hInst, m_hDialog, pAtmoConfig))
           {
             if(CAtmoTools::RecreateConnection(m_pDynData) == ATMO_TRUE) {
                CAtmoTools::SwitchEffect(m_pDynData, oldEffect);
                MessageBox( this->m_hDialog, "Connected successfully", "Info", MB_ICONINFORMATION);
             }
           }
        }
    }
}


// WM_COMMAND handler...
ATMO_BOOL CAtmoSettingsDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {

    switch(wmId) {
        case IDOK: {
           CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();

           AtmoConnectionType conType = (AtmoConnectionType)ComboBox_GetCurSel(getDlgItem(IDC_DEVICETYPE));
           pAtmoConfig->setConnectionType( conType );

           EffectMode newEffectMode = (EffectMode)ComboBox_GetCurSel(getDlgItem(IDC_EFFECTS));
           pAtmoConfig->setEffectMode(newEffectMode);

           if(UpdateColorChangeValues(ATMO_FALSE) == ATMO_FALSE)   return 0;
           if(UpdateLrColorChangeValues(ATMO_FALSE) == ATMO_FALSE) return 0;
           if(UpdateLiveViewValues(ATMO_FALSE) == ATMO_FALSE) return 0;

           if(Button_GetCheck(this->getDlgItem(IDC_CHK_USESHUTDOWNCOLOR)) == BST_CHECKED)
               pAtmoConfig->SetSetShutdownColor(1);
           else
               pAtmoConfig->SetSetShutdownColor(0);


           CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);
           // schnittstelle neu öffnen... könne ja testweise geändert wurden sein?
           CAtmoTools::RecreateConnection(this->m_pDynData);

            // Weissabgleich in Hardware Permanent speichern...
           if(m_SaveHardWhiteSetup == ATMO_TRUE) {
              CAtmoConnection *atmoConnection = m_pDynData->getAtmoConnection();
              if((atmoConnection != NULL) && (atmoConnection->isOpen())) {
                  atmoConnection->HardwareWhiteAdjust(pAtmoConfig->getHardware_global_gamma(), pAtmoConfig->getHardware_global_contrast(), pAtmoConfig->getHardware_contrast_red(),pAtmoConfig->getHardware_contrast_green(),pAtmoConfig->getHardware_contrast_blue(),pAtmoConfig->getHardware_gamma_red(),pAtmoConfig->getHardware_gamma_green(),pAtmoConfig->getHardware_contrast_blue(), ATMO_TRUE );
              }
           }

           CAtmoTools::SwitchEffect(this->m_pDynData, newEffectMode);

           pAtmoConfig->SaveSettings();

           pAtmoConfig->m_UpdateEdgeWeightningFlag = 1;

           EndDialog(this->m_hDialog, wmId);
           break;
        }

        case IDCANCEL: {
            // m_pBackupConfig --> wieder herstellen...
            CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
            pAtmoConfig->Assign(this->m_pBackupConfig);

            EffectMode backupEffectMode = pAtmoConfig->getEffectMode();

            // Effect Thread Stoppen der gerade läuft...
            CAtmoTools::SwitchEffect(this->m_pDynData, emDisabled);

            // schnittstelle neu öffnen... könne ja testweise geändert wurden sein?
            CAtmoTools::RecreateConnection(this->m_pDynData);

            // Weissabgleich Rückgängig machen...
            CAtmoConnection *atmoConnection = m_pDynData->getAtmoConnection();
            if((atmoConnection != NULL) && (atmoConnection->isOpen())) {
                atmoConnection->HardwareWhiteAdjust(pAtmoConfig->getHardware_global_gamma(), pAtmoConfig->getHardware_global_contrast(), pAtmoConfig->getHardware_contrast_red(),pAtmoConfig->getHardware_contrast_green(),pAtmoConfig->getHardware_contrast_blue(),pAtmoConfig->getHardware_gamma_red(),pAtmoConfig->getHardware_gamma_green(),pAtmoConfig->getHardware_contrast_blue(), ATMO_FALSE );
             }

            // Effect Programm wieder starten...
            CAtmoTools::SwitchEffect(this->m_pDynData, backupEffectMode);

            EndDialog(this->m_hDialog, wmId);
            break;
        }

        case IDC_BU_CHANNELASSIGNMENTS: {
             CAtmoEditChannelAssignment *editChannels = new CAtmoEditChannelAssignment(this->m_hInst, this->m_hDialog, this->m_pDynData);
             editChannels->ShowModal();

             delete editChannels;
             break;
        }

        case IDC_BU_CONFIG_DEVICE: {
             CAtmoConfig *pAtmoConfig        = m_pDynData->getAtmoConfig();
             CAtmoConnection *atmoConnection = m_pDynData->getAtmoConnection();
             if(!atmoConnection)
                break;
             
             if( atmoConnection->ShowConfigDialog(this->m_hInst, this->m_hDialog, pAtmoConfig) )
             {
                 // reconnect ...
                EffectMode oldEffect = CAtmoTools::SwitchEffect(m_pDynData, emDisabled);

                if(CAtmoTools::RecreateConnection(m_pDynData) == ATMO_TRUE) {
                   CAtmoTools::SwitchEffect(m_pDynData, oldEffect);
                   MessageBox( this->m_hDialog, "Connected successfully", "Info", MB_ICONINFORMATION);
                }
             }
             break;
        }

        case IDC_EFFECTS: {
             if(wmEvent == CBN_SELCHANGE) {
                EffectMode newEffectMode = (EffectMode)ComboBox_GetCurSel(hControl);
                // if(newEffectMode != m_pDynData->getAtmoConfig()->getEffectMode()) {
                   CAtmoTools::SwitchEffect(this->m_pDynData, newEffectMode);
                //}
             }
             break;
        }

        case IDC_DEVICETYPE: {
             if(wmEvent == CBN_SELCHANGE) {
                int i = ComboBox_GetCurSel(hControl);
                if(i>=0) {
                  AtmoConnectionType connectionType = (AtmoConnectionType)i; //
                  if(connectionType != m_pDynData->getAtmoConfig()->getConnectionType() || 
                    !m_pDynData->getAtmoConnection() ) 
                  {
                     UpdateDeviceConnection( connectionType );
                  }
                }
             }
             break;
        }

        case IDC_BU_SELSHUTDOWNCOLOR: {
             // 2
             CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
             int r,g,b;
             r = pAtmoConfig->getShutdownColor_Red();
             g = pAtmoConfig->getShutdownColor_Green();
             b = pAtmoConfig->getShutdownColor_Blue();
             if(CAtmoColorPicker::Execute(this->m_hInst, this->m_hDialog, this->m_pDynData, r, g, b) == ATMO_TRUE) {
                // werte aus r g b verarbeiten!
                pAtmoConfig->setShutdownColor_Red(r);
                pAtmoConfig->setShutdownColor_Green(g);
                pAtmoConfig->setShutdownColor_Blue(b);
             }
             break;
        }

        case IDC_BU_SELSTATICCOLOR: {
             CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
             int r,g,b;
             r = pAtmoConfig->getStaticColor_Red();
             g = pAtmoConfig->getStaticColor_Green();
             b = pAtmoConfig->getStaticColor_Blue();
             if(CAtmoColorPicker::Execute(this->m_hInst, this->m_hDialog, this->m_pDynData, r, g, b) == ATMO_TRUE) {
                // werte aus r g b verarbeiten!
                pAtmoConfig->setStaticColor_Red(r);
                pAtmoConfig->setStaticColor_Green(g);
                pAtmoConfig->setStaticColor_Blue(b);
                // wenn aktiver Effekt die statische Farbe ist - diese auch sofort anwenden
                if( pAtmoConfig->getEffectMode() == emStaticColor ) 
                    CAtmoTools::SwitchEffect( this->m_pDynData, emStaticColor);
             }
             break;
        }

        case IDC_BU_WHITEADJ_SW: {
             CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
             int r,g,b;
             ATMO_BOOL useSoftwareWhiteAdj = pAtmoConfig->isUseSoftwareWhiteAdj();
             r = pAtmoConfig->getWhiteAdjustment_Red();
             g = pAtmoConfig->getWhiteAdjustment_Green();
             b = pAtmoConfig->getWhiteAdjustment_Blue();
             if(CAtmoWhiteSetup::Execute(this->m_hInst, this->m_hDialog, this->m_pDynData, r, g, b, useSoftwareWhiteAdj) == ATMO_TRUE) {
                // werte aus r g b verarbeiten!
                pAtmoConfig->setWhiteAdjustment_Red(r);
                pAtmoConfig->setWhiteAdjustment_Green(g);
                pAtmoConfig->setWhiteAdjustment_Blue(b);
                pAtmoConfig->setUseSoftwareWhiteAdj(useSoftwareWhiteAdj);
             }
            break;
        }

        case IDC_BU_WHITEADJ_HW: {
             CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
             if(CAtmoWhiteAdjHW::Execute(this->m_hInst,this->m_hDialog, this->m_pDynData, m_SaveHardWhiteSetup) == ATMO_TRUE) {
                //  nothin special to do -> alles wichtige hat execute schon erledigt!
             }
             break;
        }

        case IDC_GRADIENT_EDIT: {
            CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
            if( CAtmoGradients::Execute( this->m_hInst, this->m_hDialog, this->m_pDynData ) == ATMO_TRUE) {
                // slide edge weightning anpassen?
                char buffer[64];
                sprintf(buffer,"Edge weighting [%d]",pAtmoConfig->getLiveView_EdgeWeighting());
                SetStaticText(IDC_TXT_EDGEWEIGHTNING, buffer);
                SendMessage(this->getDlgItem(IDC_EdgeWeighting), TBM_SETPOS, true, pAtmoConfig->getLiveView_EdgeWeighting() );
            }
            break;
        }

        case IDC_WIDESCREEN: {
            CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
            if(Button_GetCheck(hControl) == BST_CHECKED)
               pAtmoConfig->setLiveView_WidescreenMode(1);
            else
               pAtmoConfig->setLiveView_WidescreenMode(0);
            break;
        }

        case IDC_FILTERMODE: {
             /// CBN_SELCHANGE
            if(wmEvent == CBN_SELCHANGE) {
               CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
               int i = ComboBox_GetCurSel(hControl);
               pAtmoConfig->setLiveViewFilterMode((AtmoFilterMode)i);
            }
            break;
        }

        case IDC_DISPLAYS: {
            if(wmEvent == CBN_SELCHANGE) {
               CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
               int i = ComboBox_GetCurSel(hControl);
               if(i != pAtmoConfig->getLiveView_DisplayNr()) {
                  UpdateLiveViewValues( pAtmoConfig->getEffectMode() == emLivePicture );
               }
            }
            break;
        }

        case IDC_ED_COLORCHANGE_DELAY: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if(value>=10) {
                        CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                        pAtmoConfig->setColorChanger_iDelay(value);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

        case IDC_GDI_INTERLACE: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=1) && (value<CAP_HEIGHT)) {
                        CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                        pAtmoConfig->setLiveView_RowsPerFrame( value );
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

        case IDC_GDI_CAPURE_RATE: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=1) && (value<=50)) {
                        CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                        pAtmoConfig->setLiveView_GDI_FrameRate(value);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

        case IDC_ED_ZONETOPC: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=0) && (value<=64)) {
                        CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                        int z = pAtmoConfig->getZonesLRCount() * 2 + pAtmoConfig->getZonesBottomCount() + value;
                        if(z > CAP_MAX_NUM_ZONES)
                           MessageBeep(MB_ICONEXCLAMATION);
                        else
                           pAtmoConfig->setZonesTopCount(value);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

        case IDC_ED_ZONEBOTTOMC: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=0) && (value<=64)) { 
                        CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                        int z = pAtmoConfig->getZonesLRCount() * 2 + pAtmoConfig->getZonesTopCount() + value;
                        if(z > CAP_MAX_NUM_ZONES)
                           MessageBeep(MB_ICONEXCLAMATION);
                        else
                           pAtmoConfig->setZonesBottomCount(value);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

        case IDC_ED_ZONELRC: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value>=0) && (value<=64)) {
                        CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                        int z = value * 2 + pAtmoConfig->getZonesTopCount() + pAtmoConfig->getZonesBottomCount();
                        if(z > CAP_MAX_NUM_ZONES)
                           MessageBeep(MB_ICONEXCLAMATION);
                        else
                           pAtmoConfig->setZonesLRCount(value);
                    } else {
                        MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

        case IDC_CBX_ZONESUM: {
            CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
            if(Button_GetCheck(hControl) == BST_CHECKED)
               pAtmoConfig->setZoneSummary(ATMO_TRUE);
            else
               pAtmoConfig->setZoneSummary(ATMO_FALSE);
            break;
        }

        case IDC_ED_COLORCHANGE_STEPS: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if(value>=1) {
                       CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                       pAtmoConfig->setColorChanger_iSteps(value);
                    } else {
                       MessageBeep(MB_ICONEXCLAMATION);
                    }
                }
            }
            break;
        }

        case IDC_ED_LRCOLORCHANGE_STEPS: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                   int value = atoi(buffer);
                   if(value>=1) {
                       CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                       pAtmoConfig->setLrColorChanger_iSteps(value);
                   } else {
                       MessageBeep(MB_ICONEXCLAMATION);
                   }
                }
            }
            break;
        }

        case IDC_ED_LRCOLORCHANGE_DELAY: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                   int value = atoi(buffer);
                   if(value>=10) {
                       CAtmoConfig *pAtmoConfig = this->m_pDynData->getAtmoConfig();
                       pAtmoConfig->setLrColorChanger_iDelay(value);
                   } else {
                       MessageBeep(MB_ICONEXCLAMATION);
                   }
                }
            }
            break;
        }

/*
        case IDC_BU_CONFIG_LIVE:  {
            CAtmoLiveSettings *liveSettings = new CAtmoLiveSettings(this->m_hInst, this->m_hDialog, this->m_pDynData);
            liveSettings->ShowModal();
            delete liveSettings;
            break;
        }
*/

       default:
           return ATMO_FALSE;

    }

    return ATMO_TRUE;
}

