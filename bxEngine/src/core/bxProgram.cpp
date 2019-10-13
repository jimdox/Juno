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
	this->renderContext = new bxContext(1070, 1070, "v0.0.0", false);
	this->shader = new Shader("./bxEngine/res/shaders/test");

	GLuint shaderID = shader->getID();
	this->renderContext->setShader(shaderID);
}

void bxProgram::run()
{
	BBX_WARN(BX_GFX_DEVICE);

	Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));


	//splashShader();
	std::string objPath = "./bxEngine/res/stall/stall.obj";
	Mesh *stall = bxImport::loadOBJ(objPath);

	std::string texFilePath = "./bxEngine/res/fordo.png";
	std::string texType = "diffuse";
	Texture texture1(texFilePath, texType);
	stall->addTexture(&texture1);


	float dt;
	float currentTime;
	float lastTime = 0.0f;
	

	while (renderContext->isRunning())
	{
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;

		bxRender::clear();
	
		
		glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), ((float)renderContext->getWidth())/((float)renderContext->getHeight()), 0.1f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader->setMat4("projection", projection);
		shader->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.2f));
		shader->setMat4("model", model);
		shader->useProgram();
		

		bxRender::render(stall, *shader);


		renderContext->update();
	}

}

void bxProgram::exit()
{
	renderContext->destroy();
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


	std::string texFilePath = "./bxEngine/res/fordo.png";
	std::string texType = "diffuse";
	Texture texture1(texFilePath, texType);

	while(true)
	{
		bxRender::clear();

		/* -------- */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.getID());
		
		shader->useProgram();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		renderContext->update();
	}
}
