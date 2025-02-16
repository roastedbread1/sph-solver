#pragma once
#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};


struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO, VBO, EBO;
};

void initMesh(Mesh* mesh, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector< Texture> textures);
void DrawMesh(Mesh* mesh, Shader& shader);

void setupMesh(Mesh* mesh);

void DrawInstancedMesh(Mesh* mesh, Shader& shader, int instanceCount);
