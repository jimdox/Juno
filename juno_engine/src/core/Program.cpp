#include "pch.h"
#include "core/Program.h"
#include "render/glRenderCore.h"
#include "core/Log.h"
#include "core/AssetLoader.h"
#include "render/lights/Light.h"
#include "render/RenderQueue.h"
#include "entity/SkyBox.h"
#include "render/shaders/SkyBoxShader.h"
#include "events/RenderEvent.h"

#define DISPLAY_MAX_FPS 120
using namespace juno;

Program::Program() : camera(glm::vec3(0.0f,1.0f,3.0f), 0, 0, 0)
{
	camera.addListener(this);
	this->f_program_should_close = false;
	frame_time = 0;
	num_frames = 0;
	delta_time = 0;
	init();
}	

Program::~Program()
{

}

void Program::init()
{
	render_context = std::make_unique<Context>(1920, 1080, " ", false);
	shader = std::make_shared<Shader>("./juno_engine/res/shaders/basic");

	render_context->getKeyDispatcher().addListener(&camera);
	render_context->getMouseDispatcher().addListener(&camera);
	render_context->getWinEventDispatcher().addListener(&camera);
	render_context->getWinEventDispatcher().addListener(this);
	
	// JN_WARN(JN_GFX_DEVICE); 												/* for use in debugging w/ hybrid graphics */

	shader->useProgram();
	shader->loadProjectionMatrix(camera.getProjectionMatrix());				/* load the perspective, view matrix from Camera */
	shader->loadViewMatrix(&camera);
	shader->unbindProgram();

}

void Program::onAttach()
{}

void Program::onEvent(const Event& e)
{
	switch(e.getType())
	{
	case EventType::WINDOW_CLOSE:
		this->f_program_should_close = true;
		break;

	case EventType::WINDOW_RESIZE:
		glRender::resizeFrameBuffer(((const WindowResizeEvent&)e).getScreenWidth(), ((const WindowResizeEvent&)e).getScreenHeight());
		break;
	case EventType::RENDER_POLYGON_WIREFRAME:
		glRender::setModeWireframe(((const RenderWireframeEvent&)e).modeSetWireframe());
		break;
	default:
		break;
	}
}

void Program::run()
{
	Mesh susy = loadModel("./juno_engine/res/dragon.obj");
	Texture texture1("./juno_engine/res/grey.png", TX_DIFFUSE, true);
	susy.addTexture(&texture1, texture1.getTexType());

	Mesh plane = loadModel("./juno_engine/res/plane.obj");
	Texture texture_two("./juno_engine/res/green.png", TX_DIFFUSE, true);
	plane.addTexture(&texture_two, texture_two.getTexType());

	glm::vec3 ent_pos(0.0f, -2.0f, -8.0f);
	glm::vec3 ent_rot(0.0f, 12.0f, 0.00f);

	glm::vec3 ground_pos(0.0f, -2.0f, -1.0f);
	glm::vec3 ground_rot(0.0f, 0.0f, 0.0f);

	Entity entity_one(susy, ent_pos, ent_rot, 0.75f, "Entity One");
	Entity entity_two(plane, ground_pos, ground_rot, 70, "Entity Two");
	Material gr_mat;
	gr_mat.reflectivity = 0.0f;
	gr_mat.shineDamper = 1.0f;
	entity_two.getMesh().setMaterial(gr_mat);
	
	// glm::vec3 entBPos(6.0f, 0.0f, -4.4f);
	// glm::vec3 entCPos(-6.0f, 0.0f, -5.4f);

	// Entity entB(susy, entBPos, ent_rot, 1.0f, entName);
	// std::shared_ptr terrain_shader = std::make_shared<TerrainShader>("./juno_engine/res/shaders/basic");
	// Terrain terrain(0, 0, texture1);

	Light light(glm::vec3(110.5f, 100.01f, 100.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(1.0, 0.00001, 0.0005));
	Light light_b(glm::vec3(-30.5f, 10.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.9f), glm::vec3(1.0, 0.0001, 0.0005));
	
	RenderQueue queue;
	queue.submit(&entity_one, shader);
	
	queue.submit(&entity_two, shader);
	queue.addLight(light);
	queue.addLight(light_b);

	/* cubemap testing */
	// std::array<const std::string, 6> cubemapTextures = {
	// 	"./juno_engine/res/grey.png",
	// 	"./juno_engine/res/grey.png",
	// 	"./juno_engine/res/grey.png",
	// 	"./juno_engine/res/grey.png",
	// 	"./juno_engine/res/grey.png",
	// 	"./juno_engine/res/grey.png"
	// };

	//CubeMap cubemap(cubemapTextures, TX_DIFFUSE);
	// std::shared_ptr skyboxShader = std::make_shared<SkyBoxShader>();
	// SkyBox skybox(cubemap);
	//queue.submit(&skybox, skyboxShader);

	/* ----- */
	last_time = glfwGetTime();

	while (!f_program_should_close && render_context->isRunning())
	{
		render_context->updateCamera(&camera, frame_time);
		
		glm::vec3 d_rot(0.0f, 0.01f, 0.0f);
		entity_one.addRotation(d_rot);
		camera.update();

		if(delta_time > 1/(DISPLAY_MAX_FPS) )
		{
			glRender::clear();
			queue.render(&camera);
			render_context->update(delta_time);
			delta_time = 0;
		}

		frame_time = (glfwGetTime() - last_time);
		last_time = glfwGetTime();
		delta_time += frame_time;
	}
}

void Program::exit()
{
	//render_context->~Context();
}
