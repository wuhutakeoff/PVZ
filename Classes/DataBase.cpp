#include "DataBase.h"

CDataBaseMgr::CDataBaseMgr()
{
}

CDataBaseMgr::~CDataBaseMgr()
{
}

void CDataBaseMgr::loadFile(string strPath)
{
	//通过数据文件路径获取文件里面的内容，以字符串的形式获取
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);
	//创建document
	rapidjson::Document doc;
	//把文件内容字符串解析到doc里面
	doc.Parse(strData.c_str());
	this->parse(doc);
}
