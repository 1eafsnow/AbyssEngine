#pragma once
#include <stdint.h>
#include <string>
#include <GL/glew.h>

class Texture
{
public:
	std::string path;
	uint32_t tbo;
	int width, height, channels;

	Texture();
	Texture(const char* path);

	void Load(const char* path);
};