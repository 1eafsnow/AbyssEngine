#include "BoxCollision.h"

BoxCollision::BoxCollision(const float& length, const float& width, const float& height) : 
	length(length), width(width), height(height)
{
	Init();
}

BoxCollision::BoxCollision(const CollisionInitParameters& parameters, const float& length, const float& width, const float& height) : 
	length(length), width(width), height(height), Collision(parameters)
{
	Init(); 
	
	if (type != CollisionType::NoCollision)
	{
		Activate();
	}
}

void BoxCollision::Init()
{
	shape = new btBoxShape(btVector3(btScalar(length), btScalar(width), btScalar(height)));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	body = new btRigidBody(rbInfo);
	body->setRestitution(restitution);
}