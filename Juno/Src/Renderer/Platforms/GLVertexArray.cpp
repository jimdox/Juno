#include "jnpch.h"
#include "Renderer/Data/VertexArray.h"
#include "Renderer/Platforms/GLVertexArray.h"
#include <glad/glad.h>
using namespace Juno;

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