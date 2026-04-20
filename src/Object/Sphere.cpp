#include "Sphere.h"
#include "Physics/SphereCollision.h"
#include "World/World.h" 

Sphere::Sphere(const std::string& name, const float& radius, const int& xSegments, const int& ySegments) : 
	radius(radius), xSegments(xSegments), ySegments(ySegments)
{
	this->name.assign(name);
	Init();

	collision = new SphereCollision(radius);
	collision->name.assign(name).append("'s collision");
	collision->type = CollisionType::NoCollision;
	collision->mass = 0.0f;
	collision->SetWorldLocation(translation);

	material = new Material;
}

Sphere::Sphere(const std::string& name, const float& radius, const int& xSegments, const int& ySegments, const CollisionInitParameters& parameters)	: 
	radius(radius), xSegments(xSegments), ySegments(ySegments)
{
	this->name.assign(name);
	Init();

	collision = new SphereCollision(parameters, radius);
	collision->name.assign(name).append("'s collision");

	material = new Material;
}

void Sphere::Init()
{
	const float HalfPI = float(EIGEN_PI / 2.0);
	float dYaw = 2.0f * EIGEN_PI / float(xSegments);
	float dPitch = EIGEN_PI / float(ySegments);
	for (int i = 0; i <= xSegments; ++i)
	{
		float yaw = i * dYaw;
		float U = (float)i / (float)xSegments;
		for (int j = 0; j <= ySegments; ++j)
		{
			float pitch = -HalfPI - j * dPitch;
			float V = (float)j / (float)ySegments;
			float x = radius * cos(pitch) * cos(yaw);
			float y = radius * cos(pitch) * sin(yaw);
			float z = radius * sin(pitch);
			Eigen::Vector3f location(x, y, z);

			Vertex vertex;
			vertex.SetLocation(location);
			vertex.SetNormal(location.normalized());
			vertex.SetTexture({ U, V });
			vertices.push_back(vertex);
		}
	}

	for (int i = 0; i < xSegments; ++i)
	{
		for (int j = 0; j < ySegments; ++j)
		{
			triangles.push_back(Triangle(i * (ySegments + 1) + j, (i + 1) * (ySegments + 1) + j, i * (ySegments + 1) + j + 1));
			triangles.push_back(Triangle(i * (ySegments + 1) + j + 1, (i + 1) * (ySegments + 1) + j, (i + 1) * (ySegments + 1) + j + 1));
			//std::cout << triangles.back().indices[0] << ", " << triangles.back().indices[1] << ", " << triangles.back().indices[2] << std::endl;
		}
	}
}