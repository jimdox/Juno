#include "jnpch.h"
#include "Renderer/Platforms/GLRenderContext.h"
#include "Renderer/Window.h"
using namespace Juno;

// GLRenderContext::GLRenderContext()
// {

// }

// GLRenderContext::~GLRenderContext()
// {
    
// }


void GLRenderContext::Init()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		JN_CLI_ERR("Unable to load GLAD!");
		return;
	}
    JN_INFO("Initializing OpenGL(Glad) Context");
    EnableDepthTest();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);    
    
}

void GLRenderContext::ShutDown()
{
    
}

void GLRenderContext::ErrorCheck()
{
    while(GLenum error = glGetError())
    {
        JN_CRIT("OpenGL Error! Code : {}", error);
    }
    while(glGetError() != GL_NO_ERROR){}
}


void GLRenderContext::ClearScreen()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderContext::ResizeViewport(float width, float height)
{
    glViewport(0, 0, width, height);
}

void GLRenderContext::SetBackFaceCulling(bool flag)
{
    if(flag)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void GLRenderContext::SetWireFrameMode(bool flag)
{
    if(flag)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void GLRenderContext::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

void GLRenderContext::DisableDepthTest()
{

}

void GLRenderContext::DrawTriangles(unsigned int numIndices)
{
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}


void GLRenderContext::DrawLines(unsigned int numIndices)
{
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
}

void GLRenderContext::DrawPoints(unsigned int numIndices)
{
    glDrawElements(GL_POINTS, numIndices, GL_UNSIGNED_INT, 0);
}

void GLRenderContext::DrawTrianglesInstanced(unsigned int instanceCount)
{
    glDrawArraysInstanced(GL_TRIANGLES, 0, 1, instanceCount);
}

void GLRenderContext::DrawPointsInstanced(unsigned int instanceCount)
{
    glDrawArraysInstanced(GL_POINTS, 0, 1, instanceCount);
}
