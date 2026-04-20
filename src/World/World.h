#pragma once
#include <vector>
#include "Object/Mesh.h"
#include "Object/Camera.h"
#include "Object/Light.h"
#include "Physics/PhysicsSimulator.h"
#include "Material/Material.h"

class World
{
public:
	std::vector<Mesh*> meshes;
	PointLight* pointLight;
	DirectionalLight* directionalLight;
	Camera* camera;
	std::vector<Texture*> textures;

	PhysicsSimulator physics;
	bool paused = false;

	World();
	void Pause();
	void Resume();
	void Tick(const float& deltaTime);
};

World* GetWorld();
Camera* GetCamera();
PhysicsSimulator* GetPhysics();

template <typename T, typename... Args>
T* CreateObject(const Args&... args)
{
	T* object = new T(args...);
	object->id = GetWorld()->meshes.size();
	GetWorld()->meshes.push_back((Mesh*)object);
	return object;
}