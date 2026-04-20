#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class Window
{
public:
	ImVec2 size;

	virtual void Render() {};
};