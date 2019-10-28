#pragma once
#include "pch.h"
#include "render/Context.h"
#include "render/shaders/Shader.h"

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
	std::unique_ptr<Context> render_context;
	std::shared_ptr<Shader> shader;
	Camera camera; 
	

	/* time-keeping vars. */
	float dt;
	float last_time;     
	float frame_time;
	u_int16_t num_frames;


};
}