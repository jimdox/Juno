#pragma once
#include "render/Context.h"
#include "render/Shader.h"
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
	std::shared_ptr<Camera> camera; 
	

	/* time-keeping vars. */
	float dt;
	float currentTime;
	float lastTime;     
	unsigned int numFrames;


};
}