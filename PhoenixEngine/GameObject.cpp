/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Purpose: Game Object
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "GameObject.h"
#include "Components/Component.h"

#include"Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/Controller.h"
#include "Components/UpDown.h"
#include "Components/Body.h"
#include "Components/Button.h"
#include "Components/Target.h"
#include "Components/AI.h"
#include "Components/Text.h"
#include "Components/Expand.h"

#include "Managers/EventManager.h"

GameObject::GameObject() : mType(NULL), mFlag(false)
{
	for (auto c : mComponents)
		c = nullptr;
}

GameObject::~GameObject()
{
	for (auto c : mComponents)
		delete c;
	mComponents.clear();
}

void GameObject::update()
{
	for (auto c : mComponents)
		c->update();
}

Component* GameObject::AddComponent(unsigned int Type)
{
	Component *pNewComponent = nullptr;
	switch (Type)
	{
	case TRANSFORM:
		pNewComponent = new Transform();
		break;
	case SPRITE:
		pNewComponent = new Sprite();
		break;
	case CONTROLLER:
		pNewComponent = new Controller();
		break;
	case UPDOWN:
		pNewComponent = new UpDown();
		break;
	case BODY:
		pNewComponent = new Body();
		break;
	case BUTTON:
		pNewComponent = new Button();
		break;
	case TARGET:
		pNewComponent = new Target();
		break;
	case OBJAI:
		pNewComponent = new AI();
		break;
	case TEXT:
		pNewComponent = new Text();
		break;
	case EXPAND:
		pNewComponent = new Expand();
		break;

		return pNewComponent;
	}

	if (nullptr != pNewComponent)
	{
		pNewComponent->mpOwner = this;
		mComponents.push_back(pNewComponent);
	}
	return pNewComponent;
}

Component* GameObject::GetComponent(unsigned int Type)
{
	for (auto c : mComponents)
		if (c->getType() == Type)
			return c;
	return nullptr;
}

void GameObject::HandleEvent(Event *pEvent) {
	for (auto c : mComponents) {
		if(c != nullptr)
			c->HandleEvent(pEvent);
	}
}