#include "core/bxProgram.h"
#include "render/glRenderCore.h"
#include "core/Log.h"
#include "core/bxAssetLoader.h"
#include "render/Camera.h"

using namespace bbx;


bxProgram::bxProgram()
{
	init();
}

bxProgram::~bxProgram()
{

}

void bxProgram::init()
{
	this->renderContext = new bxContext(600, 600, "v0.0.1", false);
	this->shader = new Shader("./bxEngine/res/shaders/basic");

	GLuint shaderID = shader->getID();
	this->renderContext->setShader(shaderID);
	BBX_WARN(BX_GFX_DEVICE);

}

void bxProgram::run()
{
	Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 4.5f), 0.0f, 0.0f, 0.0f);
	shader->useProgram();
	shader->loadProjectionMatrix(camera->getProjectionMatrix());				/* load the perspective matrix from Camera */
	shader->loadViewMatrix(*camera);
	shader->unbindProgram();
	

	std::string objPath = "./bxEngine/res/susanne.obj";
	Mesh stall = bxImport::loadOBJ(objPath);

	
	std::string texFilePath = "./bxEngine/res/suse.png";
	std::string texType = "diffuse";
	Texture texture1(texFilePath, texType);
	stall.addTexture(&texture1);

	glm::vec3 entPos(0.0f, 0.0f, -3.0f);
	glm::vec3 entRot(0.0f, 0.0f, 0.0f);
	std::string entName = "fordo";

	Entity fordo(stall, entPos, entRot, 1.0f, entName);


	//splashShader();
	dt = 0;
	numFrames = 0; 


	while (renderContext->isRunning())
	{
		fpsCounter();

		glm::vec3 d_rot(0.0f, 0.5f, 0.0f);
		fordo.addRotation(d_rot);

		bxRender::clear();

		shader->useProgram();
		shader->loadViewMatrix(*camera);
		bxRender::renderEntity(fordo, *shader);
		shader->unbindProgram();




		//camera->setZoom(camera->getZoom() + dt);
		//camera->update();

		renderContext->update();
	}

}

void bxProgram::exit()
{
	renderContext->destroy();
}

void bxProgram::fpsCounter()
{
	currentTime = glfwGetTime();
	dt += (currentTime - lastTime);
	lastTime = currentTime;
	numFrames++;
	if(dt >= 1.0f)
	{
		BBX_INFO(numFrames);
		dt = 0.0f;
		numFrames = 0;
	}
}


/* ------------- */
void bxProgram::splashShader()
{
	this->shader = new Shader("./bxEngine/res/shaders/gui");
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
