#pragma once
#include <glad/glad.h>
#include "texture.h"

#include <assimp/Importer.hpp> //include this first otherwise some funky stuff happens
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include  <string>
#include "mesh.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

extern std::vector<Texture> textures_loaded;

struct Model
{
	std::vector<Mesh> meshes;
	std::string directory;
};

void LoadModel(Model* model, std::string path);

void ProcessNode(Model* model, aiNode* node, const  aiScene* scene);

//Mesh ProcessMesh(Model* model,aiMesh* mesh, const  aiScene* scene);
void ProcessMesh(std::string directory, Mesh* mesh, aiMesh* ai_mesh, const aiScene* scene);

std::vector<Texture> LoadMaterialTextures(std::string directory, aiMaterial* mat, aiTextureType type, std::string typeName);
void DrawModel(Model* model, Shader& shader);

void DrawInstancedModel(Model* model, Shader& shader, int instanceCount);

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);


