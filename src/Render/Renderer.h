#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "Object/Mesh.h"
#include "World/World.h"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

#define TO_STRING(x) #x
#define EXPAND_AND_STRINGIFY(x) TO_STRING(x)

static const char* LoadFile(const char* path)
{
    FILE* file;
    file = fopen(path, "r");
    if (!file)
    {
        printf("Shader file not found!");
        return nullptr;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    char* data = new char[size + 1];
    rewind(file);
    size = fread(data, 1, size, file);
    data[size] = '\0';
    fclose(file);
    return data;
}

class Shader
{
public:
    uint32_t vertexShader;
    uint32_t fragmentShader;

    uint32_t shaderProgram;

    Shader();

    void SetVertexShader(const char* filePath);
    void SetFragmentShader(const char* filePath);

    void Set();
    void Use();

    void SetUniform(const char* name, const bool& value);
    void SetUniform(const char* name, const int& value);
    void SetUniform(const char* name, const float& value);
    void SetUniform(const char* name, const Eigen::Vector3f& value);
    void SetUniform(const char* name, const Eigen::Matrix4f& value);
};

class GLBinding
{
public:
    Mesh* mesh;
    uint32_t vao;
    uint32_t vboLocations;
    uint32_t vboTextures;
    uint32_t vboNormals;
    uint32_t vboTriangleIndices;
    uint32_t vboQuadIndices;

    GLBinding(Mesh* mesh);
    float* GetVerticesData();
    float* GetTexturesData();
    float* GetNormalsData();
    uint32_t* GetTriangleIndicesData();
    uint32_t* GetQuadIndicesData();
};

class GLFrameBuffer
{
public:
    GLFrameBuffer(uint32_t width, uint32_t height);
    ~GLFrameBuffer();

    void Init();
    void Bind();
    void Unbind();
	uint32_t GetWidth();
	uint32_t GetHeight();
    uint32_t GetTextureBuffer();
    void GetTexturePixels(float x, float y, int width, int height, void* pixels);
	void Resize(uint32_t width, uint32_t height);

private:
    uint32_t fbo;
    uint32_t tbo;
    uint32_t rbo;
    uint32_t width, height;
};

class GLShadowBuffer
{
public:
    GLShadowBuffer(uint32_t width, uint32_t height);
    ~GLShadowBuffer();
    void Init();
    void Bind();
    void Unbind();
    uint32_t GetWidth();
    uint32_t GetHeight();
    uint32_t GetTextureBuffer();
	void Resize(uint32_t width, uint32_t height);

private:
    uint32_t fbo;
    uint32_t tbo;
	uint32_t width, height;
};

class Renderer
{
public:
    GLFrameBuffer* frame;
    GLShadowBuffer* shadow;
    GLFrameBuffer* idBuffer;

private:
    Shader* whiteShader;
    Shader* materialShader;
    Shader* shadowShader;
    Shader* idShader;
    Shader* currentShader;

    std::vector<GLBinding> bindings;

public:
    Renderer(uint32_t width, uint32_t height);
    void InitBindings();    
    void RenderShadow();
    void RenderMesh();
    void RenderId();
    void Tick(const float& deltaTime);
};
