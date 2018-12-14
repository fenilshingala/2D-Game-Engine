/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRateController.cpp
Purpose: Frame Rate Controller
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "FrameRateController.h"

FrameRateController::FrameRateController(Uint32 maxFrameRate) {
	mTickStart = mTickEnd = mFrameTime = 0;

	if (0 != maxFrameRate)
		mNeededTicksPerFrame = 1000 / maxFrameRate;
	else
		mNeededTicksPerFrame = 0;
}

FrameRateController::~FrameRateController() {

}

void FrameRateController::frameStart() {
	mTickStart = SDL_GetTicks();
}

void FrameRateController::frameEnd() {
	mTickEnd = SDL_GetTicks();
	while ((mTickEnd - mTickStart) < mNeededTicksPerFrame) {
		mTickEnd = SDL_GetTicks();
	}
	mFrameTime = mTickEnd - mTickStart;
}

Uint32 FrameRateController::getFrameTime() { return mFrameTime; }