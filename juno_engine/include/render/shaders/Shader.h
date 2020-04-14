#pragma once
#include "pch.h"
#include <GL/glew.h>
#include "render/Camera.h"
#include "render/lights/Light.h"
#include "render/Mesh.h"
#include "render/Material.h"

namespace juno {
//enum ShaderType { VERTEX, FRAGMENT, GEOMETRY };
class Shader
{
public:
	Shader();
	Shader(const std::string& filepath);
	virtual ~Shader();

	void setActive();
	void unbindProgram();
	GLuint getID();
	std::string& getFilepath();

	void bindAllAttribs();
	void bindAttrib(unsigned int attrib, const std::string& var);
	void cacheUniformLocations();

	void loadTransformMatrix(glm::mat4& transform);
	void loadProjectionMatrix(glm::mat4& projection);
	void loadViewMatrix(Camera* camera);

	void loadLightUniforms(std::vector<Light> &light);
	void loadMaterialVars(Material material);

	void loadInt(unsigned int loc, int value) const;
	void loadBool(unsigned int loc, bool flag) const;
	void loadFloat(unsigned int loc, float value) const;
	void loadFloat2(unsigned int loc, glm::vec2& vec) const;
	void loadFloat3(unsigned int loc, glm::vec3& vec) const;
	void loadFloat4(unsigned int loc, glm::vec4& vec) const;
	void loadMat2(unsigned int loc, glm::mat2& mat) const;
	void loadMat3(unsigned int loc, glm::mat3& mat) const;
	void loadMat4(unsigned int loc, glm::mat4& mat) const;

protected:
	void compileShader();

	bool linkErrorCheck(GLuint id, GLenum type);
	GLuint progID;
	std::string filepath;
	
	/* uniform caching: */
	unsigned int loc_transformationMatrix;
	unsigned int loc_projectionMatrix;
	unsigned int loc_viewMatrix;
	unsigned int loc_reflectivity;
	unsigned int loc_shineDamper;
	unsigned int loc_baseColor;

	static const unsigned int NUM_LIGHTS = 4;
	unsigned int loc_lightPositions[NUM_LIGHTS];
	unsigned int loc_lightColors[NUM_LIGHTS];
	unsigned int loc_attenuations[NUM_LIGHTS];

private:

};
}