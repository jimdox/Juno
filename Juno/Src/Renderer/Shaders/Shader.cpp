#include "jnpch.h"
#include "Renderer/Shaders/Shader.h"
#include "Core/AssetManager.h"
#include "Utils/Math.h"

using namespace Juno;

Shader::Shader()
{

}

Shader::Shader(const std::string& filepath)
{
	this->filepath = filepath;
	CompileShader();
	CacheUniformLocations();
}

Shader::~Shader()
{

}

void Shader::CompileShader()
{
	GLuint vertShader = AssetManager::Get().LoadShaderComponentFile(filepath, GL_VERTEX_SHADER);
	GLuint fragShader = AssetManager::Get().LoadShaderComponentFile(filepath, GL_FRAGMENT_SHADER);

	// GLint *shaderIDs = AssetManager::get().loadShaderFile(filepath);
	// GLuint vertShader = shaderIDs[0];
	// GLuint fragShader = shaderIDs[1];

	//JN_INFO("Linking shaders");
	this->progID = glCreateProgram();

	glAttachShader(progID, vertShader);
	glAttachShader(progID, fragShader);
	BindAllAttribs();
	glLinkProgram(progID);

	LinkErrorCheck(vertShader, GL_VERTEX_SHADER);
	LinkErrorCheck(fragShader, GL_FRAGMENT_SHADER);
}

bool Shader::LinkErrorCheck(GLuint id, GLenum type)
{
	int errorLength;
	GLint linkStatus;
	glGetProgramiv(progID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(id, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());

		std::string shaderTypeName;
		if(type == GL_VERTEX_SHADER)
		{
			shaderTypeName = "Vertex";
		} else if(type == GL_FRAGMENT_SHADER)
		{
			shaderTypeName = "Fragment";
		} else if(type == GL_COMPUTE_SHADER)
		{
			shaderTypeName = "Compute";
		} else if(type == GL_GEOMETRY_SHADER)
		{
			shaderTypeName = "Geometry";
		}

		JN_CLI_CRIT("Error trying to link {} Shader!", shaderTypeName);
		JN_CLI_TRACE(message);
		//std::cerr << message;
		return true;
	}
	return false;
}

GLuint Shader::GetID()
{
	const GLuint p = progID;
	return p;
}

std::string& Shader::GetFilepath()
{
	return filepath;
}

/* binds shader */
void Shader::Bind()
{
	glUseProgram(progID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::BindAllAttribs()
{
	BindAttrib(0, "position");
	BindAttrib(1, "normal");
	BindAttrib(2, "texCoordinates");
	
}

void Shader::BindAttrib(unsigned int attrib, const std::string& var)
{
	glBindAttribLocation(this->progID, attrib, var.c_str());
}

/* caches all uniform variables' locations in memory */
void Shader::CacheUniformLocations()
{
	loc_transformationMatrix = glGetUniformLocation(progID, "model");
	loc_projectionMatrix = glGetUniformLocation(progID, "projection");
	loc_viewMatrix = glGetUniformLocation(progID, "view");
	loc_reflectivity = glGetUniformLocation(progID, "reflectivity");
	loc_shineDamper = glGetUniformLocation(progID, "shineDamper");
	loc_baseColor = glGetUniformLocation(progID, "in_baseColor");

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		std::string location("lightPosition["+std::to_string(i)+"]");
		loc_lightPositions[i] = glGetUniformLocation(progID, location.c_str());
		
		location = std::string("lightColor["+std::to_string(i)+"]");
		loc_lightColors[i] = glGetUniformLocation(progID, location.c_str());
		
		location = std::string("attenuations["+std::to_string(i)+"]");
		loc_attenuations[i] = glGetUniformLocation(progID, location.c_str());
	}
}

void Shader::LoadTransformMatrix(glm::mat4& transform)
{
	LoadMat4(loc_transformationMatrix, transform);
}

void Shader::LoadProjectionMatrix(glm::mat4& projection)
{
	LoadMat4(loc_projectionMatrix, projection);
}

void Shader::LoadViewMatrix(Camera& camera)
{
	glm::mat4 viewMat = GenerateViewMatrix(camera);
	LoadMat4(loc_viewMatrix, viewMat);

}

/* lighting variables */
void Shader::LoadLightUniforms(std::vector<Light>& lights)
{
	for(unsigned int i = 0; i < NUM_LIGHTS; i++)
	{
		if(i < lights.size())
		{
			LoadFloat3(loc_lightPositions[i], lights[i].GetPosition());
			LoadFloat3(loc_lightColors[i], lights[i].GetColor());
			LoadFloat3(loc_attenuations[i], lights[i].GetAttenuation());
		} else {
			glm::vec3 empty_light(0.0, 0.0, 0.0);
			LoadFloat3(loc_lightPositions[i], empty_light);
			LoadFloat3(loc_lightColors[i], empty_light);
			LoadFloat3(loc_attenuations[i], empty_light);
		}
	}
}

void Shader::LoadMaterialVars(Material& material)
{
	LoadFloat(loc_reflectivity, material.GetReflectivity());
	LoadFloat(loc_shineDamper, material.GetShineDamper());
	LoadFloat3(loc_baseColor, material.GetBaseColor());
}

/* GLSL uniform loaders */
void Shader::LoadInt(unsigned int loc, int value) const
{
	glUniform1i(loc, (int)value);
}

void Shader::LoadBool(unsigned int loc, bool flag) const
{
	glUniform1i(loc, flag);
}

void Shader::LoadFloat(unsigned int loc, float value) const
{
	glUniform1f(loc, value);
}

void Shader::LoadFloat2(unsigned int loc, glm::vec2& vec) const
{
	glUniform2fv(loc, 1, &vec[0]);
}

void Shader::LoadFloat3(unsigned int loc, glm::vec3 &vec) const
{
	glUniform3fv(loc, 1, &vec[0]);
}

void Shader::LoadFloat4(unsigned int loc, glm::vec4& vec) const
{
	glUniform4fv(loc, 1, &vec[0]);
}

void Shader::LoadMat2(unsigned int loc, glm::mat2& mat) const
{
	glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::LoadMat3(unsigned int loc, glm::mat3& mat) const
{
	glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::LoadMat4(unsigned int loc, glm::mat4& mat) const
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}