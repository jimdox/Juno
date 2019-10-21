#include <iostream>
#include "core/Program.h"
#include "core/Log.h"

/**/
namespace bx {
const std::string BX_VERSION = "v0.0.1";

void bxInit()
{
	Log::init();
	BX_INFO("{}: Launching", BX_VERSION);
}

Program* createProgram()
{	
	Program* prog = new Program();
	return prog;
}

}


int main()
{
	bx::bxInit();
	auto program = bx::createProgram();
	program->run();
	program->exit();
	BX_CLI_INFO("Closing...");
}

