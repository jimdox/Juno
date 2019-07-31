#include "core/bxProgram.h"


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

	GLuint shaderID = shader->getProgram();
	this->renderContext->setShader(shaderID);
}

void bxProgram::run()
{
	renderContext->splashImage();


}

void bxProgram::exit()
{
	renderContext->destroy();
}
