#pragma once
#include "Object/Object.h"

class Camera : public Object
{
public:
	float focus;
	float near;
	float far;
	float fovyX;
	float fovyY;

	Eigen::Matrix4f viewTransform;
	Eigen::Matrix4f projectionTransform;

	Camera(const float& near, const float& far, const float& fovyX, const float& fovyY);

	virtual void SetWorldLocation(const Eigen::Vector3f& location) override;
	virtual void SetWorldRotation(const Eigen::Vector3f& rotation) override;

	void Move(const Eigen::Vector3f& direction);
	void Look(const Eigen::Vector3f& rotation);
};