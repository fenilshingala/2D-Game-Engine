/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.h
Purpose: Input Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL_stdinc.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_keyboard.h>

class InputManager {
public:
	InputManager();
	~InputManager();

	void update();

	bool isPressed(unsigned int keyScanCode);
	bool isTriggerred(unsigned int keyScanCode);
	bool isReleased(unsigned int keyScanCode);

	bool isButtonPressed(unsigned int keyScanCode);
	bool isButtonReleased(unsigned int keyScanCode);
	bool isButtonTriggerred(unsigned int keyScanCode);

private:
	Uint8 mCurrentState[512];
	Uint8 mPreviousState[512];

	Uint8 mMouseCurrentState[3];
	Uint8 mMousePreviousState[3];
};

#endif