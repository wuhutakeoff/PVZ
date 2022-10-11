#include "DataStruct.h"
#include <fstream>
#include "DataMgr.h"
#include <functional>

CFactory* CFactory::m_spInstance = nullptr;
CFactory::CFactory()
{
}

CFactory::~CFactory()
{
}

CFactory* CFactory::getInstance()
{
	if (!m_spInstance)
	{
		m_spInstance = new CFactory();
		m_spInstance->init();
	}
	return m_spInstance;
}

void CFactory::init()
{
	m_mapCallBack["LevelDtMgr"] = [=]() {
		return new CLevelDtMgr();
	};
	m_mapCallBack["PlantDtMgr"] = [=]() {
		return new CPlantDtMgr();
	};
	m_mapCallBack["ZombiesDtMgr"] = [=]() {
		return new CZombiesDtMgr();
	};
}


CDataBaseMgr* CFactory::getDtMgr(string strName)
{
	if (strName.empty())
	{
		return nullptr;
	}
	// m_mapCallBack[strName]是一个函数  
	return m_mapCallBack[strName]();
}
CLoadDtMgr::CLoadDtMgr()
{
}

CLoadDtMgr::~CLoadDtMgr()
{
}

void CLoadDtMgr::parse(rapidjson::Document& doc)
{
	//最外层是对象，doc就是对象。
	//通过doc拿到name属性，的值，是一个数组。
	rapidjson::Value& vName = doc["name"];
	//vName数组。
	for (int i = 0; i < vName.Size(); i++)
	{
		m_vecName.push_back(vName[i].GetString());
	}

	rapidjson::Value& vPath = doc["path"];
	for (int i = 0; i < vPath.Size(); i++)
	{
		m_vecPath.push_back(vPath[i].GetString());
	}
}


CLoader::CLoader()
{
}

CLoader::~CLoader()
{
}

void CLoader::load()
{
	CLoadDtMgr* pLoad = new CLoadDtMgr();
	pLoad->loadFile("data/Path.json");

	for (int i = 0; i < pLoad->m_vecName.size(); i++)
	{
		CDataBaseMgr* pMgr = CFactory::getInstance()->getDtMgr(pLoad->m_vecName[i]);
		pMgr->loadFile(pLoad->m_vecPath[i]);
		CDataMgr::getInstance()->addMgr(pLoad->m_vecName[i], pMgr);
	}

}



CLevelDtMgr::CLevelDtMgr()
{
	m_nCurIndex = 0;
}

CLevelDtMgr::~CLevelDtMgr()
{
}


void CLevelDtMgr::parse(rapidjson::Document& doc)
{
	//关卡数据，最外层是数组，所以可以这么理解。doc就是数组，     for循环。
	//如果最外面是对象，那么doc就是对象，访问的时候 doc["key1"]  doc["key2"]
	for (int i = 0; i < doc.Size(); i++)
	{
		//每一个对象是一个数据，每一次循环都要创建一个关卡数据
		CLevelDt* pData = new CLevelDt();
		//doc[i]就是数据数组里面的每个对象。
		//是对象，所以这里用一个 Vlaue接收。
		rapidjson::Value& vData = doc[i];//vData就是数据数组里面的每个元素对象。
		//通过中括号加key字符串获取这个对象里面的值。
		pData->nID = vData["id"].GetInt();
		pData->strMap = vData["img"].GetString();
		pData->strTmx = vData["tmx"].GetString();
		
		rapidjson::Value& vID = vData["enemyID"];
		for (int j = 0; j < vID.Size(); j++)
		{
			pData->vecEnemyID.push_back(vID[j].GetInt());
		}

		rapidjson::Value& vCount = vData["wave"];
		for (int j = 0; j < vCount.Size(); j++)
		{
			pData->vecEnemyCount.push_back(vCount[j].GetInt());
		}

		//数据存储
		m_vecDatas.push_back(pData);
	}
}

CPlantDtMgr::CPlantDtMgr()
{
}

CPlantDtMgr::~CPlantDtMgr()
{
}

void CPlantDtMgr::parse(rapidjson::Document& doc)
{
	for (int i = 0; i < doc.Size(); i++)
	{
		CPlantDt* pData = new CPlantDt();
		rapidjson::Value& vData = doc[i];
		pData->nID = vData["id"].GetInt();
		pData->strName = vData["name"].GetString();
		rapidjson::Value& vPath = vData["animPath"];
		vector<string> vecName;
		for (int i = 0; i < vPath.Size(); i++)
		{
			vecName.push_back(vPath[i].GetString());
		}
		pData->nValue = vData["value"].GetInt();
		pData->vecAnimPath = vecName;
		pData->type = vData["type"].GetString();
		pData->bulletPath = vData["bulletPath"].GetString();
		pData->nDamage = vData["damage"].GetInt();
		pData->nHp = vData["HP"].GetInt();
		m_vecDatas.push_back(pData);
	}
}

CPlantDt* CPlantDtMgr::getDataByName(string strName)
{
	for (int i = 0; i < m_vecDatas.size(); i++)
	{
		CPlantDt* dt = (CPlantDt*)m_vecDatas[i];
		if (dt->strName==strName)
		{
			return dt;
		}
	}
	return nullptr;
}

vector<string> CPlantDtMgr::getNames()
{
	vector<string> vecNames;
	for (int i = 0; i < m_vecDatas.size(); i++)
	{
		CPlantDt* dt = (CPlantDt*)m_vecDatas[i];
		vecNames.push_back(dt->strName);
	}
	return vecNames;
}


CZombiesDtMgr::CZombiesDtMgr()
{
}

CZombiesDtMgr::~CZombiesDtMgr()
{
}

void CZombiesDtMgr::parse(rapidjson::Document& doc)
{
	for (int i = 0; i < doc.Size(); i++)
	{
		CZombiesDt* pData = new CZombiesDt();
		rapidjson::Value& vData = doc[i];
		pData->nID = vData["id"].GetInt();
		pData->strName = vData["name"].GetString();
		rapidjson::Value& vName = vData["animName"];
		vector<string> vecName;
		for (int j = 0; j < vName.Size(); j++)
		{
			vecName.push_back(vName[j].GetString());
		}

		rapidjson::Value& vPath = vData["animPath"];
		for (int j = 0; j < vPath.Size(); j++)
		{
			vector<string> vecPath;
			//第j个是数组
			rapidjson::Value& vPathName = vPath[j];
			for (int m = 0; m < vPathName.Size(); m++)
			{
				vecPath.push_back(vPathName[m].GetString());
			}

			pData->mapAnimateName[vecName[j]] = vecPath;
		}


		rapidjson::Value& vDelay = vData["delay"];
		for (int j = 0; j < vDelay.Size(); j++)
		{
			pData->mapDelay[vecName[j]] = vDelay[j].GetFloat();
		}

		pData->nDamge = vData["damage"].GetInt();
		pData->nHp = vData["HP"].GetInt();

		m_vecDatas.push_back(pData);
	}
}