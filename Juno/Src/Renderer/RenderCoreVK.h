#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Renderer/Window.h"

#include "Renderer/Shaders/Shader.h"
#include "Renderer/Shaders/ComputeShader.h"

#include "Entities/Entity.h"
#include "Renderer/Mesh.h"
#include "Renderer/Textures/Texture.h"

#include "Core/Log.h"
#include "Utils/Math.h"

#define JN_GFX_DEVICE "something"

namespace RenderCall
{

static void Init(GLFWwindow* window)
{
	JN_INFO("Initializing Vulkan Context...");
    if(false)
    {

        JN_CLI_ERR("Unable to load Vulkan!");
    }


}

static void ErrorCheck()
{
    // validation layer stuff goes here
}

static void Clear()
{

}

static void ResizeFrameBuffer(float width, float height)
{

}


static void SetBackFaceCulling(bool flag)
{
    if(flag)
    {

    } else {

    }
}


static void setModeWireFrame(bool flag)
{
    if(flag)
    {

    } else {

    }
}

static void RenderEntity(Juno::Entity& entity, Juno::Shader& shader)
{

}

static void InstancedEntityRender(std::vector<Juno::Entity*>& entities, std::shared_ptr<Juno::Shader>& shader)
{

}


}

/* --- 2D Stuff --- */

static void RenderQuad()
{

}





static void ApplyComputeShader(Juno::ComputeShader& cs)
{

}
