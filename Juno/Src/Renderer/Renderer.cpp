#include "jnpch.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderObjects/ThirdPersonCamera.h"
#include "Core/SceneManager.h"

// #ifdef JN_RENDERER_OPENGL
//     //#include "Renderer/Platforms/GLRenderAPI.h"
// #else 
#include "Renderer/Platforms/VKRenderAPI.h"


using namespace Juno;

static GraphicsAPI s_gfxAPI = GraphicsAPI::OpenGL;

void Renderer::SetGraphicsAPI(GraphicsAPI rendererAPI)
{
    s_gfxAPI = rendererAPI;
}


Renderer::Renderer(int sWidth, int sHeight, const std::string& wTitle) : window(sWidth, sHeight, wTitle), thirdPersonCamera({0,0,0},{0,0,0})
{
    VKRenderCmd::Init();
    currentCamera = std::make_shared<ThirdPersonCamera>(Vec3(0,0,0), Vec3(0,0,0));
}

Window& Renderer::GetWindow()
{
    return window;
}

SPtr<Scene> Renderer::GetCurrentScene()
{
    return currentScene;
}

SPtr<Camera> Renderer::GetCurrentCamera()
{
    return currentCamera;
}

void Renderer::Submit(SPtr<Scene> scene)
{
    
    currentCamera = scene->GetCamera();
    currentScene = scene;
}

void Renderer::Begin(float dt)
{
    //RenderCmd::ClearScreen();
    //currentScene->GetCamera()->Update(dt);

    if(currentScene != nullptr)
    {
        for(unsigned int i = 0; i < currentScene->GetEntities().size(); i++)
        {
            //RenderEntity(*currentScene->GetEntities()[i]);
        }
    }

}

void Renderer::End()
{
    #ifdef JN_BUILD_DEBUG
        //RenderCmd::ErrorCheck();
    #endif
    window.Update();
}

void Renderer::RenderEntity(Entity& entity)
{   
    // entity.GetMesh()->GetMaterial()->Bind();
    // VertexArray& vao = *entity.GetMesh()->GetVao();

    // Mat4 transform = Math::CreateTransformationMatrix(entity.GetPosition(), entity.GetRotation(), entity.GetScale());
    // entity.GetMesh()->GetMaterial()->LoadModelMatrix(transform);
    // entity.GetMesh()->GetMaterial()->LoadViewMatrix(currentScene->GetCamera()->GetViewMatrix());
    // entity.GetMesh()->GetMaterial()->LoadProjectionMatrix(currentScene->GetCamera()->GetProjectionMatrix());
    // entity.GetMesh()->GetMaterial()->LoadLights(currentScene->GetLights());

    // vao.Bind();
    // vao.EnableAttribute(0);
    // vao.EnableAttribute(1);

    // RenderCmd::DrawTriangles(entity.GetMesh()->GetNumIndices());

    // vao.DisableAttribute(1);
    // vao.DisableAttribute(0);
    // vao.Unbind();
    // entity.GetMesh()->GetMaterial()->Unbind();
}

void Renderer::RenderEntityInstanced(std::vector<SPtr<Entity>>& entity)
{

}

// void Renderer::RunComputeShader(GLComputeShader* cs, float dt)
// {
    // float forceRadii = 5;
    // cs->BindCS();
    // // cs->LoadFloat(glGetUniformLocation(cs->GetCSID(), "timestep"), dt);
    // // cs->LoadFloat(glGetUniformLocation(cs->GetCSID(), "forceRadii"), forceRadii);
    
    // cs->Compute();

    // cs->SetProjMatrix(currentScene->GetCamera()->GetProjectionMatrix());
    // cs->SetViewMatrix(currentScene->GetCamera()->GetViewMatrix());
    
    // RenderCmd::DrawPointsInstanced(cs->GetNumObjects());
    
    // glBindVertexArray(0);
    // cs->Unbind();
//}

GraphicsAPI Renderer::GetCurrentGraphicsAPI()
{
    return VKRenderCmd::GetActiveAPI();
}

void Renderer::SwitchGraphicsAPI(GraphicsAPI rendererAPI)
{
    // /this->rendererAPI = rendererAPI;
    /* now do a bunch of shutdowns and re-initialization of the renderer / asset loader! */
}



