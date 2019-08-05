#include "core/bxProgram.h"
#include "render/glRenderCore.h"
#include "core/Log.h"
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
	this->shader = new Shader();
	BBX_INFO("Loading shaders");
	shader->loadShader();
	BBX_INFO("Shaders loaded.");

	GLuint shaderID = shader->getID();
	this->renderContext->setShader(shaderID);
}

void bxProgram::run()
{
	BBX_WARN(BX_GFX_DEVICE);
	bxRender::init();
	//renderContext->splashImage();
	while (renderContext->isRunning())
	{
		bxRender::clear();
		bxRender::instancedRender






		//renderContext->update();
	}

}

void bxProgram::exit()
{
	renderContext->destroy();
}
