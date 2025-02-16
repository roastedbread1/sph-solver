#include "model.h"
#include "SPHsystem.h"
#include "shader.h"



void SYS::initSettings(SPHSettings* settings, float mass, float restDensity, float gasConst, float visocity, float h, float g, float tension)
{
	settings->mass = mass;
	settings->restDensity = restDensity;
	settings->gasConst = gasConst;
	settings->viscosity = visocity;
	settings->h = h;
	settings->g = g;
	settings->tension = tension;
}

void SYS::initParticles(SPHsys* system, SYS::SPHSettings& settings, size_t particleCubeWidth)
{
	system->particleCubeWidth = particleCubeWidth;
	system->particleCount = particleCubeWidth * particleCubeWidth * particleCubeWidth;
	system->particles = (Particle*)malloc(sizeof(Particle) * system->particleCount);

	system->settings = settings;
	system->model = new Model();
	LoadModel(system->model, "D:/codes/more codes/c++/sph/sph-solver/models/low/scene.gltf");

	system->sphereModelMatrix = new glm::mat4[system->particleCount];




	genParticles(system);

	for (size_t i = 0; i < system->particleCount; i++)
	{
		Particle* particle = &system->particles[i];


		glm::mat4 instanceMatrix = glm::mat4(1.0f);
		instanceMatrix = glm::translate(instanceMatrix, particle->position);
		system->sphereModelMatrix[i] = instanceMatrix;
	}



	glGenBuffers(1, &system->instanceSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, system->instanceSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, system->particleCount * sizeof(glm::mat4), system->sphereModelMatrix, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, system->instanceSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	system->started = false;

}

void SYS::genParticles(SPHsys* system)
{
	std::srand(1024);

	float particleSeperation = system->settings.h + 0.01f;

	for (int i = 0; i < system->particleCubeWidth; i++)
	{
		for (int j = 0; j < system->particleCubeWidth; j++)
		{
			for (int k = 0; k < system->particleCubeWidth; k++)
			{
				float ranX = (float(rand()) / float((RAND_MAX)) * 0.5f - 1) * system->settings.h/10;

				float ranY = (float(rand()) / float((RAND_MAX)) * 0.5f - 1) * system->settings.h / 10;

				float ranZ =  (float(rand()) / float((RAND_MAX)) * 0.5f - 1) * system->settings.h / 10;

				glm::vec3 nParticlePos = glm::vec3(
					i * particleSeperation + ranX - 1.5f,
					j * particleSeperation + ranY + system->settings.h + 0.1f,
					k * particleSeperation + ranZ - 1.5f
				);

				size_t particleIndex = i + (j + system->particleCubeWidth * k) * system->particleCubeWidth;

				Particle* particle = &system->particles[particleIndex];
				particle->position = nParticlePos;
				particle->velocity = glm::vec3(0);

				system->sphereModelMatrix[particleIndex] = glm::translate(particle->position) * system->settings.sphereScale;
			}
		}
	}
}

void SYS::destroyParticles(SPHsys* system)
{
	free(system->sphereModelMatrix);
	delete[](system->particles);
	glDeleteBuffers(1, &system->instanceSSBO);
}

void SYS::updateParticles(SPHsys* system)
{
}

void SYS::drawParticles(SPHsys* system, Shader* shader)
{

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, system->instanceSSBO);

	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, system->particleCount * sizeof(glm::mat4), system->sphereModelMatrix);

	for (unsigned int i = 0; i < system->model->meshes.size(); i++)
	{
		glBindVertexArray(system->model->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, system->model->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, system->particleCount);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
