#include "StdAfx.h"
#include "atmoregistryconfig.h"



CAtmoRegistryConfig::CAtmoRegistryConfig(char *regKey) : CAtmoConfig(), CAtmoRegistry(HKEY_LOCAL_MACHINE)
{
  // Speicher Strukturen wenns welche gibt mit new holen
  configRootKey[0] = 0;
  if(regKey!=NULL)
     strcpy(this->configRootKey, regKey);
  m_IsShowConfigDialog = 0;
  psz_language = NULL;
}

CAtmoRegistryConfig::~CAtmoRegistryConfig(void)
{
  delete []psz_language;
}

void CAtmoRegistryConfig::SaveSettings() {
    // alle Variabel etc. in Registry speichern
    char regKeyName[100],valueName[32];
	WriteRegistryInt(configRootKey,"comport",this->m_Comport);

    WriteRegistryInt(configRootKey,"comport_1", m_Comports[0] );
    WriteRegistryInt(configRootKey,"comport_2", m_Comports[1] );
    WriteRegistryInt(configRootKey,"comport_3", m_Comports[2] );


    // don't destroy config in that case..
    if(m_eAtmoConnectionType != actNUL)
       WriteRegistryInt(configRootKey,"ConnectionType",(int)m_eAtmoConnectionType);

    WriteRegistryInt(configRootKey,"EffectMode",(int)m_eEffectMode);

	WriteRegistryInt(configRootKey,"WhiteAdjustment_Red",m_WhiteAdjustment_Red);
	WriteRegistryInt(configRootKey,"WhiteAdjustment_Green",m_WhiteAdjustment_Green);
	WriteRegistryInt(configRootKey,"WhiteAdjustment_Blue",m_WhiteAdjustment_Blue);
	WriteRegistryInt(configRootKey,"UseSoftwareWhiteAdj",(int)m_UseSoftwareWhiteAdj);

    
    WriteRegistryInt(configRootKey,"UsePerChWhiteAdj",(int)m_WhiteAdjPerChannel);

    sprintf(regKeyName,"%sWhiteAdjPerChannel\\",configRootKey);
    WriteRegistryInt(regKeyName,"count",(int)m_chWhiteAdj_Count);

    for(int i=0; (i<m_chWhiteAdj_Count) && m_chWhiteAdj_Red && m_chWhiteAdj_Blue && m_chWhiteAdj_Green; i++) 
    {
        sprintf(valueName,"red_%d",i);
        WriteRegistryInt(regKeyName, valueName, m_chWhiteAdj_Red[i] );

        sprintf(valueName,"green_%d",i);
        WriteRegistryInt(regKeyName, valueName, m_chWhiteAdj_Green[i] );

        sprintf(valueName,"blue_%d",i);
        WriteRegistryInt(regKeyName, valueName, m_chWhiteAdj_Blue[i] );
    }


    WriteRegistryInt(configRootKey,"ColorChanger_iSteps",this->m_ColorChanger_iSteps);
    WriteRegistryInt(configRootKey,"ColorChanger_iDelay",this->m_ColorChanger_iDelay);

    WriteRegistryInt(configRootKey,"LrColorChanger_iSteps",this->m_LrColorChanger_iSteps);
    WriteRegistryInt(configRootKey,"LrColorChanger_iDelay",this->m_LrColorChanger_iDelay);

    WriteRegistryInt(configRootKey,"StaticColor_Red",this->m_StaticColor_Red);
    WriteRegistryInt(configRootKey,"StaticColor_Green",this->m_StaticColor_Green);
    WriteRegistryInt(configRootKey,"StaticColor_Blue",this->m_StaticColor_Blue);

    WriteRegistryInt(configRootKey,"isSetShutdownColor",m_IsSetShutdownColor);
	WriteRegistryInt(configRootKey,"ShutdownColor_red",m_ShutdownColor_Red);
	WriteRegistryInt(configRootKey,"ShutdownColor_green",m_ShutdownColor_Green);
	WriteRegistryInt(configRootKey,"ShutdownColor_blue",m_ShutdownColor_Blue);

    WriteRegistryInt(configRootKey,"LiveViewFilterMode",(int)m_LiveViewFilterMode);
    WriteRegistryInt(configRootKey,"LiveViewFilter_PercentNew",m_LiveViewFilter_PercentNew);
    WriteRegistryInt(configRootKey,"LiveViewFilter_MeanLength",m_LiveViewFilter_MeanLength);
    WriteRegistryInt(configRootKey,"LiveViewFilter_MeanThreshold",m_LiveViewFilter_MeanThreshold);

    WriteRegistryInt(configRootKey,"LiveView_EdgeWeighting",m_LiveView_EdgeWeighting);
    WriteRegistryInt(configRootKey,"LiveView_RowsPerFrame",m_LiveView_RowsPerFrame);

    WriteRegistryInt(configRootKey,"LiveView_BrightCorrect",m_LiveView_BrightCorrect);
    WriteRegistryInt(configRootKey,"LiveView_DarknessLimit",m_LiveView_DarknessLimit);
    WriteRegistryInt(configRootKey,"LiveView_HueWinSize",m_LiveView_HueWinSize);
    WriteRegistryInt(configRootKey,"LiveView_SatWinSize",m_LiveView_SatWinSize);
    WriteRegistryInt(configRootKey,"LiveView_WidescreenMode",m_LiveView_WidescreenMode);

    WriteRegistryInt(configRootKey,"LiveView_HOverscanBorder",m_LiveView_HOverscanBorder);
    WriteRegistryInt(configRootKey,"LiveView_VOverscanBorder",m_LiveView_VOverscanBorder);
    WriteRegistryInt(configRootKey,"LiveView_DisplayNr",m_LiveView_DisplayNr);
    WriteRegistryInt(configRootKey,"LiveView_FrameDelay",m_LiveView_FrameDelay);
    WriteRegistryInt(configRootKey,"LiveView_GDI_FrameRate",m_LiveView_GDI_FrameRate);

    WriteRegistryInt(configRootKey,"ZonesTopCount",m_ZonesTopCount);
    WriteRegistryInt(configRootKey,"ZonesBottomCount",m_ZonesBottomCount);
    WriteRegistryInt(configRootKey,"ZonesLRCount",m_ZonesLRCount);
    WriteRegistryInt(configRootKey,"ZoneSummary",m_ZoneSummary);
    


    WriteRegistryInt(configRootKey,"Hardware_global_gamma",m_Hardware_global_gamma);
    WriteRegistryInt(configRootKey,"Hardware_global_contrast",m_Hardware_global_contrast);
    WriteRegistryInt(configRootKey,"Hardware_contrast_red",m_Hardware_contrast_red);
    WriteRegistryInt(configRootKey,"Hardware_contrast_green",m_Hardware_contrast_green);
    WriteRegistryInt(configRootKey,"Hardware_contrast_blue",m_Hardware_contrast_blue);
    WriteRegistryInt(configRootKey,"Hardware_gamma_red",m_Hardware_gamma_red);
    WriteRegistryInt(configRootKey,"Hardware_gamma_green",m_Hardware_gamma_green);
    WriteRegistryInt(configRootKey,"Hardware_gamma_blue",m_Hardware_gamma_blue);

    WriteRegistryInt(configRootKey,"Software_gamma_mode",(int)m_Software_gamma_mode);
    WriteRegistryInt(configRootKey,"Software_gamma_red",m_Software_gamma_red);
    WriteRegistryInt(configRootKey,"Software_gamma_green",m_Software_gamma_green);
    WriteRegistryInt(configRootKey,"Software_gamma_blue",m_Software_gamma_blue);
    WriteRegistryInt(configRootKey,"Software_gamma_global",m_Software_gamma_global);

    WriteRegistryString( configRootKey, "DMX_BaseChannels", m_DMX_BaseChannels );
    WriteRegistryInt(configRootKey,"DMX_RGB_Channels",m_DMX_RGB_Channels);
    WriteRegistryInt(configRootKey,"DMX_BaudrateIndex", m_DMX_BaudrateIndex );  

    WriteRegistryInt(configRootKey,"MoMo_Channels", m_MoMo_Channels );

    WriteRegistryInt(configRootKey,"Fnordlicht_Amount", m_Fnordlicht_Amount );

    WriteRegistryInt(configRootKey,"hAtmoClLeds", m_AtmoClLeds);

    WriteRegistryInt(configRootKey,"NumChannelAssignments",getNumChannelAssignments());
    WriteRegistryInt(configRootKey,"CurrentChannelAssignment",m_CurrentChannelAssignment);
    for(int i=1;i<10;i++) {
        CAtmoChannelAssignment *ta = this->m_ChannelAssignments[i];
        if(ta!=NULL) {
           sprintf(regKeyName,"%sChannelAssignment_%d\\",configRootKey,i);
           WriteRegistryString(regKeyName,"name", ta->getName() );
           WriteRegistryInt(regKeyName,"count", ta->getSize() );
           for(int c=0;c<ta->getSize();c++) {
               sprintf(valueName,"channel_%d",c);
               WriteRegistryInt(regKeyName,valueName,ta->getZoneIndex(c));
           }
        }
    }
}

