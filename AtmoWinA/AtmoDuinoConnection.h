/*
 * AtmoCom.h: Class for communication with the serial hardware of Atmo Light,
 * opens and configures the serial port
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id: 7d0c56abf5249105a54bbbec9dff09876c1e18b1 $
 */
#ifndef _AtmoDuinoConnection_h_
#define _AtmoDuinoConnection_h_

#include "AtmoDefs.h"
#include "AtmoConnection.h"
#include "AtmoConfig.h"

#if defined(WIN32)
#   include <windows.h>
#endif


class CAtmoDuinoConnection : public CAtmoConnection {
private:
    HANDLE m_hComport;
    unsigned char m_frame_buffer[CAP_MAX_NUM_ZONES * 3 + 4];

#if defined(WIN32)
    DWORD  m_dwLastWin32Error;
public:
    DWORD getLastError() { return m_dwLastWin32Error; }
#endif

public:
    CAtmoDuinoConnection(CAtmoConfig *cfg);
    virtual ~CAtmoDuinoConnection(void);

    virtual ATMO_BOOL OpenConnection();

    virtual void CloseConnection();

    virtual ATMO_BOOL isOpen(void);

    virtual ATMO_BOOL SendData(pColorPacket data);

    virtual ATMO_BOOL HardwareWhiteAdjust(int global_gamma,
        int global_contrast,
        int contrast_red,
        int contrast_green,
        int contrast_blue,
        int gamma_red,
        int gamma_green,
        int gamma_blue,
        ATMO_BOOL storeToEeprom);

    virtual int getNumChannels();
    virtual int getBufferSize();

    virtual ATMO_BOOL setChannelColor(int channel, tRGBColor color);
    virtual ATMO_BOOL setChannelValues(int numValues, unsigned char *channel_values);


    virtual const char *getDevicePath() { return "atmoduino"; }

#if !defined(_ATMO_VLC_PLUGIN_)
    virtual char *getChannelName(int ch);
    virtual ATMO_BOOL ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg);
#endif

    virtual ATMO_BOOL CreateDefaultMapping(CAtmoChannelAssignment *ca);
};

#endif
