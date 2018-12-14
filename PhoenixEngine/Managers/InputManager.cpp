/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.cpp
Purpose: Input Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/03/2018
- End Header --------------------------------------------------------*/

#include "InputManager.h"
#include <iostream>

extern int width;
extern int height;
extern bool appIsRunning;

InputManager::InputManager() {
	SDL_memset(mCurrentState, 0, 512 * sizeof(Uint8));
	SDL_memset(mPreviousState, 0, 512 * sizeof(Uint8));
	SDL_memset(mMouseCurrentState, 0, 3 * sizeof(Uint8));
	SDL_memset(mMousePreviousState, 0, 3 * sizeof(Uint8));
}

InputManager::~InputManager() {}

void InputManager::update() {
	Uint8 pCurrentMouseStates[3] = { 0 };
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			appIsRunning = false;
		}
		if (e.type == SDL_WINDOWEVENT) {
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				width = e.window.data1;
				height = e.window.data2;
				glViewport(0, 0, width, height);
			}
		}

		// MOUSE STATES
		switch (e.type)
		{
		case SDL_MOUSEBUTTONUP:
			pCurrentMouseStates[e.button.button-1] = 0;
			break;

		case SDL_MOUSEBUTTONDOWN:
			pCurrentMouseStates[e.button.button-1] = 1;
			break;
		}
	}

	SDL_memcpy(mMousePreviousState, mMouseCurrentState, 3 * sizeof(Uint8));
	SDL_memcpy(mMouseCurrentState, pCurrentMouseStates, 3 * sizeof(Uint8));

	// KEYBOARD STATES
	int numberOfFetchedkeys = 0;
	const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOfFetchedkeys);

	if (numberOfFetchedkeys > 512)
		numberOfFetchedkeys = 512;

	SDL_memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));
	SDL_memcpy(mCurrentState, pCurrentKeyStates, numberOfFetchedkeys * sizeof(Uint8));
}


// MOUSE
bool InputManager::isButtonPressed(unsigned int keyScanCode)
{
	if (keyScanCode-1 >= 3)
		return false;
	if (mMouseCurrentState[keyScanCode-1])
		return true;

	return false;
}

bool InputManager::isButtonTriggerred(unsigned int keyScanCode) {
	if (keyScanCode-1 >= 3)
		return false;
	if (mMouseCurrentState[keyScanCode-1] && !mMousePreviousState[keyScanCode-1])
		return true;

	return false;
}

bool InputManager::isButtonReleased(unsigned int keyScanCode)
{
	if (keyScanCode-1 >= 3)
		return false;
	if (!mMouseCurrentState[keyScanCode-1] && mMousePreviousState[keyScanCode-1])
		return true;

	return false;
}



// KEYBOARD
bool InputManager::isPressed(unsigned int keyScanCode) {
	if (keyScanCode >= 512)
		return false;
	if (mCurrentState[keyScanCode])
		return true;

	return false;
}

bool InputManager::isTriggerred(unsigned int keyScanCode) {
	if (keyScanCode >= 512)
		return false;
	if (mCurrentState[keyScanCode] && !mPreviousState[keyScanCode])
		return true;

	return false;
}

bool InputManager::isReleased(unsigned int keyScanCode) {
	if (keyScanCode >= 512)
		return false;
	if (!mCurrentState[keyScanCode] && mPreviousState[keyScanCode])
		return true;

	return false;
}