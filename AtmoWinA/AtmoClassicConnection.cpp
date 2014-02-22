/*
 * AtmoSerialConnection.cpp: Class for communication with the serial hardware of
 * Atmo Light, opens and configures the serial port
 *
 * See the README.txt file for copyright information and how to reach the author(s).
 *
 * $Id: edd37fb84029decae0236b3f1d787d16c779591e $
 */

#include "stdafx.h"

#include "AtmoDefs.h"
#include "AtmoClassicConnection.h"

#if !defined(_ATMO_VLC_PLUGIN_)
# include "AtmoClassicConfigDialog.h"
#endif

#include <stdio.h>
#include <fcntl.h>

#if !defined(WIN32)
#include <termios.h>
#include <unistd.h>
#endif


CAtmoClassicConnection::CAtmoClassicConnection(CAtmoConfig *cfg) : CAtmoConnection(cfg) {
    m_hComport = INVALID_HANDLE_VALUE;
    memset(m_frame_buffer, 0, sizeof(m_frame_buffer));
    m_frame_buffer[0] = 0xFF;  // Start Byte
    m_frame_buffer[1] = 0x00;  // Start channel 0
    m_frame_buffer[2] = 0x00;  // Start channel 0
    m_frame_buffer[3] = 15; // num PWM Channels 5*3
}

CAtmoClassicConnection::~CAtmoClassicConnection() {
}

