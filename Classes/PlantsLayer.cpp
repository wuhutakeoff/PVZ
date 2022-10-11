#include "PlantsLayer.h"

CPlantsLayer::CPlantsLayer()
{
}

CPlantsLayer::~CPlantsLayer()
{
}

bool CPlantsLayer::init()
{
	if (!Node::init())
	{
		return false;
	}



	return true;
}

bool CPlantsLayer::havePlant(Vec2 pos)
{
	Vector<Node*> vecChild = this->getChildren();
	for (int i = 0; i < vecChild.size(); i++)
	{
		if (vecChild.at(i)->getPosition()==pos)
		{
			return true;
		}
	}
	return false;
}

bool CPlantsLayer::isInBox(Rect box)
{
	Vector<Node*> vecChild = this->getChildren();
	for (int i = 0; i < vecChild.size(); i++)
	{
		if (box.containsPoint(vecChild.at(i)->getPosition()))
		{
			return true;
		}
	}
	return false;
}

