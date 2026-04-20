#include "GUI.h"
#include "Input.h"

GUI::GUI()
{

}

GUI::~GUI()
{

}

void GUI::Open()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {

    }

    const char* glsl_version = "#version 400";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    glWindow = glfwCreateWindow((int)(width * main_scale), (int)(height * main_scale), "Abyss Engine", nullptr, nullptr);
    if (glWindow == nullptr)
    {

    }
    glfwMakeContextCurrent(glWindow);
    glfwSwapInterval(1); // Enable vsync
    //static bool opt_fullscreen = false;

    glEnable(GL_DEPTH_TEST);
    glewInit();

    const GLubyte* rendererer = glGetString(GL_RENDERER); // get rendererer string
    const GLubyte* version = glGetString(GL_VERSION);   // version as a string
    printf("Renderer: %s\n", rendererer);
    printf("OpenGL version supported %s\n", version);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigDockingWithShift = true;
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(glWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    /*
    const GLubyte* rendererer = glGetString(GL_RENDERER); // get rendererer string
    const GLubyte* version = glGetString(GL_VERSION);   // version as a string
    printf("Renderer: %s\n", rendererer);
    printf("OpenGL version supported %s\n", version);
    */

    //glViewport(0, 300, width, height);
    //glRasterPos2f(-1, -1);

    input = new Input;
    input->SetGUI(this);

    showMainWindow = true;
    renderWindowHovered = false;

    worldWindow.gravity = GetPhysics()->GetGravity();

    cameraWindow.location = GetCamera()->GetWorldLocation();
    cameraWindow.rotation = GetCamera()->GetWorldRotation();
    cameraWindow.direction = GetCamera()->GetWorldDirection();

    for (auto& object : GetWorld()->meshes)
    {
        ObjectWindow objectWindow;
        objectWindow.object = object;
        objectWindow.location = object->GetWorldLocation();
        objectWindow.rotation = TO_DEGREES(object->GetWorldRotation());
        objectWindow.direction = object->GetWorldDirection();
        objectWindow.collisionType = object->collision->type;        
        Collision* collision = object->collision;
        objectWindow.mass = collision->mass;
        objectWindow.restitution = collision->restitution;

        objectWindows.push_back(objectWindow);
    }

	for (auto& texture : GetWorld()->textures)
    {
        TextureWindow textureWindow;
        textureWindow.texture = texture;
        textureWindows.push_back(textureWindow);
    }
	
}

void GUI::Close()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(glWindow);
    glfwTerminate();
}

void GUI::RenderRenderWindow()
{
    ImGui::Begin("Render");
    renderWindowHovered = ImGui::IsWindowHovered();
    
    //ImVec2 wsize = ImGui::GetWindowSize();
    //ImVec2 wsize(renderer->width, renderer->height);
    ImVec2 wsize = ImGui::GetContentRegionAvail();
	renderWindow.width = (int)wsize.x;
	renderWindow.height = (int)wsize.y;
    if (renderWindow.width * renderWindow.height > 0 && (renderWindow.width != renderWindow.renderer->frame->GetWidth() || renderWindow.height != renderWindow.renderer->frame->GetHeight()))
    {
		renderWindow.renderer->frame->Resize(renderWindow.width, renderWindow.height);
        //std::cout << renderWindow.width << ", " << renderWindow.height << std::endl;
    }

    uint32_t textureID = renderWindow.renderer->frame->GetTextureBuffer();
    //uint32_t textureID = renderWindow.renderer->shadow->GetTextureBuffer();
    //ImGui::Image(reinterpret_cast<void*>(textureID), wsize, { 0, 1 }, { 1, 0 });
    
    glViewport(0, 0, wsize.x, wsize.y);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void*)textureID,
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + wsize.x, pos.y + wsize.y),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );
    
    ImGui::End();
}

void GUI::RenderWorldWindow()
{
    ImGui::Begin("World");

    ImGui::InputFloat("Gravity", &worldWindow.gravity, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::IsItemDeactivated())
    {
        GetPhysics()->SetGravity(worldWindow.gravity);
    }
    
    ImGui::Button("Pause");
    if (ImGui::IsItemDeactivated())
    {
        GetWorld()->Pause();
    }
    ImGui::Button("Resume");
    if (ImGui::IsItemDeactivated())
    {
        GetWorld()->Resume();
    }
    ImGui::End();
}

