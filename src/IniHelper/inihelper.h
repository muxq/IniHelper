#pragma once
#include <string>
#include <vector>
#include "Buffer.h"
#ifdef INIHELPER_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

enum{
	ST_SUCESS					= 0,
	ST_FAILD					= -1,
	ST_ERROR_LINE				= -2,

};

enum VALUE_TYPE{
	INVALID_DATA_STYLE			= 0,
	SECTION_VALUE_STYLE,
	KEY_VALUE_STYLE,
	VALUE_TYEPE_STYLE,
	IGNORE_VALUE_STYLE,
};

enum ITEM_STATUS{
	NO_MODIFY					= 0,
	VALUE_MODIFY,
	ADD_VALUE,
	KEY_DEL,
};
typedef struct ConfigItem_st{
	std::string strSection; 
	std::string strKeyName;	
	std::string strValue;	
	int nValue;	
	int nStatus;
	ConfigItem_st():strSection(""), strKeyName(""), strValue(""), nValue(0), nStatus(NO_MODIFY)
	{

	}
}ConfigItem;

#define SECTION_TYPE		std::vector<std::string>
#define SECTION_VALUE_TYPE	std::vector<ConfigItem>

#ifdef _WIN32
class API CIniHelper{
#else
class CIniHelper{
#endif
public:
	CIniHelper();
	~CIniHelper();
private:
	int ParseFile();
	int PaseLine(const char *pszLine, CBuffer<char> &cbSection, CBuffer<char> &cbKey, CBuffer<char> &cbValue, CBuffer<char> &cbDesc);
	int ModifyData(CBuffer<char> &cbData, const char *pszSection, const char *pszKey, const char *pszValue, int nStatus);
private:
    std::string         m_strConfigPath; 
    SECTION_VALUE_TYPE  m_all_config;   
public:
    int Load();
    int SetConfigPath(const char *pszConfigPath);
    int GetAllSection(SECTION_TYPE &vcSections);
    int GetValue(const char *pszSection, const char *pszKeyName, char *pszValue);
    int GetValue(const char *pszSection, const char *pszKeyName, int &nValue);
    int SetValue(const char *pszSection, const char *pszKeyName, const char *pszValue);
    int SetValue(const char *pszSection, const char *pszKeyName, int nValue);
    int GetSectionAllValue(const char *pszSection, SECTION_VALUE_TYPE &vcSectionValue);
    int Save();
};
