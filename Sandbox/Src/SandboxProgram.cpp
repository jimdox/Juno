#define JN_PLATFORM_LINUX
#define JN_RENDERER_OPENGL
#include <Juno.h>
#include <StartPoint.h>

using namespace Juno;

static ComputeShader* g_computeShader;

class SandboxProgram : public Juno::Program
{
	void OnUserCreate()
	{
		Mesh plane = AssetManager::Get().LoadMesh("./Resources/Models/plane.obj");
		Mesh dragon = AssetManager::Get().LoadMesh("./Resources/Models/dragon.obj");

		plane.GetMaterial().SetBaseColor(Vec3(0.1f, 0.1f, 0.1f));
		plane.GetMaterial().SetReflectivity(0.12);

		dragon.GetMaterial().SetBaseColor(Vec3(0.4f, 0.1f, 0.1f));
		dragon.GetMaterial().SetShineDamper(0.99f);
		//dragon.GetMaterial().SetReflectivity(0.25f);
		
		Entity entityOne (plane, Vec3(0.0f, -5.5f, 0.0f), Vec3(0,0, 0), 100.0f, "default plane");
		Entity entityTwo (dragon, Vec3(60, -5, -30), Vec3(0,-50,0), 5.0f, "Big Boi");
		
		Light lightA(Vec3(-110.5f, 500.01f, 100.0f), Vec3(0.9f, 0.9f, 0.9f), Vec3(1.0, 1, 0.005));
		Light lightB(Vec3(300,100,-1010), Vec3(0.4f, 0.4f, 0.70f), Vec3(1.0, 0.00001f, 0.0005f));

		// scene.add(entityOne);
		// scene.add(entityTwo);
		// scene.add(lightA);
		// scene.add(lightB);
		Renderer::Get().Submit(scene);

		/* sample code for how compute shaders can be used */
		std::vector<Particle> particles;
		for(unsigned int i = 0; i < 100000; i++)
		{
			float x = 100 * sinf(i) * cosf(i) + 60;
			float z = 100 * cosf(i) * cosf(i) - 90;
			float y = 100 * sinf(i * M_PI/300) + 50;
			glm::vec3 position(x,y,z);
			glm::vec3 velocity(sinf(2 * M_PI * i) + cosf(-i), cosf(i/(2 * M_PI)), sinf(2 * M_PI / i));   
			particles.emplace_back(position, velocity, 1.0f);
		}

		g_computeShader = new ComputeShader(particles, "./Resources/Shaders/ComputeShaders/example", 512);
		

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
		Renderer::Get().RunComputeShader(*g_computeShader, frameTime);

	}


	void OnUserExit()
	{
		/* mem. cleanup */
		delete g_computeShader;
		
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