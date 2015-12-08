## 接口说明

### 说明
	CIniHelper是使用C++编写的解析ini文件的帮助库，主要封装了对INI文件的操作，目前可以的Windows下，debian 7下编译测试通过。
	目前INI只封装了获取值，修改的值的功能，暂未实现删除一个值，一个Section，添加一个值，一个Section的操作

### 主要接口
主要导出的类及封装接口说明如下所示：

```
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
```

### 使用方法

```
#include "inihelper.h"
#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	int n;
	int nIndex;
	for(int nTime = 0; nTime < 100; nTime ++)
	{
		CIniHelper helper;
#ifdef _WIN32
        const char *pFileName = ".\\test.ini";
#else
        const char *pFileName = "./test.ini";
#endif
		// 设置INI文件路径
		helper.SetConfigPath(pFileName);
		// 加载所有测试
		helper.Load();
		SECTION_TYPE sections;
		SECTION_VALUE_TYPE section_value;

		// 获取所有Section
		if (ST_SUCESS != helper.GetAllSection(sections))
		{
			printf("获取所有的section失败!\n");
			goto err;
		}
		printf("section is :\n");
		for(nIndex = 0; nIndex < sections.size(); nIndex++)
		{
			printf("  %s\n", sections[nIndex].c_str());
		}

		// 获取每个section下的值
		for(nIndex = 0; nIndex < sections.size(); nIndex++)
		{
			printf("section %s key-value is:\n", sections[nIndex].c_str());
			if (ST_SUCESS == helper.GetSectionAllValue(sections[nIndex].c_str(), section_value))
			{
				for(n = 0; n < section_value.size(); n++)
				{
					printf("  section:%s\n", section_value[n].strSection.c_str());
					printf("  key:%s\n", section_value[n].strKeyName.c_str());
					printf("  value:%s\n", section_value[n].strValue.c_str());
				}
			}
		}
		// 修改值测试
		if (ST_SUCESS != helper.SetValue(NULL, "test", "测试@test123123\\276276"))
		{
			printf("set test key value error\n");
			goto err;
		}
		// 修改值测试
		if (ST_SUCESS != helper.SetValue("section1", "key1", "测试\\276276"))
		{
			printf("set test key value error\n");
			goto err;
		}
		// 保存修改测试
		if (ST_SUCESS != helper.Save())
		{
			printf("ini helper save config error!\n");
			goto err;
		}
	}

err:
	getchar();
	return 0;
}
```