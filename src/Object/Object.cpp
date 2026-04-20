#include "Object.h"

Object::Object() : 
	translation(0.0f, 0.0f, 0.0f), quaternion(0.0f, 0.0f, 0.0f, 0.0f), transform(Eigen::Matrix4f::Identity()),
	forward(OriginForward), rightward(OriginRightward), upward(OriginUpward)
{

}

Eigen::Vector3f Object::GetWorldLocation()
{
	return translation;
}

Eigen::Vector3f Object::GetWorldRotation()
{
	//return QuaternionToRotation(quaternion);
	return transform.topLeftCorner<3, 3>().eulerAngles(2, 0, 1);
}

Eigen::Vector3f Object::GetWorldDirection()
{
	return forward;
}

void Object::SetWorldLocation(const Eigen::Vector3f& location)
{
	translation = location;
	transform.col(3).head<3>() = translation;
}
;
void Object::SetWorldRotation(const Eigen::Vector3f& rotation)
{
	quaternion = RotationToQuaternion(rotation);
	transform.topLeftCorner<3, 3>() = quaternion.toRotationMatrix();

	forward = quaternion * OriginForward;
	rightward = quaternion * OriginRightward;
	upward = quaternion * OriginUpward;
}

void Object::AddLocalLocation(const Eigen::Vector3f& location)
{
	Eigen::Vector3f worldLocation = quaternion * location;
	translation += worldLocation;
	transform.col(3).head<3>() = translation;
}

void Object::AddLocalRotation(const Eigen::Vector3f& rotation)
{
	//quaternion = RotationToQuaternion(TO_RADIANS(rotation)) * quaternion;
	//transform.topLeftCorner<3, 3>() = quaternion.toRotationMatrix();
	Eigen::AngleAxisf yaw(Eigen::AngleAxisf(rotation(0), upward));
	Eigen::AngleAxisf pitch(Eigen::AngleAxisf(rotation(1), rightward));
	Eigen::AngleAxisf roll(Eigen::AngleAxisf(rotation(2), forward));
	
	Eigen::Quaternionf localQuaternion = (yaw * pitch * roll);

	quaternion = localQuaternion * quaternion;
	transform.topLeftCorner<3, 3>() = quaternion.toRotationMatrix();

	//forward = localQuaternion * forward;
	//rightward = localQuaternion * rightward;
	//upward = localQuaternion * upward;

	forward = quaternion * OriginForward;
	rightward = quaternion * OriginRightward;
	upward = quaternion * OriginUpward;

	/*
	Eigen::Matrix3f rotationMatrix = RotationToQuaternion(TO_RADIANS(rotation)).toRotationMatrix();
	Eigen::Matrix4f transformMatrix = Eigen::Matrix4f::Identity();
	transformMatrix.topLeftCorner<3, 3>() = rotationMatrix;
	
	transform = transform * transformMatrix;
	quaternion = Eigen::Quaternionf(transform.topLeftCorner<3, 3>());
	*/
}

void Object::Tick(const float& deltaTime) 
{
	
}