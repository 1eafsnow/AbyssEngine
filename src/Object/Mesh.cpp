#include "Mesh.h"

Vertex::Vertex() : 
	location(Eigen::Vector3f(0.0f, 0.0f, 0.0f)), normal(Eigen::Vector3f(0.0f, 0.0f, 0.0f)), texture(Eigen::Vector2f(0.0f, 0.0f))
{

}

void Vertex::SetLocation(const Eigen::Vector3f& location)
{
	this->location = location;
}

void Vertex::SetNormal(const Eigen::Vector3f& normal)
{
	this->normal = normal;
}

void Vertex::SetTexture(const Eigen::Vector2f& texture)
{
	this->texture = texture;
}

void Mesh::SetWorldLocation(const Eigen::Vector3f& location)
{
	Object::SetWorldLocation(location);
	collision->SetWorldLocation(translation);
}

void Mesh::SetCollision(CollisionType collisionType)
{
	collision->type = collisionType;
	switch (collision->type)
	{
	case CollisionType::NoCollision:
		collision->Deactivate();
		std::cout << name << ": Set collision (NoCollision)" << std::endl;
		break;
	case CollisionType::Static:
		collision->SetMass(0.0f);
		collision->SetWorldLocation(translation);
		collision->Activate();
		std::cout << name << ": Set collision (Static)" << std::endl;
		break;
	case CollisionType::Dynamic:
		collision->SetMass(1.0f);
		collision->SetWorldLocation(translation);
		collision->Activate();
		std::cout << name << ": Set collision (Dynamic)" << std::endl;
		break;
	case CollisionType::Physics:
		collision->SetMass(1.0f);
		collision->SetWorldLocation(translation);
		collision->Activate();
		std::cout << name << ": Set collision (Physics)" << std::endl;
		break;
	default:
		break;
	}
}


void Mesh::Tick(const float& deltaTime)
{
	switch (collision->type)
	{
	case CollisionType::NoCollision:
		break;
	case CollisionType::Static:
		break;
	case CollisionType::Dynamic:
		translation = collision->GetWorldLocation();
		transform.col(3).head<3>() = translation;
		break;
	case CollisionType::Physics:
		translation = collision->GetWorldLocation();
		transform.col(3).head<3>() = translation;
		break;
	default:
		break;
	}
}