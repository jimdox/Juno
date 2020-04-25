#include "jnpch.h"
#include "VertexArray.h"
#include "Renderer/Renderer.h"
using namespace Juno;


VertexArray* VertexArray::Generate()
{
    if(Renderer::Get().GetCurrentGraphicsAPI() == GraphicsAPI::Vulkan)
    {
        return new VKVertexArray();
    } else {
        return new GLVertexArray();
    }
}

unsigned int VertexArray::GetVaoID()
{
    return this->vaoID;
}




GLVertexArray::GLVertexArray()
{
    glGenVertexArrays(1, &vaoID);
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &vaoID);
}

void GLVertexArray::Bind()
{
    glBindVertexArray(vaoID);
}

void GLVertexArray::Unbind()
{
    glBindVertexArray(0);
}

void GLVertexArray::EnableAttribute(unsigned int attribNum)
{
    glEnableVertexAttribArray(attribNum);
}

void GLVertexArray::DisableAttribute(unsigned int attribNum)
{
    glDisableVertexAttribArray(attribNum);
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

void VKVertexArray::EnableAttribute(unsigned int attribNum)
{

}

void VKVertexArray::DisableAttribute(unsigned int attribNum)
{

}