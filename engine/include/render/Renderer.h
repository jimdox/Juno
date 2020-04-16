#pragma once 
#include "render/Window.h"
#include "render/Camera.h"
#include "entity/Scene.h"
#include "render/shaders/ComputeShader.h"
#include "render/shaders/SkyBoxShader.h"
namespace juno {

class Renderer 
{
public:
    static Renderer& init(float sc_width, float sc_height, const std::string& window_title, glm::vec3 cam_pos, glm::vec3 cam_rot)
    {
        static Renderer s_instance(sc_width, sc_height,window_title, cam_pos, cam_rot);
        return s_instance;
    }

    Renderer(const Renderer& renderer) = delete;

    Window& getWindow();
    Camera& getCamera();
    Scene& getScene();

    void submit(Scene* scene);
    void submit(Entity& entity);
    void submit(Entity& entity, std::shared_ptr<Shader> shader);
    void submit(Light& light);

    void update(float delta_time);

    void runComputeShader(float dt);

private:
    Renderer(float sc_width, float sc_height, const std::string& window_title, glm::vec3 cam_pos, glm::vec3 cam_rot);

    Window window;
    Scene* scene;
    Camera camera;
    Shader* defaultShader;
    ComputeShader computeShader;
    //SkyBoxShader* skyboxShader;

    // static Renderer s_instance;
};
}