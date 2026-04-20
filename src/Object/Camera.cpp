#include "Camera.h"

Camera::Camera(const float& near, const float& far, const float& fovyX, const float& fovyY) : 
	near(near), far(far), fovyX(fovyX), fovyY(fovyY), 
	viewTransform(Eigen::Matrix4f::Identity()),
	projectionTransform(Eigen::Matrix4f::Zero())
{
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

void Camera::SetWorldLocation(const Eigen::Vector3f& location)
{
	Object::SetWorldLocation(location);
	viewTransform = TransformInverse(transform);
}

void Camera::SetWorldRotation(const Eigen::Vector3f& rotation)
{
	Object::SetWorldRotation(rotation);
	viewTransform = TransformInverse(transform);
}

void Camera::Move(const Eigen::Vector3f& direction)
{
	AddLocalLocation(direction * 0.01f);
	viewTransform = TransformInverse(transform);
}

void Camera::Look(const Eigen::Vector3f& rotation)
{
	AddLocalRotation(rotation * 0.1f);
	viewTransform = TransformInverse(transform);
}