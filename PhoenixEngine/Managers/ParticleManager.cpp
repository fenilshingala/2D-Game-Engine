/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ParticleManager.cpp
Purpose: Particle Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "ParticleManager.h"

extern GameObjectManager *pGameObjectManager;

ParticleManager::ParticleManager() : lastUsedParticle(0), nr_particles(500), mPlayer(nullptr)
{
	for (unsigned int i = 0; i < nr_particles; ++i)
		particles.push_back(Particle());

	pShader = new Shader("../Resources/Shaders/Particle.shader");
}

ParticleManager::~ParticleManager() {}


void ParticleManager::update()
{
	mPlayer = pGameObjectManager->mPlayer;
	
	unsigned int nr_new_particles = 2;
	// Add new particles
	for (unsigned int i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		glm::vec2 offset(5.0f, 5.0f);
		RespawnParticle(particles[unusedParticle], mPlayer, offset);
	}
	// Uupdate all particles
	for (unsigned int i = 0; i < nr_particles; ++i)
	{
		Particle &p = particles[i];
		p.Life -= 0.99f; // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			p.Position -= p.Velocity * 0.99f;
			p.Color.a -= 0.99f * 2.5f;
		}
	}


	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	pShader->useProgram();
	for (Particle particle : particles)
	{
		if (particle.Life > 0.0f)
		{
			glUniform2fv(glGetUniformLocation(pShader->getID(), "offset"), 1, &particle.Position[0]);
			glUniform4fv(glGetUniformLocation(pShader->getID(), "color"), 1, &particle.Color[0]);
			
			//glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int ParticleManager::FirstUnusedParticle()
{
	// Search from last used particle, this will usually return almost instantly
	for (unsigned int i = lastUsedParticle; i < nr_particles; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
}


void ParticleManager::RespawnParticle(Particle &particle, GameObject* object, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);

	Body *pBody = static_cast<Body*>(object->GetComponent(BODY));
	glm::vec2 objectPosition(pBody->mPosX, pBody->mPosY);
	glm::vec2 objectVelocity(pBody->mVelX, pBody->mVelY);

	particle.Position = objectPosition + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = objectVelocity * 0.1f;
}