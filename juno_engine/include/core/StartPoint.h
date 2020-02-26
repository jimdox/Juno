#include <iostream>
#include "core/Program.h"
#include "core/Log.h"

extern juno::Program* createProgram();

namespace juno {
const std::string JN_VERSION = "v0.0.1.1";

void junoInit()
{
	Log::init();
	JN_INFO("{}: Launching", JN_VERSION);
}

// Program* createProgram()
// {	
// 	Program* prog = new Program();
// 	return prog;
// }

}


int main()
{
	juno::junoInit();
	auto program = createProgram();
	program->init();
	program->run(); 
	program->exit();
	JN_CLI_INFO("Closing...");
	return 0;
}
