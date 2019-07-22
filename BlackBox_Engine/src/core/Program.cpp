#include "core/Program.h"


using namespace bbx;


Program::Program()
{
	// setup config file fetching here
}

Program::~Program()
{

}

void Program::init()
{
	this->renderContext = new RenderContext(440, 440, " ", false);
	//renderContext->init();
	this->shader = new Shader();
	BBX_INFO("Loading shaders");
	shader->loadShader();
	BBX_INFO("Shaders loaded.");

	GLuint shaderID = shader->getProgram();
	this->renderContext->setShader(shaderID);
}

void Program::run()
{
	renderContext->splashImage();


}

void Program::exit()
{
	renderContext->destroy();
}
