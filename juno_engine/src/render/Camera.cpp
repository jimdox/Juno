#include "render/Camera.h"
#include "core/EngineConfig.h"
#include "core/Log.h"
#include "core/jnMath.h"
//#include "core/InputStates.h"
//#include "render/Context.h"
using namespace juno;



Camera::Camera(glm::vec3 pos, float yaw, float pitch, float roll) : position(pos)
{

	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
	this->moveSpeed = 2.5f;
	this->mouseSpeed = 0.1f;
	this->zoom = 0.0f;
	mouse_delta_x = 0;
	mouse_delta_y = 0;
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
	 this->position.x = pos.x * sinf(jnMath::toRadians(dRot.y));
	 this->position.z = pos.z * cosf(jnMath::toRadians(dRot.y));
	 this->pitch = dRot.y;
	 this->yaw = dRot.z;
	 this->roll = dRot.x;
}


void Camera::update(glm::vec3& dPos, glm::vec3& dRot, float deltaZoom, MouseInputData &mouse_data)
{
	//this->velocity = dPos;
	this->roll += dRot.x;
	this->pitch += dRot.y;
	this->yaw += dRot.z;
	this->position.x += dPos.x; // * sinf(jnMath::toRadians(yaw)); // * cosf(jnMath::toRadians(yaw)) + velocity.z * sinf(jnMath::toRadians(yaw));
	this->position.y += dPos.y;
	this->position.z += dPos.z;
	this->pivot = position;
	/* update camera pivot elements */
	setZoom(mouse_data.SCROLL_Y * -5);
	//distanceToPivot = mouse_data.SCROLL_Y * 1;


	if(mouse_data.LM_BUTTON_PRESS)
	{
		// angle_around_pivot -= mouse_data.CURSOR_DX * 0.002f;
		// pitch += mouse_data.CURSOR_DY * 0.002;
		// yaw += mouse_data.CURSOR_DX * 0.001;
	}
	calculateCameraPos();


}

void Camera::updateZoom(float scroll_y)
{
	distanceToPivot += scroll_y;
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
	float rad_theta = jnMath::toRadians(pitch);// jnMath::toRadians(pitch);
	float dist_x = ((float) distanceToPivot * cos(rad_theta));
	float dist_y = ((float) distanceToPivot * sin(rad_theta));
	
	float cam_angle = pivot.y + angle_around_pivot;
	float cam_x_offset = dist_x * sin(jnMath::toRadians(cam_angle));
	float cam_z_offset = dist_x * cos(jnMath::toRadians(cam_angle));

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
	//JN_ERR("zoom {}", z);
	if(z < MAX_ZOOM)
	{
		this->zoom = MAX_ZOOM;
	} else if(z > MIN_ZOOM) {
		this->zoom = MIN_ZOOM;
	} else {
	this->zoom = z;
	}
}

glm::mat4& Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

void Camera::generateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV+zoom), getAspectRatio(), NEAR_PLANE, FAR_PLANE);	
}

glm::mat4& Camera::regenProjectionMatrix()
{
	generateProjectionMatrix();
	return this->projectionMatrix;
}