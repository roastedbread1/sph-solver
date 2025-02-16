#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <string>
#include <iostream>

struct Image
{
	int width;
	int height;
	int channels;
	unsigned char* data;
};

void LoadImage(Image* image, const std::string& name);
void CreateTexture(Image* image, unsigned int* texture, unsigned int id, GLenum format);
void FreeImage(Image* image);