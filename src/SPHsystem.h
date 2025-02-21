#include "model.h"
#include "particle.h"


namespace SYS
{


	struct ComputeBuffers
	{
		GLuint particleBuffer;
		GLuint matrixBuffer;
		GLuint cellParticleBuffer;
		GLuint cellBoundsBuffer;
	};

	struct SPHSettings
	{
		float mass; 
		float restDensity; 
		float gasConst; 
		float viscosity; 
		float h; 
		float g; 
		float tension;
		//moved to compute shader
		//float massPoly6Product;
		//float h2;
		//float selfDens;
		//float poly6;
		//float spikyGrad;
		//float spikyLap;


		glm::mat4 sphereScale;

		float dt;
		float boundaryDamping;
		glm::vec3 bounds;
		float cellSize;
	};

	

	struct SPHsys
	{
		SPHSettings settings;
		size_t particleCubeWidth;

		GLuint instanceSSBO;
		
		bool started;
		bool keyPressed;
		Particle* particles;

		size_t particleCount;
		

		Model* model;
		glm::mat4* sphereModelMatrix;

		float modelScale;
		//compute
		Shader* computeShader;
		ComputeBuffers computeBuffers;

		Shader* sortShader;
		Shader* densityShader;
		Shader* forceShader;
		Shader* integrationShader;
		Shader* bitonicSortShader;
		Shader* cellBoundsShader;
		Shader* updateShader;
	};

	void initSettings(SPHSettings* settings, float mass, float restDensity, float gasConst,
		float viscosity, float h, float g, float tension, float cellSize,
		const glm::vec3& bounds, float dt, float boundaryDamping);

	void initParticles(SPHsys* system, SYS::SPHSettings &settings, size_t particleCubeWidth);

	void genParticles(SPHsys* system);

	void destroyParticles(SPHsys* system);

	void updateParticles(SPHsys* system);

	void drawParticles(SPHsys* system, Shader* shader);

	void setupModelAttributes(SPHsys* system, Model* model);

	void debugParticles(SPHsys* system, const char* stage);
	
}