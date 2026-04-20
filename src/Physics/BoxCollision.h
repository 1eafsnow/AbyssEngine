#pragma once
#include "Collision.h"

class BoxCollision : public Collision
{
public:
	float length;
	float width;
	float height;

	BoxCollision(const float& length, const float& width, const float& height);
	BoxCollision(const CollisionInitParameters& parameters, const float& length, const float& width, const float& height);

	virtual void Init() override;
};