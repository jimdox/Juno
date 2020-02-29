#include "core/StartPoint.h"

using namespace juno;

class SandboxProgram : public juno::Program
{
	void onCreate()
	{
		renderer = &Renderer::init(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,0,0));
		

		Mesh susy = loadModel("./juno_engine/res/asteroid1.obj");
		Texture texture_one("./juno_engine/res/grey.png", TX_DIFFUSE, true);
		susy.addTexture(texture_one);
		Entity entity_one(susy, glm::vec3(0.0f, 2.8f, -8.0f), glm::vec3(0,0,0), 4.75f, "Entity One");
		
		Mesh plane = loadModel("./juno_engine/res/terrain.obj");
		Texture texture_two("./juno_engine/res/green.png", TX_DIFFUSE, true);
		plane.addTexture(texture_two);
		Entity entity_two(plane, glm::vec3(0,0,0), glm::vec3(0,0,0), 75.0f, "Default Plane");


		Light light(glm::vec3(110.5f, 100.01f, 100.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(1.0, 0.00001, 0.0005));
		Light light_b(glm::vec3(-30.5f, 10.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.9f), glm::vec3(1.0, 0.0001, 0.0005));
		
		scene.addEntity(entity_one);
		scene.addEntity(entity_two);
		scene.addLight(light);
		scene.addLight(light_b);
		renderer->submit(scene);
	}

	void onDestroy()
	{
		
	}

	void onUpdate()
	{

	}

	void onFrameBufferUpdate()
	{

	}

};








juno::Program* createProgram()
{
 	juno::Program* prog = new SandboxProgram();
 	return prog;
}