#include "render/Camera.h"
#include "core/EngineConfig.h"
#include "core/Log.h"
#include "core/bxMath.h"
#include "core/InputStates.h"

using namespace bx;



Camera::Camera(glm::vec3 pos, float yaw, float pitch, float roll) : position(pos)
{

	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
	this->moveSpeed = 2.5f;
	this->mouseSpeed = 0.1f;
	this->zoom = 0.0f;
	generateProjectionMatrix();
	velocity = glm::vec3(0,0,0);

}

Camera::~Camera()
{

}

void Camera::setPivot(glm::vec3* Pivot)
{
	this->pivot = *Pivot;
}

void Camera::move(glm::vec3& pos, glm::vec3& dRot)
{
	 this->position.x = pos.x * sinf(bxMath::toRadians(dRot.y));
	 this->position.z = pos.z * cosf(bxMath::toRadians(dRot.y));
	 this->pitch = dRot.y;
	 this->yaw = dRot.z;
	 this->roll = dRot.x;
}


void Camera::update(glm::vec3& dPos, glm::vec3& dRot, float deltaZoom)
{
	this->velocity = dPos;

	this->pivot.x += velocity.x * cosf(bxMath::toRadians(dRot.y));
	this->pivot.z += velocity.z * cosf(bxMath::toRadians(dRot.y)); 
	this->roll += dRot.x;
	//this->pitch += dRot.y;
	this->yaw += dRot.z;

	/* update camera pivot elements */
	float mouse_dx = CURSOR_DX;
	setZoom(SCROLL_Y);
	distanceToPivot += SCROLL_DY * 1;
	pitch += CURSOR_DY * 0.3; 
	MOUSE_INPUT_RECIEVED = true;
	calculateCameraPos();


	if(LMB_PRESSED || true)
	{
		angle_around_pivot -= mouse_dx * 0.3f;
	}

}

void Camera::updateZoom()
{
	//distanceToPivot += SCROLL_DY;
}

void Camera::updatePitch()
{

}

void Camera::updateYaw()
{

}


/* angle must be in radians */
void Camera::calculateCameraPos()
{
	float rad_theta = bxMath::toRadians(pitch);
	float dist_x = ((float) distanceToPivot * cos(rad_theta));
	float dist_y = ((float) distanceToPivot * sin(rad_theta));
	
	float cam_angle = pivot.y + angle_around_pivot;
	float cam_x_offset = dist_x * sin(bxMath::toRadians(cam_angle));
	float cam_z_offset = dist_x * cos(bxMath::toRadians(cam_angle));

	position.x = pivot.x - cam_x_offset;
	position.y = pivot.y + dist_y;
	position.z = pivot.z - cam_z_offset;
	
}

glm::vec3& Camera::getPosition()
{
	return this->position;
}

float Camera::getZoom()
{
	return this->zoom;
}

float Camera::getRoll()
{
	return this->roll;
}

float Camera::getPitch()
{
	return this->pitch;
}

float Camera::getYaw()
{
	return this->yaw;
}

void Camera::setZoom(float z)
{
	BX_ERR("zoom {}", z);
	this->zoom = z;
}

glm::mat4& Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

void Camera::generateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV+zoom), getAspectRatio(), NEAR_PLANE, FAR_PLANE);	
}
