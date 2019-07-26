#include <iostream>
#include "core/bxProgram.h"
#include "core/Log.h"


namespace bbx {
const std::string BBX_VERSION = "v0.0.1";

void bxInit()
{
	Log::init();
	BBX_INFO(BBX_VERSION + ": " + " Launching");
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
	program->init();
	program->run();
	program->exit();
	BBX_CLI_INFO("Closing...");
}

