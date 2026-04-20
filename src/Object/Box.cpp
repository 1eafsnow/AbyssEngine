#include "Box.h"
#include "Physics/BoxCollision.h"
#include "World/World.h" 

Box::Box(const std::string& name, const float& length, const float& width, const float& height) : 
	length(length), width(width), height(height)
{
	this->name.assign(name);
	Init();

	collision = new BoxCollision(length, width, height);
	collision->name.assign(name).append("'s collision");
	collision->type = CollisionType::NoCollision;
	collision->mass = 0.0f;
	collision->SetWorldLocation(translation);

	material = new Material;
}

Box::Box(const std::string& name, const float& length, const float& width, const float& height, const CollisionInitParameters& parameters) : 
	length(length), width(width), height(height)
{
	this->name.assign(name);
	Init();

	collision = new BoxCollision(parameters, length, width, height);
	collision->name.assign(name).append("'s collision");

	material = new Material;
}

void Box::Init()
{
	float halfL = length / 2;
	float halfW = width / 2;
	float halfH = height / 2;

	Vertex v;

	//Front face
	v.SetNormal(Eigen::Vector3f(0.0f, 1.0f, 0.0f));
	v.SetLocation(Eigen::Vector3f(-halfW, halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, halfL, -halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(-halfW, halfL, -halfH));
	vertices.push_back(v);
	quads.push_back(Quad(0, 1, 2, 3));

	//Back face
	v.SetNormal(Eigen::Vector3f(0.0f, -1.0f, 0.0f));
	v.SetLocation(Eigen::Vector3f(-halfW, -halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, -halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, -halfL, -halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(-halfW, -halfL, -halfH));
	vertices.push_back(v);
	quads.push_back(Quad(4, 5, 6, 7));

	//Top face
	v.SetNormal(Eigen::Vector3f(0.0f, 0.0f, 1.0f));
	v.SetLocation(Eigen::Vector3f(-halfW, halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, -halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(-halfW, -halfL, halfH));
	vertices.push_back(v);
	quads.push_back(Quad(8, 9, 10, 11));

	//Bottom face
	v.SetNormal(Eigen::Vector3f(0.0f, 0.0f, -1.0f));
	v.SetLocation(Eigen::Vector3f(-halfW, halfL, -halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, halfL, -halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, -halfL, -halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(-halfW, -halfL, -halfH));
	vertices.push_back(v);
	quads.push_back(Quad(12, 13, 14, 15));

	//Left face
	v.SetNormal(Eigen::Vector3f(-1.0f, 0.0f, 0.0f));
	v.SetLocation(Eigen::Vector3f(-halfW, -halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(-halfW, halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(-halfW, halfL, -halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(-halfW, -halfL, -halfH));
	vertices.push_back(v);
	quads.push_back(Quad(16, 17, 18, 19));

	//Right face
	v.SetNormal(Eigen::Vector3f(1.0f, 0.0f, 0.0f));
	v.SetLocation(Eigen::Vector3f(halfW, -halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, halfL, halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, halfL, -halfH));
	vertices.push_back(v);
	v.SetLocation(Eigen::Vector3f(halfW, -halfL, -halfH));
	vertices.push_back(v);
	quads.push_back(Quad(20, 21, 22, 23));
}