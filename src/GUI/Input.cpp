#include "Input.h"
#include "GUI.h"

void Input::Press(uint32_t input)
{
    state |= input;
}
void Input::Release(uint32_t input)
{
    state &= ~input;
}
bool Input::Check(uint32_t input)
{
    return (state & input);
}

void Input::SetGUI(GUI* gui)
{
    this->gui = gui;
}

void Input::Tick(float deltaTime)
{
    if (glfwGetKey(gui->glWindow, GLFW_KEY_ESCAPE))
    {
        //glfwSetWindowShouldClose(gui->glWindow, 1);
    }
    if (ImGui::IsKeyDown(ImGuiKey_W))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Move({ 0.0, 1.0, 0.0 });
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_S))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Move({ 0.0, -1.0, 0.0 });
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_A))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Move({ -1.0, 0.0, 0.0 });
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_D))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Move({ 1.0, 0.0, 0.0 });
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_UpArrow))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Look(TO_RADIANS(Eigen::Vector3f(0.0f, 5.0f, 0.0f)));
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_DownArrow))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Look(TO_RADIANS(Eigen::Vector3f(0.0f, -5.0f, 0.0f)));
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_LeftArrow))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Look(TO_RADIANS(Eigen::Vector3f(5.0f, 0.0f, 0.0f)));
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_RightArrow))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Look(TO_RADIANS(Eigen::Vector3f(-5.0f, 0.0f, 0.0f)));
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_Q))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Look(TO_RADIANS(Eigen::Vector3f(0.0f, 0.0f, -5.0f)));
        }
    }
    if (ImGui::IsKeyDown(ImGuiKey_E))
    {
        if (gui->renderWindowHovered)
        {
            GetCamera()->Look(TO_RADIANS(Eigen::Vector3f(0.0f, 0.0f, 5.0f)));
        }
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        if (gui->renderWindowHovered)
        {
            double x;
            double y;
            glfwGetCursorPos(gui->glWindow, &x, &y);
            if (Check(MOUSE_RIGHT))
            {
                Eigen::Vector3f r(mouseX - x, mouseY - y, 0.0f);
                //std::cout << r << std::endl;
                GetCamera()->Look(TO_RADIANS(r));
            }
            mouseX = x;
            mouseY = y;
        }        
        Press(MOUSE_RIGHT);
    }
    else
    {
        Release(MOUSE_RIGHT);
    }
    glfwPollEvents();
}