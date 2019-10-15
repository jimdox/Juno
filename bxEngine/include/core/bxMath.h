#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

namespace bxMath
{

static float toRadians(float degrees)
{
    return degrees * (M_PI/180.0f);
}


/* rotation needs to be in degrees */
static glm::mat4 createTransformationMat(glm::vec3 translation, glm::vec3 rotation, float scale)
{
    glm::mat4 transform = glm::mat4(1.0f);              /* set transformation matrix to identity mat */
    transform = glm::translate(transform, translation);
    
    transform = glm::rotate(transform, toRadians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, toRadians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, toRadians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = glm::scale(transform, glm::vec3(scale, scale, scale));

    return transform;
}


static glm::mat4 generateViewMatrix(bbx::Camera& camera)
{
    glm::mat4 viewMat = glm::mat4(1.0f);
    viewMat = glm::rotate(viewMat, toRadians(camera.getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMat = glm::rotate(viewMat, toRadians(camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    viewMat = glm::rotate(viewMat, toRadians(camera.getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::vec3 camPos = camera.getPosition();
    glm::vec3 negativeCamPos = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
    
    viewMat = glm::translate(viewMat, negativeCamPos);
    
    return viewMat;
}
}