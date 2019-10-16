#include <iostream>
#include "core/bxProgram.h"
#include "core/Log.h"

/**/
namespace bbx {
const std::string BX_VERSION = "v0.0.1";

void bxInit()
{
	Log::init();
	BX_INFO(BX_VERSION + ": " + " Launching");
}

bxProgram* createProgram()
{	
	bxProgram* prog = new bxProgram();
	return prog;
}

}


int main()
{
	bbx::bxInit();
	auto program = bbx::createProgram();
	program->run();
	program->exit();
	BX_CLI_INFO("Closing...");
}

