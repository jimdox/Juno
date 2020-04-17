#pragma once
#include "Src/Core/Program.h"
#include "Src/Core/Log.h"
#include "Src/Core/Config.h"

extern Juno::Program* CreateProgram();

namespace Juno {

void JunoInit()
{
	Log::Init();
	JN_INFO("v{} Launching...", JN_VERSION);
}
}


int main()
{
	Juno::JunoInit();
	auto program = CreateProgram();
	program->Init();
	program->Run(); 
	program->Exit();
	JN_INFO("Exiting...");
	return 0;
}


/* example of how to create your program */
// Program* CreateProgram()
// {	
// 	DerivedProgram* prog = new DerivedProgram();
// 	return prog;
// }