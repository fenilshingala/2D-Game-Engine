#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionManager.h
Purpose: Collision Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "..\Components\Body.h"
#include <list>
class Body;

class Shape {
public:
	enum ShapeType
	{
		CIRCLE,
		RECTANGLE,

		NUM
	};
	Shape(ShapeType type);
	virtual ~Shape() {}
	virtual bool TestPoint(float PointX, float PointY) = 0;
public:
	Body *mpOwnerBody;
	ShapeType mType;
};



class ShapeCircle : public Shape {
public:
	ShapeCircle();
	~ShapeCircle();
	bool TestPoint(float PointX, float PointY);
public:
	float mRadius;
};


class ShapeAABB : public Shape {
public:
	ShapeAABB();
	~ShapeAABB();
	bool TestPoint(float PointX, float PointY);
public:
	float mTop, mBottom, mLeft, mRight;
};


class Contact {
public:
	Contact();
	~Contact() {}
	Body *mBodies[2];
};

class CollisionManager {
public:
	CollisionManager();
	~CollisionManager();

	void Reset();
	bool ChechCollisionAndGenerate(Shape *pShape1, float pos1X, float pos1Y,
		Shape *pShape2, float pos2X, float pos2Y);
public:
	std::list<Contact *> mContacts;

	// 2D array of function pointers, used to store collision fun add
	bool(*CollisionFuncitons[Shape::NUM][Shape::NUM])(
		Shape *pShape1, float pos1X, float pos1Y,
		Shape *pShape2, float pos2X, float pos2Y,
		std::list<Contact* > &Contacts
	);

};