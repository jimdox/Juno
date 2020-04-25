#include "jnpch.h"
#include "Renderer/Platforms/GraphicsAPIContext.h"
#include "Renderer/Platforms/GLRenderContext.h"
#include "Renderer/Platforms/VKRenderContext.h"
using namespace Juno;

UPtr<GraphicsAPIContext> GraphicsAPIContext::Create(GraphicsAPI currentAPI)
{
    if(currentAPI == GraphicsAPI::Vulkan)
    {
        //s_activeAPI = GraphicsAPI::Vulkan;
        return std::make_unique<VKRenderContext>();
    }
    //s_activeAPI = GraphicsAPI::OpenGL;
    return std::make_unique<GLRenderContext>();
}

// GraphicsAPIContext::~GraphicsAPIContext()
// {

// }