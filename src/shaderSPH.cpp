#include "helper.h"
#include "shaderSPH.h"

#include <string>
#include <vector>
#include <fstream>

#include <glad/glad.h>
void SPHSHADER::InitShaderSPH(ShaderSPH* shaderSPH, const char* filename, ShaderSPH::ShaderType type)
{

	std::string fileData = "";
	std::fstream stream(filename, std::ios::in);

	if (!stream.is_open())
	{
		printf("ERROR: Can't open shader file '%s'\n", filename);
		shaderSPH->ID = 0;
		return;
	}




	std::string line = "";
	while (getline(stream, line))
	{
		fileData += "\n" + line;
	}

	stream.close();

	std::string shaderSource;

	switch (type)
	{
	case ShaderSPH::eGeometry:
		shaderSource = "#define GEOMETRY_SHADER\n" + fileData;
		shaderSPH->ID = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case ShaderSPH::eVertex:
		shaderSource = "#define VERTEX_SHADER\n" + fileData;
		shaderSPH->ID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderSPH::eFragment:
		shaderSource = "#define FRAGMENT_SHADER\n" + fileData;
		shaderSPH->ID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case ShaderSPH::eCompute:
		shaderSource = "#define COMPUTE_SHADER\n" + fileData;
		shaderSPH->ID = glCreateShader(GL_COMPUTE_SHADER);
		break;
	}

	shaderSource = "#version 430\n" + shaderSource;

	const char* rawShaderSource = shaderSource.c_str();
	glShaderSource(shaderSPH->ID, 1, &rawShaderSource, NULL);
	glCompileShader(shaderSPH->ID);

	GLint isCompiled = 0;

	glGetShaderiv(shaderSPH->ID, GL_INFO_LOG_LENGTH, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		printf("ERROR: Can't compile shader '%s'\n", filename);

		GLint maxLength = 0;

		glGetShaderiv(shaderSPH->ID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderSPH->ID, maxLength, &maxLength, &errorLog[0]);
		printf("GL ERRORS:\n%s\n", &errorLog[0]);

		// Delete shader
		glDeleteShader(shaderSPH->ID);
		shaderSPH->ID = 0;
			
	}

}


void SPHSHADER::DestroyShaderSPH(ShaderSPH* shaderSPH)
{
	glDeleteShader(shaderSPH->ID);
}

void SPHSHADER::InitShaderSPHProgram(ShaderSPHProgram* shaderSPHProgram, ShaderSPHProgram::ProgramType type, const char* filename)
{
	shaderSPHProgram->type = type;
	for (int i = 0; i < shaderSPHProgram->eMaxShaders; i++)
	{
		shaderSPHProgram->shadersSPH[i] = nullptr;
	}


	if (type == ShaderSPHProgram::eGeometry)
	{
		shaderSPHProgram->shadersSPH[0] = new ShaderSPH;
		InitShaderSPH(shaderSPHProgram->shadersSPH[0], filename, ShaderSPH::eVertex);
		shaderSPHProgram->shadersSPH[1] = new ShaderSPH;
		InitShaderSPH(shaderSPHProgram->shadersSPH[1], filename, ShaderSPH::eGeometry);

		shaderSPHProgram->shadersSPH[2] = new ShaderSPH;
		InitShaderSPH(shaderSPHProgram->shadersSPH[2], filename, ShaderSPH::eFragment);
	}
	else if (type == ShaderSPHProgram::eRender)
	{
		shaderSPHProgram->shadersSPH[0] = new ShaderSPH;
		InitShaderSPH(shaderSPHProgram->shadersSPH[0], filename, ShaderSPH::eVertex);

		shaderSPHProgram->shadersSPH[1] = new ShaderSPH;
		InitShaderSPH(shaderSPHProgram->shadersSPH[1], filename, ShaderSPH::eFragment);
	}
	else if (type == ShaderSPHProgram::eCompute)
	{
		shaderSPHProgram->shadersSPH[0] = new ShaderSPH;
		InitShaderSPH(shaderSPHProgram->shadersSPH[0], filename, ShaderSPH::eCompute);
	}

	shaderSPHProgram->programID = glCreateProgram();


	for (int i = 0; i < shaderSPHProgram->eMaxShaders; i++)
	{
		if (shaderSPHProgram->shadersSPH[i] != nullptr)
		{
			glAttachShader(shaderSPHProgram->programID, shaderSPHProgram->shadersSPH[i]->ID);
		}
	}
	glLinkProgram(shaderSPHProgram->programID);


	///////////////error checking///////////////////
	GLint isLinked = 0;
	glGetProgramiv(shaderSPHProgram->programID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		printf("ERROR: Failed to link shader program\n");

		// Print error message
		GLint maxLength = 0;
		glGetProgramiv(shaderSPHProgram->programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(shaderSPHProgram->programID, maxLength, &maxLength, &errorLog[0]);
		printf("GL ERRORS:\n%s\n", &errorLog[0]);

		// Clean up
		glDeleteProgram(shaderSPHProgram->programID);
		for (int i = 0; i < ShaderSPHProgram::eMaxShaders; i++)
		{
			if (shaderSPHProgram->shadersSPH[i] != nullptr)
			{
				glDeleteShader(shaderSPHProgram->shadersSPH[i]->ID);
				delete shaderSPHProgram->shadersSPH[i];
			}
		}
		shaderSPHProgram->programID = 0;
	}
}

void SPHSHADER::DestroyShaderSPHProgram(ShaderSPHProgram* shaderSPHProgram)
{

	if (shaderSPHProgram->programID != 0)
	{
		// Detach 
		for (int i = 0; i < ShaderSPHProgram::eMaxShaders; i++)
		{
			if (shaderSPHProgram->shadersSPH[i] != nullptr)
			{
				glDetachShader(shaderSPHProgram->programID, shaderSPHProgram->shadersSPH[i]->ID);
				glDeleteShader(shaderSPHProgram->shadersSPH[i]->ID);
				delete shaderSPHProgram->shadersSPH[i];
			}
		}

		glDeleteProgram(shaderSPHProgram->programID);
		shaderSPHProgram->programID = 0;
	}
}
