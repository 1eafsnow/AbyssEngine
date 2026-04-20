#include "Collision.h"
#include "World/World.h"

Collision::Collision()
{
	Init();
}

Collision::Collision(const CollisionInitParameters& parameters)
{
	type = parameters.type;
	mass = parameters.mass;
	restitution = parameters.restitution;
	transform.setOrigin(btVector3(parameters.location.x(), parameters.location.y(), parameters.location.z()));
	transform.setRotation(btQuaternion(parameters.rotation.x(), parameters.rotation.y(), parameters.rotation.z()));
}

Eigen::Vector3f Collision::GetWorldLocation()
{
	return Eigen::Vector3f(float(transform.getOrigin().getX()),
		float(transform.getOrigin().getY()),
		float(transform.getOrigin().getZ()));
}

void Collision::SetWorldLocation(const Eigen::Vector3f& location)
{		
	btTransform newTransform = transform;
	newTransform.setOrigin(btVector3(location.x(), location.y(), location.z()));
	body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	body->setWorldTransform(newTransform);

	body->setInterpolationWorldTransform(body->getWorldTransform());
	body->setInterpolationLinearVelocity(body->getLinearVelocity());
	body->setInterpolationAngularVelocity(body->getAngularVelocity());
	
	body->clearForces();
	body->activate();
	
	transform = newTransform;
}

void Collision::SetMass(const float& mass)
{
	this->mass = mass;
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		shape->calculateLocalInertia(this->mass, localInertia);
	}
	body->setMassProps(this->mass, localInertia);
	body->updateInertiaTensor();
	std::cout << "Object: " << name << "Set mass to " << this->mass << std::endl;
}

void Collision::SetRestitution(const float& restitution)
{
	this->restitution = restitution;
	body->setRestitution(this->restitution);
}

void Collision::Activate()
{
	GetPhysics()->Add(this);
}

void Collision::Deactivate()
{
	GetPhysics()->Remove(this);
}

void Collision::Tick(const float& deltaTime)
{
	if (type == CollisionType::Dynamic && body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(transform);
	}
	//printf("world pos object = %f,%f,%f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
}