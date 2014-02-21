#ifndef _AtmoRegistryConfig_h_
#define _AtmoRegistryConfig_h_

#include "atmoconfig.h"

class CAtmoRegistryConfig :  public CAtmoConfig
{
private:
       void WriteRegistryInt(char *path, char *valueName, int value);
       void WriteRegistryString(char *path, char *valueName, char *value);
       void WriteRegistryDouble(char *path, char *valueName, double value);

       int ReadRegistryInt(char *path, char *valueName, int default_value);
       char * ReadRegistryString(char *path, char *valueName, char *default_value);
       double ReadRegistryDouble(char *path, char *valueName, double default_value);

	   int RegistryKeyExists(char *path);
	   int RegistryValueExists(char *path, char *valueName);

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

