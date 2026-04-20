#pragma once
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "Function/AE_Math.h"
#include "Material/Texture.h"

class Material
{
public:
	Eigen::Vector3f ambientColor = Eigen::Vector3f(0.3, 0.3, 0.3);
	Eigen::Vector3f diffuseColor = Eigen::Vector3f(0.5, 0.5, 0.5);
	Eigen::Vector3f specularColor = Eigen::Vector3f(0.5, 0.5, 0.5);
};