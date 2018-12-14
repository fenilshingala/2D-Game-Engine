#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: Shader
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "GL\glew.h"
#include "iostream"

class Shader
{
public:
	Shader(const char* shaderSource);
	~Shader();

	unsigned int getID();
	void useProgram();
private:
	unsigned int ID;
	void checkCompileErrors(GLuint shader, std::string type);
};

