#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace bbx { 
class Camera
{
public:
	Camera(glm::vec3 pos);
    Camera(glm::vec3 pos, glm::vec3 upDir, float yaw, float pitch);
	~Camera();

	glm::mat4 getViewMatrix();
	float getZoom();
	float getRoll();
	float getPitch();
	float getYaw();

	void update();


private:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 globalUp;

	float yaw;
	float pitch;
	float roll;
	float moveSpeed;
	float mouseSpeed;
	float zoom;

};
}