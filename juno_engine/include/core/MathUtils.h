#include "pch.h"
#pragma once

namespace juno
{

inline float toRadians(float degrees)
{
    return degrees * (M_PI/180.0f);
}

/* rotation needs to be in degrees */
static glm::mat4 createTransformationMat(glm::vec3 translation, glm::vec3 rotation, float scale)
{
    glm::mat4 transform = glm::mat4(1.0f);                              /* identity matrix */
    transform = glm::translate(transform, translation);                 /* apply translation */
    
    transform = glm::rotate(transform, toRadians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, toRadians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, toRadians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = glm::scale(transform, glm::vec3(scale, scale, scale));

    return transform;
}


static glm::mat4 generateViewMatrix(juno::Camera* camera)
{
    glm::mat4 viewMat = glm::mat4(1.0f);
    viewMat = glm::rotate(viewMat, toRadians(camera->getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMat = glm::rotate(viewMat, toRadians(camera->getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    viewMat = glm::rotate(viewMat, toRadians(camera->getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::vec3 camPos = camera->getPosition();
    glm::vec3 negativeCamPos = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
    
    viewMat = glm::translate(viewMat, negativeCamPos);
    
    return viewMat;
}

inline float baryCentricInterpolation(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec3& pos)
{
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float L_1 = ((p2.z-p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float L_2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float L_3 = 1.0f - L_1 - L_2;
    return (L_1 * p1.y) + (L_2 * p2.y) + (L_3 * p3.y);
}

// needs: fast fourier transform impl. \ 

}