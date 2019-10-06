#include <glm/glm.hpp>

namespace bbx {

enum CameraTranslation 
{
	FORWARD, BACKWARD, LEFT, RIGHT
};

	
class Camera
{
public:
	Camera(glm::vec3& pos, glm::vec3& axisUp, glm::vec3& rotation);
	~Camera();


	glm::mat4& getViewMat();
	void update();

private:
	glm::vec3 positition;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 globalUp;

	glm::vec3 rotation;

	float zoom;


	const float DEFAULT_SPEED = 1.1f;
	//float speed;
	float mouseSensitivity;

};
}