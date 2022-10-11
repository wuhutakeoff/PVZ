#include "DataMgr.h"


CDataMgr* CDataMgr::m_spInstance = nullptr;
CDataMgr::CDataMgr()
{	
}

CDataMgr* CDataMgr::getInstance()
{
	if (!m_spInstance)
	{
		m_spInstance = new CDataMgr();
	}
	return m_spInstance;
}

void CDataMgr::addMgr(string strName, CDataBaseMgr* pDataBaseMgr)
{
	//如果名字是空或者指针不存在空，不存储
	if (strName.empty() || !pDataBaseMgr)
	{
		return;
	}
	m_mapMgr.insert(make_pair(strName, pDataBaseMgr));
	//m_mapMgr[strName] = pDataBaseMgr;
}
