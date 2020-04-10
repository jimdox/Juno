#include "core/Program.h"
#include "core/Log.h"
#include "EngineConfig.h"
extern juno::Program* createProgram();

namespace juno {

void junoInit()
{
	Log::init();
	JN_INFO("ver {} -- Launching...", JN_VERSION);
}
}


int main()
{
	juno::junoInit();
	auto program = createProgram();
	program->init();
	program->run(); 
	program->exit();
	JN_INFO("Closing...");
	return 0;
}

/* example of how to init engine */
// Program* createProgram()
// {	
// 	DerivedProgram* prog = new DerivedProgram();
// 	return prog;
// }