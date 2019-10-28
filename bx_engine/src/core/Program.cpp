#include "core/Program.h"
#include "render/glRenderCore.h"
#include "core/Log.h"
#include "core/AssetLoader.h"
#include "render/lights/Light.h"
#include "render/RenderQueue.h"

using namespace bx;


Program::Program(): camera(glm::vec3(0,0,3.0f), 0, 0, 0)
{
	init();
}

Program::~Program()
{

}

void Program::init()
{
	render_context = std::make_unique<Context>(800, 800, "v0.0.1", false);
	shader = std::make_shared<Shader>("./bx_engine/res/shaders/basic");

	BX_WARN(BX_GFX_DEVICE);

	shader->useProgram();
	shader->loadProjectionMatrix(camera.getProjectionMatrix());				/* load the perspective matrix from Camera */
	shader->loadViewMatrix(&camera);
	shader->unbindProgram();

}

void Program::run()
{
	Mesh stall = bxImport::loadModel("./bx_engine/res/dragon.obj");
	Texture texture1("./bx_engine/res/stall_tex.png", TX_DIFFUSE, true);
	stall.addTexture(&texture1, texture1.getTexType());

	Mesh ground = bxImport::loadModel("./bx_engine/res/susanne.obj");
	Texture texture_two("./bx_engine/res/stall_tex.png", TX_DIFFUSE, true);
	ground.addTexture(&texture_two, texture_two.getTexType());

	glm::vec3 ent_pos(0.0f, -2, -5.0f);
	glm::vec3 ent_rot(0.0f, 0.0f, 0.00f);

	glm::vec3 ground_pos(-7, 5, -7);
	glm::vec3 ground_rot(0.0, 10, 45);

	Entity entity_one(stall, ent_pos, ent_rot, 0.6f, "Entity One");
	Entity entity_two(ground, ground_pos, ground_rot, 0.3, "Entity Two");
	
	// glm::vec3 entBPos(6.0f, 0.0f, -4.4f);
	// glm::vec3 entCPos(-6.0f, 0.0f, -5.4f);

	// Entity entB(stall, entBPos, ent_rot, 1.0f, entName);

	// std::shared_ptr terrain_shader = std::make_shared<TerrainShader>("./bx_engine/res/shaders/basic");
	// Terrain terrain(0, 0, texture1);


	Light light(glm::vec3(113.5f, 0.01f, 30.0f), glm::vec3(0.41f, 0.41f, 0.41f));
	Light light_b(glm::vec3(-111.5f, -0.01f, 1.0f), glm::vec3(0.41f, 0.6f, 0.9f));
	
	RenderQueue queue;
	queue.submit(&entity_one, shader);
	queue.submit(&entity_two, shader);
	//queue.addLight(light);
	queue.addLight(light_b);
	/* ----- */
	frame_time = 0;
	num_frames = 0;
	dt = 0;
	last_time = glfwGetTime();

	while (render_context->isRunning())
	{
		render_context->updateCamera(&camera, frame_time);
		
		glm::vec3 d_rot(0.0f, 0.1f, 0.0f);
		//entity_one.addRotation(d_rot);

		bxRender::clear();

		queue.render(&camera);
		
		render_context->update();
		fpsCounter();

	}
}

void Program::exit()
{

}

void Program::fpsCounter()
{
	frame_time = (glfwGetTime() - last_time);
	last_time = glfwGetTime();

	num_frames++;
	dt += frame_time;
	if(dt >= 1.0)
	{
		BX_CLI_INFO("FPS: {}", (num_frames));
		dt = 0;
		num_frames = 0;
	}
}


// /* ------------- */
// void Program::splashShader()
// {
// 	this->shader = std::make_shared<Shader>("./bxEngine/res/shaders/gui");
// 	float vertices[] = {
// 		// positions          // colors           // texture coords
// 		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
// 		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
// 		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
// 		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
// 	};

// 	unsigned int indices[] = {
// 		0, 1, 3, // first triangle
// 		1, 2, 3  // second triangle
// 	};

// 	unsigned int VBO, VAO, EBO;
// 	glGenVertexArrays(1, &VAO);
// 	glGenBuffers(1, &VBO);
// 	glGenBuffers(1, &EBO);

// 	glBindVertexArray(VAO);

// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// 	// position attribute
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);
// 	// color attribute
// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// 	glEnableVertexAttribArray(1);
// 	// texture coord attribute
// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
// 	glEnableVertexAttribArray(2);


// 	std::string tex_filepath = "./bxEngine/res/bx_logo-400.png";
// 	std::string tex_type = "diffuse";
// 	Texture texture1(tex_filepath, tex_type);

// 	while(true)
// 	{
// 		glClearColor(0.055f, 0.055f, 0.055f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 		/* -------- */
// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_2D, texture1.getID());
		
// 		shader->useProgram();
// 		glBindVertexArray(VAO);
// 		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
// 		render_context->update();
// 	}
// }