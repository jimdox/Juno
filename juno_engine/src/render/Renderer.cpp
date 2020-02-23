#include "render/Renderer.h"

using namespace juno;

Renderer::Renderer(float sc_width, float sc_height, const std::string& w_title, glm::vec3 cam_pos, glm::vec3 cam_rot) : context(sc_width, sc_height, w_title), camera(cam_pos, cam_rot)
{
	// JN_WARN(JN_GFX_DEVICE);
    default_shader = std::make_shared<Shader>("./juno_engine/res/shaders/basic");												        /* useful for debugging w/ hybrid graphics */
	context.getKeyDispatcher().addListener(&camera);
	context.getMouseDispatcher().addListener(&camera);
	context.getWinEventDispatcher().addListener(&camera);
	default_shader->useProgram();
	default_shader->loadProjectionMatrix(camera.getProjectionMatrix());				/* load the perspective, view matrix from Camera */
	default_shader->loadViewMatrix(&camera);
	default_shader->unbindProgram();
}

Context& Renderer::getContext()
{
    return context;
}

RenderQueue& Renderer::getQueue()
{
    return queue;
}

Camera& Renderer::getCamera()
{
    return camera;
}

void Renderer::submit(Entity& entity)
{
    queue.submit(&entity, default_shader);
}

void Renderer::submit(Entity& entity, std::shared_ptr<Shader> shader)
{
    queue.submit(&entity, shader);
}

void Renderer::submit(Light &light)
{
    queue.addLight(light);
}

void Renderer::update(float delta_time)
{
    camera.update();
    glRender::clear();

    std::vector<Entity *>& entities = queue.getEntities();
    std::vector<std::shared_ptr<Shader>> entity_shaders = queue.getEntityShaders();
    std::vector<Light> lights = queue.getLights();

    for(unsigned int i = 0; i < queue.getEntities().size(); i++)
    {   
        entity_shaders[i]->useProgram();
	    entity_shaders[i]->loadProjectionMatrix(camera.getProjectionMatrix());			
        entity_shaders[i]->loadViewMatrix(&camera);
        entity_shaders[i]->loadLightUniforms(lights);
        
        glRender::renderEntity(entities[i] , entity_shaders[i]);
        
        entity_shaders[i]->unbindProgram();
    }

    context.update(delta_time);
}