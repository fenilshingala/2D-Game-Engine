/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionManager.cpp
Purpose: Collision Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "CollisionManager.h"
#include "../Components/Transform.h"
#include "../GameObject.h"

// SHAPE
Shape::Shape(ShapeType type) {
	this->mType = type;
}


// SHAPE CIRCLE
ShapeCircle::ShapeCircle() : Shape(CIRCLE) {
	mRadius = 0.0f;
}

ShapeCircle::~ShapeCircle() {}

bool ShapeCircle::TestPoint(float PointX, float PointY) {
	return false;
}


// SHAPE AABB
ShapeAABB::ShapeAABB() : Shape(RECTANGLE) {
	mTop = mBottom = mLeft = mRight = 0.0f;
}

ShapeAABB::~ShapeAABB() {}

bool ShapeAABB::TestPoint(float PointX, float PointY) {
	return false;
}


// CONTACT
Contact::Contact() {
	mBodies[0] = nullptr;
	mBodies[1] = nullptr;
}

// IMPLEMENTED
bool CheckCollisionCircleCircle(Shape *pShape1, float pos1X, float pos1Y,
	Shape *pShape2, float pos2X, float pos2Y, std::list<Contact* > &Contacts)
{
	ShapeCircle* s1 = static_cast<ShapeCircle*>(pShape1);
	ShapeCircle* s2 = static_cast<ShapeCircle*>(pShape2);

	if (sqrt(((pos1X - pos2X) * (pos1X - pos2X)) +
		(pos1Y - pos2Y) * (pos1Y- pos2Y)) > (s1->mRadius + s2->mRadius))
		return false;
	// Intersection!!

	// Create new contact & add it
	Contact* pNewContact = new Contact();
	pNewContact->mBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mBodies[1] = pShape2->mpOwnerBody;

	Contacts.push_back(pNewContact);

	return true;
}

// IMPLEMENTED
bool CheckCollisionCircleAABB(Shape *pShape1, float pos1X, float pos1Y,
	Shape *pShape2, float pos2X, float pos2Y, std::list<Contact* > &Contacts)
{
	ShapeCircle* s1 = static_cast<ShapeCircle*>(pShape1);
	ShapeAABB* s2 = static_cast<ShapeAABB*>(pShape2);

	Transform *pTr = static_cast<Transform*>(s2->mpOwnerBody->mpOwner->GetComponent(TRANSFORM));
	float Width = pTr->scaleX/2.0f + s1->mRadius;
	float Height = pTr->scaleY/2.0f + s1->mRadius;

	if (((pos2X - (Width)) > pos1X) || ((pos2X + (Width)) < pos1X) ||
		((pos2Y - (Height)) > pos1Y) || ((pos2Y + (Height)) < pos1Y))
		return false;

	// Intersection!!

	// Create new contact & add it
	Contact* pNewContact = new Contact();
	pNewContact->mBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mBodies[1] = pShape2->mpOwnerBody;

	Contacts.push_back(pNewContact);

	return true;
}


// IMPLEMENTED
bool CheckCollisionAABBAABB(Shape *pShape1, float pos1X, float pos1Y,
	Shape *pShape2, float pos2X, float pos2Y, std::list<Contact* > &Contacts)
{
	ShapeAABB* s1 = static_cast<ShapeAABB*>(pShape1);
	ShapeAABB* s2 = static_cast<ShapeAABB*>(pShape2);

	float Width0 = s1->mRight - s1->mLeft;
	float Height0 = s1->mTop - s1->mBottom;

	float Width1 = s2->mRight - s2->mLeft;
	float Height1 = s2->mTop - s2->mBottom;

	if (   ((pos1X + Width0/2.0f) < (pos2X - Width1/2.0f))
		|| ((pos2X + Width1/2.0f) < (pos1X - Width0/2.0f))
		|| ((pos1Y + Height0/2.0f) < (pos2Y - Height1/2.0f))
		|| ((pos2Y + Height1/2.0f) < (pos1Y - Height0/2.0f)))
		return false;

	// Intersection!!
	else {
		// Create new contact & add it
		Contact* pNewContact = new Contact();
		pNewContact->mBodies[0] = pShape1->mpOwnerBody;
		pNewContact->mBodies[1] = pShape2->mpOwnerBody;

		Contacts.push_back(pNewContact);
	}
	return true;
}

// IMPLEMENTED
bool CheckCollisionAABBCircle(Shape *pShape1, float pos1X, float pos1Y,
	Shape *pShape2, float pos2X, float pos2Y, std::list<Contact* > &Contacts)
{
	return CheckCollisionCircleAABB(pShape2, pos2X, pos2Y, pShape1, pos1X, pos1Y, Contacts);
}


CollisionManager::CollisionManager()
{
	CollisionFuncitons[Shape::CIRCLE][Shape::CIRCLE] = CheckCollisionCircleCircle;
	CollisionFuncitons[Shape::CIRCLE][Shape::RECTANGLE] = CheckCollisionCircleAABB;
	CollisionFuncitons[Shape::RECTANGLE][Shape::CIRCLE] = CheckCollisionAABBCircle;
	CollisionFuncitons[Shape::RECTANGLE][Shape::RECTANGLE] = CheckCollisionAABBAABB;
}

CollisionManager::~CollisionManager()
{
	Reset();
}

void CollisionManager::Reset()
{
	for (auto c : mContacts) {
		delete c;
	}
	mContacts.clear();
}


bool CollisionManager::ChechCollisionAndGenerate(
	Shape *pShape1, float pos1X, float pos1Y,
	Shape *pShape2, float pos2X, float pos2Y
	)
{
	return CollisionFuncitons[pShape1->mType][pShape2->mType](pShape1, pos1X, pos1Y,
		pShape2, pos2X, pos2Y, mContacts);
}