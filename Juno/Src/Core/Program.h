#pragma once
#include "Events/Events.h"
#include "Renderer/Renderer.h"
#include "Core/Scene.h"
#include "Gui/Dock.h"
namespace Juno {
class Program : public EventListener
{
public:
	Program();
	~Program();

	void Init();
	void Run();
	void Exit();

	virtual void OnUserCreate() = 0;
	virtual void OnUserUpdate() = 0;
	virtual void OnUserUpdateWindow() = 0;
	virtual void OnUserExit() = 0;

	void OnAttach();
	void OnEvent(const Event& e);

protected:
	bool ProgramShouldClose();
	float frameTime;

private:
	void RunExampleParticleSim();
	void RunExamplePhysics();
	void RunExampleGeneral();
	Renderer* renderer;
	SPtr<Scene> scene;
	Dock dock;
	float lastTime;     
	float deltaTime;
	bool f_programShouldClose;
};
}