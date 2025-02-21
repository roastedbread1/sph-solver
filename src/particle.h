#pragma once
#include <glm/glm.hpp>

struct Particle {
    glm::vec3 position;
    float density;  
    glm::vec3 velocity;
    float pressure; 
    glm::vec3 force;
    float padding;  
};

struct CellParticle
{
	uint64_t key;
	uint32_t index;
};

