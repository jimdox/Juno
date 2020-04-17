#pragma once
#include "render/Renderer.h"
#include "core/AssetManager.h"
#include "events/RenderEvent.h"
#include "entity/SceneManager.h"

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
	virtual void onUpdate() = 0;
	virtual void onFrameBufferUpdate() = 0;
	virtual void onExit() = 0;

	void onAttach();
	void onEvent(const Event& e);

	Scene& getScene();

protected:
	bool programShouldClose();

	Renderer* renderer;
	AssetManager* assetManager; 
	Scene& scene;
	/* time-keeping vars. */
	float last_time;     
	float frame_time;
	float delta_time;
	u_int16_t num_frames;

private:
	void runExampleParticleSim();
	void runExamplePhysics();
	void runExampleGeneral();

	bool f_program_should_close;
};
}