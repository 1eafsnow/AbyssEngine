#pragma once
#include <vector>
#include <cstdarg>
#include <iostream>
#include "Object/Object.h"
#include "Material/Material.h"

class Vertex
{
public:
	Eigen::Vector3f location;
	Eigen::Vector3f normal;
	Eigen::Vector2f texture;

	Vertex();
	void SetLocation(const Eigen::Vector3f& location);
	void SetNormal(const Eigen::Vector3f& normal);
	void SetTexture(const Eigen::Vector2f& texture);
};

template <int Size>
class Face
{
public:
	int indices[Size];
	Face(int index, ...)
	{
		va_list args;
		va_start(args, Size);
		for (int i = 0; i < Size; i++)
		{
			indices[i] = va_arg(args, int);
		}
		va_end(args);
	}

	int& operator[](int i)
	{
		return indices[i];
	}
};

using Triangle = Face<3>;
using Quadrilateral = Face<4>;
using Quad = Quadrilateral;

class Mesh : public Object
{
public:
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;
	std::vector<Quad> quads;

	Collision* collision;
	Material* material;

	virtual void SetWorldLocation(const Eigen::Vector3f& location) override;
	virtual void SetCollision(CollisionType collisionType);

	virtual void Tick(const float& deltaTime) override;
};