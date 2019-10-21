#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace bx { 
class Camera
{
public:
	//Camera(glm::vec3& pos);
    Camera(glm::vec3 pos, float yaw, float pitch, float roll);
	~Camera();

	void move(glm::vec3 pos, glm::vec3 rot);

	glm::vec3& getPosition();
	float getZoom();
	float getRoll();
	float getPitch();
	float getYaw();
	
	void setZoom(float z);
	void update();

	glm::mat4& getProjectionMatrix();
	glm::mat4& getViewMatrix();

private:
	void generateProjectionMatrix();

	glm::vec3 position;
	//glm::vec3 rotation;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	float yaw;
	float pitch;
	float roll;
	float moveSpeed;
	float mouseSpeed;
	float zoom;

	const float FOV = 80.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;

};
}