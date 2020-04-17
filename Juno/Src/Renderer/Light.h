#pragma once
namespace Juno
{
#define MAX_NUM_LIGHTS 4
class Light
{
public:
    Light(const glm::vec3& pos, const glm::vec3& color);
    Light(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& a);
    ~Light();

    glm::vec3& GetPosition();
    glm::vec3& GetColor();
    glm::vec3& GetAttenuation();
    void SetPosition(const glm::vec3& pos);
    void SetColor(glm::vec3 color);
    void SetAttenuation(const glm::vec3& a);

private:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 attenuationFactors;
};
}