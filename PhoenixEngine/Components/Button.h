#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Button.h
Purpose: Button Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Component.h"
#include "../Managers/EventManager.h"

enum GAME_MODES {
	START,
	HOWTOPLAY,
	QUIT,
	PAUSE_CONTINUE,
	PAUSE_QUIT,
	MAINMENU,
	PAUSE_MAINMENU,
	RESTART,
	SKIP,
	CREDITS
};

class PressedEvent : public Event {
public:
	PressedEvent() : Event(PRESSED) {}
	~PressedEvent() {}
};

class Button : public Component
{
public:
	Button();
	~Button();

	void Serialize(GenericObject<false, Value::ValueType>);
	void update();
	void HandleEvent(Event *pEvent);

public:
	unsigned int mGameMode;
};