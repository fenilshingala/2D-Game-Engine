#ifndef FRAME_RATE_CONTROLLER_H
#define FRAME_RATE_CONTROLLER_H

/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRateController.h
Purpose: Frame Rate Controller
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include <SDL_stdinc.h>
#include <SDL_timer.h>

class FrameRateController{
public:
	FrameRateController(Uint32 maxFrameRate);
	~FrameRateController();

	void frameStart();
	void frameEnd();
	Uint32 getFrameTime();
private:
	Uint32 mTickStart;
	Uint32 mTickEnd;
	Uint32 mFrameTime;
	Uint32 mNeededTicksPerFrame;
};

#endif