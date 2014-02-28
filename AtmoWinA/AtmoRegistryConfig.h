#ifndef _AtmoRegistryConfig_h_
#define _AtmoRegistryConfig_h_

#include "atmoconfig.h"
#include "AtmoRegistry.h"

class CAtmoRegistryConfig :  public CAtmoConfig, protected CAtmoRegistry
{
private:
	   int CheckByteValue(int value);
	   int Check8BitValue(int value);
 
    private:
       char *psz_language; 

    private:
       // zwei Möglichkeiten alle Variablen Private und nur über Methoden darauf zugreifen
       // wäre zwar sauberer find ich - aber weniger performant wenn man bedenkt wie häufig
       // darauf zugegriffen wird - daher lieber alle Variablen Public lassen...?
       char configRootKey[80]; /* \HKLM\Software\AtmoWin\ */

public:
     CAtmoRegistryConfig(char *regKey);
     virtual ~CAtmoRegistryConfig(void);

     void SaveSettings();
     void LoadSettings();

     char *getLanguage() { return(psz_language); }

};

#endif

