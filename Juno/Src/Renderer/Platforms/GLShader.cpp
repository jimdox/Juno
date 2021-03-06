#include "jnpch.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/Platforms/GLShader.h"
#include "Core/AssetManager.h"
#include <glad/glad.h>
using namespace Juno;


GLShader::GLShader(std::array<ShaderComponentType, 3>& components, const std::string& fp)
{
	shaderID = glCreateProgram();
	unsigned int ids[3];
	ShaderComponentType activeComponents[3];

	for(unsigned int i = 0; i < 3; i++)
	{
		if(components[i] != ShaderComponentType::Empty)
		{
			ids[i] = BuildComponent(fp, components[i]);
			activeComponents[i] = components[i];
			glAttachShader(shaderID, ids[i]);
		}
	}

	glBindAttribLocation(shaderID, 0, "position");
	glBindAttribLocation(shaderID, 1, "normal");
	//glBindAttribLocation(shaderID, 2, "texCoordinates");

	//JN_WARN("Linking shader...");
	glLinkProgram(shaderID);

	for(unsigned int i = 0; i < 3; i++)
	{
		if(components[i] != ShaderComponentType::Empty)
			LinkErrorCheck(ids[i], components[i]);
	}

}

GLShader::~GLShader()
{
	glDeleteProgram(shaderID);
}


void GLShader::Bind()
{
	glUseProgram(shaderID);
}

void GLShader::Unbind()
{
	glUseProgram(0);
}

unsigned int GLShader::BuildComponent(const std::string& fp, ShaderComponentType componentType)
{
	unsigned int componentID;
	GLenum shaderTypeGL;
	switch(componentType)
	{
	case ShaderComponentType::Vertex:
		shaderTypeGL = GL_VERTEX_SHADER; 
		break;	
	case ShaderComponentType::Fragment:
		shaderTypeGL = GL_FRAGMENT_SHADER; 
		break;	
	case ShaderComponentType::Geometry:
		shaderTypeGL = GL_GEOMETRY_SHADER; 
		break;	
	case ShaderComponentType::Compute:
		shaderTypeGL = GL_COMPUTE_SHADER; 
		break;	
	};
	componentID = glCreateShader(shaderTypeGL);
	std::string source = AssetManager::Get().ReadShaderComponentFile(fp, componentType).c_str();
	char const* srcPtr = source.c_str();
	glShaderSource(componentID, 1, &srcPtr, NULL);
	glCompileShader(componentID);
	return componentID;
}

void GLShader::Compile()
{


}

void GLShader::BindAttribute(unsigned int attribID, const std::string& varName)
{
	glBindAttribLocation(shaderID, attribID, varName.c_str());
}

unsigned int GLShader::GetUniformLocation(const std::string& varName)
{
    return glGetUniformLocation(shaderID, varName.c_str());
}


void GLShader::LoadInt(unsigned int loc, int value)
{
    glUniform1i(loc, (int)value);
}

void GLShader::LoadBool(unsigned int loc, bool flag)
{
	glUniform1i(loc, flag);
}

void GLShader::LoadFloat(unsigned int loc, float value)
{
	glUniform1f(loc, value);
}

void GLShader::LoadFloat2(unsigned int loc, const Vec2& vec)
{
	glUniform2fv(loc, 1, &vec[0]);
}	

void GLShader::LoadFloat3(unsigned int loc, const Vec3& vec)
{
	glUniform3fv(loc, 1, &vec[0]);
}

void GLShader::LoadFloat4(unsigned int loc, const Vec4& vec)
{
	glUniform4fv(loc, 1, &vec[0]);
}

void GLShader::LoadMat2(unsigned int loc, const Mat2& mat)
{
	glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void GLShader::LoadMat3(unsigned int loc, const Mat3& mat)
{
	glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void GLShader::LoadMat4(unsigned int loc, const Mat4& mat)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}


bool GLShader::LinkErrorCheck(unsigned int id, ShaderComponentType componentType)
{
    int errorLength;
	GLint linkStatus;
	glGetProgramiv(shaderID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(id, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		std::string shaderTypeName;

        switch(componentType)
        {
        case ShaderComponentType::Vertex:
			shaderTypeName = "Vertex";
            break;
        case ShaderComponentType::Fragment:
			shaderTypeName = "Fragment";
            break;
        case ShaderComponentType::Geometry:
			shaderTypeName = "Geometry";
            break;
        case ShaderComponentType::Compute:
			shaderTypeName = "Compute";
            break;
        };

		JN_CLI_CRIT("Error trying to link {} Shader!", shaderTypeName);
		JN_CLI_TRACE(message);
		return true;
	}
	return false;
}