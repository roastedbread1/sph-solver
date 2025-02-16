#include "shader.h"
void ReadAndCompileShader(Shader* shader, const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "error::shader::file not successfully read\n";
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//compiling shader

	unsigned int vertex, fragment;

	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "vertex shader compilation failed\n" << infoLog << "\n";
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);\
		glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "fragment shader compilation failed\n" << infoLog << "\n";
	}

	//shader program

	shader->ID = glCreateProgram();
	glAttachShader(shader->ID, vertex);
	glAttachShader(shader->ID, fragment);
	glLinkProgram(shader->ID);

	glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader->ID, 512, NULL, infoLog);
		std::cout << "shader program linking failed" << infoLog << "\n";

	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void UseShader(Shader* shader)
{
	glUseProgram(shader->ID);
}

void SetShaderBool(Shader* shader, const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), (int)value);
}

void SetShaderInt(Shader* shader, const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), value);
}

void SetShaderFloat(Shader* shader, const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(shader->ID, name.c_str()), value);
}

void SetShaderVec3(Shader* shader, const std::string& name, glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shader->ID, name.c_str()), 1, &value[0]);
}

void SetShaderVec3Scalar(Shader* shader, const std::string& name, float x, float y, float z)
{

	glUniform3f(glGetUniformLocation(shader->ID, name.c_str()), x, y, z);
}
