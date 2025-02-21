#include "model.h"
#include "SPHsystem.h"
#include "shader.h"


void SYS::initSettings(SPHSettings* settings, float mass, float restDensity, float gasConst, float viscosity, float h, float g, float tension, float cellSize, const glm::vec3& bounds, float dt, float boundaryDamping)
{

	settings->mass = mass;
	settings->restDensity = restDensity;
	settings->gasConst = gasConst;
	settings->viscosity = viscosity;
	settings->h = h;
	settings->g = g;
	settings->tension = tension;
	settings->cellSize = cellSize;
	settings->bounds = bounds;
	settings->dt = dt;
	settings->boundaryDamping = boundaryDamping;
}

void SYS::initParticles(SPHsys* system, SYS::SPHSettings& settings, size_t particleCubeWidth)
{
	system->particleCubeWidth = particleCubeWidth;
	system->particleCount = particleCubeWidth * particleCubeWidth * particleCubeWidth;

	system->settings = settings;
	system->model = new Model();
	LoadModel(system->model, "D:/codes/more codes/c++/sph/sph-solver/models/sphere/part.obj");
	setupModelAttributes(system, system->model);

	ComputeBuffers& buffers = system->computeBuffers;
	
	
	glGenBuffers(1, &buffers.particleBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers.particleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * system->particleCount, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers.particleBuffer);
//	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32F, GL_RED, GL_FLOAT, nullptr);
	

	glGenBuffers(1, &buffers.matrixBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers.matrixBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * system->particleCount, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffers.matrixBuffer);

	//cell to store particle hashes
	glGenBuffers(1, &buffers.cellParticleBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers.cellParticleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(CellParticle) * system->particleCount, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffers.cellParticleBuffer);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32F, GL_RED, GL_FLOAT, nullptr);
	

	
	const uint32_t TABLE_SIZE = 1048576; 
	glGenBuffers(1, &buffers.cellBoundsBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers.cellBoundsBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * 2 * TABLE_SIZE, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, buffers.cellBoundsBuffer);



	system->computeBuffers = buffers;

	system->computeShader = new Shader();
	system->sortShader = new Shader();
	system->densityShader = new Shader();
	system->forceShader = new Shader();
	system->integrationShader = new Shader();
	system->bitonicSortShader = new Shader();
	system->cellBoundsShader = new Shader();
	
	ReadAndCompileCompShader(system->computeShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/dam_break.comp");
	//ReadAndCompileCompShader(system->computeShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/generate.comp");
	ReadAndCompileCompShader(system->sortShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/sort.comp");
	ReadAndCompileCompShader(system->densityShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/density.comp");
	ReadAndCompileCompShader(system->forceShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/force.comp");
	ReadAndCompileCompShader(system->integrationShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/integrate.comp");
	ReadAndCompileCompShader(system->bitonicSortShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/bitonic.comp");
	ReadAndCompileCompShader(system->cellBoundsShader, "D:/codes/more codes/c++/sph/sph-solver/shaders/bounds.comp");

	genParticles(system);
	system->started = false;

}

void SYS::genParticles(SPHsys* system)
{
	UseCompShader(system->computeShader);

	SetShaderFloat(system->computeShader, "particleSeperation", 0.03f);
	
	SetShaderInt(system->computeShader, "particleCubeWidth", system->particleCubeWidth);
	SetShaderFloat(system->computeShader, "h", system->settings.h);
	SetShaderFloat(system->computeShader, "modelScale", system->modelScale);
	
	uint32_t numGroups = (system->particleCount + 127) / 128;
	glDispatchCompute(numGroups, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	debugParticles(system, "After Gen");
}

void SYS::destroyParticles(SPHsys* system)
{
	delete system->model;
	delete system->computeShader;
	delete system->sortShader;
	delete system->densityShader;
	delete system->forceShader;
	delete system->integrationShader;
	delete system->bitonicSortShader;
	delete system->cellBoundsShader;

	glDeleteBuffers(1, &system->computeBuffers.particleBuffer);
	glDeleteBuffers(1, &system->computeBuffers.matrixBuffer);
	glDeleteBuffers(1, &system->computeBuffers.cellParticleBuffer);
	glDeleteBuffers(1, &system->computeBuffers.cellBoundsBuffer);
}

void SYS::updateParticles(SPHsys* system)
{
	if (!system->started) return;

	uint32_t numGroups = (system->particleCount + 127) / 128;

	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, system->computeBuffers.cellBoundsBuffer);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
	

// get cell hashes
	UseCompShader(system->sortShader);
	SetShaderFloat(system->sortShader, "cellSize", system->settings.h);
	SetShaderVec3(system->sortShader, "bounds", system->settings.bounds);
	glDispatchCompute(numGroups, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS); 
	
	// Bitonic 
	uint32_t numElements = system->particleCount;
	uint32_t numStages = uint32_t(ceil(log2(numElements)));
	UseCompShader(system->bitonicSortShader);

	for (uint32_t stage = 1; stage <= numStages; ++stage) {
		for (uint32_t passStage = stage; passStage > 0; --passStage) {
			SetShaderUint(system->bitonicSortShader, "stage", stage);
			SetShaderUint(system->bitonicSortShader, "passStage", passStage);
			SetShaderUint(system->bitonicSortShader, "direction", 0);
			glDispatchCompute(numGroups, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
	}

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	
	UseCompShader(system->cellBoundsShader);
	glDispatchCompute(numGroups, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	
	debugParticles(system, "before density");

	//calc density
	UseCompShader(system->densityShader);
	SetShaderFloat(system->densityShader, "mass", system->settings.mass);
	SetShaderFloat(system->densityShader, "h", system->settings.h);
	SetShaderFloat(system->densityShader, "restDensity", system->settings.restDensity);
	SetShaderFloat(system->densityShader, "gasConst", system->settings.gasConst);
	glDispatchCompute(numGroups, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	debugParticles(system, "after density");

	debugParticles(system, "before force");
	//calc forces
	UseCompShader(system->forceShader);
	SetShaderFloat(system->forceShader, "mass", system->settings.mass);
	SetShaderFloat(system->forceShader, "h", system->settings.h);
	SetShaderFloat(system->forceShader, "viscosity", system->settings.viscosity);
	SetShaderFloat(system->forceShader, "tension", system->settings.tension);
	SetShaderFloat(system->forceShader, "g", system->settings.g);
	SetShaderFloat(system->forceShader, "deltaTime", system->settings.dt);   
	SetShaderFloat(system->forceShader, "pressureMultiplier", 500.0f);    
	SetShaderFloat(system->forceShader, "nearPressureMultiplier", 2.0f);  
	glDispatchCompute(numGroups, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	debugParticles(system, "after force");


	debugParticles(system, "before integration");
	// Integration
	UseCompShader(system->integrationShader);
	SetShaderFloat(system->integrationShader, "deltaTime", system->settings.dt);
	SetShaderFloat(system->integrationShader, "boundaryDamping", system->settings.boundaryDamping);
	SetShaderVec3(system->integrationShader, "bounds", system->settings.bounds);
	SetShaderFloat(system->integrationShader, "modelScale", system->modelScale);

	glDispatchCompute(numGroups, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	debugParticles(system, "after integrate");
}

void SYS::drawParticles(SPHsys* system, Shader* shader)
{
	UseShader(shader);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, system->computeBuffers.matrixBuffer);

	for (unsigned int i = 0; i < system->model->meshes.size(); i++)
	{
		glBindVertexArray(system->model->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES,
			system->model->meshes[i].indices.size(),
			GL_UNSIGNED_INT,
			0,
			system->particleCount);
	}


	glBindVertexArray(0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SYS::setupModelAttributes(SPHsys* system, Model* model)
{
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		Mesh* mesh = &model->meshes[i];

		glBindVertexArray(mesh->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, system->computeBuffers.matrixBuffer);

		for (int i = 0; i < 4; i++)
		{
			glEnableVertexAttribArray(3 + i);
			glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
				(void*)(sizeof(glm::vec4) * i));
			glVertexAttribDivisor(3 + i, 1);
		}
		glBindVertexArray(0);
	}
}


void SYS::debugParticles(SPHsys* system, const char* stage) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, system->computeBuffers.particleBuffer);
	Particle* particles = (Particle*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	if (particles) {
		std::cout << "\n=== " << stage << " ===\n";
		for (int i = 0; i < 5; i++) {
			std::cout << "Particle " << i << ":\n";
			std::cout << "  Position: " << particles[i].position.x << ", "
				<< particles[i].position.y << ", " << particles[i].position.z << "\n";
			std::cout << "  Velocity: " << particles[i].velocity.x << ", "
				<< particles[i].velocity.y << ", " << particles[i].velocity.z << "\n";
			std::cout << "  Density: " << particles[i].density << "\n";
			std::cout << "  Pressure: " << particles[i].pressure << "\n";
			std::cout << "  Force: " << particles[i].force.x << ", "
				<< particles[i].force.y << ", " << particles[i].force.z << "\n";
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
}
