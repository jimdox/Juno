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
	render_context = std::make_unique<Context>(1400, 1000, "v0.0.1.1", false);
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
	Texture texture1("./bx_engine/res/grey.png", TX_DIFFUSE, true);
	stall.addTexture(&texture1, texture1.getTexType());

	Mesh plane = bxImport::loadModel("./bx_engine/res/plane.obj");
	Texture texture_two("./bx_engine/res/suse.png", TX_DIFFUSE, true);
	plane.addTexture(&texture_two, texture_two.getTexType());

	glm::vec3 ent_pos(0.0f, -1.5f, -8.0f);
	glm::vec3 ent_rot(0.0f, 180.0f, 0.00f);

	glm::vec3 ground_pos(0.0f, -2.0f, -1.0f);
	glm::vec3 ground_rot(0.0f, 0.0f, 0.0f);

	Entity entity_one(stall, ent_pos, ent_rot, 0.45f, "Entity One");
	Entity entity_two(plane, ground_pos, ground_rot, 70, "Entity Two");
	Material gr_mat;
	gr_mat.reflectivity = 0.0f;
	gr_mat.shineDamper = 1.0f;
	entity_two.getMesh().setMaterial(gr_mat);
	// glm::vec3 entBPos(6.0f, 0.0f, -4.4f);
	// glm::vec3 entCPos(-6.0f, 0.0f, -5.4f);

	// Entity entB(stall, entBPos, ent_rot, 1.0f, entName);

	// std::shared_ptr terrain_shader = std::make_shared<TerrainShader>("./bx_engine/res/shaders/basic");
	// Terrain terrain(0, 0, texture1);


	Light light(glm::vec3(110.5f, 100.01f, 100.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(1.0, 0.00001, 0.0005));
	Light light_b(glm::vec3(-30.5f, 10.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.9f), glm::vec3(1.0, 0.0001, 0.0005));
	
	RenderQueue queue;
	queue.submit(&entity_one, shader);
	queue.submit(&entity_two, shader);
	queue.addLight(light);
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
		
		render_context->update(frame_time);
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