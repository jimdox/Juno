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

protected:
	std::unique_ptr<Context> render_context;
	Camera camera; 
	

	/* time-keeping vars. */
	float last_time;     
	float frame_time;
	float delta_time;
	u_int16_t num_frames;
	

private:
	std::shared_ptr<Shader> shader;

};
}