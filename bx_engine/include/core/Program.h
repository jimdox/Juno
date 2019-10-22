#pragma once
#include "render/Context.h"
#include "render/shaders/Shader.h"
#include <memory>

namespace bx {
class Program
{
public:
	Program();
	~Program();

	void init();
	void run();
	void exit();

	void splashShader();
	void fpsCounter();
	// todo: config-file manager for core engine

protected:
private:
	std::unique_ptr<Context> renderContext;
	std::shared_ptr<Shader> shader;
	Camera camera; 
	

	/* time-keeping vars. */
	float dt;
	float currentTime;
	float lastTime;     
	unsigned int numFrames;


};
}