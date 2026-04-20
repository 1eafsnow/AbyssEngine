#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() : 
	path(""),
	tbo(0),
	width(0),
	height(0),
	channels(0)
{

}

Texture::Texture(const char* path) : 
	path(""),
	tbo(0),
	width(0),
	height(0),
	channels(0)
{
	Load(path);
}

void Texture::Load(const char* path)
{
	this->path = path;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data)
	{
		glGenTextures(1, &tbo);
		glBindTexture(GL_TEXTURE_2D, tbo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (channels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			printf("Texture format not supported: %s\n", path);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		printf("Failed to load texture: %s\n", path);
	}
}