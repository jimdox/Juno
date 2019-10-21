#include "render/Shader.h"

#include <ios>
#include <sstream>
#include "core/Log.h"
#include "core/bxMath.h"

using namespace bbx;

Shader::Shader()
{
	vertFilePath = "res/shaders/basic.vert";
	fragFilePath = "res/shaders/basic.frag";

}


Shader::Shader(const std::string& filepath)
{
	vertFilePath = filepath + ".vert";
	fragFilePath = filepath + ".frag";
	loadShader();

}


Shader::~Shader()
{

}


bool Shader::loadShader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertSource;
	std::string fragSource;

	std::ifstream sourceStream(vertFilePath, std::ios::in);
	if (sourceStream.is_open())
	{
		std::stringstream src;
		src << sourceStream.rdbuf();
		vertSource = src.str();
		sourceStream.close();
	}
	else {
		BX_ERR("Error: cannot access file: " + vertFilePath);
	}

	std::ifstream sourceStream2(fragFilePath, std::ios::in);
	if (sourceStream2.is_open())
	{

		std::stringstream src;
		src << sourceStream2.rdbuf();
		fragSource = src.str();
		sourceStream2.close();
	}
	else {
		BX_ERR("Error: cannot access file: " + fragFilePath);
		return false;
	}

	GLint compileFlag = GL_FALSE;
	int errorLength;
	BX_WARN("Compiling shaders");

	char const* vertSrcPtr = vertSource.c_str();
	char const* fragSrcPtr = fragSource.c_str();

	glShaderSource(vertexShader, 1, &vertSrcPtr, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileFlag);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{

		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(vertexShader, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		BX_CRIT(message);
	}

	glShaderSource(fragmentShader, 1, &fragSrcPtr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileFlag);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(fragmentShader, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		BX_CRIT(message);

	}

	BX_INFO("Linking shaders");
	this->progID = glCreateProgram();

	glAttachShader(progID, vertexShader);
	glAttachShader(progID, fragmentShader);
	bindAllAttribs();
	glLinkProgram(progID);


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
	loc_lightPosition = glGetUniformLocation(progID, "lightPosition");
	loc_lightColor = glGetUniformLocation(progID, "lightColor");
}

void Shader::loadTransformMatrix(glm::mat4& transform)
{
	setMat4(loc_transformationMatrix, transform);
}

void Shader::loadProjectionMatrix(glm::mat4& projection)
{
	setMat4(loc_projectionMatrix, projection);
}

void Shader::loadViewMatrix(std::shared_ptr<Camera> & camera)
{
	glm::mat4 viewMat = bxMath::generateViewMatrix(camera);
	setMat4(loc_viewMatrix, viewMat);

}

/* lighting variables */
void Shader::loadLightUniforms(Light& light)
{
	setVec3(loc_lightPosition, light.getPosition());
	setVec3(loc_lightColor, light.getColor());

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