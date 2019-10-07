#include "core/bxProgram.h"
#include "render/glRenderCore.h"
#include "core/Log.h"
#include "core/bxAssetLoader.h"

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
	this->renderContext = new bxContext(670, 670, " ", false);
	this->shader = new Shader("/home/dox/dev/BlackBox/bxEngine/res/shaders/basic");

	GLuint shaderID = shader->getID();
	this->renderContext->setShader(shaderID);
}

void bxProgram::run()
{
	BBX_WARN(BX_GFX_DEVICE);
	
	Vertex vertex;
	vertex.position = glm::vec3(0.2f, 0.4f, 0.6f);
	vertex.normal = glm::vec3(0.2f, 0.4f, 0.6f);
	vertex.textureCoord = glm::vec2(1.0f, 0.0f);
	

	while (renderContext->isRunning())
	{
		bxRender::clear();


		renderContext->update();
	}

}

void bxProgram::exit()
{
	renderContext->destroy();
}
