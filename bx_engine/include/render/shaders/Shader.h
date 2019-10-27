#pragma once
#include "pch.h"
#include <GL/glew.h>
#include "render/Camera.h"
#include "render/lights/Light.h"
#include "render/Mesh.h"

namespace bx {
class Shader
{
public:
	Shader();
	Shader(const std::string& filepath);
	virtual ~Shader();
	bool loadShader(const std::string& filepath);
	void useProgram();
	void unbindProgram();
	GLuint getID();

	void bindAllAttribs();
	void bindAttrib(unsigned int attrib, const std::string& var);
	void cacheUniformLocations();

	void loadTransformMatrix(glm::mat4& transform);
	void loadProjectionMatrix(glm::mat4& projection);
	void loadViewMatrix(Camera* camera);

	void loadLightUniforms(std::vector<Light> &light);
	void loadPBRVars(Material material);

	void setInt(unsigned int loc, int value) const;
	void setBool(unsigned int loc, bool flag) const;
	void setFloat(unsigned int loc, float value) const;
	void setVec2(unsigned int loc, glm::vec2& vec) const;
	void setVec3(unsigned int loc, glm::vec3& vec) const;
	void setVec4(unsigned int loc, glm::vec4& vec) const;
	void setMat2(unsigned int loc, glm::mat2& mat) const;
	void setMat3(unsigned int loc, glm::mat3& mat) const;
	void setMat4(unsigned int loc, glm::mat4& mat) const;


protected:
	GLuint progID;
	std::string shaderPath;
	enum Shdr { VERTEX, FRAGMENT, GEOMETRY };
	std::string vertFilePath;
	std::string fragFilePath;
	static const unsigned int NUM_LIGHTS = 4;
	
	unsigned int loc_transformationMatrix;
	unsigned int loc_projectionMatrix;
	unsigned int loc_viewMatrix;
	unsigned int loc_reflectivity;
	unsigned int loc_shineDamper;

	unsigned int loc_lightPositions[NUM_LIGHTS];
	unsigned int loc_lightColors[NUM_LIGHTS];

	
};
}