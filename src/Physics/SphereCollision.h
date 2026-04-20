#pragma once
#include "Collision.h"

class SphereCollision : public Collision
{
public:
	float radius;

	SphereCollision(const float& radius);
	SphereCollision(const CollisionInitParameters& parameters, const float& radius);

	virtual void Init() override;
};