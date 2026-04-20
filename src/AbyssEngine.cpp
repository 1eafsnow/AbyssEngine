#include "Object/Object.h"
#include "Object/Sphere.h"
#include "Object/Box.h"
#include "Physics/PhysicsSimulator.h"
#include "Physics/Collision.h"
#include "Render/Renderer.h"
#include "GUI/GUI.h"
#include "GUI/Input.h"
#include "Eigen/Dense"
#include <iostream>
#include <Windows.h>

int main()
{	
	Eigen::Vector3f r(90.0, -45.0, 90.0);
	Eigen::Quaternion q = RotationToQuaternion(TO_RADIANS(r));
	Eigen::Vector3f r2 = QuaternionToRotation(q);
	//std::cout << r2.transpose() << std::endl;
	//return 0;

	GetCamera()->SetWorldLocation({ 0.0, -15.0, 5.0 });
	GetCamera()->SetWorldRotation(TO_RADIANS(Eigen::Vector3f(0.0f, 0.0f, 0.0f)));

	CollisionInitParameters param;
	param.type = CollisionType::Dynamic;
	param.mass = 1.0f;
	param.restitution = 1.0f;

	Sphere* sphere = CreateObject<Sphere>("Sphere1", 1.0f, 16, 16, param);
	sphere->SetWorldLocation(Eigen::Vector3f(0.5, 0.0, -15.0));

	Sphere* sphere2 = CreateObject<Sphere>("Sphere2", 1.0f, 16, 16, param);
	sphere2->SetWorldLocation(Eigen::Vector3f(0.0, 0.0, 10.0));

	param.type = CollisionType::Static;
	param.mass = 0.0f;
	
	Box* box = CreateObject<Box>("Box1", 10.0, 10.0, 0.1, param);

	GetWorld()->directionalLight = new DirectionalLight;
	GetWorld()->directionalLight->SetWorldLocation({ 0.0, 0.0, 20.0 });
	GetWorld()->directionalLight->SetWorldRotation(TO_RADIANS(Eigen::Vector3f(0.0f, -80.0f, 0.0f)));
	GetWorld()->directionalLight->color = Eigen::Vector3f(1.0, 1.0, 1.0);
	GetWorld()->directionalLight->intensity = 1.0;

	GetWorld()->pointLight = new PointLight;
	GetWorld()->pointLight->SetWorldLocation({ 0.0, 0.0, 15.0 });
	GetWorld()->pointLight->SetWorldRotation(TO_RADIANS(Eigen::Vector3f(0.0f, -90.0f, 0.0f)));
	GetWorld()->pointLight->color = Eigen::Vector3f(1.0, 1.0, 1.0);
	GetWorld()->pointLight->intensity = 1.0;
	/*
	Eigen::Vector4f v;
	v.head<3>() = sphere.GetWorldLocation();
	v.w() = 1;
	std::cout << (sphere.transform * v).transpose() << std::endl;
	std::cout << (GetCamera()->viewTransform * sphere.transform * v).transpose() << std::endl;
	std::cout << (CoordinateSystemProjection * GetCamera()->viewTransform * sphere.transform * v).transpose() << std::endl;
	std::cout << (GetCamera()->projectionTransform * CoordinateSystemProjection * GetCamera()->viewTransform * sphere.transform * v).transpose() << std::endl;
	*/

	GUI gui;
	gui.Open();

	Renderer renderer(1280, 720);
	renderer.InitBindings();

	gui.renderWindow.renderer = &renderer;

	//Input input;

	while (1)
	{
		GetWorld()->Tick(1);
		renderer.Tick(1);		
		gui.Tick(1);
	}
	return 0;

}