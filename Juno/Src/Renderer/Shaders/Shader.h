#pragma once
#include "jnpch.h"
#include "Renderer/Camera.h"
#include "Renderer/Light.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"
#include "Renderer/Data/VertexArray.h"
#include <glad/glad.h>

namespace Juno {
//enum ShaderType { VERTEX, FRAGMENT, GEOMETRY };
class Shader
{
public:
	Shader();
	Shader(const std::string& filepath);
	virtual ~Shader();

	void Bind();
	void Unbind();
	GLuint GetID();
	std::string& GetFilepath();

	void LoadTransformMatrix(glm::mat4& transform);
	void LoadProjectionMatrix(glm::mat4& projection);
	void LoadViewMatrix(Camera& camera);

	void LoadLightUniforms(std::vector<Light>& lights);
	void LoadMaterialVars(Material& material);

	void LoadInt(unsigned int loc, int value) const;
	void LoadBool(unsigned int loc, bool flag) const;
	void LoadFloat(unsigned int loc, float value) const;
	void LoadFloat2(unsigned int loc, glm::vec2& vec) const;
	void LoadFloat3(unsigned int loc, glm::vec3& vec) const;
	void LoadFloat4(unsigned int loc, glm::vec4& vec) const;
	void LoadMat2(unsigned int loc, glm::mat2& mat) const;
	void LoadMat3(unsigned int loc, glm::mat3& mat) const;
	void LoadMat4(unsigned int loc, glm::mat4& mat) const;




protected:
	void CompileShader();
	bool LinkErrorCheck(GLuint id, GLenum type);
	
    void BindAllAttribs();
	void BindAttrib(unsigned int attrib, const std::string& var);
	void CacheUniformLocations();
    
    
    
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