#include "compute_shader.h"



void ReadAndCompileCompShader(compShader* cShader, const char* computePath)
{
	std::string computeCode;

	std::ifstream cShaderFile;

	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		cShaderFile.open(computePath);

		std::stringstream cShaderStream;
		cShaderStream << cShaderFile.rdbuf();

		cShaderFile.close();

		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure &e)
	{
		std::cout << "compute shader file not successfully read" << e.what() << std::endl;
	}
	const char* cShaderCode = computeCode.c_str();


	unsigned int compute;
	compute = glCreateShader(GL_COMPUTE_SHADER);


	glShaderSource(compute, 1, &cShaderCode, NULL);

	glCompileShader(compute);
	checkCompileErrors(compute, "COMPUTE");

	cShader->ID = glCreateProgram();
	glAttachShader(cShader->ID, compute);
	checkCompileErrors(cShader->ID, "PROGRAM");

	glDeleteShader(compute);
}


void checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void UseCompShader(compShader* cShader)
{
	glUseProgram(cShader->ID);
}

void SetCompShaderBool(compShader* cShader, const std::string& name, bool value)
{
}

void SetCompShaderInt(compShader* cShader, const std::string& name, int value)
{
}

void SetCompShaderFloat(compShader* cShader, const std::string& name, float value)
{
}

void SetCompShaderVec3(compShader* cShader, const std::string& name, glm::vec3& value)
{
}

void SetCompShaderVec3Scalar(compShader* cShader, const std::string& name, float x, float y, float z)
{
}
