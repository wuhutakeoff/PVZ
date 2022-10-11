#pragma once
#include "DataBase.h"
#include <map>
#include <string>
using namespace std;

//使用继承多态处理数据方面。
//解耦//只做数据的存取操作。
//不做加载操作（代理模式，代理加载）

class CDataMgr
{
public:
	CDataMgr();
	static CDataMgr* getInstance();
	//添加数据操作类接口:名字，数据操作类指针。
	void addMgr(string strName, CDataBaseMgr* pDataBaseMgr);
	//获取接口。
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
	//定义一个map容器存储名字跟具体数据操作基类指针。
	map<string, CDataBaseMgr*> m_mapMgr;
};

