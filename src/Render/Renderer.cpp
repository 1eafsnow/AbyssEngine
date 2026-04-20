#include "Render/Renderer.h"

Shader::Shader() : shaderProgram(glCreateProgram())
{

}

void Shader::SetVertexShader(const char* filePath)
{
    const char* buffer = LoadFile(filePath);
    //printf("%s\n", Buffer);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &buffer, NULL);
    glCompileShader(vertexShader);

    int compileResult;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileResult);

    if (!compileResult)
    {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", log);
    }
}

void Shader::SetFragmentShader(const char* filePath)
{
    const char* buffer = LoadFile(filePath);
    //printf("%s\n", Buffer);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &buffer, NULL);
    glCompileShader(fragmentShader);

    int compileResult;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileResult);

    if (!compileResult)
    {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", log);
    }
}

void Shader::Set()
{
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
}

void Shader::Use()
{
    glUseProgram(shaderProgram);
}

void Shader::SetUniform(const char* name, const bool& value)
{
    int location = glGetUniformLocation(shaderProgram, name);
    glUniform1i(location, (int)value);
}

void Shader::SetUniform(const char* name, const int& value)
{
    int location = glGetUniformLocation(shaderProgram, name);
    glUniform1i(location, value);
}

void Shader::SetUniform(const char* name, const float& value)
{
    int location = glGetUniformLocation(shaderProgram, name);
    glUniform1f(location, value);
}

void Shader::SetUniform(const char* name, const Eigen::Vector3f& value)
{
    int location = glGetUniformLocation(shaderProgram, name);
    glUniform3fv(location, 1, value.data());
}

void Shader::SetUniform(const char* name, const Eigen::Matrix4f& value)
{
    int location = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
}

GLBinding::GLBinding(Mesh* mesh)
{
    this->mesh = mesh;

    float* verticesData = GetVerticesData();
    glGenBuffers(1, &vboLocations);
    glBindBuffer(GL_ARRAY_BUFFER, vboLocations);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->vertices.size(), verticesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete verticesData;

    float* normalsData = GetNormalsData();
    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->vertices.size(), normalsData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete normalsData;

    float* texturesData = GetTexturesData();
    glGenBuffers(1, &vboTextures);
    glBindBuffer(GL_ARRAY_BUFFER, vboTextures);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->vertices.size(), texturesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete texturesData;

    if (mesh->triangles.size())
    {
        uint32_t* indicesData = GetTriangleIndicesData();
        glGenBuffers(1, &vboTriangleIndices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTriangleIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 3 * mesh->triangles.size(), indicesData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        delete indicesData;
    }
    if (mesh->quads.size())
    {
        uint32_t* indicesData = GetQuadIndicesData();
        glGenBuffers(1, &vboQuadIndices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboQuadIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 4 * mesh->quads.size(), indicesData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        delete indicesData;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vboLocations);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, vboTextures);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);

    glBindVertexArray(0);
}

float* GLBinding::GetVerticesData()
{
    float* array = new float[3 * mesh->vertices.size()];
    for (int i = 0; i < mesh->vertices.size(); i++)
    {
        array[i * 3] = mesh->vertices[i].location.x();
        array[i * 3 + 1] = mesh->vertices[i].location.y();
        array[i * 3 + 2] = mesh->vertices[i].location.z();
        //printf("ver: %f %f %f\n", array[i * 3], array[i * 3 + 1], array[i * 3 + 2]);
    }
    return array;
}

float* GLBinding::GetTexturesData()
{
    float* array = new float[2 * mesh->vertices.size()];
    for (int i = 0; i < mesh->vertices.size(); i++)
    {
        array[i * 2] = mesh->vertices[i].texture.x();
        array[i * 2 + 1] = mesh->vertices[i].texture.y();
        //printf("%f %f\n", array[i * 2], array[i * 2 + 1]);
    }
    return array;
}

float* GLBinding::GetNormalsData()
{
    float* array = new float[3 * mesh->vertices.size()];
    for (int i = 0; i < mesh->vertices.size(); i++)
    {
        array[i * 3] = mesh->vertices[i].normal.x();
        array[i * 3 + 1] = mesh->vertices[i].normal.y();
        array[i * 3 + 2] = mesh->vertices[i].normal.z();
        //printf("%f %f %f\n", array[i * 3], array[i * 3 + 1], array[i * 3 + 2]);
    }
    return array;
}

uint32_t* GLBinding::GetTriangleIndicesData()
{
    uint32_t* array = new uint32_t[3 * mesh->triangles.size()];
    for (int i = 0; i < mesh->triangles.size(); i++)
    {
        array[i * 3] = mesh->triangles[i][0];
        array[i * 3 + 1] = mesh->triangles[i][1];
        array[i * 3 + 2] = mesh->triangles[i][2];
        //printf("tri: %d %d %d\n", array[i * 3], array[i * 3 + 1], array[i * 3 + 2]);
    }
    return array;
}

