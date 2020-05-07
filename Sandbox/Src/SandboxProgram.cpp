#include <Juno.h>
#include <StartPoint.h>
#include "Utils/Memory.h"
#include "Physics/Particle.h"
using namespace Juno;

//static GLComputeShader* g_computeShader;

class SandboxProgram : public Program
{ 
	void OnUserCreate()
	{
		// SPtr<Mesh> plane = AssetManager::Get().LoadMesh("./Resources/Models/plane.obj");
		// SPtr<Mesh> dragon = AssetManager::Get().LoadMesh("./Resources/Models/dragon.obj");

		// plane->GetMaterial()->SetBaseColor({0.1f, 0.1f, 0.1f});
		// plane->GetMaterial()->SetReflectivity(0.12);
		
		// SPtr<Scene> scene = SceneManager::Get().GetScene();

		// scene->CreateEntity(plane, {0, -5.5, 0}, {0,0,0}, 100.0f, "Default Plane");
		// scene->CreateEntity(dragon, {60, -5, -30}, {0,-50, 0}, 5.0f, "Big Boi");
		// scene->CreateLight({1000, 1000, 1000}, {0.5f, 0.5f, 1.0f});
		// scene->CreateLight({-1000, 1000, 0}, {1,1,1});
		// Renderer::Get().Submit(scene);

		/* sample code for how compute shaders can be used */
		// std::vector<Particle> particles;
		// for(unsigned int i = 0; i < 10000; i++)
		// {
		// 	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		// 	float x = 100.0f * r;
		// 	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		// 	float y = 100 * r2;
		// 	float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		// 	float z =  100 * r3 - 50;
		// 	glm::vec3 position(x, y, z);
		// 	glm::vec3 velocity(0, 0, 0);   
		// 	particles.emplace_back(position, velocity, 0.0001f);
		// }

		// g_computeShader = new GLComputeShader(particles, "./Resources/Shaders/ComputeShaders/example", 512);
		

		//renderer->getWindow().getGuiDock().enableDebug();
	}

	/* updates every cycle */
	void OnUserUpdate()
	{
		//delta_x += 0.001f;
	
	}

	/* updates on each new frame */
	void OnUserUpdateWindow()
	{
		// Renderer::Get().RunComputeShader(g_computeShader, frameTime);

	}


	void OnUserExit()
	{
		/* mem. cleanup */
		//delete g_computeShader;
		
	}

private:
	float delta_x = 0.0f;
	float counter = 0;
};





Juno::Program* CreateProgram()
{	
	SandboxProgram* prog = new SandboxProgram();
	return prog;
}