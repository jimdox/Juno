#include "core/Program.h"
#include "render/glRenderCore.h"
#include "core/Log.h"
#include "core/bxAssetLoader.h"
#include "render/Camera.h"
#include "render/lights/Light.h"
#include "render/RenderQueue.h"

using namespace bx;


Program::Program()
{
	init();
}

Program::~Program()
{

}

void Program::init()
{
	renderContext = std::make_unique<Context>(800, 800, "v0.0.1", false);
	shader = std::make_shared<Shader>("./bx_engine/res/shaders/basic");

	GLuint shaderID = shader->getID();
	this->renderContext->setShader(shaderID);
	BX_WARN(BX_GFX_DEVICE);

	camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 4.5f), 0.0f, 0.0f, 0.0f);
	Light light(glm::vec3(0,0,0), glm::vec3(0,0,0));


}

void Program::run()
{
	
	shader->useProgram();
	shader->loadProjectionMatrix(camera->getProjectionMatrix());				/* load the perspective matrix from Camera */
	shader->loadViewMatrix(camera);
	shader->unbindProgram();

	std::string objPath = "./bx_engine/res/dragon.obj";
	Mesh stall = bxImport::loadOBJ(objPath);
	
	std::string texFilePath = "./bx_engine/res/grey.png";
	std::string texType = "diffuse";
	Texture texture1(texFilePath, texType);
	stall.addTexture(&texture1, texType);

	glm::vec3 entPos(0.0f, -1.5f, -5.0f);
	glm::vec3 entRot(0.0f, 0.0f, 0.0f);
	std::string entName = "fordo";
	Entity fordo(stall, entPos, entRot, 0.6f, entName);

	glm::vec3 entBPos(6.0f, 0.0f, -4.4f);
	glm::vec3 entCPos(-6.0f, 0.0f, -5.4f);
	Entity entB(stall, entBPos, entRot, 1.0f, entName);
	Entity entC(stall, entCPos, entRot, 1.0f, entName);

	Light light(glm::vec3(113.5f, 0.01f, 30.0f), glm::vec3(0.91f, 0.41f, 0.41f));

	dt = 0;
	numFrames = 0; 

	RenderQueue queue(camera);
	queue.submit(&fordo, shader);
	queue.addLight(light);


	while (renderContext->isRunning())
	{
		fpsCounter();

		glm::vec3 d_rot(0.0f, 0.5f, 0.0f);
		fordo.addRotation(d_rot);

		bxRender::clear();

		queue.render();

		camera->move(glm::vec3(0.0f, 0.0f, 0.0005f), glm::vec3(0.0f, 0.0f, 0.0f));
		//camera->setZoom(camera->getZoom() + dt);
		camera->update();

		renderContext->update();
	}
}

void Program::exit()
{
	renderContext->destroy();
}

void Program::fpsCounter()
{
	currentTime = glfwGetTime();
	dt += (currentTime - lastTime);
	lastTime = currentTime;
	numFrames++;
	if(dt >= 1.0f)
	{
		BX_INFO(numFrames);
		dt = 0.0f;
		numFrames = 0;
	}
}


/* ------------- */
void Program::splashShader()
{
	this->shader = std::make_shared<Shader>("./bxEngine/res/shaders/gui");
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	std::string texFilePath = "./bxEngine/res/bx_logo-400.png";
	std::string texType = "diffuse";
	Texture texture1(texFilePath, texType);

	while(true)
	{
		glClearColor(0.055f, 0.055f, 0.055f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/* -------- */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.getID());
		
		shader->useProgram();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		renderContext->update();
	}
}
