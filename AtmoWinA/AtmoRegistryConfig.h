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
       // zwei M�glichkeiten alle Variablen Private und nur �ber Methoden darauf zugreifen
       // w�re zwar sauberer find ich - aber weniger performant wenn man bedenkt wie h�ufig
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

