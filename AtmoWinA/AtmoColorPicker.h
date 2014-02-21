/*
 * AtmoColorPicker.h: Dialog for choosing a color by its components (red, green, blue) - with live preview on the hardware
 *
 * See the README file for copyright information and how to reach the author(s).
 *
 * $Id$
 */

#pragma once
#include "basicdialog.h"
#include "AtmoDynData.h"
#include "AtmoCustomColorPicker.h"

class CAtmoColorPicker :  public CAtmoCustomColorPicker
{
protected:
    virtual void outputColor(int red,int green,int blue);

public:
    CAtmoColorPicker(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int red,int green,int blue);
    virtual ~CAtmoColorPicker(void);

    static ATMO_BOOL Execute(HINSTANCE hInst, HWND parent, CAtmoDynData *pAtmoDynData, int &red, int &green, int &blue);

};
