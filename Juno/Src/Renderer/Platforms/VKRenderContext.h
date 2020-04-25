#pragma once
#include "Renderer/Platforms/GraphicsAPIContext.h"
namespace Juno
{
class VKRenderContext : public GraphicsAPIContext
{
public:
    
    void Init() override;
    void ShutDown() override;

    void ErrorCheck() override;

    void ClearScreen() override;
    void ResizeViewport(float width, float height) override;
    void SetBackFaceCulling(bool flag) override;
    void SetWireFrameMode(bool flag) override;
    void EnableDepthTest() override;
    void DisableDepthTest() override;
    
    void DrawTriangles(unsigned int numIndices) override;
    void DrawLines(unsigned int numIndices) override;
    void DrawPoints(unsigned int numIndices) override;
    void DrawTrianglesInstanced(unsigned int instanceCount);
    void DrawPointsInstanced(unsigned int instanceCount);
    
};
}