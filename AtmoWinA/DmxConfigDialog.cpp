#include "StdAfx.h"
#include "dmxconfigdialog.h"
#include "DmxTools.h"
#include "resource.h"

CDmxConfigDialog::CDmxConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *pConfig) : 
     CBasicDialog(hInst, IDD_CFG_DMX_DLG, parent)
{
   this->m_pConfig = pConfig;

   // m_MaxChannels
   m_MaxChannels = 1;
   if(pConfig->getConnectionType() == actEnttecDMX) 
      m_MaxChannels = 512;
        
   if(pConfig->getConnectionType() == actDMX) 
      m_MaxChannels = 256;

}

CDmxConfigDialog::~CDmxConfigDialog(void)
{
}


ATMO_BOOL CDmxConfigDialog::InitDialog(WPARAM wParam) {

        m_hCbxComports = getDlgItem(IDC_COMPORT);
        InitDialog_ComPorts( m_hCbxComports );

        int com = m_pConfig->getComport() - 1;
        if(com < 0) com = 0;
        ComboBox_SetCurSel(m_hCbxComports, com);

        m_hCbxBaudrate = getDlgItem(IDC_BAUDRATE);
        ComboBox_AddString(m_hCbxBaudrate, "115200");
        ComboBox_AddString(m_hCbxBaudrate, "250000");
        ComboBox_SetCurSel(m_hCbxBaudrate, m_pConfig->getDMX_BaudrateIndex());


        char buf[10];

        HWND control;

        control = getDlgItem(IDC_EDT_DMX_BASE);
        Edit_LimitText( control , 256);
        Edit_SetText(control, m_pConfig->getDMX_BaseChannels() );

        control = getDlgItem(IDC_EDT_DMX_RGB);
        Edit_LimitText(control, 2);
        sprintf(buf,"%d",m_pConfig->getDMX_RGB_Channels());
        Edit_SetText(control, buf);

        return ATMO_FALSE;
}


ATMO_BOOL CDmxConfigDialog::ExecuteCommand(HWND hControl,int wmId, int wmEvent) {

    HWND control;
    char buf[500];
    char channels[500];
    int  rgb;
    int chc;

    switch(wmId) {
        case IDOK: {
           control = getDlgItem(IDC_EDT_DMX_BASE);
           Edit_GetText(control, channels, sizeof(channels));

           control = getDlgItem(IDC_EDT_DMX_RGB);
           Edit_GetText(control, buf, sizeof(buf));
           rgb = atoi(buf);
           
           if((rgb<1) || (rgb>84))  {
              MessageBox(m_hDialog,"Anzahl RGB liegt ausserhalb des g�ltigen Bereich 1..84","Fehler",MB_ICONERROR);
              break;
           }

           chc = IsValidDmxStartString(channels, m_MaxChannels-3);
           if((chc == -2) || (chc == -3)) {
              sprintf(buf,"Ung�ltiger DMX Kanal gefunden, nicht im Bereich 0..%d",m_MaxChannels-3);
              MessageBox(m_hDialog, buf, "Fehler",MB_ICONERROR);
              break;
           }
           if((chc == -1) || (chc == 0)) {
              MessageBox(m_hDialog , "Mindestens ein Startkanal muss angegeben werden!" , "Fehler" , MB_ICONERROR );
              break;
           }
           if(chc == -4) {
              MessageBox(m_hDialog , "Ung�ltiges Zeichen in der Startkanalliste gefunden.\nEs sind nur Ziffern [0..9], Trenner [,;] und Leerzeichen erlaubt." , "Fehler" , MB_ICONERROR );
              break;
           }

           if(chc < rgb) {
              MessageBox(m_hDialog, "Es wurden weniger Startkan�le angegeben als RGB Kan�le, die fehlende Startkan�le werden auf Basis des letzten Kanals ermittelt.", "Hinweis", MB_ICONINFORMATION);
           }

           int comportSel = ComboBox_GetCurSel(getDlgItem(IDC_COMPORT));
           m_pConfig->setComport(comportSel + 1);

           m_pConfig->setDMX_BaudrateIndex( ComboBox_GetCurSel(getDlgItem(IDC_BAUDRATE)) );

           m_pConfig->setDMX_BaseChannels( channels );
           m_pConfig->setDMX_RGB_Channels( rgb );

           EndDialog(this->m_hDialog, wmId);
           break;
        }

        case IDCANCEL: {
            // m_pBackupConfig --> wieder herstellen...
           EndDialog(this->m_hDialog, wmId);
           break;
        }

        case IDC_EDT_DMX_RGB: {
            if(wmEvent == EN_CHANGE) {
                char buffer[20];
                if(Edit_GetText(hControl,buffer,sizeof(buffer))>0) {
                    int value = atoi(buffer);
                    if((value<1) || (value>64)) 
                        MessageBeep(MB_ICONEXCLAMATION);
                }
            }
            break;
        }

        case IDC_EDT_DMX_BASE: {
            if(wmEvent == EN_CHANGE) {
                if(Edit_GetText(hControl,channels,sizeof(channels))>0) {
                   chc = IsValidDmxStartString(channels, m_MaxChannels-3);
                   if(chc < 0)
                      MessageBeep(MB_ICONEXCLAMATION);
                }
            }
            break;
        }


       default:
           return ATMO_FALSE;

    }

    return ATMO_TRUE;
}


