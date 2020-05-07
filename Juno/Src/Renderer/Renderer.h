#pragma once 
#include "Utils/Memory.h"
#include "Renderer/Window.h"
#include "Renderer/RenderObjects/Camera.h"
#include "Core/Scene.h"
#include "Core/Config.h"
#include "Renderer/RenderObjects/ThirdPersonCamera.h"
// #include "Renderer/Platforms/GLRenderContext.h"
// #include "Renderer/Platforms/VKRenderContext.h"

// #include "Renderer/Platforms/GLComputeShader.h"

namespace Juno 
{
class Renderer 
{
public:

    static Renderer& Get()
    {	
        static Renderer s_instance(1920, 1080, " ");
        return s_instance;
    }
    Renderer(const Renderer& renderer) = delete;

    static void SetGraphicsAPI(GraphicsAPI renderAPI);


    Window& GetWindow();
    SPtr<Scene>  GetCurrentScene();
    SPtr<Camera> GetCurrentCamera();

    void Submit(SPtr<Scene> scene);

    void Begin(float dt);
    void End();

    void RenderEntity(Entity& entity);
    void RenderEntityInstanced(std::vector<SPtr<Entity>>& entities);

    //void RunComputeShader(GLComputeShader* cs, float dt);

    // void SetClearColorBrightness(GLclampf brightness);
    // GLclampf GetClearColorBrightness();
    
    GraphicsAPI GetCurrentGraphicsAPI();
    void SwitchGraphicsAPI(GraphicsAPI graphicsAPI);

private:
    Renderer(int sWidth, int sHeight, const std::string& wTitle);

    // UPtr<GraphicsAPIContext> renderAPIContext;
    // GraphicsAPI currentGraphicsAPI = GraphicsAPI::OpenGL;
    VkInstance vulkanInstance;
    
    Window window;
    SPtr<Scene> currentScene;
    SPtr<Camera> currentCamera;
    ThirdPersonCamera thirdPersonCamera;
    SPtr<Shader> defaultShader;
    float clearColorBrightness;
    friend class VKRenderCmd;
};
}