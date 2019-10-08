#pragma once
#include "render/bxContext.h"
#include "render/Shader.h"
namespace bbx {
class bxProgram
{
public:
	bxProgram();
	~bxProgram();

	void init();
	void run();
	void exit();

	void splashShader();
	// todo: config-file manager for core engine

protected:
private:
	bxContext* renderContext;
	Shader* shader;


};
}