/*
 * AtmoColorPicker.cpp: Dialog for choosing a color by its components (red, green, blue) - with live preview on the hardware
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id$
 */
#include "StdAfx.h"
#include "resource.h"
#include "windowsx.h"
#include "atmocolorpicker.h"
#include "AtmoConfig.h"
#include "AtmoConnection.h"
#include "AtmoTools.h"
#include "commctrl.h"

CAtmoColorPicker::CAtmoColorPicker(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int red,int green,int blue):
                  CAtmoCustomColorPicker(hInst,parent,IDD_COLORPICKER,pAtmoDynData,red,green,blue) {
    this->m_pAtmoDynData = pAtmoDynData;
    outputColor(red,green,blue);
    m_iRed   = red;
    m_iGreen = green;
    m_iBlue  = blue;
}

CAtmoColorPicker::~CAtmoColorPicker(void)
{
}


void CAtmoColorPicker::outputColor(int red,int green,int blue) {
     CAtmoConnection *pAtmoConnection = this->m_pAtmoDynData->getAtmoConnection();
     CAtmoConfig *pAtmoConfig = this->m_pAtmoDynData->getAtmoConfig();

     if((pAtmoConnection==NULL) || (pAtmoConnection->isOpen()==ATMO_FALSE)) return;

     pColorPacket packet;

     AllocColorPacket(packet, pAtmoConfig->getZoneCount());

     for(int i=0;i<packet->numColors;i++) {
         packet->zone[i].r  = red;
         packet->zone[i].g  = green;
         packet->zone[i].b  = blue;
     }

     packet = CAtmoTools::ApplyGamma(pAtmoConfig, packet);

     if(pAtmoConfig->isUseSoftwareWhiteAdj()) 
        packet = CAtmoTools::WhiteCalibration(pAtmoConfig, packet);

     pAtmoConnection->SendData( packet );

     delete (char *)packet;
}

ATMO_BOOL CAtmoColorPicker::Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int &red, int &green, int &blue) {
       // so kann ColorPicker sein aktuellen Farbwert ausgeben - ohne etwas zu stören...
       EffectMode oldEffectMode = CAtmoTools::SwitchEffect(pAtmoDynData, emDisabled);

       CAtmoColorPicker *pAtmoColorPicker = new CAtmoColorPicker(hInst, parent, pAtmoDynData, red, green, blue);

       ATMO_BOOL result (pAtmoColorPicker->ShowModal() == IDOK);
       if(result == ATMO_TRUE) {
           red   = pAtmoColorPicker->m_iRed;
           green = pAtmoColorPicker->m_iGreen;
           blue  = pAtmoColorPicker->m_iBlue;
       }
       delete pAtmoColorPicker;

       CAtmoTools::SwitchEffect(pAtmoDynData, oldEffectMode);

       return result;
}
