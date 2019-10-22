#pragma once
#include "render/shaders/Shader.h"


namespace bx{
class TerrainShader
{
public:
    TerrainShader();
	TerrainShader(const std::string& filepath);
	virtual ~TerrainShader();
	bool loadShader();
	void useProgram();
	void unbindProgram();
	GLuint getID();

	void bindAllAttribs();
	void bindAttrib(unsigned int attrib, const std::string& var);
	void cacheUniformLocations();

	void loadTransformMatrix(glm::mat4& transform);
	void loadProjectionMatrix(glm::mat4& projection);
	void loadViewMatrix(Camera* camera);

	void loadLightUniforms(Light& light);
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


private:
	GLuint progID;
	std::string shaderPath;
	enum Shdr { VERTEX, FRAGMENT, GEOMETRY };
	std::string vertFilePath;
	std::string fragFilePath;
	
	unsigned int loc_transformationMatrix;
	unsigned int loc_projectionMatrix;
	unsigned int loc_viewMatrix;
	unsigned int loc_lightPosition;
	unsigned int loc_lightColor;
	unsigned int loc_reflectivity;
	unsigned int loc_shineDamper;
};
}