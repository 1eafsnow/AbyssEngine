#pragma once
#include "Object/Mesh.h"

class Box : public Mesh
{
public:
	float length;
	float width;
	float height;

	Box(const std::string& name, const float& length, const float& width, const float& height);
	Box(const std::string& name, const float& length, const float& width, const float& height, const CollisionInitParameters& parameters);
	void Init();
};