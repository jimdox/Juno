#include "jnpch.h"
#ifndef JN_RENDERAPI_OPENGL
#include "Renderer/Platforms/VKRenderAPI.h"
using namespace Juno;

static VkInstance s_vulkanInstance;


static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, 
                                                    const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
    JN_ERR("Validation Layer error: {0}", callbackData->pMessage);
    return VK_FALSE;
}
                                                                                   

void RenderCmd::Init()
{
    JN_INFO("Initializing Vulkan Context...");





}

void RenderCmd::ShutDown()
{
    JN_INFO("Destroying Vulkan Context...");
    vkDestroyInstance(s_vulkanInstance, nullptr);
    
}

GraphicsAPI RenderCmd::GetActiveAPI()
{
    return GraphicsAPI::Vulkan;
}

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

/*____*/
#endif