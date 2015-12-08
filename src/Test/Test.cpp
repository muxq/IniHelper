// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "inihelper.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int n;
	int nIndex;
	for(int nTime = 0; nTime < 100; nTime ++)
	{
		CIniHelper helper;
		helper.SetConfigPath(".\\test.ini");
		helper.Load();
		SECTION_TYPE sections;
		SECTION_VALUE_TYPE section_value;


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
		if (ST_SUCESS != helper.SetValue(NULL, "test", "穆小庆@test123123\\276276"))
		{
			printf("set test key value error\n");
			goto err;
		}
		if (ST_SUCESS != helper.SetValue("section1", "key1", "爬山的\\276276"))
		{
			printf("set test key value error\n");
			goto err;
		}
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

