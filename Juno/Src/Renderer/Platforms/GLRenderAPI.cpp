#include "jnpch.h"
#include "Renderer/Platforms/GLRenderAPI.h"
#include "Renderer/Window.h"
using namespace Juno;

void RenderCmd::Init()
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

void RenderCmd::ShutDown()
{
    
}

GraphicsAPI RenderCmd::GetActiveAPI()
{
    return GraphicsAPI::OpenGL;
}


void RenderCmd::ErrorCheck()
{
    while(GLenum error = glGetError())
    {
        JN_CRIT("OpenGL Error! Code : {}", error);
    }
    while(glGetError() != GL_NO_ERROR){}
}


void RenderCmd::ClearScreen()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCmd::ResizeViewport(float width, float height)
{
    glViewport(0, 0, width, height);
}

void RenderCmd::SetBackFaceCulling(bool flag)
{
    if(flag)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void RenderCmd::SetWireFrameMode(bool flag)
{
    if(flag)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void RenderCmd::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

void RenderCmd::DisableDepthTest()
{

}

void RenderCmd::DrawTriangles(unsigned int numIndices)
{
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}


void RenderCmd::DrawLines(unsigned int numIndices)
{
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
}

void RenderCmd::DrawPoints(unsigned int numIndices)
{
    glDrawElements(GL_POINTS, numIndices, GL_UNSIGNED_INT, 0);
}

void RenderCmd::DrawTrianglesInstanced(unsigned int instanceCount)
{
    glDrawArraysInstanced(GL_TRIANGLES, 0, 1, instanceCount);
}

void RenderCmd::DrawPointsInstanced(unsigned int instanceCount)
{
    glDrawArraysInstanced(GL_POINTS, 0, 1, instanceCount);
}
