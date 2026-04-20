#include "AE_Math.h"

Eigen::Quaternionf RotationToQuaternion(const Eigen::Vector3f& rotation)
{
	Eigen::AngleAxisf yaw(Eigen::AngleAxisf(rotation(0), Eigen::Vector3f::UnitZ()));
	Eigen::AngleAxisf pitch(Eigen::AngleAxisf(rotation(1), Eigen::Vector3f::UnitX()));
	Eigen::AngleAxisf roll(Eigen::AngleAxisf(rotation(2), Eigen::Vector3f::UnitY()));
	return yaw * pitch * roll;
}

Eigen::Vector3f QuaternionToRotation(const Eigen::Quaternionf& quaternion)
{
	return quaternion.toRotationMatrix().eulerAngles(2, 0, 1);
}

Eigen::Matrix4f TransformInverse(Eigen::Matrix4f transform)
{
	Eigen::Matrix4f rotationInverse = Eigen::Matrix4f::Identity();
	rotationInverse.topLeftCorner<3, 3>() = transform.topLeftCorner<3, 3>().transpose();

	Eigen::Matrix4f translation = Eigen::Matrix4f::Identity();
	translation.col(3).head<3>() = -transform.col(3).head<3>();

	return rotationInverse * translation;
}