uint32_t* GLBinding::GetQuadIndicesData()
{
    uint32_t* array = new uint32_t[4 * mesh->quads.size()];
    for (int i = 0; i < mesh->quads.size(); i++)
    {
        array[i * 4] = mesh->quads[i][0];
        array[i * 4 + 1] = mesh->quads[i][1];
        array[i * 4 + 2] = mesh->quads[i][2];
        array[i * 4 + 3] = mesh->quads[i][3];
    }
    return array;
}

GLFrameBuffer::GLFrameBuffer(uint32_t width, uint32_t height) : width(width), height(height)
{
    Init();
}

GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteTextures(1, &tbo);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}

void GLFrameBuffer::Init()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! (";
        switch (status)
        {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)";
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "GL_FRAMEBUFFER_UNSUPPORTED)";
            break;

        default:
            std::cout << "UNKNOW: " << std::hex << status << ")";
        }
        std::cout << std::endl;
    }
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //glBindTexture(GL_TEXTURE_2D, tbo);
    //glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

void GLFrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

uint32_t GLFrameBuffer::GetWidth()
{
	return width;
}

uint32_t GLFrameBuffer::GetHeight()
{
    return height;
}

uint32_t GLFrameBuffer::GetTextureBuffer()
{
    return tbo;
}

void GLFrameBuffer::GetTexturePixels(float x, float y, int width, int height, void* pixels)
{
    
}

void GLFrameBuffer::Resize(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;
    
    glDeleteTextures(1, &tbo);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
    Init();
}

GLShadowBuffer::GLShadowBuffer(uint32_t width, uint32_t height) : width(width), height(height)
{
    Init();
}

GLShadowBuffer::~GLShadowBuffer()
{
    glDeleteTextures(1, &tbo);
    glDeleteFramebuffers(1, &fbo);
}

void GLShadowBuffer::Init()
{
    glGenFramebuffers(1, &fbo);

    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tbo, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! (";
        switch (status)
        {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)";
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "GL_FRAMEBUFFER_UNSUPPORTED)";
            break;

        default:
            std::cout << "UNKNOW: " << std::hex << status << ")";
        }
        std::cout << std::endl;
    }

    //glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLShadowBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //glBindTexture(GL_TEXTURE_2D, tbo);
    //glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

void GLShadowBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

uint32_t GLShadowBuffer::GetWidth()
{
    return width;
}

uint32_t GLShadowBuffer::GetHeight()
{
    return height;
}

uint32_t GLShadowBuffer::GetTextureBuffer()
{
    return tbo;
}

void GLShadowBuffer::Resize(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;

    glDeleteTextures(1, &tbo);
    glDeleteFramebuffers(1, &fbo);
    Init();
}

Renderer::Renderer(uint32_t width, uint32_t height)
{        
    //glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_GREATER, 0.5);
    //glEnable(GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);  
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    //materialShader.SetVertexShader("Shader/vs.glsl");
    //materialShader.SetFragmentShader("Shader/fs_material.glsl");
    //materialShader.Set();

    std::string shaderDir;
    #ifdef PROGRAM_DIR
    char dir[] = EXPAND_AND_STRINGIFY(PROGRAM_DIR);
    shaderDir.assign(dir);
    shaderDir.append("/src/Shader");
    #else
    #endif
    std::cout << "Shader directory: " << shaderDir << std::endl;
    //system("pause");
    std::string vsPath;
    std::string fsPath;

    vsPath.assign(shaderDir);
    fsPath.assign(shaderDir);
    vsPath.append("/vs_white.glsl");
    fsPath.append("/fs_white.glsl");
    whiteShader = new Shader;
    whiteShader->SetVertexShader(vsPath.c_str());
    whiteShader->SetFragmentShader(fsPath.c_str());
    whiteShader->Set();

    vsPath.assign(shaderDir);
    fsPath.assign(shaderDir);
    vsPath.append("/vs_white.glsl");
    fsPath.append("/fs_material.glsl");
    materialShader = new Shader;
    materialShader->SetVertexShader(vsPath.c_str());
    materialShader->SetFragmentShader(fsPath.c_str());
    materialShader->Set();

    vsPath.assign(shaderDir);
    fsPath.assign(shaderDir);
    vsPath.append("/vs_shadow.glsl");
    fsPath.append("/fs_shadow.glsl");
    shadowShader = new Shader;
    shadowShader->SetVertexShader(vsPath.c_str());
    shadowShader->SetFragmentShader(fsPath.c_str());
    shadowShader->Set();

    vsPath.assign(shaderDir);
    fsPath.assign(shaderDir);
    vsPath.append("/vs_id.glsl");
    fsPath.append("/fs_id.glsl");
    idShader = new Shader;
    idShader->SetVertexShader(vsPath.c_str());
    idShader->SetFragmentShader(fsPath.c_str());
    idShader->Set();

    frame = new GLFrameBuffer(width, height);
	shadow = new GLShadowBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);
	idBuffer = new GLFrameBuffer(width, height);
}

void Renderer::InitBindings()
{
    for (auto& mesh : GetWorld()->meshes)
    {
        bindings.push_back(GLBinding(mesh));
    }
}

