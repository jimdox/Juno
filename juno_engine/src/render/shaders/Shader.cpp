#include "render/shaders/Shader.h"
#include <ios>
#include <sstream>
#include "core/Log.h"
#include "core/jMath.h"
#include "core/AssetLoader.h"

using namespace juno;

Shader::Shader()
{
	vertFilePath = "res/shaders/basic.vert";
	fragFilePath = "res/shaders/basic.frag";

}


Shader::Shader(const std::string& filepath)
{
	loadShader(filepath);

}


Shader::~Shader()
{

}


bool Shader::loadShader(const std::string& filepath)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	juno::loadShader(filepath, vertexShader, fragmentShader);

	JN_INFO("Linking shaders");
	this->progID = glCreateProgram();

	glAttachShader(progID, vertexShader);
	glAttachShader(progID, fragmentShader);
	bindAllAttribs();
	glLinkProgram(progID);

	int errorLength;
	GLint linkStatus;
	glGetProgramiv(progID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(fragmentShader, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		JN_CRIT("Shader linking failed!");
		JN_CRIT(message);
	}
	cacheUniformLocations();

	return true;
}


GLuint Shader::getID()
{
	const GLuint p = progID;
	return p;
}

/* binds shader */
void Shader::useProgram()
{
	glUseProgram(progID);
}

void Shader::unbindProgram()
{
	glUseProgram(0);
}

void Shader::bindAllAttribs()
{
	bindAttrib(0, "position");
	bindAttrib(1, "normal");
	bindAttrib(2, "texCoordinates");
	
}

void Shader::bindAttrib(unsigned int attrib, const std::string& var)
{
	glBindAttribLocation(this->progID, attrib, var.c_str());
}

/* caches all uniform variables' locations */
void Shader::cacheUniformLocations()
{
	loc_transformationMatrix = glGetUniformLocation(progID, "model");
	loc_projectionMatrix = glGetUniformLocation(progID, "projection");
	loc_viewMatrix = glGetUniformLocation(progID, "view");
	loc_reflectivity = glGetUniformLocation(progID, "reflectivity");
	loc_shineDamper = glGetUniformLocation(progID, "shineDamper");

	// for(int i = 0; i < NUM_LIGHTS; i++)
	// {
	// 	std::string location = "lightPosition[";
	// 	location += i + "]";
	// 	loc_lightPositions[i] = glGetUniformLocation(progID, location.c_str());
	// 	location = "lightColor[";
	// 	location += i + "]";
	// 	loc_lightColors[i] = glGetUniformLocation(progID, location.c_str());
	// 	location = "attenuations[";
	// 	location +=  i + "]";
	// 	loc_attenuations[i] = glGetUniformLocation(progID, location.c_str());
	// }
		std::string location = "lightPosition[0]";
		loc_lightPositions[0] = glGetUniformLocation(progID, location.c_str());
		location = "lightColor[0]";
		loc_lightColors[0] = glGetUniformLocation(progID, location.c_str());
		location = "attenuations[";
		loc_attenuations[0] = glGetUniformLocation(progID, location.c_str());
		location = "lightPosition[1]";
		loc_lightPositions[1] = glGetUniformLocation(progID, location.c_str());
		location = "lightColor[1]";
		loc_lightColors[1] = glGetUniformLocation(progID, location.c_str());
		location = "attenuations[1]";
		loc_attenuations[1] = glGetUniformLocation(progID, location.c_str());
}

void Shader::loadTransformMatrix(glm::mat4& transform)
{
	loadMat4(loc_transformationMatrix, transform);
}

void Shader::loadProjectionMatrix(glm::mat4& projection)
{
	loadMat4(loc_projectionMatrix, projection);
}

void Shader::loadViewMatrix(Camera* camera)
{
	glm::mat4 viewMat = jMath::generateViewMatrix(camera);
	loadMat4(loc_viewMatrix, viewMat);

}

/* lighting variables */
void Shader::loadLightUniforms(std::vector<Light> &lights)
{

	// for(unsigned int i = 0; i < NUM_LIGHTS; i++)
	// {
	// 	if(i >= lights.size())
	// 	{
	// 		glm::vec3 empty_light(1.0, 0.0, 0.0);
	// 		loadVec3(loc_lightPositions[i], empty_light);
	// 		loadVec3(loc_lightColors[i], empty_light);
	// 		loadVec3(loc_attenuations[i], empty_light);
	// 	} else {
	// 		loadVec3(loc_lightPositions[i], lights[1].getPosition());
	// 		loadVec3(loc_lightColors[i], lights[1].getColor());
	// 		loadVec3(loc_attenuations[i], lights[1].getAttenuation());
	// 	}
	// }
			loadVec3(loc_lightPositions[0], lights[0].getPosition());
			loadVec3(loc_lightColors[0], lights[0].getColor());
			loadVec3(loc_attenuations[0], lights[0].getAttenuation());
			loadVec3(loc_lightPositions[1], lights[1].getPosition());
			loadVec3(loc_lightColors[1], lights[1].getColor());
			loadVec3(loc_attenuations[1], lights[1].getAttenuation());
}

void Shader::loadPBRVars(Material material)
{
	loadFloat(loc_reflectivity, material.reflectivity);
	loadFloat(loc_shineDamper, material.shineDamper);
}

/* GLSL uniform loaders */
void Shader::loadInt(unsigned int loc, int value) const
{
	glUniform1i(loc, (int)value);
}


void Shader::loadBool(unsigned int loc, bool flag) const
{
	glUniform1i(loc, flag);
}

void Shader::loadFloat(unsigned int loc, float value) const
{
	glUniform1f(loc, value);
}

void Shader::loadVec2(unsigned int loc, glm::vec2& vec) const
{
	glUniform2fv(loc, 1, &vec[0]);
}

void Shader::loadVec3(unsigned int loc, glm::vec3 &vec) const
{
	glUniform3fv(loc, 1, &vec[0]);
}

void Shader::loadVec4(unsigned int loc, glm::vec4& vec) const
{
	glUniform4fv(loc, 1, &vec[0]);
}

void Shader::loadMat2(unsigned int loc, glm::mat2& mat) const
{
	glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::loadMat3(unsigned int loc, glm::mat3& mat) const
{
	glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::loadMat4(unsigned int loc, glm::mat4& mat) const
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}