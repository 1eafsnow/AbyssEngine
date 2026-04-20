#pragma once
#include <stdio.h>
#include <iostream>
#include "btBulletDynamicsCommon.h"
#include "Collision.h"

#define GRAVITY -9.81

class PhysicsSimulator
{
public:
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<Collision*> collisions;

	PhysicsSimulator();
	float GetGravity();
	void SetGravity(const float& gravity);
	void Add(Collision* collision);
	void Remove(Collision* collision);

	void Tick(float deltaTime);
};