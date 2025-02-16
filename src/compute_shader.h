#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>


struct compShader
{
	unsigned int ID;
};

void ReadAndCompileCompShader(compShader* cShader, const char* computePath);
void checkCompileErrors(GLuint shader, std::string type);
void UseCompShader(compShader* cShader);
void SetCompShaderBool(compShader* cShader, const std::string& name, bool value);
void SetCompShaderInt(compShader* cShader, const std::string& name, int value);
void SetCompShaderFloat(compShader* cShader, const std::string& name, float value);
void SetCompShaderVec3(compShader* cShader, const std::string& name, glm::vec3& value);
void SetCompShaderVec3Scalar(compShader* cShader, const std::string& name, float x, float y, float z);