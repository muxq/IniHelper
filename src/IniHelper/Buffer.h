#pragma once
#include <stdlib.h>
template<class T>

class CBuffer{
public:
	CBuffer():m_data(NULL),m_Totallen(0),m_UseLen(0)
	{

	}
	~CBuffer()
	{
		FreeData();
	}
	CBuffer(const CBuffer &buf)
	{
		m_Totallen = buf.m_Totallen;
		m_data = (T *)malloc(sizeof(T) * m_Totallen);
		if (m_data)
		{
			memcpy(m_data, buf.m_data, sizeof(T) * m_Totallen);
		}
	}

	CBuffer &operator = (const CBuffer &rhs)
	{
		this->FreeData();
		this->AppendMemory(rhs.m_data, rhs.m_UseLen);
		return *this;
	}

	bool operator == (const CBuffer &rhs)
	{
		if (this->m_Totallen != rhs.GetBufferLength())
		{
			return false;
		}
		if (0 != memcmp(this->m_data, rhs.m_data, this->m_Totallen * sizeof(T)))
		{
			return false;
		}
		return true;
	}

	bool operator < (const CBuffer &rhs) const
	{
		if (m_Totallen < rhs.m_Totallen)
		{
			return false;
		}
		return true;
	}

	bool operator()(const CBuffer &lhs, const CBuffer &rhs)
	{
		if (this->GetBufferLength() < rhs.GetBufferLength())
		{
			return false;
		}
		return true;
	}

	T &operator [](int n)
	{
		if (m_Totallen > n)
		{
			return m_data[n];
		}
		return 0;
	}

	void RigthTrim()
	{
		int nIndex = m_UseLen - 1;
		for(; nIndex > 0; nIndex --)
		{
			if (' ' != m_data[nIndex])
			{
				break;
			}
		}	
		if (nIndex == 0)
		{
			m_UseLen = 0;
		}
		else
		{
			// ��Ҫת��Ϊ����
			m_UseLen = nIndex + 1;
		}
	}

	void LeftTrim()
	{
		int nIndex = 0;
		for(; nIndex < m_UseLen; nIndex ++)
		{
			if (' ' != m_data[nIndex])
			{
				break;
			}
		}	
		if (0 == nIndex)
		{
			return;
		}
		
		if (nIndex == m_UseLen)
		{
			m_UseLen = 0;
		}
		else
		{
			memcpy(m_data, m_data + nIndex, m_UseLen - nIndex);
			m_UseLen = m_UseLen - nIndex;
		}
	}

	void Trim()
	{
		RigthTrim();
		LeftTrim();
	}
	
protected:
private:
	T*		m_data;			// ��ǰ����
	int		m_Totallen;		// �ܳ���
	int		m_UseLen;		// �Ѿ�ʹ�ó���
public:
	// �����ڴ�
	void *AllocMemory(int nLen)
	{
		int nMemoryLen = m_Totallen + nLen;
		T *pData = (T *)malloc(nMemoryLen * sizeof(T));
		if (NULL == pData)
		{
			return NULL;
		}
		memset(pData, 0, nMemoryLen);

		if (m_data && 0 < m_Totallen)
		{	
			memcpy(pData, m_data, m_Totallen * sizeof(T));
			free(m_data);
			m_data = NULL;
		}
		m_data = pData;
		m_Totallen = nMemoryLen;
		return m_data;
	}
	// ׷������
	void *AppendMemory(T *pData, int nLen)
	{
		if (NULL == pData && 0 >= nLen)
		{
			return NULL;
		}
		if (nLen > GetBufferFreeLength())
		{
			if (NULL == AllocMemory(m_Totallen + nLen))
			{
				return NULL;
			}
		}
		memcpy(m_data + m_UseLen, pData, nLen * sizeof(T));
		m_UseLen += nLen;
		return m_data + m_UseLen - nLen;
	}
	// ׷������
	void *AppendMemory(T p)
	{
		if (sizeof(T) > GetBufferFreeLength())
		{
			if (NULL == AllocMemory(m_Totallen + sizeof(T)))
			{
				return NULL;
			}
		}
		memcpy(m_data + m_UseLen, &p, sizeof(T));
		m_UseLen += sizeof(T);
		return m_data + m_UseLen - sizeof(T);
	}
	// ��ͷ��ɾ������
	void *DelFront(int nLen)
	{
		if (m_Totallen < nLen)
		{
			return NULL;
		}
		// ����ǰ��
		memcpy(m_data, m_data + nLen, (m_Totallen - nLen) * sizeof(T));
		m_UseLen = (m_UseLen - nLen) > 0 ? (m_UseLen - nLen):0;
		return m_data;
	}

	// ��ĩβɾ������
	void *DelBack(int nLen)
	{
		if (m_Totallen < nLen)
		{
			return NULL;
		}
		int n = m_Totallen - nLen;
		m_UseLen = (m_UseLen > n) ? (m_UseLen - n):m_UseLen;
	}


	// ����ڴ�
	void FreeData(){ 
		if (m_data)
		{
			free(m_data);
			m_data = NULL;
			m_Totallen = 0;
			m_UseLen = 0;
		}
	}
	// �����������
	void DelData(){ 
		if (m_data)
		{
			memset(m_data, 0, m_Totallen);
			m_UseLen = 0;
		}
	}
	// ��ȡ�ܳ���
	int GetBufferLength(){return m_Totallen;};
	// ������
	T *GetBufferData(){return m_data;};
	// ��ȡ���ݻ�����ó���
	int GetBufferFreeLength(){return m_Totallen - m_UseLen;};
	// ��ȡ���ݳ���
	int GetDataLength(){return m_UseLen;};
};
