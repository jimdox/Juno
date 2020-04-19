#include "jnpch.h"
#include "VertexArray.h"
#include "Renderer/Renderer.h"
using namespace Juno;


std::shared_ptr<VertexArray> VertexArray::Generate()
{
    if(Renderer::Get().GetCurrentRendererAPI() == RendererAPI::Vulkan)
    {
        return std::make_shared<VKVertexArray>();
    } else {
        return std::make_shared<GLVertexArray>();
    }
}





GLVertexArray::GLVertexArray()
{
    //glGenVertexArrays(1, &vaoID);
}

GLVertexArray::~GLVertexArray()
{

}

void GLVertexArray::Bind()
{
    glBindVertexArray(vaoID);
}

void GLVertexArray::Unbind()
{

}

VKVertexArray::VKVertexArray()
{

}

VKVertexArray::~VKVertexArray()
{

}

void VKVertexArray::Bind()
{

}

void VKVertexArray::Unbind()
{

}