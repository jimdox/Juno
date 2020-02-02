#pragma once
#include "render/Context.h"
#include "render/shaders/Shader.h"


//int main(int argc, char** argv);

namespace juno {
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