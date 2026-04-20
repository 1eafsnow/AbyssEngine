#include "Light.h"

PointLight::PointLight() :
	viewTransform(Eigen::Matrix4f::Identity()),
	projectionTransform(Eigen::Matrix4f::Zero())
{
	float near = 0.01;
	float far = 1000;
	float fovyX = 90.0;
	float fovyY = 90.0;

	float left = -near * tan(TO_RADIANS(fovyX) / 2);
	float right = near * tan(TO_RADIANS(fovyX) / 2);
	float top = near * tan(TO_RADIANS(fovyY) / 2);
	float bottom = -near * tan(TO_RADIANS(fovyY) / 2);

	projectionTransform(0, 0) = near / right;
	projectionTransform(1, 1) = near / top;
	projectionTransform(2, 2) = -(far + near) / (far - near);
	projectionTransform(2, 3) = (-2 * far * near) / (far - near);
	projectionTransform(3, 2) = -1;

	projectionTransform = projectionTransform * CoordinateSystemProjection;
}

void PointLight::SetWorldLocation(const Eigen::Vector3f& location)
{
	Object::SetWorldLocation(location);
	viewTransform = TransformInverse(transform);
}

void PointLight::SetWorldRotation(const Eigen::Vector3f& rotation)
{
	Object::SetWorldRotation(rotation);
	viewTransform = TransformInverse(transform);
}

DirectionalLight::DirectionalLight() :
	viewTransform(Eigen::Matrix4f::Identity()),
	projectionTransform(Eigen::Matrix4f::Zero())
{
	float left = -100.0;
	float right = 100.0;
	float bottom = -100.0;
	float top = 100.0;
	float near = 1.0;
	float far = 100.0;	

	projectionTransform(0, 0) = 2 / (right - left);
	projectionTransform(1, 1) = 2 / (top - bottom);
	projectionTransform(2, 2) = -2 / (far - near);
	projectionTransform(0, 3) = -(right + left) / (right - left);
	projectionTransform(1, 3) = -(top + bottom) / (top - bottom);
	projectionTransform(2, 3) = -(far + near) / (far - near);
	projectionTransform(3, 3) = 1;

	projectionTransform = projectionTransform * CoordinateSystemProjection;
}

void DirectionalLight::SetWorldLocation(const Eigen::Vector3f& location)
{
	Object::SetWorldLocation(location);
	viewTransform = TransformInverse(transform);
}

void DirectionalLight::SetWorldRotation(const Eigen::Vector3f& rotation)
{
	Object::SetWorldRotation(rotation);
	viewTransform = TransformInverse(transform);
}