void Renderer::RenderShadow()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    shadow->Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0, 4.0);

    currentShader = shadowShader;
    currentShader->Use();

    currentShader->SetUniform("view_transform", GetWorld()->directionalLight->viewTransform);
    currentShader->SetUniform("projection_transform", GetWorld()->directionalLight->projectionTransform);
    //currentShader->SetUniform("view_transform", GetWorld()->pointLight->viewTransform);
    //currentShader->SetUniform("projection_transform", GetWorld()->pointLight->projectionTransform);

    for (auto& binding : bindings)
    {
        currentShader->SetUniform("model_transform", binding.mesh->transform);

        glBindVertexArray(binding.vao);

        if (binding.mesh->triangles.size())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.vboTriangleIndices);
            glDrawElements(GL_TRIANGLES, 3 * binding.mesh->triangles.size(), GL_UNSIGNED_INT, 0);
        }
        if (binding.mesh->quads.size())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.vboQuadIndices);
            glDrawElements(GL_QUADS, 4 * binding.mesh->quads.size(), GL_UNSIGNED_INT, 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    glDisable(GL_POLYGON_OFFSET_FILL);
    shadow->Unbind();
}

void Renderer::RenderMesh()
{
    //glActiveTexture(GL_TEXTURE1);
    glViewport(0, 0, frame->GetWidth(), frame->GetHeight());
    frame->Bind();
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_POLYGON_OFFSET_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadow->GetTextureBuffer());

    currentShader = materialShader;
    currentShader->Use();

    currentShader->SetUniform("view_transform", GetCamera()->viewTransform);
    currentShader->SetUniform("projection_transform", GetCamera()->projectionTransform);
    currentShader->SetUniform("viewLocation", GetCamera()->GetWorldLocation());

    currentShader->SetUniform("shadow_view_transform", GetWorld()->directionalLight->viewTransform);
    currentShader->SetUniform("shadow_projection_transform", GetWorld()->directionalLight->projectionTransform);
    currentShader->SetUniform("shadowSampler", 0);

    currentShader->SetUniform("directionalLight.direction", GetWorld()->directionalLight->forward);
    currentShader->SetUniform("directionalLight.color", GetWorld()->directionalLight->color);
    currentShader->SetUniform("directionalLight.intensity", GetWorld()->directionalLight->intensity);
    /*
    currentShader->SetUniform("shadow_view_transform", GetWorld()->pointLight->viewTransform);
    currentShader->SetUniform("shadow_projection_transform", GetWorld()->pointLight->projectionTransform);
    currentShader->SetUniform("shadowSampler", 0);

    currentShader->SetUniform("pointLight.location", GetWorld()->pointLight->GetWorldLocation());
    currentShader->SetUniform("pointLight.color", GetWorld()->pointLight->color);
    currentShader->SetUniform("pointLight.intensity", GetWorld()->pointLight->intensity);
    */

    for (auto& binding : bindings)
    {
        currentShader->SetUniform("model_transform", binding.mesh->transform);
        //std::cout << "model: " << std::endl;
        //std::cout << binding->mesh->transform << std::endl;

        currentShader->SetUniform("material.ambientColor", binding.mesh->material->ambientColor);
        currentShader->SetUniform("material.diffuseColor", binding.mesh->material->diffuseColor);
        currentShader->SetUniform("material.specularColor", binding.mesh->material->specularColor);
        currentShader->SetUniform("material.hasDiffuseTexture", false);
        currentShader->SetUniform("material.hasSpecularTexture", false);

        glBindVertexArray(binding.vao);

        if (binding.mesh->triangles.size())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.vboTriangleIndices);
            glDrawElements(GL_TRIANGLES, 3 * binding.mesh->triangles.size(), GL_UNSIGNED_INT, 0);
        }
        if (binding.mesh->quads.size())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.vboQuadIndices);
            glDrawElements(GL_QUADS, 4 * binding.mesh->quads.size(), GL_UNSIGNED_INT, 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    frame->Unbind();

    //glActiveTexture(GL_TEXTURE0);
}

void Renderer::RenderId()
{
    glViewport(0, 0, frame->GetWidth(), frame->GetHeight());
    idBuffer->Bind();
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_POLYGON_OFFSET_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    currentShader = idShader;
    currentShader->Use();
    currentShader->SetUniform("view_transform", GetCamera()->viewTransform);
    currentShader->SetUniform("projection_transform", GetCamera()->projectionTransform);

    for (auto& binding : bindings)
    {
        currentShader->SetUniform("model_transform", binding.mesh->transform);
        currentShader->SetUniform("id", binding.mesh->id);

        glBindVertexArray(binding.vao);

        if (binding.mesh->triangles.size())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.vboTriangleIndices);
            glDrawElements(GL_TRIANGLES, 3 * binding.mesh->triangles.size(), GL_UNSIGNED_INT, 0);
        }
        if (binding.mesh->quads.size())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.vboQuadIndices);
            glDrawElements(GL_QUADS, 4 * binding.mesh->quads.size(), GL_UNSIGNED_INT, 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    idBuffer->Unbind();
}

void Renderer::Tick(const float& deltaTime)
{    
    RenderShadow();
    RenderId();
    RenderMesh();
}