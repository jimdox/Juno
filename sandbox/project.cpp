#include "core/StartPoint.h"


class ExampleProgram : public juno::Program
{
	void onCreate(){}

	void onUpdate(){}
	
	void onDestroy(){}
};








juno::Program* createProgram()
{
 	juno::Program* prog = new juno::Program();
 	return prog;
}