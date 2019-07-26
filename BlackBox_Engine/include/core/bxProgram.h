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

protected:
private:
	bxContext* renderContext;

	Shader* shader;


};
}