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
	ST_SUCESS					= 0, //�������سɹ�
	ST_FAILD					= -1,//��������ʧ��
	ST_ERROR_LINE				= -2,//�����д���

};

enum VALUE_TYPE{
	INVALID_DATA_STYLE			= 0,//�Ƿ�����
	SECTION_VALUE_STYLE,			//sectionֵ����
	KEY_VALUE_STYLE,				//Keyֵ����
	VALUE_TYEPE_STYLE,				//Valueֵ����
	IGNORE_VALUE_STYLE,				//���Դ���ֵ����
};

enum ITEM_STATUS{
	NO_MODIFY					= 0,//û���޸�
	VALUE_MODIFY,					//�޸Ĳ���
	ADD_VALUE,						//���Ӳ���
	KEY_DEL,						//ɾ������
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
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc:	  ���ز���������
     */
    int Load();

    /**
     * Method:    SetConfigPath
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Parameter: const char * pszConfigPath
	 * Desc:	  ���������ļ�·��
     */
    int SetConfigPath(const char *pszConfigPath);

    /**
     * Method:    GetAllSection
     * Access:    public 
     * Parameter: SECTION_TYPE & vcSections
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc: 	  ��ȡ����Section
     */
    int GetAllSection(SECTION_TYPE &vcSections);
    /**
     * Method:    GetValue
     * Access:    public 
     * Parameter: const char * pszSection Section������
     * Parameter: const char * pszKeyName Key������
     * Parameter: char * pszValue		  ���ص��ַ���ֵ	
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc: 	  ��ȡ����ֵ
     */
    int GetValue(const char *pszSection, const char *pszKeyName, char *pszValue);
    /**
     * Method:    GetValue
     * Access:    public 
	 * Parameter: const char * pszSection Section������
	 * Parameter: const char * pszKeyName Key������
     * Parameter: int & nValue			  ���ص�����ֵ
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc: 	  ��ȡ����ֵ
     */
    int GetValue(const char *pszSection, const char *pszKeyName, int &nValue);
    /**
     * Method:    SetValue
     * Access:    public 
	 * Parameter: const char * pszSection  Section������
	 * Parameter: const char * pszKeyName  Key������
     * Parameter: const char * pszValue    ���ò������ַ���ֵ
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc: 	  ���ò���ֵ
     */
    int SetValue(const char *pszSection, const char *pszKeyName, const char *pszValue);
    /**
     * Method:    SetValue
     * Access:    public 
	 * Parameter: const char * pszSection Section������
	 * Parameter: const char * pszKeyName Key������
     * Parameter: int nValue			  ���ò���������ֵ	
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc: 	  ���ò���ֵ
     */
    int SetValue(const char *pszSection, const char *pszKeyName, int nValue);
    /**
     * Method:    GetSectionAllValue
     * Access:    public 
     * Parameter: const char * pszSection				Section������
     * Parameter: SECTION_VALUE_TYPE & vcSectionValue   ��ȡ������ֵ
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc: 	  ��ȡһ��Section�µ�����ֵ
     */
    int GetSectionAllValue(const char *pszSection, SECTION_VALUE_TYPE &vcSectionValue);
    /**
     * Method:    Save
     * Access:    public 
     * Returns:   ST_SUCESS �ɹ�, ����ʧ��
     * Desc: 	  �����޸ĵ�����
     */
    int Save();
};
