#include "DataBase.h"

CDataBaseMgr::CDataBaseMgr()
{
}

CDataBaseMgr::~CDataBaseMgr()
{
}

void CDataBaseMgr::loadFile(string strPath)
{
	//ͨ�������ļ�·����ȡ�ļ���������ݣ����ַ�������ʽ��ȡ
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);
	//����document
	rapidjson::Document doc;
	//���ļ������ַ���������doc����
	doc.Parse(strData.c_str());
	this->parse(doc);
}
