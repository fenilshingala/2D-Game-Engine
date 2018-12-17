#pragma once

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