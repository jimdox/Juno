#pragma once
#include "Events/Events.h"

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

	float lastTime;     
	float deltaTime;
	bool f_programShouldClose;
};
}