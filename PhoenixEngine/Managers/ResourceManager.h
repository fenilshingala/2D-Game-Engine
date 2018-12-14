/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.h
Purpose: Resource Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class Mesh;
class Shader;

enum OBJECT_TYPE {
	RECTANGLE,
	TRIANGLE,
	CIRCLE
};

#include <unordered_map>

class ResourceManager {
public:
	unsigned int circleVAO;
	unsigned int lineVAO;

	ResourceManager();
	~ResourceManager();
	unsigned int getShaderID();
	unsigned int LoadMesh(unsigned int meshName);
	int getTextureID(const char* path);

private:
	std::unordered_map<unsigned int, unsigned int> mMeshes;
	//std::unordered_map<const char *, unsigned int> mTextures;
	std::unordered_map<std::string, unsigned int> mTextures;
	Shader* shader1;

	unsigned int LoadRect();
	unsigned int LoadTriangle(unsigned int texture1 = 0);
	unsigned int LoadCircle();
	unsigned int LoadLine();
};

#endif