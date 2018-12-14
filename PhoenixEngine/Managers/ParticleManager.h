#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ParticleManager.h
Purpose: Particle Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include <glm/glm.hpp>
#include <vector>
#include "../GameObject.h"
#include "../Components/Body.h"
#include "GameObjectManager.h"
#include "../Shader.h"

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();
	void update();
	unsigned int FirstUnusedParticle();
	void RespawnParticle(Particle &particle, GameObject* object, glm::vec2 offset);

public:
	unsigned int nr_particles;
	std::vector<Particle> particles;
	unsigned int lastUsedParticle;

private:
	GameObject* mPlayer;
	Shader* pShader;
};