void GUI::RenderCameraWindow()
{
    ImGui::Begin("Camera");
    
    ImGui::InputFloat3("CameraLocation", cameraWindow.location.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::IsItemDeactivated())
    {
        GetCamera()->SetWorldLocation(cameraWindow.location);
    }
    if (!ImGui::IsItemActivated())
    {
        cameraWindow.location = GetCamera()->GetWorldLocation();
    }
    ImGui::InputFloat3("CameraRotation", cameraWindow.rotation.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::IsItemDeactivated())
    {
        GetCamera()->SetWorldRotation(TO_RADIANS(cameraWindow.rotation));
    }
    if (!ImGui::IsItemActivated())
    {
        Eigen::Vector3f r = TO_DEGREES(GetCamera()->GetWorldRotation());
        int c = 0;
        if (abs(r.x() - cameraWindow.rotation.x()) > 90) c++;
        if (abs(r.y() - cameraWindow.rotation.y()) > 90) c++;
        if (abs(r.z() - cameraWindow.rotation.z()) > 90) c++;
        if (c > 1)
        {
            r.x() = r.x() - 180.0f;
            r.y() = -(r.y() + 180.0f);
            r.z() = r.z() - 180.0f;
        }
        cameraWindow.rotation = r;
        //cameraWindow.rotation = TO_DEGREES(GetCamera()->GetWorldRotation());
    }
    ImGui::Text("Forward: %f, %f, %f", GetCamera()->forward.x(), GetCamera()->forward.y(), GetCamera()->forward.z());
    ImGui::Text("Rightward: %f, %f, %f", GetCamera()->rightward.x(), GetCamera()->rightward.y(), GetCamera()->rightward.z());
    ImGui::Text("Upward: %f, %f, %f", GetCamera()->upward.x(), GetCamera()->upward.y(), GetCamera()->upward.z());

    ImGui::End();
}

void GUI::RenderLightWindow()
{
    ImGui::Begin("Light");
    
    ImGui::InputFloat3("LightRotation", lightWindow.rotation.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::IsItemDeactivated())
    {
        GetWorld()->directionalLight->SetWorldRotation(TO_RADIANS(lightWindow.rotation));
    }
    if (!ImGui::IsItemActivated())
    {
        lightWindow.rotation = TO_DEGREES(GetWorld()->directionalLight->GetWorldRotation());
    }
    ImGui::Text("Direction: %f, %f, %f", GetWorld()->directionalLight->forward.x(), GetWorld()->directionalLight->forward.y(), GetWorld()->directionalLight->forward.z());

    ImGui::End();
}

