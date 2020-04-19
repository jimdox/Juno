#pragma once 
#include "Renderer/Window.h"
#include "Renderer/Camera.h"
#include "Core/Scene.h"
#include "Renderer/Shaders/ComputeShader.h"

// #ifdef JN_RENDERER_MODE_VK
//     #include "Renderer/RenderCoreVK.h"
// #else
// 	#include "Renderer/RenderCoreGL.h"
// #endif

namespace Juno {

enum class RendererAPI
{
    OpenGL,
    Vulkan
};


class Renderer 
{
public:
    static Renderer& Get()
    {	
        static Renderer s_instance(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,20.0f,0));
        return s_instance;
    }

    Renderer(const Renderer& renderer) = delete;

    Window& GetWindow();
    Camera& GetCamera();
    Scene& GetScene();

    void Submit(Scene& scene);
    void Submit(Entity& entity);
    void Submit(Entity& entity, std::shared_ptr<Shader> shader);
    void Submit(Light& light);

    void Begin(float dt);
    void End(float dt);

    void RunComputeShader(ComputeShader& cs, float dt);

    void SetClearColorBrightness(GLclampf brightness);
    GLclampf GetClearColorBrightness();
    RendererAPI GetCurrentRendererAPI();
    void SetRendererAPI(RendererAPI rendererAPI);

private:
    Renderer(float scWidth, float scHeight, const std::string& wTitle, glm::vec3 camPos, glm::vec3 camRot);
    RendererAPI currentRenderAPI;
    Window window;
    Scene* scene;
    Camera camera;
    //Shader* defaultShader;
    float clearColorBrightness;

};
}