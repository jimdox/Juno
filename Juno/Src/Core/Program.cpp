#include "jnpch.h"
#include "Core/Program.h"
#include "Core/Log.h"
#include "Renderer/Renderer.h"
#include "Core/AssetManager.h"
#include "Core/SceneManager.h"
#include "Events/RenderEvents.h"

using namespace Juno;

Program::Program() 
{
	this->f_programShouldClose = false;
	frameTime = 0;
	deltaTime = 0;
}	

Program::~Program()
{

}

void Program::Init()
{
	AssetManager* assetManager = &AssetManager::Get();
	Renderer* renderer = &Renderer::Get();

	renderer->GetWindow().GetWinEventDispatcher().AddListener(this);
	renderer->GetCamera().AddListener(this);
	OnUserCreate();

}

void Program::OnAttach()
{
	
}

void Program::OnEvent(const Event& e)
{
	switch(e.GetType())
	{
	case EventType::WINDOW_CLOSE:
		this->f_programShouldClose = true;
		break;

	case EventType::WINDOW_RESIZE:
		// RenderCall::ResizeFrameBuffer(((const WindowResizeEvent&)e).GetScreenWidth(), ((const WindowResizeEvent&)e).GetScreenHeight());
		break;
	case EventType::RENDER_POLYGON_WIREFRAME:
		// RenderCall::SetModeWireframe(((const RenderWireFrameEvent&)e).IsModeWireFrame());
		break;
	default:
		break;
	}
}

void Program::Run()
{
	deltaTime = 0;
	frameTime = 0;
	lastTime = glfwGetTime();

	while (!ProgramShouldClose())
	{
		OnUserUpdate();

		if(frameTime > (1/120.0f))
		{
			Renderer::Get().Begin(frameTime);
			OnUserUpdateWindow();
			Renderer::Get().End(frameTime);
			frameTime = 0;
		}

		deltaTime = (glfwGetTime() - lastTime);
		lastTime = glfwGetTime();
		frameTime += deltaTime;
	}
}

bool Program::ProgramShouldClose()
{
	return f_programShouldClose || !Renderer::Get().GetWindow().IsRunning();
}

void Program::Exit()
{

}









/* placeholders for proper scene and context switching system */
// void Program::RunExampleParticleSim()
// {
// 	SceneManager::Get().GetScene().Clear();
// 	deltaTime = 0;
// 	frameTime = 0;
// 	lastTime = glfwGetTime();
	
// 	std::vector<Particle> particles;
	
// 	for(unsigned int i = 0; i < 10000; i++)
//     {
//         glm::vec3 p (100 * cosf(i), 100*sin(i)*cosf(i), 100 * sinf(i * M_PI/300));
//         glm::vec3 v (sinf(2 * M_PI * i) + cosf(-i), cosf(i/(2 * M_PI)), sinf(2 * M_PI / i));   
//         particles.emplace_back(p, v, 1.0f);
//     }
	
// 	ComputeShader cs(particles, "./Resources/Shaders/ComputeShaders/example", 512);

// 	while (!ProgramShouldClose())
// 	{

// 		if(frameTime > (1/120.0f))
// 		{
// 			renderer->Begin(frameTime);
// 			renderer->RunComputeShader(cs, frameTime);
// 			renderer->End(frameTime);
// 			frameTime = 0;
// 		}

// 		deltaTime = (glfwGetTime() - lastTime);
// 		lastTime = glfwGetTime();
// 		frameTime += deltaTime;
// 	}
// }

// void Program::RunExamplePhysics()
// {

// }

// void Program::RunExampleGeneral()
// {
// 	SceneManager::Get().GetScene().Clear();
// 	deltaTime = 0;
// 	frameTime = 0;
// 	lastTime = glfwGetTime();

// 	Mesh plane = AssetManager::Get().LoadMesh("./Resources/Models/plane.obj");
// 	Mesh susy = AssetManager::Get().LoadMesh("./Resources/Models/susanne.obj");
// 	Mesh dragon = AssetManager::Get().LoadMesh("./Resources/Models/dragon.obj");

// 	plane.GetMaterial().SetBaseColor(glm::vec3(0.1f, 0.1f, 0.1f));
// 	plane.GetMaterial().SetReflectivity(0.12);
// 	dragon.GetMaterial().SetBaseColor(glm::vec3(0.4f, 0.1f, 0.1f));
// 	dragon.GetMaterial().SetShineDamper(0.99f);
// 	dragon.GetMaterial().SetReflectivity(0.25f);
	
// 	Entity entityOne (plane, glm::vec3(0.0f, -5.5f, 0.0f), glm::vec3(0,0, 0), 100.0f, "default plane");
// 	Entity entityTwo (susy,  glm::vec3(-30.0f, 1.0f, -5.0f), glm::vec3(-50,40,0), 5.0f, "susanne 0");
// 	Entity entityThree(susy, glm::vec3(-50, 1.0f, 5.0f), glm::vec3(-9, 90, 0), 6.0f, "susanne 1");
// 	Entity entityFour(susy, glm::vec3(40, 1.0f, 5.0f), glm::vec3(12, 30, 20), 6.5f, "susanne 2");

// 	Entity entityDragon (dragon, glm::vec3(60, -5, -30), glm::vec3(0,-50,0), 5.0f, "Big Boi");
	
// 	Light light(glm::vec3(-110.5f, 500.01f, 100.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(1.0, 1, 0.005));
// 	Light lightB(glm::vec3(300,100,-1010), glm::vec3(0.4f, 0.4f, 0.70f), glm::vec3(1.0, 0.00001f, 0.0005f));

// 	scene.Add(entityOne);
// 	scene.Add(entityTwo);
// 	scene.Add(entityThree);
// 	scene.Add(entityFour);
// 	scene.Add(entityDragon);


// 	scene.Add(light);
// 	scene.Add(lightB);

// 	while (!ProgramShouldClose())
// 	{

// 		if(frameTime > (1/120.0f))
// 		{
// 			renderer->Begin(frameTime);

// 			renderer->End(frameTime);
// 			frameTime = 0;
// 		}

// 		deltaTime = (glfwGetTime() - lastTime);
// 		lastTime = glfwGetTime();
// 		frameTime += deltaTime;
// 		//Exit();
// 	}
// }


