#include "jnpch.h"
#include "Renderer/Platforms/VKRenderAPI.h"
using namespace Juno;

// VKRenderContext::VKRenderContext()
// {

// }

void RenderCmd::Init()
{
    JN_INFO("Initializing Vulkan Context!");


}

void RenderCmd::ShutDown()
{

}

GraphicsAPI RenderCmd::GetActiveAPI()
{
    return GraphicsAPI::Vulkan;
}


// VKRenderContext::~VKRenderContext()
// {

// }

void RenderCmd::ErrorCheck()
{
    
}

void RenderCmd::ClearScreen()
{

}

void RenderCmd::ResizeViewport(float width, float height)
{

}

void RenderCmd::SetBackFaceCulling(bool flag)
{

}

void RenderCmd::SetWireFrameMode(bool flag)
{

}

void RenderCmd::EnableDepthTest()
{

}

void RenderCmd::DisableDepthTest()
{

}

void RenderCmd::DrawTriangles(unsigned int numIndices)
{

}

void RenderCmd::DrawLines(unsigned int numIndices)
{

}

void RenderCmd::DrawPoints(unsigned int numIndices)
{

}

void RenderCmd::DrawTrianglesInstanced(unsigned int instanceCount)
{

}

void RenderCmd::DrawPointsInstanced(unsigned int instanceCount)
{
    
}

