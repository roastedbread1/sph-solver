#include "helper.h"

#include "model.h"
#include "particle.h"
#include "compute_shader.h"


namespace SYS
{
	struct SPHSettings
	{
		float mass; 
		float restDensity; 
		float gasConst; 
		float viscosity; 
		float h; 
		float g; 
		float tension;
		float massPoly6Product;
		float h2;
		float selfDens;
		float poly6;
		float spikyGrad;
		float spikyLap;


		glm::mat4 sphereScale;
	};

	

	struct SPHsys
	{
		SPHSettings settings;
		size_t particleCubeWidth;

		GLuint instanceSSBO;
		
		bool started;
		Particle* particles;

		size_t particleCount;
		

		Model* model;
		glm::mat4* sphereModelMatrix;


		//compute
		compShader particleGenbuffer;
		GLuint particleBuffer;
		
	};


	void initSettings(SPHSettings* settings, float mass,float restDensity,float gasConst, float visocity,float h,float g,float tension);

	void initParticles(SPHsys* system, SYS::SPHSettings &settings, size_t particleCubeWidth);

	void genParticles(SPHsys* system);

	void destroyParticles(SPHsys* system);

	void updateParticles(SPHsys* system);

	void drawParticles(SPHsys* system, Shader* shader);

}