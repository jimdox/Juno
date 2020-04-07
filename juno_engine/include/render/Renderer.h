#pragma once 
#include "render/Context.h"
#include "render/Camera.h"
#include "entity/Scene.h"
#include "render/glRenderCore.h"
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

    Context& getContext();
    Camera& getCamera();
    Scene& getScene();

    void submit(Scene* scene);
    void submit(Entity& entity);
    void submit(Entity& entity, std::shared_ptr<Shader> shader);
    void submit(Light& light);

    void update(float delta_time);


private:
    Renderer(float sc_width, float sc_height, const std::string& window_title, glm::vec3 cam_pos, glm::vec3 cam_rot);

    Context context;
    Scene* scene;
    Camera camera;
    std::shared_ptr<Shader> default_shader;
    std::shared_ptr<SkyBoxShader> skybox_shader;

    static Renderer s_instance;
};
}