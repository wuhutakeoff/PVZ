#pragma once
#include <map>
#include "DataBase.h"
#include "DataMgr.h"
#include <functional>
typedef function<CDataBaseMgr* ()> CallBack;

class CLoadDtMgr : public CDataBaseMgr
{
public:
	CLoadDtMgr();
	~CLoadDtMgr();
	virtual void parse(rapidjson::Document& doc);
	//���ؽ��������ָ�·���ֱ�洢����������
	vector<string> m_vecName;
	vector<string> m_vecPath;

};


class CFactory
{
public:
	CFactory();
	~CFactory();
	static CFactory* getInstance();
	void init();
	//ͨ�����ֻ�ȡ�������ӿڡ�
	CDataBaseMgr* getDtMgr(string strName);
private:
	static CFactory* m_spInstance;
	//�����ָ��ص�������һ��
	map<string, CallBack> m_mapCallBack;
};

//�����࣬����������ݡ�
class CLoader
{
public:
	CLoader();
	~CLoader();
	static void load();
	/*template <class T>
	static void loadRes(string strPath,string strName)
	{
		T* pMgr = new T();
		pMgr->loadFile(strPath);
		CDataMgr::getInstance()->addMgr(strName, pMgr);

	}*/
private:

};



class CLevelDt : public CDataBase
{
public:
	string strMap;
	string strTmx;
	vector<int> vecEnemyID;
	vector<int> vecEnemyCount;
};


class CLevelDtMgr : public CDataBaseMgr
{
public:
	CLevelDtMgr();
	~CLevelDtMgr();
	virtual void parse(rapidjson::Document& doc);
	//��ȡ��ǰ�ؿ����ݣ�������Ϸ��������
	CLevelDt* getCurLevelDt()
	{
		return static_cast<CLevelDt*>(m_vecDatas[m_nCurIndex]);
	}
	CC_SYNTHESIZE(int, m_nCurIndex, CurIndex);
private:

};


class CPlantDt : public CDataBase
{
public:
	string strName;
	vector<string> vecAnimPath;
	string type;
	string bulletPath;
	int nValue;
	int nDamage;
	int nHp;
};

class CPlantDtMgr : public CDataBaseMgr
{
public:
	CPlantDtMgr();
	~CPlantDtMgr();
	virtual void parse(rapidjson::Document& doc);
	CPlantDt* getDataByName(string strName);
	vector<string> getNames();
private:

};


class CZombiesDt : public CDataBase
{
public:
	string strName;
	int nHp;
	int nDamge;
	//�������ָ� ��Ӧ�ľ���֡��������󶨡�
	map<string, vector<string>> mapAnimateName;
	map<string, float> mapDelay;
};

class CZombiesDtMgr : public CDataBaseMgr
{
public:
	CZombiesDtMgr();
	~CZombiesDtMgr();
	virtual void parse(rapidjson::Document& doc);

private:

};
