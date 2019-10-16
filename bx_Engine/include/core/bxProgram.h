#pragma once
#include "render/bxContext.h"
#include "render/Shader.h"
#include <memory>
namespace bbx {
class bxProgram
{
public:
	bxProgram();
	~bxProgram();

	void init();
	void run();
	void exit();

	void splashShader();
	void fpsCounter();
	// todo: config-file manager for core engine

protected:
private:
	std::unique_ptr<bxContext> renderContext;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Camera> camera; 
	

	/* time-keeping vars. */
	float dt;
	float currentTime;
	float lastTime;     
	unsigned int numFrames;


};
}