void CAtmoRegistryConfig::LoadSettings() 
{
     // alle Variabel etc. aus Registry lesen
    char regKeyName[100],valueName[32];

    // m_IgnoreConnectionErrorOnStartup 
    if(ReadRegistryInt(configRootKey,"IgnoreConnectionErrorOnStartup",0) == 1)
       m_IgnoreConnectionErrorOnStartup = ATMO_TRUE;  

    m_eAtmoConnectionType = (AtmoConnectionType)ReadRegistryInt(configRootKey,"ConnectionType",(int)actClassicAtmo);


	m_Comport  = ReadRegistryInt(configRootKey,"comport",-1); // -1 als Indikator ob Setup noch erfolgen muss!
    if(m_Comport < 1) { m_IsShowConfigDialog = 1; m_Comport=1; } // die Variable beim Laden der Settings immer setzen wenn ein key uns nicht gefällt - so das der Setup Dialog
	                                          // zwanghaft gezeigt wird - dafür sparen wir uns das Flag minimiert - solange die Config Ok ist - brauchen
	                                          // wir den Dialog ja nicht - dafür gibts ja das Trayicon?

    m_Comports[0] = ReadRegistryInt(configRootKey,"comport_1",-1);
    m_Comports[1] = ReadRegistryInt(configRootKey,"comport_2",-1);
    m_Comports[2] = ReadRegistryInt(configRootKey,"comport_3",-1);

    psz_language    = ReadRegistryString( configRootKey, "language", "de");  

    m_eEffectMode = (EffectMode)ReadRegistryInt(configRootKey,"EffectMode",(int)m_eEffectMode);

    m_WhiteAdjustment_Red    = CheckByteValue( ReadRegistryInt(configRootKey,"WhiteAdjustment_Red",m_WhiteAdjustment_Red) );
    m_WhiteAdjustment_Green  = CheckByteValue( ReadRegistryInt(configRootKey,"WhiteAdjustment_Green",m_WhiteAdjustment_Green) );
    m_WhiteAdjustment_Blue   = CheckByteValue( ReadRegistryInt(configRootKey,"WhiteAdjustment_Blue",m_WhiteAdjustment_Blue) );
	m_UseSoftwareWhiteAdj    = (ReadRegistryInt(configRootKey,"UseSoftwareWhiteAdj",m_UseSoftwareWhiteAdj) != 0);


    m_WhiteAdjPerChannel     = (ReadRegistryInt(configRootKey,"UsePerChWhiteAdj",m_WhiteAdjPerChannel) != 0);
    sprintf(regKeyName,"%sWhiteAdjPerChannel\\",configRootKey);
    m_chWhiteAdj_Count       = ReadRegistryInt(regKeyName,"count",0);
    delete []m_chWhiteAdj_Red;
    delete []m_chWhiteAdj_Green;
    delete []m_chWhiteAdj_Blue; 
    if(m_chWhiteAdj_Count > 0) 
    { 
       m_chWhiteAdj_Red   = new int[ m_chWhiteAdj_Count ];
       m_chWhiteAdj_Green = new int[ m_chWhiteAdj_Count ];
       m_chWhiteAdj_Blue  = new int[ m_chWhiteAdj_Count ];
    } else {
       m_chWhiteAdj_Red   = NULL;
       m_chWhiteAdj_Green = NULL;
       m_chWhiteAdj_Blue  = NULL;
    }
    for(int i=0; (i<m_chWhiteAdj_Count); i++) 
    {
        sprintf(valueName,"red_%d",i);
        m_chWhiteAdj_Red[i] = Check8BitValue( ReadRegistryInt(regKeyName, valueName, 256 ) );

        sprintf(valueName,"green_%d",i);
        m_chWhiteAdj_Green[i] = Check8BitValue( ReadRegistryInt(regKeyName, valueName, 256 ) );

        sprintf(valueName,"blue_%d",i);
        m_chWhiteAdj_Blue[i] = Check8BitValue( ReadRegistryInt(regKeyName, valueName, 256 ) );
    }


	m_ColorChanger_iSteps    = ReadRegistryInt(configRootKey,"ColorChanger_iSteps",m_ColorChanger_iSteps);
	m_ColorChanger_iDelay    = ReadRegistryInt(configRootKey,"ColorChanger_iDelay",m_ColorChanger_iDelay);

	m_LrColorChanger_iSteps    = ReadRegistryInt(configRootKey,"LrColorChanger_iSteps",m_LrColorChanger_iSteps);
	m_LrColorChanger_iDelay    = ReadRegistryInt(configRootKey,"LrColorChanger_iDelay",m_LrColorChanger_iDelay);

    m_IsSetShutdownColor   = ReadRegistryInt(configRootKey,"isSetShutdownColor",m_IsSetShutdownColor);
	m_ShutdownColor_Red    = CheckByteValue( ReadRegistryInt(configRootKey,"ShutdownColor_red",m_ShutdownColor_Red  ));
	m_ShutdownColor_Green  = CheckByteValue( ReadRegistryInt(configRootKey,"ShutdownColor_green",m_ShutdownColor_Green));
	m_ShutdownColor_Blue   = CheckByteValue( ReadRegistryInt(configRootKey,"ShutdownColor_blue",m_ShutdownColor_Blue) );

    m_StaticColor_Red   = ReadRegistryInt(configRootKey,"StaticColor_Red",m_StaticColor_Red);
    m_StaticColor_Green = ReadRegistryInt(configRootKey,"StaticColor_Green",m_StaticColor_Green);
    m_StaticColor_Blue  = ReadRegistryInt(configRootKey,"StaticColor_Blue",m_StaticColor_Blue);

    m_LiveViewFilterMode       = (AtmoFilterMode)ReadRegistryInt(configRootKey,"LiveViewFilterMode",(int)m_LiveViewFilterMode);
    m_LiveViewFilter_PercentNew      = ReadRegistryInt(configRootKey,"LiveViewFilter_PercentNew",m_LiveViewFilter_PercentNew);
    m_LiveViewFilter_MeanLength      = ReadRegistryInt(configRootKey,"LiveViewFilter_MeanLength",m_LiveViewFilter_MeanLength);
    m_LiveViewFilter_MeanThreshold   = ReadRegistryInt(configRootKey,"LiveViewFilter_MeanThreshold",m_LiveViewFilter_MeanThreshold);

    m_show_statistics                = (ReadRegistryInt(configRootKey,"show_statistics",ATMO_FALSE) != 0);  

    m_LiveView_RowsPerFrame   = ReadRegistryInt(configRootKey,"LiveView_RowsPerFrame",m_LiveView_RowsPerFrame);
    if(m_LiveView_RowsPerFrame < 1) m_LiveView_RowsPerFrame = 1;
    if(m_LiveView_RowsPerFrame >= CAP_HEIGHT) m_LiveView_RowsPerFrame = CAP_HEIGHT - 1;

    m_LiveView_EdgeWeighting  = ReadRegistryInt(configRootKey,"LiveView_EdgeWeighting",m_LiveView_EdgeWeighting);
    m_LiveView_BrightCorrect  = ReadRegistryInt(configRootKey,"LiveView_BrightCorrect",m_LiveView_BrightCorrect);
    m_LiveView_DarknessLimit  = ReadRegistryInt(configRootKey,"LiveView_DarknessLimit",m_LiveView_DarknessLimit);
    m_LiveView_HueWinSize     = ReadRegistryInt(configRootKey,"LiveView_HueWinSize",m_LiveView_HueWinSize);
    m_LiveView_SatWinSize     = ReadRegistryInt(configRootKey,"LiveView_SatWinSize",m_LiveView_SatWinSize);
    m_LiveView_WidescreenMode = ReadRegistryInt(configRootKey,"LiveView_WidescreenMode",m_LiveView_WidescreenMode);

    m_LiveView_HOverscanBorder  = ReadRegistryInt(configRootKey,"LiveView_HOverscanBorder",m_LiveView_HOverscanBorder);
    m_LiveView_VOverscanBorder  = ReadRegistryInt(configRootKey,"LiveView_VOverscanBorder",m_LiveView_VOverscanBorder);
    m_LiveView_DisplayNr        = ReadRegistryInt(configRootKey,"LiveView_DisplayNr",m_LiveView_DisplayNr);

    m_LiveView_FrameDelay      = ReadRegistryInt(configRootKey,"LiveView_FrameDelay",m_LiveView_FrameDelay);

    m_LiveView_GDI_FrameRate   = ReadRegistryInt(configRootKey,"LiveView_GDI_FrameRate",m_LiveView_GDI_FrameRate);

    m_ZonesTopCount            = ReadRegistryInt(configRootKey,"ZonesTopCount",m_ZonesTopCount);
    m_ZonesBottomCount         = ReadRegistryInt(configRootKey,"ZonesBottomCount",m_ZonesBottomCount);
    m_ZonesLRCount             = ReadRegistryInt(configRootKey,"ZonesLRCount",m_ZonesLRCount);
    m_ZoneSummary              = (ReadRegistryInt(configRootKey,"ZoneSummary",m_ZoneSummary) != 0);
    UpdateZoneCount();


    m_Hardware_global_gamma    = ReadRegistryInt(configRootKey,"Hardware_global_gamma",m_Hardware_global_gamma); // 0..255
    m_Hardware_global_contrast = ReadRegistryInt(configRootKey,"Hardware_global_contrast",m_Hardware_global_contrast);
    m_Hardware_contrast_red    = ReadRegistryInt(configRootKey,"Hardware_contrast_red",m_Hardware_contrast_red);
    m_Hardware_contrast_green  = ReadRegistryInt(configRootKey,"Hardware_contrast_green",m_Hardware_contrast_green);
    m_Hardware_contrast_blue   = ReadRegistryInt(configRootKey,"Hardware_contrast_blue",m_Hardware_contrast_blue);

    m_Hardware_gamma_red       = ReadRegistryInt(configRootKey,"Hardware_gamma_red",m_Hardware_gamma_red);
    m_Hardware_gamma_green     = ReadRegistryInt(configRootKey,"Hardware_gamma_green",m_Hardware_gamma_green);
    m_Hardware_gamma_blue      = ReadRegistryInt(configRootKey,"Hardware_gamma_blue",m_Hardware_gamma_blue);


    m_Software_gamma_mode   = (AtmoGammaCorrect)ReadRegistryInt(configRootKey,"Software_gamma_mode",(int)m_Software_gamma_mode);
    m_Software_gamma_red    = ReadRegistryInt(configRootKey,"Software_gamma_red",m_Software_gamma_red);
    m_Software_gamma_green  = ReadRegistryInt(configRootKey,"Software_gamma_green",m_Software_gamma_green);
    m_Software_gamma_blue   = ReadRegistryInt(configRootKey,"Software_gamma_blue",m_Software_gamma_blue);
    m_Software_gamma_global = ReadRegistryInt(configRootKey,"Software_gamma_global",m_Software_gamma_global);
    
    m_Software_gamma_red    = min(max(m_Software_gamma_red, 5), 35);  // 0,5 - 3,5
    m_Software_gamma_green  = min(max(m_Software_gamma_green, 5), 35);  // 0,5 - 3,5
    m_Software_gamma_blue   = min(max(m_Software_gamma_blue, 5), 35);  // 0,5 - 3,5
    m_Software_gamma_global = min(max(m_Software_gamma_global, 5), 35);  // 0,5 - 3,5


    int tmpChannel          = ReadRegistryInt(configRootKey, "DMX_BaseChannel", 0 );
    if((tmpChannel < 0) || (tmpChannel>253)) tmpChannel = 0;
    char buf[16];
    sprintf(buf,"%d",tmpChannel);
    char *dmx_ch = ReadRegistryString( configRootKey, "DMX_BaseChannels", buf);
    this->setDMX_BaseChannels( dmx_ch );
    free( dmx_ch );

    m_DMX_RGB_Channels      = ReadRegistryInt(configRootKey,"DMX_RGB_Channels", m_DMX_RGB_Channels );
    m_DMX_BaudrateIndex     = ReadRegistryInt(configRootKey,"DMX_BaudrateIndex", m_DMX_BaudrateIndex );  

    m_MoMo_Channels         = ReadRegistryInt(configRootKey,"MoMo_Channels", m_MoMo_Channels );
    if(m_MoMo_Channels > 254) m_MoMo_Channels = 254;
    if(m_MoMo_Channels < 0)   m_MoMo_Channels = 2;

    m_Fnordlicht_Amount     = ReadRegistryInt(configRootKey,"Fnordlicht_Amount", m_Fnordlicht_Amount );
    if(m_Fnordlicht_Amount > 254) m_Fnordlicht_Amount = 254;
    if(m_Fnordlicht_Amount < 0)   m_Fnordlicht_Amount = 1;

    m_AtmoClLeds            = ReadRegistryInt(configRootKey,"hAtmoClLeds", m_AtmoClLeds );
    if (m_AtmoClLeds > 256) m_AtmoClLeds = 256;
    if (m_AtmoClLeds < 1)   m_AtmoClLeds = 1;

    clearChannelMappings(); // clear channel mappings except default!
    m_CurrentChannelAssignment = ReadRegistryInt(configRootKey,"CurrentChannelAssignment",m_CurrentChannelAssignment);
    int numChannels = ReadRegistryInt(configRootKey,"NumChannelAssignments",0);
    if(m_CurrentChannelAssignment>=numChannels)
       m_CurrentChannelAssignment = 0;

    for(int i=1;i<numChannels;i++) {
        CAtmoChannelAssignment *ta = this->m_ChannelAssignments[i];
        if(ta==NULL) {
           ta = new CAtmoChannelAssignment();
           this->m_ChannelAssignments[i] = ta;
        }

        sprintf(regKeyName,"%sChannelAssignment_%d\\",configRootKey,i);
        char *name = ReadRegistryString(regKeyName,"name","?");
        ta->setName(name);
        ta->system = ATMO_FALSE;
        free(name);
        int chCount = ReadRegistryInt(regKeyName,"count",0);

        ta->setSize(chCount);

        for(int c=0;c<chCount;c++) {
            sprintf(valueName,"channel_%d",c);
            ta->setZoneIndex(c, ReadRegistryInt(regKeyName,valueName,c) );
        }
    }

    UpdateZoneDefinitionCount();
}

int CAtmoRegistryConfig::CheckByteValue(int value) 
{
	if(value < 0) value = 0;
	if(value > 255) value = 255;
	return value;
}

int CAtmoRegistryConfig::Check8BitValue(int value) 
{
	if(value < 0) value = 0;
	if(value > 256) value = 256;
	return value;
}

