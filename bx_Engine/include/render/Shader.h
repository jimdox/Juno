#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "render/Camera.h"
namespace bbx {
class Shader
{
public:
	Shader();
	Shader(const std::string& filepath);
	virtual ~Shader();
	bool loadShader();
	void useProgram();
	void unbindProgram();
	GLuint getID();

	void bindAllAttribs();
	void bindAttrib(unsigned int attrib, const std::string& var);
	void getAllUniformLoc();

	void loadTransformMatrix(glm::mat4& transform);
	void loadProjectionMatrix(glm::mat4& projection);
	void loadViewMatrix(Camera& camera);


	void setInt(const std::string& var, int value) const;
	void setBool(const std::string& var, bool flag) const;
	void setFloat(const std::string& var, float value) const;
	void setVec2(const std::string& var, glm::vec2& vec) const;
	void setVec3(const std::string& var, glm::vec3& vec) const;
	void setVec4(const std::string& var, glm::vec4& vec) const;
	void setMat2(const std::string& var, glm::mat2& mat) const;
	void setMat3(const std::string& var, glm::mat3& mat) const;
	void setMat4(const std::string& var, glm::mat4& mat) const;


private:
	GLuint progID;
	std::string shaderPath;
	enum Shdr { VERTEX, FRAGMENT, GEOMETRY };
	std::string vertFilePath;
	std::string fragFilePath;
	
	unsigned int loc_transformationMatrix;
	unsigned int loc_projectionMatrix;
	unsigned int loc_viewMatrix;

};
}