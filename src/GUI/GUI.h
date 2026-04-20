#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdio.h>
#include <vector>
#include <map>
#include "Function/AE_Math.h"
#include "Render/Renderer.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

class Input;

struct WorldWindow
{
	float gravity;
};

struct CameraWindow
{
	Eigen::Vector3f location;
	Eigen::Vector3f rotation;
	Eigen::Vector3f direction;
};

struct LightWindow
{
	Eigen::Vector3f location;
	Eigen::Vector3f rotation;
	Eigen::Vector3f direction;
};

struct ObjectWindow
{
	Object* object;
	Eigen::Vector3f location;
	Eigen::Vector3f rotation;
	Eigen::Vector3f direction;

	CollisionType collisionType;
	float mass;
	float restitution;

	Eigen::Vector3f ambientColor;
	Eigen::Vector3f diffuseColor;
	Eigen::Vector3f specularColor;
};

struct RenderWindow
{
	Renderer* renderer;
	int width;
	int height;
};

struct TextureWindow
{
	Texture* texture;
};

class GUI
{
public:
	int width = 1600;
	int height = 900;

	GLFWwindow* glWindow;
	//ImGuiIO* io;
	
	RenderWindow renderWindow;
	WorldWindow worldWindow;
	CameraWindow cameraWindow;
	LightWindow lightWindow;
	std::vector<ObjectWindow> objectWindows;
	ObjectWindow* objectSelected = nullptr;
	std::vector<TextureWindow> textureWindows;

	Input* input;

	ImGuiID dockspaceID;
	ImGuiID leftNode;
	ImGuiID rightNode;
	ImGuiID leftUpNode;
	ImGuiID leftDownNode;

	bool showMainWindow;
	bool renderWindowHovered;

	GUI();
	~GUI();

	void Open();
	void Close();

	void RenderRenderWindow();
	void RenderWorldWindow();
	void RenderCameraWindow();
	void RenderLightWindow();
	void RenderObjectWindow();
	void RenderObjectListWindow();
	void RenderTextureWindow();
	void Tick(float deltaTime);
};