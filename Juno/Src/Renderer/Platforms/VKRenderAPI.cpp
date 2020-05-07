#include "jnpch.h"
#include "Renderer/Platforms/VKRenderAPI.h"
using namespace Juno;

static VkInstance s_vulkanInstance;


static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, 
                                                    const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
    JN_ERR("Validation Layer: {}", callbackData->pMessage);
    return VK_FALSE;
}


void VKRenderCmd::Init()
{
	

    
}

void VKRenderCmd::ShutDown()
{
    vkDestroyInstance(s_vulkanInstance, nullptr);
    
}

GraphicsAPI VKRenderCmd::GetActiveAPI()
{
    return GraphicsAPI::Vulkan;
}

void VKRenderCmd::ErrorCheck()
{
    
}

void VKRenderCmd::ClearScreen()
{

}

void VKRenderCmd::ResizeViewport(float width, float height)
{

}

void VKRenderCmd::SetBackFaceCulling(bool flag)
{

}

void VKRenderCmd::SetWireFrameMode(bool flag)
{

}

void VKRenderCmd::EnableDepthTest()
{

}

void VKRenderCmd::DisableDepthTest()
{

}

void VKRenderCmd::DrawTriangles(unsigned int numIndices)
{

}

void VKRenderCmd::DrawLines(unsigned int numIndices)
{

}

void VKRenderCmd::DrawPoints(unsigned int numIndices)
{

}

void VKRenderCmd::DrawTrianglesInstanced(unsigned int instanceCount)
{

}

void VKRenderCmd::DrawPointsInstanced(unsigned int instanceCount)
{
    
}

