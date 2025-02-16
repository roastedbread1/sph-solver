#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

struct Shader
{
	unsigned int ID;
};

void ReadAndCompileShader(Shader* shader, const char* vertexPath, const char* fragmentPath);
void UseShader(Shader* shader);
void SetShaderBool(Shader* shader, const std::string& name, bool value);
void SetShaderInt(Shader* shader, const std::string& name, int value);
void SetShaderFloat(Shader* shader, const std::string& name, float value);
void SetShaderVec3(Shader* shader, const std::string& name, glm::vec3& value);
void SetShaderVec3Scalar(Shader* shader, const std::string& name, float x, float y, float z);