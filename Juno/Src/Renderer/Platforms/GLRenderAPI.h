#pragma once
#include "Utils/Memory.h"
#include "Core/Config.h"
#include "Renderer/Renderer.h"

#ifdef JN_RENDERAPI_OPENGL
namespace Juno
{
class RenderCmd
{
protected:   

    static void Init();
    static void ShutDown();
    static GraphicsAPI GetActiveAPI();

    static void ErrorCheck();
    static void ClearScreen();
    
    static void ResizeViewport(float width, float height);    
    static void SetBackFaceCulling(bool flag);
    static void SetWireFrameMode(bool flag);
    static void EnableDepthTest();
    static void DisableDepthTest();    
    
    static void DrawTriangles(unsigned int numIndices);
    static void DrawLines(unsigned int numIndices);
    static void DrawPoints(unsigned int numIndices);
    static void DrawTrianglesInstanced(unsigned int instanceCount);
    static void DrawPointsInstanced(unsigned int instanceCount);



friend class Renderer;
};
}

#endif