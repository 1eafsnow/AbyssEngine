#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>

#define TO_RADIANS(degrees) (degrees * EIGEN_PI / 180.0)
#define TO_DEGREES(radians) (radians * 180.0 / EIGEN_PI)

Eigen::Quaternionf RotationToQuaternion(const Eigen::Vector3f& rotation);
Eigen::Vector3f QuaternionToRotation(const Eigen::Quaternionf& quaternion);
Eigen::Matrix4f TransformInverse(Eigen::Matrix4f transform);