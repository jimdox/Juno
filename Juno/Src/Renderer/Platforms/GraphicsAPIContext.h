#pragma once
#include "Utils/Memory.h"
#include <vector>
#include "Core/Config.h"
#include "Entities/Entity.h"

namespace Juno
{
enum class GraphicsAPI
{
    OpenGL, Vulkan
};

class GraphicsAPIContext
{
public:
    static UPtr<GraphicsAPIContext> Create(GraphicsAPI currentAPI);
    //static GraphicsAPI GetActiveAPI() { return s_activeAPI; }

    virtual void Init() = 0;
    virtual void ShutDown() = 0;
    virtual void ErrorCheck() = 0;
    virtual void ClearScreen() = 0;
    virtual void ResizeViewport(float width, float height) = 0;
    
    virtual void SetBackFaceCulling(bool flag) = 0;
    virtual void SetWireFrameMode(bool flag) = 0;
    virtual void EnableDepthTest() = 0;
    virtual void DisableDepthTest() = 0;

    virtual void DrawTriangles(unsigned int numIndices) = 0;
    virtual void DrawLines(unsigned int numIndices) = 0;
    virtual void DrawPoints(unsigned int numIndices) = 0;
    virtual void DrawTrianglesInstanced(unsigned int instanceCount) = 0;
    virtual void DrawPointsInstanced(unsigned int instanceCount) = 0;

    // unsigned int LoadTexture(unsigned char* imageData, TextureType texType);
    // unsigned int LoadCubeMap(std::array<unsigned char*, 6> imageData, TextureType texType);






protected:
    //static GraphicsAPI s_activeAPI;

private:

};
}


