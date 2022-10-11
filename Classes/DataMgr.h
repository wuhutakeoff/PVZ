#pragma once
#include "DataBase.h"
#include <map>
#include <string>
using namespace std;

//ʹ�ü̳ж�̬�������ݷ��档
//����//ֻ�����ݵĴ�ȡ������
//�������ز���������ģʽ��������أ�

class CDataMgr
{
public:
	CDataMgr();
	static CDataMgr* getInstance();
	//������ݲ�����ӿ�:���֣����ݲ�����ָ�롣
	void addMgr(string strName, CDataBaseMgr* pDataBaseMgr);
	//��ȡ�ӿڡ�
	template <class T>
	T* getMgr(string strName)
	{
		if (strName.empty())
		{
			return nullptr;
		}
		int nCount = m_mapMgr.count(strName);
		if (nCount > 0)
		{
			return static_cast<T*>(m_mapMgr[strName]);
		}
		return nullptr;
	}
private:
	static CDataMgr* m_spInstance;
	//����һ��map�����洢���ָ��������ݲ�������ָ�롣
	map<string, CDataBaseMgr*> m_mapMgr;
};

