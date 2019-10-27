#include "render/shaders/Shader.h"
#include <ios>
#include <sstream>
#include "core/Log.h"
#include "core/bxMath.h"
#include "core/AssetLoader.h"

using namespace bx;

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

	bxImport::loadShader(filepath, vertexShader, fragmentShader);

	BX_INFO("Linking shaders");
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
		BX_CRIT("Shader linking failed!");
		BX_CRIT(message);
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
	loc_transformationMatrix = glGetUniformLocation(progID, "transformationMatrix");
	loc_projectionMatrix = glGetUniformLocation(progID, "projectionMatrix");
	loc_viewMatrix = glGetUniformLocation(progID, "viewMatrix");
	loc_reflectivity = glGetUniformLocation(progID, "reflectivity");
	loc_shineDamper = glGetUniformLocation(progID, "shineDamper");

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		std::string location = "lightPosition[";
		location += i + "]";
		loc_lightPositions[i] = glGetUniformLocation(progID, location.c_str());
		location = "lightColor[" + i;
		location += i + "]";
		loc_lightColors[i] = glGetUniformLocation(progID, location.c_str());
	}
}

void Shader::loadTransformMatrix(glm::mat4& transform)
{
	setMat4(loc_transformationMatrix, transform);
}

void Shader::loadProjectionMatrix(glm::mat4& projection)
{
	setMat4(loc_projectionMatrix, projection);
}

void Shader::loadViewMatrix(Camera* camera)
{
	glm::mat4 viewMat = bxMath::generateViewMatrix(camera);
	setMat4(loc_viewMatrix, viewMat);

}

/* lighting variables */
void Shader::loadLightUniforms(std::vector<Light> &lights)
{
	// setVec3(loc_lightPosition, light.getPosition());
	// setVec3(loc_lightColor, light.getColor());
	for(unsigned int i = 0; i < NUM_LIGHTS; i++)
	{
		if(i >= lights.size())
		{
			glm::vec3 empty_light(0.0, 0.0, 0.0);
			setVec3(loc_lightPositions[i], empty_light);
			setVec3(loc_lightColors[i], empty_light);
		} else {
			setVec3(loc_lightPositions[i], lights[i].getPosition());
			setVec3(loc_lightColors[i], lights[i].getColor());
		}
	}

}

void Shader::loadPBRVars(Material material)
{
	setFloat(loc_reflectivity, material.reflectivity);
	setFloat(loc_shineDamper, material.shineDamper);
}

/* GLSL uniform loaders */
void Shader::setInt(unsigned int loc, int value) const
{
	glUniform1i(loc, (int)value);
}


void Shader::setBool(unsigned int loc, bool flag) const
{
	glUniform1i(loc, flag);
}

void Shader::setFloat(unsigned int loc, float value) const
{
	glUniform1f(loc, value);
}

void Shader::setVec2(unsigned int loc, glm::vec2& vec) const
{
	glUniform2fv(loc, 1, &vec[0]);
}

void Shader::setVec3(unsigned int loc, glm::vec3 &vec) const
{
	glUniform3fv(loc, 1, &vec[0]);
}

void Shader::setVec4(unsigned int loc, glm::vec4& vec) const
{
	glUniform4fv(loc, 1, &vec[0]);
}

void Shader::setMat2(unsigned int loc, glm::mat2& mat) const
{
	glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(unsigned int loc, glm::mat3& mat) const
{
	glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(unsigned int loc, glm::mat4& mat) const
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}