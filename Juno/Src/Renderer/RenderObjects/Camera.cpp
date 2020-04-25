#include "jnpch.h"
#include "Renderer/RenderObjects/Camera.h"
#include "Core/Config.h"
#include "Core/Log.h"
#include "Utils/Math.h"
#include "Events/RenderEvents.h"
#include <GLFW/glfw3.h>
using namespace Juno;

Camera::Camera(const glm::vec3& pos, const glm::vec3& rot) : pivot(pos)
{
	this->roll = Math::ToRadians(rot.x);
	this->pitch = Math::ToRadians(rot.y);
	this->yaw = Math::ToRadians(rot.z);
	this->zoom = 0.0f;
	position = glm::vec3(0,0,0);
	velocity = glm::vec3(0,0,0);
	
	GenerateProjectionMatrix();
}

Camera::~Camera()
{
	
}

void Camera::OnAttach()
{

}

void Camera::OnEvent(const Event& e)
{

}


// void Camera::Move(glm::vec3& pos, glm::vec3& dRot)
// {

// }

// void Camera::Update(float dt)
// {

// 	CalculatePosition(dt);
// }

// /* 3rd person camera */
// void Camera::CalculatePosition(float dt)
// {

// }


Mat4 Camera::GetViewMatrix()
{
	return Math::GenerateViewMatrix({position.x, position.y, position.z}, {pitch, yaw, roll});
}

const Mat4& Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

Mat4 Camera::GetViewProjectionMatrix()
{
	return viewMatrix * projectionMatrix;
}

void Camera::GenerateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV+zoom), camAspectRatio, NEAR_PLANE, FAR_PLANE);	
}

// glm::mat4& Camera::ResetProjectionMatrix()
// {
// 	GenerateProjectionMatrix();
// 	return projectionMatrix;
// }

void Camera::SetPivot(glm::vec3& pivot)
{
	this->pivot = pivot;
}

glm::vec3& Camera::GetPosition()
{
	return position;
}

void Camera::SetZoom(float z)
{
	if(z < MAX_ZOOM)
	{
		this->zoom = MAX_ZOOM;
	} else if(z > MIN_ZOOM) {
		this->zoom = MIN_ZOOM;
	} else {
	this->zoom = z;
	}
}