ATMO_BOOL CAtmoClassicConnection::OpenConnection() {
#if defined(_ATMO_VLC_PLUGIN_)
     char *serdevice = m_pAtmoConfig->getSerialDevice();
     if(!serdevice)
        return ATMO_FALSE;
#else
     int portNummer = m_pAtmoConfig->getComport();
     m_dwLastWin32Error = 0;
	 if(portNummer < 1) return ATMO_FALSE; // make no real sense;-)
#endif

	 CloseConnection();

#if !defined(_ATMO_VLC_PLUGIN_)
     char serdevice[16];  // com4294967295
     sprintf(serdevice,"com%d",portNummer);
#endif

#if defined(WIN32)

     m_hComport = CreateFile(serdevice, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
     if(m_hComport == INVALID_HANDLE_VALUE) {
//      we have a problem here can't open com port... somebody else may use it?
	    m_dwLastWin32Error = GetLastError();
	    return ATMO_FALSE;
     }
     /* change serial settings (Speed, stopbits etc.) */
     DCB dcb; // fuer comport-parameter
     dcb.DCBlength = sizeof(DCB);
     GetCommState (m_hComport, &dcb); // ger current serialport settings
     dcb.BaudRate  = 38400;        // set speed
     dcb.ByteSize  = 8;            // set databits
     dcb.Parity    = NOPARITY;     // set parity
     dcb.StopBits  = ONESTOPBIT;   // set one stop bit
     SetCommState (m_hComport, &dcb);    // apply settings

#else

     int bconst = B38400;
     m_hComport = open(serdevice,O_RDWR |O_NOCTTY);
     if(m_hComport < 0) {
	    return ATMO_FALSE;
     }

     struct termios tio;
     memset(&tio,0,sizeof(tio));
     tio.c_cflag = (CS8 | CREAD | HUPCL | CLOCAL);
     tio.c_iflag = (INPCK | BRKINT);
     cfsetispeed(&tio, bconst);
     cfsetospeed(&tio, bconst);
     if(!tcsetattr(m_hComport, TCSANOW, &tio)) {
         tcflush(m_hComport, TCIOFLUSH);
     } else {
         // can't change parms
        close(m_hComport);
        m_hComport = -1;
        return false;
     }

#endif

     return true;
}

void CAtmoClassicConnection::CloseConnection() {
  if(m_hComport!=INVALID_HANDLE_VALUE) {
#if defined(WIN32)
     CloseHandle(m_hComport);
#else
     close(m_hComport);
#endif
	 m_hComport = INVALID_HANDLE_VALUE;
  }
}

ATMO_BOOL CAtmoClassicConnection::isOpen(void) {
	 return (m_hComport != INVALID_HANDLE_VALUE);
}

ATMO_BOOL CAtmoClassicConnection::HardwareWhiteAdjust(int global_gamma,
                                                     int global_contrast,
                                                     int contrast_red,
                                                     int contrast_green,
                                                     int contrast_blue,
                                                     int gamma_red,
                                                     int gamma_green,
                                                     int gamma_blue,
                                                     ATMO_BOOL storeToEeprom) {
     if(m_hComport == INVALID_HANDLE_VALUE)
   	    return ATMO_FALSE;

     DWORD iBytesWritten;
/*
[0] = 255
[1] = 00
[2] = 00
[3] = 101

[4]  brightness  0..255 ?

[5]  Contrast Red     11 .. 100
[6]  Contrast  Green  11 .. 100
[7]  Contrast  Blue   11 .. 100

[8]   Gamma Red    11 .. 35
[9]   Gamma Red    11 .. 35
[10]  Gamma Red    11 .. 35

[11]  Globale Contrast  11 .. 100

[12]  Store Data: 199 (else 0)

*/
     unsigned char sendBuffer[16];
     sendBuffer[0] = 0xFF;
     sendBuffer[1] = 0x00;
     sendBuffer[2] = 0x00;
     sendBuffer[3] = 101;

     sendBuffer[4] = (global_gamma & 255);

     sendBuffer[5] = (contrast_red & 255);
     sendBuffer[6] = (contrast_green & 255);
     sendBuffer[7] = (contrast_blue & 255);

     sendBuffer[8]  = (gamma_red & 255);
     sendBuffer[9]  = (gamma_green & 255);
     sendBuffer[10] = (gamma_blue & 255);

     sendBuffer[11] = (global_contrast & 255);

     if(storeToEeprom == ATMO_TRUE)
        sendBuffer[12] = 199; // store to eeprom!
     else
        sendBuffer[12] = 0;

#if defined(WIN32)
     WriteFile(m_hComport, sendBuffer, 13, &iBytesWritten, NULL); // send to COM-Port
#else
     iBytesWritten = write(m_hComport, sendBuffer, 13);
     tcdrain(m_hComport);
#endif

     return (iBytesWritten == 13) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CAtmoClassicConnection::SendData(pColorPacket data) {
   if(m_hComport == INVALID_HANDLE_VALUE)
	  return ATMO_FALSE;

   DWORD iBytesWritten;

   int iBuffer = 4;
   int zoneIdx;

   Lock();

   for(int channel = 0; channel < 5 ; channel++) {
       if(m_ChannelAssignment && (channel < m_NumAssignedChannels))
          zoneIdx = m_ChannelAssignment[ channel ];
       else
          zoneIdx = -1;


       if((zoneIdx>=0) && (zoneIdx<data->numColors))
       {
          if(m_pAtmoConfig->isWhiteAdjPerChannel()) 
          {
             int wr;
             int wg;
             int wb;
             m_pAtmoConfig->getChannelWhiteAdj( channel, wr, wg, wb);   

             m_frame_buffer[iBuffer]   = (data->zone[zoneIdx].r * wr) / 256;
             m_frame_buffer[iBuffer+1] = (data->zone[zoneIdx].g * wg) / 256;
             m_frame_buffer[iBuffer+2] = (data->zone[zoneIdx].b * wb) / 256;

          } else {

             m_frame_buffer[iBuffer]   = data->zone[zoneIdx].r;
             m_frame_buffer[iBuffer+1] = data->zone[zoneIdx].g;
             m_frame_buffer[iBuffer+2] = data->zone[zoneIdx].b;

          }
       }

       iBuffer+=3;
   }

#if defined(WIN32)
   WriteFile(m_hComport, m_frame_buffer, 19, &iBytesWritten, NULL); // send to COM-Port
#else
   iBytesWritten = write(m_hComport, m_frame_buffer, 19);
   tcdrain(m_hComport);
#endif

   Unlock();

   return (iBytesWritten == 19) ? ATMO_TRUE : ATMO_FALSE;
}


ATMO_BOOL CAtmoClassicConnection::CreateDefaultMapping(CAtmoChannelAssignment *ca)
{
   if(!ca) return ATMO_FALSE;
   ca->setSize(5);
   ca->setZoneIndex(0, 4); // Zone 5
   ca->setZoneIndex(1, 3);
   ca->setZoneIndex(2, 1);
   ca->setZoneIndex(3, 0);
   ca->setZoneIndex(4, 2);
   return ATMO_TRUE;
}

#if !defined(_ATMO_VLC_PLUGIN_)

ATMO_BOOL CAtmoClassicConnection::setChannelColor(int channel, tRGBColor color)
{
	DWORD iBytesWritten = 0;
    if((channel>=0) && (channel<=4))
    {
        Lock();

        m_frame_buffer[channel+0+4]=color.r;
	    m_frame_buffer[channel+1+4]=color.g;
	    m_frame_buffer[channel+2+4]=color.b;

#if defined(WIN32)
        WriteFile(m_hComport, m_frame_buffer, 19, &iBytesWritten, NULL); // send to COM-Port
#else
        iBytesWritten = write(m_hComport, m_frame_buffer, 19);
        tcdrain(m_hComport);
#endif
        Unlock();
    }
	return (iBytesWritten == 19) ? ATMO_TRUE : ATMO_FALSE;
}

ATMO_BOOL CAtmoClassicConnection::setChannelValues(int numValues,unsigned char *channel_values)
{
	DWORD iBytesWritten;
    if((numValues & 1) || !channel_values)
       return ATMO_FALSE; // numValues must be even!

    /*
       the array shall contain
    */
	Lock();
    int Index;

    for (int i = 0; i < numValues; i+=2) {
         Index = ((int)channel_values[i]);
         if((Index>=0) && (Index<=14))
            m_frame_buffer[Index+4] = channel_values[i+1];
    }
#if defined(WIN32)
    WriteFile(m_hComport, m_frame_buffer, 19, &iBytesWritten, NULL); // send to COM-Port
#else
    iBytesWritten = write(m_hComport, m_frame_buffer, 19);
    tcdrain(m_hComport);
#endif
    Unlock();

	return (iBytesWritten == 19) ? ATMO_TRUE : ATMO_FALSE;
}


char *CAtmoClassicConnection::getChannelName(int ch)
{
  if(ch < 0) return NULL;
  char buf[30];

  switch(ch) {
      case 0:
          sprintf(buf,"Summen Kanal [%d]",ch);
          break;
      case 1:
          sprintf(buf,"Linker Kanal [%d]",ch);
          break;
      case 2:
          sprintf(buf,"Rechter Kanal [%d]",ch);
          break;
      case 3:
          sprintf(buf,"Oberer Kanal [%d]",ch);
          break;
      case 4:
          sprintf(buf,"Unterer Kanal [%d]",ch);
          break;
      default:
          sprintf(buf,"Kanal [%d]",ch);
          break;
  }

  return strdup(buf);
}

ATMO_BOOL CAtmoClassicConnection::ShowConfigDialog(HINSTANCE hInst, HWND parent, CAtmoConfig *cfg)
{
    CAtmoClassicConfigDialog *dlg = new CAtmoClassicConfigDialog(hInst, parent, cfg);

    INT_PTR result = dlg->ShowModal();

    delete dlg;

    if(result == IDOK)
      return ATMO_TRUE;
    else
      return ATMO_FALSE;
}

#endif
