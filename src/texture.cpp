#include "texture.h"
void LoadImage(Image* image, const std::string& name)
{
	stbi_set_flip_vertically_on_load(true);
	image->data = stbi_load(name.c_str(), &image->width, &image->height, &image->channels, 0);

	if (!image->data)
	{
		std::cout << "failed to load image\n";
	}

}

void CreateTexture(Image* image, unsigned int* texture, unsigned int id, GLenum format)
{
	glGenTextures(id, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);
	glGenerateMipmap(GL_TEXTURE_2D);
	FreeImage(image);
}
void FreeImage(Image* image)
{
	stbi_image_free(image->data);
}
