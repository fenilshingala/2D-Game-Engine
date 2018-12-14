#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.h
Purpose: Game Object Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include <vector>

class GameObject;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
public:
	std::vector<GameObject *> mGameObjects;
	GameObject* mPlayer;
};