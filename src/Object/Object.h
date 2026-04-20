#pragma once
#include <string>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "Function/AE_Math.h"
#include "Physics/Collision.h"

//const Eigen::Matrix4f CoordinateSystemTransform = (Eigen::Matrix4f() << 1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1).finished();
const Eigen::Matrix4f CoordinateSystemProjection = (Eigen::Matrix4f() << 1, 0, 0, 0,
0, 0, 1, 0,
0, -1, 0, 0,
0, 0, 0, 1).finished();

const Eigen::Vector3f OriginForward = (Eigen::Vector3f() << 0, 1, 0).finished();
const Eigen::Vector3f OriginRightward = (Eigen::Vector3f() << 1, 0, 0).finished();
const Eigen::Vector3f OriginUpward = (Eigen::Vector3f() << 0, 0, 1).finished();

class Object
{
public:
	int id;
	std::string name;
	Eigen::Vector3f translation;
	Eigen::Quaternionf quaternion;
	Eigen::Matrix4f transform;

	Eigen::Vector3f forward;
	Eigen::Vector3f rightward;
	Eigen::Vector3f upward;

	std::vector<Object*> subObjects;

	Object();

	virtual Eigen::Vector3f GetWorldLocation();
	virtual Eigen::Vector3f GetWorldRotation();
	virtual Eigen::Vector3f GetWorldDirection();
	virtual void SetWorldLocation(const Eigen::Vector3f& location);
	virtual void SetWorldRotation(const Eigen::Vector3f& rotation);
	virtual void AddLocalLocation(const Eigen::Vector3f& location);
	virtual void AddLocalRotation(const Eigen::Vector3f& rotation);

	virtual void Tick(const float& deltaTime);

};