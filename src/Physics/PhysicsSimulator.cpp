#include "PhysicsSimulator.h"

PhysicsSimulator::PhysicsSimulator()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, GRAVITY));
}

float PhysicsSimulator::GetGravity()
{
	return dynamicsWorld->getGravity().z();
}

void PhysicsSimulator::SetGravity(const float& gravity)
{
	dynamicsWorld->setGravity(btVector3(0, 0, gravity));
}

void PhysicsSimulator::Add(Collision* collision)
{
	auto it = std::find(collisions.begin(), collisions.end(), collision);
	if (it != collisions.end())
	{
		return;
	}
	collisions.push_back(collision);
	dynamicsWorld->addRigidBody(collision->body);
}

void PhysicsSimulator::Remove(Collision* collision)
{	
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{		
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (collision->body == body)
		{
			dynamicsWorld->removeCollisionObject(dynamicsWorld->getCollisionObjectArray()[i]);
			auto it = std::find(collisions.begin(), collisions.end(), collision);
			if (it != collisions.end())
			{
				collisions.erase(it);
			}
			break;
		}
	}
}

void PhysicsSimulator::Tick(float deltaTime)
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	for (auto& collision : collisions)
	{
		collision->Tick(deltaTime);
		//std::cout << collision->name << ": " << collision->GetWorldLocation().transpose() << std::endl;
	}

	//print positions of all objects
	/*
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		
		//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}
	*/
}