#pragma once

#include "helper.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace Geo {


	struct Geometry
	{
		std::vector<glm::vec3> points;
		std::vector<glm::vec3> normals;
		std::vector<glm::uvec3> triangles;
		std::vector<glm::uvec3> normalIndices;
	};


	void init();
	void draw();
	std::vector<unsigned int> indices;
	gluint vao, vbo, vbo_n, ebo;
}
