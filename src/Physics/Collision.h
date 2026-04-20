#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "btBulletDynamicsCommon.h"

enum class CollisionType
{
	NoCollision = 0,
	Static,
	Dynamic,
	Physics
};

struct CollisionInitParameters
{
	CollisionType type = CollisionType::NoCollision;
	btScalar mass = 0.0f;
	btScalar restitution = 0.0f;
	Eigen::Vector3f location = Eigen::Vector3f::Zero();
	Eigen::Vector3f rotation = Eigen::Vector3f::Zero();
};

class Collision
{
public:
	std::string name;
	CollisionType type = CollisionType::NoCollision;
	btScalar mass = 0.0f;
	btTransform transform = btTransform::getIdentity();
	btCollisionShape* shape;
	btVector3 localInertia = btVector3(0.0f, 0.0f, 0.0f);
	btDefaultMotionState* motionState;
	btRigidBody* body;

	btScalar restitution;

	//Eigen::Vector3f location;
	//Eigen::Quaternionf quaternion;

	Collision();
	Collision(const CollisionInitParameters& parameters);
	~Collision() {};

	virtual void Init() {};
	virtual Eigen::Vector3f GetWorldLocation();
	virtual void SetWorldLocation(const Eigen::Vector3f& location);
	virtual void SetMass(const float& mass);
	virtual void SetRestitution(const float& restitution);

	void Activate();
	void Deactivate();

	virtual void Tick(const float& deltaTime);
};


