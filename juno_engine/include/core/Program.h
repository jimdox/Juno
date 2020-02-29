#pragma once
#include "render/Renderer.h"
#include "core/AssetLoader.h"
#include "events/RenderEvent.h"

namespace juno {
class Program : public EventListener
{
public:
	Program();
	~Program();

	void init();
	void run();
	void exit();

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;
	virtual void onUpdate() = 0;
	virtual void onFrameBufferUpdate() = 0;

	

	void onAttach();
	void onEvent(const Event& e);

	Scene& getScene();



protected:
	bool programShouldClose();

	Renderer *renderer;
	Scene scene;
	/* time-keeping vars. */
	float last_time;     
	float frame_time;
	float delta_time;
	u_int16_t num_frames;
	

private:
	std::shared_ptr<Shader> shader;
	bool f_program_should_close;
};
}