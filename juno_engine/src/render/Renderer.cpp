#include "render/Renderer.h"
#include "render/glRenderCore.h"
#include "core/AssetManager.h"
using namespace juno;



Renderer::Renderer(float sc_width, float sc_height, const std::string& w_title, glm::vec3 cam_pos, glm::vec3 cam_rot) : window(sc_width, sc_height, w_title), camera(cam_pos, cam_rot), computeShader("./juno_engine/res/shaders/comp_test", 10000, 128)
{
	// JN_WARN(JN_GFX_DEVICE);                                                                          /* useful for debugging w/ hybrid graphics */
    defaultShader = &AssetManager::get().getDefaultShader();                      
    computeShader.init();
	//skybox_shader = std::make_shared<SkyBoxShader>("./juno_engine/res/shaders/skybox");
    window.getKeyDispatcher().addListener(&camera);
	window.getMouseDispatcher().addListener(&camera);
	window.getWinEventDispatcher().addListener(&camera);
	defaultShader->setActive();
	defaultShader->loadProjectionMatrix(camera.getProjectionMatrix());				                    /* load the perspective, view matrix from Camera */
	defaultShader->loadViewMatrix(&camera);
	defaultShader->unbindProgram();
}

Window& Renderer::getWindow()
{
    return window;
}

Camera& Renderer::getCamera()
{
    return camera;
}

Scene& Renderer::getScene()
{
    return *scene;
}

void Renderer::submit(Scene* scene)
{
    this->scene = scene;
}

void Renderer::submit(Entity& entity)
{
}

void Renderer::submit(Entity& entity, std::shared_ptr<Shader> shader)
{
}

void Renderer::submit(Light &light)
{
}

void Renderer::update(float delta_time)
{
    camera.update(delta_time);
    glRender::clear();

    std::vector<Entity>& entities = scene->getEntities();
    //std::vector<std::shared_ptr<Shader>> entity_shaders = queue.getEntityShaders();
    std::vector<Light> lights = scene->getLights();



    for(unsigned int i = 0; i < scene->getEntities().size(); i++)
    {   
        defaultShader->setActive();
        defaultShader->loadProjectionMatrix(camera.getProjectionMatrix());			
        defaultShader->loadViewMatrix(&camera);
        defaultShader->loadLightUniforms(lights);
        
        glRender::renderEntity(entities[i] , *defaultShader);
        defaultShader->unbindProgram();
    }

    /* render skybox */
    // skybox_shader->setActive();
    // skybox_shader->loadProjectionMatrix(camera.getProjectionMatrix());
    // skybox_shader->loadViewMatrix(&camera);
    // glRender::renderSkyBox(scene->getSkyBox(), skybox_shader);

    window.update(*scene, delta_time);
}

void Renderer::runComputeShader(float dt)
{   
    /* prepare compute shader */
    // glm::vec4 * forces = (glm::vec4*) glMapBufferRange(GL_ARRAY_BUFFER, 0, 1000 * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    // GLuint id_forceBuffer;
    // for(unsigned int i = 0; i < 1000; i++)
    // {
    //     v_computePositions[i].x = sinf(dt) * (rand()%300)/100;
    //     v_computePositions[i].y = cosf(dt) * (rand()%300)/100; 
    //     v_computePositions[i].z = tanf(dt) * (rand()%300)/100;
    //     v_computePositions[i].w = 1.0;
    // }
    // glUnmapBuffer(GL_ARRAY_BUFFER);

    glUseProgram(computeShader.getCSID());
    
    computeShader.loadFloat(glGetUniformLocation(computeShader.getCSID(), "timestep"), dt);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeShader.getPositionBufferID());
    
    /* run compute shader */
    glDispatchCompute(computeShader.getNumObjects()/128, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    
    /* cleanup cs */
    glUseProgram(0);

	/* prepare rendering shader */
    glm::vec4 color(0.4, 0.5, 0.7, 0.9);
    glm::mat4 viewMatrix = generateViewMatrix(&camera);
    
    glDisable(GL_CULL_FACE);

    glUseProgram(computeShader.getID()); 
	computeShader.loadMat4(glGetUniformLocation(computeShader.getID(), "projection"), camera.getProjectionMatrix());
    computeShader.loadMat4(glGetUniformLocation(computeShader.getID(), "view"), viewMatrix);
    //computeShader.loadFloat4(glGetUniformLocation(computeShader.getID(), "color"), color);


    glBindBuffer(GL_ARRAY_BUFFER, computeShader.getPositionBufferID());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glDrawArrays(GL_POINTS, 0, computeShader.getNumObjects());

    glEnable(GL_CULL_FACE);
    glUseProgram(0);
    /* cleanup rendering shader */

}