#include "stdafx.h"
#include "inihelper.h"
#include <fstream>
#include <istream>
#include "Buffer.h"

#ifdef _WIN32
#include <io.h>
#endif

#define CH_PARAM(a, b, c) if (NULL == a || NULL == b || NULL == c)break
#define CH_PARAM2(a) if(NULL == a) break


#define MAX_LINE_LEN	256
#define MAX_BUF_LEN		2048

CIniHelper::CIniHelper():
	m_strConfigPath("")
{

}

CIniHelper::~CIniHelper()
{

}

int CIniHelper::Load()
{
    int nRet = ST_FAILD;
    do{
        if (ST_SUCESS != ParseFile())
        {
			break;
        }
        
        nRet = ST_SUCESS;
    }while(0);
    return nRet;
}

int CIniHelper::SetConfigPath(const char *pszConfigPath)
{
    int nRet = ST_FAILD;
    do{
        if(NULL == pszConfigPath)
        {
            break;
        }
#ifdef _WIN32
		if (0 != _access(pszConfigPath, 0))
		{
			break;
		}
#endif

        m_strConfigPath = pszConfigPath;
        nRet = ST_SUCESS;
    }while(0);
    return nRet;
}

int CIniHelper::GetAllSection(SECTION_TYPE &vcSections)
{
	int nRet = ST_FAILD;
	do 
	{
		int nIndex = 0;
		for (; nIndex < m_all_config.size(); nIndex++)
		{
			SECTION_TYPE::iterator iter = vcSections.begin();
			for (; iter != vcSections.end(); iter ++)
			{
				if (0 == stricmp(iter->c_str(), m_all_config[nIndex].strSection.c_str()))
				{
					break;
				}
			}

			if (iter == vcSections.end())
			{
				vcSections.push_back(m_all_config[nIndex].strSection);
			}
		}
		
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

int CIniHelper::GetValue(const char *pszSection, const char *pszKeyName, char *pszValue)
{
	int nRet = ST_FAILD;
	do 
	{
		if(NULL == pszSection)
			pszSection = "default";
		CH_PARAM(pszSection, pszKeyName, pszValue);
		int nIndex = 0;
		for(; nIndex < m_all_config.size(); nIndex++)
		{
			if (0 == stricmp(pszSection, m_all_config[nIndex].strSection.c_str()) &&
				0 == stricmp(pszKeyName, m_all_config[nIndex].strKeyName.c_str()))
			{
				strcpy(pszValue, m_all_config[nIndex].strValue.c_str());
				nRet = ST_SUCESS;
			}
		}
	} while (0);
	return nRet;
}

int CIniHelper::GetValue(const char *pszSection, const char *pszKeyName, int &nValue)
{
	int nRet = ST_FAILD;
	do 
	{
		char szValue[MAX_LINE_LEN] = {0};
		if (ST_SUCESS != GetValue(pszSection, pszKeyName, szValue))
		{
			break;
		}
		nValue = atoi(szValue);
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

int CIniHelper::SetValue(const char *pszSection, const char *pszKeyName, const char *pszValue)
{
	int nRet = ST_FAILD;
	do 
	{
		if(NULL == pszSection)
			pszSection = "default";
		CH_PARAM(pszSection, pszKeyName, pszValue);
		int nIndex = 0;
		for(; nIndex < m_all_config.size(); nIndex++)
		{
			if (0 == stricmp(pszSection, m_all_config[nIndex].strSection.c_str()) &&
				0 == stricmp(pszKeyName, m_all_config[nIndex].strKeyName.c_str()))
			{
				m_all_config[nIndex].strValue = pszValue;
				m_all_config[nIndex].nStatus = VALUE_MODIFY;
				nRet = ST_SUCESS;
			}
		}
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

int CIniHelper::SetValue(const char *pszSection, const char *pszKeyName, int nValue)
{
	int nRet = ST_FAILD;
	do 
	{
		char szValue[MAX_LINE_LEN] = {0};
		sprintf(szValue, "%d", nValue);
		if (ST_SUCESS != SetValue(pszSection, pszKeyName, szValue))
		{
			break;
		}
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

int CIniHelper::GetSectionAllValue(const char *pszSection, SECTION_VALUE_TYPE &vcSectionValue)
{
	int nRet = ST_FAILD;
	do 
	{
		CH_PARAM2(pszSection);
		vcSectionValue.clear();
		for(int nIndex = 0; nIndex < m_all_config.size(); nIndex++)
		{
			if (0 == stricmp(pszSection, m_all_config[nIndex].strSection.c_str()))
			{
				vcSectionValue.push_back(m_all_config[nIndex]);
			}
		}
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

// 暂不考虑使用中被删除，或不修改注视中的值
int CIniHelper::Save()
{
	int nRet = ST_FAILD;
	do 
	{
		FILE *pFile = fopen(m_strConfigPath.c_str(), "rb");
		if (NULL == pFile)
		{
			break;
		}

		CBuffer<char> buf;
		char szBuffer[MAX_BUF_LEN] = {0};
		int	 nLen = sizeof(szBuffer);
		int nReadLen = fread(szBuffer, sizeof(char), nLen, pFile);
		while(nReadLen > 0)
		{
			if (NULL == buf.AppendMemory(szBuffer, nReadLen))
			{
				break;
			}
	
			nReadLen = fread(szBuffer, sizeof(char), nLen, pFile);
		}
		fclose(pFile);
		pFile = NULL;
		
		// 遍历每一项，保存该项
		int nIndex = 0;
		for (; nIndex < m_all_config.size(); nIndex++)
		{
			if (ST_SUCESS != ModifyData(buf, m_all_config[nIndex].strSection.c_str(), m_all_config[nIndex].strKeyName.c_str(), 
				m_all_config[nIndex].strValue.c_str(), m_all_config[nIndex].nStatus))
			{
				break;
			}	
		}

		if (nIndex != m_all_config.size())
		{
			break;
		}

		pFile = fopen(m_strConfigPath.c_str(), "wb");
		if (NULL == pFile)
		{
			break;
		}
		
		int nWriteLen = 0;
		int nWrite = fwrite(buf.GetBufferData(), sizeof(char), buf.GetDataLength(), pFile);
		while(nWrite > 0)
		{
			nWriteLen += nWrite;
			if (nWriteLen == buf.GetDataLength() - 1)
			{
				break;
			}
			nWrite = fwrite(buf.GetBufferData() + nWriteLen, sizeof(char), buf.GetDataLength() - nWriteLen, pFile);
		}
		fclose(pFile);
		pFile = NULL;
		
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

int CIniHelper::ParseFile()
{
	int nRet = ST_FAILD;
	do 
	{
		m_all_config.clear();
		std::string strSection = "default";
		std::ifstream ifs(m_strConfigPath.c_str());
		char szLine[MAX_LINE_LEN] = {0};
		CBuffer<char> buf;
		while(ifs.getline(szLine, sizeof(szLine)))
		{
			CBuffer<char> cbSection, cbKey, cbValue, cbDesc;
			if (ST_SUCESS == PaseLine(szLine, cbSection, cbKey, cbValue, cbDesc))
			{
				if (cbSection.GetDataLength() > 0)
				{
					cbSection.Trim();
					cbSection.AppendMemory('\0');
					strSection = cbSection.GetBufferData();
					continue;
				}
				if (cbKey.GetDataLength() > 0)
				{
					cbKey.Trim();
					cbValue.Trim();
					cbKey.AppendMemory('\0');
					cbValue.AppendMemory('\0');

					ConfigItem Item;
					Item.strSection = strSection;
					Item.strKeyName = cbKey.GetBufferData();
					Item.strValue = cbValue.GetBufferData();
					Item.nValue = atoi(Item.strValue.c_str());
					m_all_config.push_back(Item);
				}
				// 暂时对注释不做处理
			}
		}
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

//解析行
int CIniHelper::PaseLine(const char *pszLine, CBuffer<char> &cbSection, CBuffer<char> &cbKey, CBuffer<char> &cbValue, CBuffer<char> &cbDesc)
{
	int nRet = ST_FAILD;
	do 
	{
		if(NULL == pszLine)
			break;
		int nValueType = INVALID_DATA_STYLE;
		int nLineLen = strlen(pszLine);
		for (int n = 0; n < nLineLen; n++)
		{
			
			if ('[' == pszLine[n])
			{
				nValueType = SECTION_VALUE_STYLE;
				n++;
			}
			else if (']' == pszLine[n])
			{
				nValueType = IGNORE_VALUE_STYLE;
			}
			else if('=' == pszLine[n] && nValueType == KEY_VALUE_STYLE)
			{
				nValueType = VALUE_TYEPE_STYLE;
				n++;
			}
			else if (' ' != pszLine[n] && nValueType == INVALID_DATA_STYLE && cbValue.GetBufferData() == 0)
			{
				nValueType = KEY_VALUE_STYLE;
			}
			if ('#' == pszLine[n] || ';' == pszLine[n])
			{
				cbDesc.AppendMemory((char *)(pszLine + n + 1), strlen(pszLine) - n - 1);
				nRet = ST_SUCESS;
				break;
			}

			if (n == nLineLen)
			{
				break;
			}
			
			switch(nValueType)
			{
			case  SECTION_VALUE_STYLE:
				cbSection.AppendMemory(pszLine[n]);
				break;
			case KEY_VALUE_STYLE:
				cbKey.AppendMemory(pszLine[n]);
				break;
			case VALUE_TYEPE_STYLE:
				cbValue.AppendMemory(pszLine[n]);
				break;
			}
		}
		if (cbSection.GetDataLength() > 0 && nValueType != IGNORE_VALUE_STYLE)
		{
			break;
		}
		if (cbValue.GetDataLength() > 0 && cbKey.GetDataLength() == 0)
		{
			break;
		}
		nRet = ST_SUCESS;
	} while (0);
	return nRet;
}

int CIniHelper::ModifyData(CBuffer<char> &cbData, const char *pszSection, const char *pszKey, const char *pszValue, int nStatus)
{
	int nRet = ST_FAILD;
	do 
	{
		// 如果该
		if (NULL == pszSection)
		{
			break;
		}
		switch(nStatus)
		{
		case VALUE_MODIFY:
			{
				if ( NULL == pszKey)
				{
					break;
				}
				
				char *pszPos = NULL;
				if (0 == stricmp(pszSection, "default"))
				{
					pszPos = cbData.GetBufferData();
				}
				else
				{
					pszPos = strstr(cbData.GetBufferData(), pszSection);
				}
				
				if (NULL == pszPos)
				{
					break;
				}
				char *pszData = strstr(pszPos, pszKey);
				if (NULL == pszData || NULL == (pszData += strlen(pszKey)))
				{
					break;
				}

				int nLentoBegine = (int)(pszData - cbData.GetBufferData());
				
				int nLen = cbData.GetDataLength() - nLentoBegine;
				int nIndex = 0;
				int nBegin = 0;
				int nEnd = nLen;
				for (; nIndex < nLen; nIndex++)
				{
					if ('=' != pszData[nIndex] && ' ' != pszData[nIndex] && 0 == nBegin)
					{
						nBegin = nIndex;
					}
					if ('\n' == pszData[nIndex] || '\r' == pszData[nIndex])
					{
						nEnd = nIndex;
						break;
					}
				}
				if (nEnd < nBegin)
				{
					break;
				}
				CBuffer<char> cbTemp;
				cbTemp.AppendMemory((char *)cbData.GetBufferData(), nLentoBegine + nBegin);
				cbTemp.AppendMemory((char *)pszValue, strlen(pszValue));
				cbTemp.AppendMemory((char *)(cbData.GetBufferData() + nLentoBegine + nEnd), cbData.GetDataLength() - nLentoBegine - nEnd);
				cbData = cbTemp;
				nRet = ST_SUCESS;
			}
			break;
		case ADD_VALUE:
			{
				
			}
			break;
		case KEY_DEL:
			{
				
			}
			break;
		case NO_MODIFY:
		default:
			{
				nRet = ST_SUCESS;
			}
			break;
		}
		
	} while (0);
	return nRet;
}

