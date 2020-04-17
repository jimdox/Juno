#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Juno 
{
#define Vec2 glm::vec2
#define Vec3 glm::vec3
#define Vec4 glm::vec4

inline float Max(float a, float b)
{
    if(a > b)
        return a;
    return b;
}

inline float Min(float a, float b)
{
    if(a < b)
        return a;
    return b;
}

inline float ToRadians(float degrees)
{
    return degrees * (M_PI/180.0f);
}

/* rotation needs to be in degrees */
static glm::mat4 CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, float scale)
{
    glm::mat4 transform = glm::mat4(1.0f);                              /* identity matrix */
    transform = glm::translate(transform, translation);                 /* apply translation */
    
    transform = glm::rotate(transform, ToRadians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, ToRadians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, ToRadians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = glm::scale(transform, glm::vec3(scale, scale, scale));

    return transform;
}

static glm::mat4 GenerateViewMatrix(Camera& camera)
{
    glm::mat4 viewMat = glm::mat4(1.0f);
    viewMat = glm::rotate(viewMat, ToRadians(camera.GetPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMat = glm::rotate(viewMat, ToRadians(camera.GetYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    viewMat = glm::rotate(viewMat, ToRadians(camera.GetRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::vec3 camPos = camera.GetPosition();
    glm::vec3 negativeCamPos = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
    
    viewMat = glm::translate(viewMat, negativeCamPos);
    return viewMat;
}

inline float BaryCentricInterpolation(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec3& pos)
{
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float L_1 = ((p2.z-p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float L_2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float L_3 = 1.0f - L_1 - L_2;
    return (L_1 * p1.y) + (L_2 * p2.y) + (L_3 * p3.y);
}





}