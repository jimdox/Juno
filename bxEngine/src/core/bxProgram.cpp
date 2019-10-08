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
	this->renderContext = new bxContext(1070, 1070, " ", false);
	this->shader = new Shader("/home/dox/dev/BlackBox/bxEngine/res/shaders/test");

	GLuint shaderID = shader->getID();
	this->renderContext->setShader(shaderID);
}

void bxProgram::run()
{
	BBX_WARN(BX_GFX_DEVICE);
	std::string entityPath = "/home/dox/dev/BlackBox/bxEngine/res/nanosuit/nanosuit.obj";
	Entity testEntity(entityPath);
	Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));




	while (renderContext->isRunning())
	{


		bxRender::clear();

		glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), ((float)renderContext->getWidth())/((float)renderContext->getHeight()), 0.1f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader->setMat4("projection", projection);
		shader->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shader->setMat4("model", model);
		
		bxRender::renderEntity(testEntity, *shader);


		renderContext->update();
	}

}

void bxProgram::exit()
{
	renderContext->destroy();
}
