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
	ST_SUCESS					= 0, //函数返回成功
	ST_FAILD					= -1,//函数返回失败
	ST_ERROR_LINE				= -2,//解析行错误

};

enum VALUE_TYPE{
	INVALID_DATA_STYLE			= 0,//非法数据
	SECTION_VALUE_STYLE,			//section值类型
	KEY_VALUE_STYLE,				//Key值类型
	VALUE_TYEPE_STYLE,				//Value值类型
	IGNORE_VALUE_STYLE,				//忽略处理值类型
};

enum ITEM_STATUS{
	NO_MODIFY					= 0,//没有修改
	VALUE_MODIFY,					//修改操作
	ADD_VALUE,						//增加操作
	KEY_DEL,						//删除操作
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
    /**
     * Method:    Load
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc:	  加载并解析策略
     */
    int Load();

    /**
     * Method:    SetConfigPath
     * Returns:   ST_SUCESS 成功, 其他失败
     * Parameter: const char * pszConfigPath
	 * Desc:	  设置配置文件路径
     */
    int SetConfigPath(const char *pszConfigPath);

    /**
     * Method:    GetAllSection
     * Access:    public 
     * Parameter: SECTION_TYPE & vcSections
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc: 	  获取所有Section
     */
    int GetAllSection(SECTION_TYPE &vcSections);
    /**
     * Method:    GetValue
     * Access:    public 
     * Parameter: const char * pszSection Section的名字
     * Parameter: const char * pszKeyName Key的名字
     * Parameter: char * pszValue		  返回的字符串值	
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc: 	  获取参数值
     */
    int GetValue(const char *pszSection, const char *pszKeyName, char *pszValue);
    /**
     * Method:    GetValue
     * Access:    public 
	 * Parameter: const char * pszSection Section的名字
	 * Parameter: const char * pszKeyName Key的名字
     * Parameter: int & nValue			  返回的整形值
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc: 	  获取参数值
     */
    int GetValue(const char *pszSection, const char *pszKeyName, int &nValue);
    /**
     * Method:    SetValue
     * Access:    public 
	 * Parameter: const char * pszSection  Section的名字
	 * Parameter: const char * pszKeyName  Key的名字
     * Parameter: const char * pszValue    设置参数的字符串值
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc: 	  设置参数值
     */
    int SetValue(const char *pszSection, const char *pszKeyName, const char *pszValue);
    /**
     * Method:    SetValue
     * Access:    public 
	 * Parameter: const char * pszSection Section的名字
	 * Parameter: const char * pszKeyName Key的名字
     * Parameter: int nValue			  设置参数的整形值	
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc: 	  设置参数值
     */
    int SetValue(const char *pszSection, const char *pszKeyName, int nValue);
    /**
     * Method:    GetSectionAllValue
     * Access:    public 
     * Parameter: const char * pszSection				Section的名字
     * Parameter: SECTION_VALUE_TYPE & vcSectionValue   获取到所有值
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc: 	  获取一个Section下的所有值
     */
    int GetSectionAllValue(const char *pszSection, SECTION_VALUE_TYPE &vcSectionValue);
    /**
     * Method:    Save
     * Access:    public 
     * Returns:   ST_SUCESS 成功, 其他失败
     * Desc: 	  保存修改的配置
     */
    int Save();
};
