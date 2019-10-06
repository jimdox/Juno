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

	//GLuint shaderID = shader->getID();
	//this->renderContext->setShader(shaderID);
}

void bxProgram::run()
{
	BBX_WARN(BX_GFX_DEVICE);
	//bxRender::init();

	
	std::vector<Entity> entities;
	//loadOBJ("res/Artemis.obj");
	//renderContext->splashImage();

	while (renderContext->isRunning())
	{
		bxRender::clear();
		//bxRender::instancedRender(entities, shader, renderContext);






		renderContext->update();
	}

}

void bxProgram::exit()
{
	renderContext->destroy();
}
