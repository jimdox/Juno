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
	loc_transformationMatrix = 0;
	loc_projectionMatrix = 0;
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
		BBX_ERR("Error: cannot access file: " + vertFilePath);
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
		BBX_ERR("Error: cannot access file: " + fragFilePath);
		return false;
	}

	GLint compileFlag = GL_FALSE;
	int errorLength;
	BBX_WARN("Compiling shaders");

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
		BBX_CRIT(message);
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
		BBX_CRIT(message);

	}

	BBX_INFO("Linking shaders");
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
		BBX_CRIT("Shader linking failed!");
		BBX_CRIT(message);
	}
	getAllUniformLoc();





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
	bindAttrib(1, "texCoordinates");

}

void Shader::bindAttrib(unsigned int attrib, const std::string& var)
{
	glBindAttribLocation(this->progID, attrib, var.c_str());
}

/* locates all uniform variables' locations */
void Shader::getAllUniformLoc()
{
	loc_transformationMatrix = glGetUniformLocation(progID, "transformationMatrix");
	loc_projectionMatrix = glGetUniformLocation(progID, "projectionMatrix");
	loc_viewMatrix = glGetUniformLocation(progID, "viewMatrix");
}

void Shader::loadTransformMatrix(glm::mat4& transform)
{
	glUniformMatrix4fv(loc_transformationMatrix, 1, GL_FALSE, &transform[0][0]);
}

void Shader::loadProjectionMatrix(glm::mat4& projection)
{
	glUniformMatrix4fv(loc_projectionMatrix, 1, GL_FALSE, &projection[0][0]);
}

void Shader::loadViewMatrix(Camera& camera)
{
	glm::mat4 viewMat = bxMath::generateViewMatrix(camera);
	glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, &viewMat[0][0]);

}

void Shader::setInt(const std::string& var, int value) const
{
	glUniform1i(glGetUniformLocation(progID, var.c_str()), (int)value);
}


void Shader::setBool(const std::string& var, bool flag) const
{
	glUniform1i(glGetUniformLocation(progID, var.c_str()), flag);
}

void Shader::setFloat(const std::string& var, float value) const
{
	glUniform1f(glGetUniformLocation(progID, var.c_str()), value);
}

void Shader::setVec2(const std::string& var, glm::vec2& vec) const
{
	glUniform2fv(glGetUniformLocation(progID, var.c_str()), 1, &vec[0]);
}

void Shader::setVec3(const std::string& var, glm::vec3& vec) const
{
	glUniform3fv(glGetUniformLocation(progID, var.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string& var, glm::vec4& vec) const
{
	glUniform4fv(glGetUniformLocation(progID, var.c_str()), 1, &vec[0]);
}

void Shader::setMat2(const std::string& var, glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(progID, var.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& var, glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(progID, var.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& var, glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(progID, var.c_str()), 1, GL_FALSE, &mat[0][0]);
}