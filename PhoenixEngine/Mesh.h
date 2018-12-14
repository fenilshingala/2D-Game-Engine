#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: Mesh Class
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class Shader;
class Transform;
class Sprite;

class Mesh {
public:
	Mesh(Shader*,float* vertices, int size, bool isCircle,
		unsigned int* indices = NULL, unsigned int indicesSize = 0
	);

	~Mesh();
	unsigned int getTextureID() { return textureID; }
	unsigned int getVAO() { return VAO; }
private:
	/*  Mesh Data  */

	//VERTICES
	float* vertices;
	int size;
	// INDEXES
	unsigned int* indices;
	unsigned int indicesSize;
	bool isCircle;
	// TEXTURES
	unsigned int textureID;

	Shader* shader;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	void setupMesh();
};