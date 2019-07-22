#pragma once
#include "render/RenderContext.h"
#include "render/Shader.h"

namespace bbx {
class Program
{
public:
	Program();
	~Program();

	void init();
	void run();
	void exit();

protected:
private:
	RenderContext* renderContext;

	Shader* shader;


};
}