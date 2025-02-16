#pragma once


#include "helper.h"

namespace SPHSHADER
{
	struct ShaderSPH
	{
		enum ShaderType {eGeometry, eVertex, eFragment, eCompute};
		ShaderType type;
		unsigned int ID;

		unsigned int GetProgramID() const { return ID; };
	};

	void InitShaderSPH(ShaderSPH* shaderSPH, const char* filename, ShaderSPH::ShaderType type);
	void DestroyShaderSPH(ShaderSPH* shaderSPH);





	struct ShaderSPHProgram
	{
		enum ProgramType {eGeometry, eRender, eCompute};
		enum {eMaxShaders = 3};
		unsigned int programID;
		ProgramType type;
		ShaderSPH* shadersSPH[eMaxShaders];
		unsigned int GetProgramID() const { return programID; }
	};

	void InitShaderSPHProgram(ShaderSPHProgram* shaderSPHProgram, ShaderSPHProgram::ProgramType type, const char* filename);
	void DestroyShaderSPHProgram(ShaderSPHProgram* shaderSPHProgram);

}



