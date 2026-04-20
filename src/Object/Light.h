#pragma once
#include "Object/Object.h"

class PointLight : public Object
{
public:
	Eigen::Vector3f color = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;

	Eigen::Matrix4f viewTransform;
	Eigen::Matrix4f projectionTransform;

	PointLight();

	virtual void SetWorldLocation(const Eigen::Vector3f& location) override;
	virtual void SetWorldRotation(const Eigen::Vector3f& rotation) override;
};

class DirectionalLight : public Object
{
public:
	Eigen::Vector3f color = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;

	Eigen::Matrix4f viewTransform;
	Eigen::Matrix4f projectionTransform;

	DirectionalLight();
	virtual void SetWorldLocation(const Eigen::Vector3f& location) override;
	virtual void SetWorldRotation(const Eigen::Vector3f& rotation) override;
};