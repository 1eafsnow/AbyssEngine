#include "World.h"

World world;

World::World()
{
	camera = new Camera(0.01, 1000, 60, 60);
}

void World::Pause()
{
	paused = true;
}

void World::Resume()
{
	paused = false;
}

void World::Tick(const float& deltaTime)
{
	if (paused)
	{
		return;
	}

	physics.Tick(deltaTime);

	for (auto& mesh : meshes)
	{
		mesh->Tick(deltaTime);
	}
}

World* GetWorld()
{
	return &world;
}

Camera* GetCamera()
{
	return world.camera;
}

PhysicsSimulator* GetPhysics()
{
	return &world.physics;
}