#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define BX_PLATFORM_WIN
#define BX_API_OPENGL
#include "core/Program.h"
#include "core/Log.h"


namespace bbx {
const std::string BBX_VERSION = "v0.0.1";

void init()
{
	Log::init();
	BBX_INFO(BBX_VERSION + ": " + " Launching");
}

Program* createProgram()
{

	return new Program();
}
}


int main()
{
	bbx::init();
	auto program = bbx::createProgram();
	program->run();
	program->exit();
	delete program;
}

