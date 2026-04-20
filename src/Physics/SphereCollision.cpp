#include "SphereCollision.h"

SphereCollision::SphereCollision(const float& radius) : 
	radius(radius)
{
	Init();
}

SphereCollision::SphereCollision(const CollisionInitParameters& parameters, const float& radius) : 
	radius(radius), Collision(parameters)
{
	Init();

	if (type != CollisionType::NoCollision)
	{
		Activate();
	}
}

void SphereCollision::Init()
{
	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	shape = new btSphereShape(btScalar(radius));
	//collisionShapes.push_back(colShape);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, shape, localInertia);
	body = new btRigidBody(rbInfo);
	body->setRestitution(restitution);
}