void GUI::RenderObjectWindow()
{
    if (objectSelected)
    {
        Mesh* mesh = (Mesh*)objectSelected->object;
        //ImGui::DockBuilderDockWindow("ObjectInfo", leftDownNode);
        ImGui::Begin("Object");
        ImGui::Text("Name: %s", mesh->name.c_str());
        ImGui::InputFloat3("Location", objectSelected->location.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated())
        {
            mesh->SetWorldLocation(objectSelected->location);
        }
        if (!ImGui::IsItemActivated())
        {
            objectSelected->location = mesh->GetWorldLocation();
        }
        ImGui::InputFloat3("Rotation", objectSelected->rotation.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated())
        {
            mesh->SetWorldRotation(objectSelected->rotation);
        }
        if (!ImGui::IsItemActivated())
        {
            objectSelected->rotation = TO_DEGREES(mesh->GetWorldRotation());
        }
        ImGui::InputFloat3("Direction", objectSelected->direction.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated())
        {
            //mesh->SetWorldLocation(objectSelected->location);
        }
        if (ImGui::Combo("Collision", (int*)&objectSelected->collisionType, "NoCollision\0Static\0Dynamic\0Physics\0"))
        {
            mesh->SetCollision(objectSelected->collisionType);
        }
        if (objectSelected->collisionType != CollisionType::NoCollision)
        {
            ImGui::InputFloat("Mass", &objectSelected->mass, 0.0f, 0.0f, "%f", ImGuiInputTextFlags_EnterReturnsTrue);
            if (ImGui::IsItemDeactivated())
            {
                mesh->collision->SetMass(objectSelected->mass);
            }

            ImGui::InputFloat("Restitution", &objectSelected->restitution, 0.0f, 0.0f, "%f", ImGuiInputTextFlags_EnterReturnsTrue);
            if (ImGui::IsItemDeactivated())
            {
                mesh->collision->SetRestitution(objectSelected->restitution);
            }
        }    

		Material* material = mesh->material;
        ImGui::InputFloat3("ambient", objectSelected->ambientColor.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated())
        {
			material->ambientColor = objectSelected->ambientColor;
        }
        if (!ImGui::IsItemActivated())
        {
			objectSelected->ambientColor = material->ambientColor;
        }
        ImGui::InputFloat3("diffuse", objectSelected->diffuseColor.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated())
        {
            material->diffuseColor = objectSelected->diffuseColor;
        }
        if (!ImGui::IsItemActivated())
        {
            objectSelected->diffuseColor = material->diffuseColor;
        }
        ImGui::InputFloat3("specular", objectSelected->specularColor.data(), "%f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemDeactivated())
        {
            material->specularColor = objectSelected->specularColor;
        }
        if (!ImGui::IsItemActivated())
        {
            objectSelected->specularColor = material->specularColor;
        }
        ImGui::End();
    }
}

void GUI::RenderObjectListWindow()
{
    ImGui::Begin("ObjectList");

    for (auto& object : objectWindows)
    {
        if (ImGui::Selectable(object.object->name.c_str()))
        {
            objectSelected = &object;
        }
    }    
    ImGui::End();
}

void GUI::RenderTextureWindow()
{
    /*
    ImGui::Begin("Texture");
    
    uint32_t textureID = textureWindows[0].texture->tbo;
    //uint32_t textureID = renderWindow.renderer->shadow->GetTextureBuffer();
    //ImGui::Image(reinterpret_cast<void*>(textureID), wsize, { 0, 1 }, { 1, 0 });
	ImVec2 wsize = ImGui::GetContentRegionAvail();
    glViewport(0, 0, wsize.x, wsize.y);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void*)textureID,
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + wsize.x, pos.y + wsize.y),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );
	ImGui::End();
    */
}

void GUI::Tick(float deltaTime)
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    if (glfwGetWindowAttrib(glWindow, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();

    //ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    /*
    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    */
    //ImGui::ShowDemoWindow();

    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::Begin("AbyssEngine", &showMainWindow, ImGuiWindowFlags_MenuBar);
    
    dockspaceID = ImGui::GetID("MainDockspace");
    if (!ImGui::DockBuilderGetNode(dockspaceID))
    {
        //ImGui::SetWindowSize(ImVec2(370.0f, 680.0f));
        //ImGui::SetWindowPos(ImVec2(640.0f, 55.0f));

        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID);

        ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.8f, &leftNode, &rightNode);
        ImGui::DockBuilderSplitNode(leftNode, ImGuiDir_Up, 0.7f, &leftUpNode, &leftDownNode);
        ImGui::DockBuilderDockWindow("Render", leftUpNode);
        ImGui::DockBuilderDockWindow("World", leftDownNode);
        ImGui::DockBuilderDockWindow("Camera", leftDownNode);
        ImGui::DockBuilderDockWindow("Light", leftDownNode);
        ImGui::DockBuilderDockWindow("Object", leftDownNode);
        ImGui::DockBuilderDockWindow("ObjectList", rightNode);

        ImGui::DockBuilderFinish(dockspaceID);
    }

    ImGui::DockSpace(dockspaceID);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            ImGui::Button("ResetDocking");
            if (ImGui::IsItemDeactivated())
            {
                ImGui::DockBuilderRemoveNode(dockspaceID);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();

    RenderRenderWindow();
    RenderWorldWindow();
    RenderCameraWindow();
    RenderLightWindow();
    RenderObjectWindow();
    RenderObjectListWindow();
    RenderTextureWindow();

    // Rendering
    ImGui::Render();
    //Sleep(1000);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(glWindow);

    input->Tick(deltaTime);
}