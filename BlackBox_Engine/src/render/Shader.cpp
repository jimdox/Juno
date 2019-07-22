#include "render/Shader.h"

#include <ios>
#include <sstream>
#include "core/Log.h"

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






	return true;
}


GLuint Shader::getProgram()
{
	const GLuint p = progID;
	return p;
}


void Shader::useProgram()
{
	glUseProgram(this->progID);
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
	glUniformMatrix3fv(glGetUniformLocation(progID, var.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& var, glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(progID, var.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& var, glm::mat4& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(progID, var.c_str()), 1, GL_FALSE, &mat[0][0]);
}