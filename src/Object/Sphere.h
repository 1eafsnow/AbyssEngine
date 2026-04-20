#pragma once
#include "Object/Mesh.h"

class Sphere : public Mesh
{
public:
	float radius;
	int xSegments;
	int ySegments;

	Sphere(const std::string& name, const float& radius, const int& xSegments, const int& ySegments);
	Sphere(const std::string& name, const float& radius, const int& xSegments, const int& ySegments, const CollisionInitParameters& parameters);
	void Init();
};